#include "./Logger.hpp"
#include "../../BeShell.hpp"
#include <stdlib.h>  // 为 malloc/free
#include <string.h>  // 为字符串函数
#include <stdio.h>   // 为 snprintf
#include "logger-imp.hpp"

#ifdef ESP_PLATFORM
// ESP32 中定义 ssize_t
#ifndef _SSIZE_T_DEFINED
typedef int ssize_t;
#define _SSIZE_T_DEFINED
#endif
extern "C" {

#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
    int __real_printf(const char *format, ...);
    int __real_vprintf(const char *format, va_list args);
    ssize_t __real_write(int fd, const void *buf, size_t count);
#else
    #define __real_printf printf
#endif

}

// 声明 printf 函数
extern "C" int printf(const char *format, ...);

#endif

namespace be {
    
    char const * const Logger::name = "logger" ;

    Logger::Logger(JSContext * ctx, const char * name)
        : NativeModule(ctx, name, 0)
    {
        // exportClass<ClassName>() ;
        EXPORT_FUNCTION(setup)
        EXPORT_FUNCTION(pause)
        EXPORT_FUNCTION(resume)
        EXPORT_FUNCTION(clear)
        EXPORT_FUNCTION(erase)
        EXPORT_FUNCTION(length)
        EXPORT_FUNCTION(read)
        EXPORT_FUNCTION(write)
        EXPORT_FUNCTION(flush)
        EXPORT_FUNCTION(top)
        EXPORT_FUNCTION(tail)
    }

    /**
     * 启动日志功能
     * 
     * 需要传入一个配置对象，指定日志分区名称。成功启动后，所有的 printf 和 write 输出都会被自动捕获并写入到指定的分区中。
     * 日志会先写入SRAM缓冲区，当缓冲区满时自动刷新到flash分区，可以显著减少flash写入次数，提高性能和flash寿命。
     * 
     * 配置对象格式：
     * ```js
     * {
     *     partition: string       // 日志分区名称
     *     bufferSize: number      // 可选，SRAM缓冲区大小（字节），默认为1024
     * }
     * ```
     * 
     * @module logger
     * @function setup
     * @param options:object 配置选项
     * @return undefined
     */
    JSValue Logger::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if (argc < 1) {
            JSTHROW("setup requires an options object");
        }

        // 检查参数是否为对象
        if (!JS_IsObject(argv[0])) {
            JSTHROW("options must be an object");
        }

        // 获取partition属性
        JSValue partition_val = JS_GetPropertyStr(ctx, argv[0], "partition");
        if (JS_IsException(partition_val)) {
            return partition_val;
        }

        if (JS_IsUndefined(partition_val) || JS_IsNull(partition_val)) {
            JS_FreeValue(ctx, partition_val);
            JSTHROW("partition property is required");
        }

        // 转换partition为字符串
        const char* partition_name = JS_ToCString(ctx, partition_val);
        JS_FreeValue(ctx, partition_val);
        
        if (!partition_name) {
            JSTHROW("partition must be a string");
        }
        
        // 获取bufferSize属性（可选）
        size_t buffer_size = 1024; // 默认值
        JSValue buffer_val = JS_GetPropertyStr(ctx, argv[0], "bufferSize");
        if (!JS_IsException(buffer_val) && !JS_IsUndefined(buffer_val) && !JS_IsNull(buffer_val)) {
            int64_t size;
            if (JS_ToInt64(ctx, &size, buffer_val) >= 0 && size > 0) {
                buffer_size = (size_t)size;
            }
        }
        JS_FreeValue(ctx, buffer_val);

        // 调用C++版本的setup函数
        int result = setup(*(JSEngine::fromJSContext(ctx)->beshell), partition_name, buffer_size);
        JS_FreeCString(ctx, partition_name);
        
        // 根据返回值处理错误
        switch (result) {
            case Logger::SUCCESS:
                return JS_UNDEFINED;
            case Logger::ERROR_INVALID_PARTITION_NAME:
                JSTHROW("Invalid partition name");
            case Logger::ERROR_PARTITION_NOT_FOUND:
                JSTHROW("Partition not found");
            case Logger::ERROR_PARTITION_READ_FAILED:
                JSTHROW("Failed to read partition metadata");
            case Logger::ERROR_PARTITION_WRITE_FAILED:
                JSTHROW("Failed to write partition metadata");
            case Logger::ERROR_MEMORY_ALLOCATION_FAILED:
                JSTHROW("Failed to allocate memory for buffer");
            default:
                JSTHROW("Unknown error occurred");
        }
    }

    /**
     * 暂停日志捕获功能
     * 
     * 调用此函数后，printf 和 write 的输出将停止被捕获到日志分区中。
     * 
     * @module logger
     * @function pause
     * @return undefined
     */
    JSValue Logger::pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        pause();
        return JS_UNDEFINED;
    }

    /**
     * 恢复日志捕获功能
     * 
     * 调用此函数后，printf 和 write 的输出将重新开始被捕获到日志分区中。
     * 
     * @module logger
     * @function resume
     * @return undefined
     */
    JSValue Logger::resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        resume();
        return JS_UNDEFINED;
    }
    
    /**
     * 清空日志数据
     * 
     * 清除分区中的所有日志数据，重置写入位置到起始状态。此操作不可逆。
     * 
     * @module logger
     * @function clear
     * @return undefined
     */
    JSValue Logger::clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        int result = clear();
        
        switch (result) {
            case Logger::SUCCESS:
                return JS_UNDEFINED;
            case Logger::ERROR_PARTITION_NOT_FOUND:
                JSTHROW("Logger not initialized - call setup() first");
            case Logger::ERROR_PARTITION_WRITE_FAILED:
                JSTHROW("Failed to update metadata");
            default:
                JSTHROW("Unknown error occurred");
        }
    }

    /**
     * 获取日志数据长度
     * 
     * 返回当前日志分区中已写入的日志数据总字节数。
     * 如果日志发生了循环写入，返回值为可用空间大小（分区大小减去元数据大小）。
     * 如果未发生循环，返回值为当前写入位置。
     * 
     * @module logger
     * @function length
     * @return number 日志数据字节数
     */
    JSValue Logger::length(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        size_t log_length = length();
        return JS_NewInt64(ctx, (int64_t)log_length);
    }
    
    /**
     * 从日志分区读取数据
     * 
     * 从指定位置开始读取指定长度的原始日志数据。返回的数据包含时间戳前缀和原始日志内容。
     * 
     * 使用示例：
     * ```js
     * // 读取前1024字节的日志数据
     * const data = logger.read(0, 1024);
     * console.log(data.asString());
     * 
     * // 从第512字节开始读取512字节
     * const data2 = logger.read(512, 512);
     * ```
     * 
     * @module logger
     * @function read
     * @param start_pos:number=0 读取起始位置（相对于日志数据区）
     * @param length number=1024 读取长度（字节数）
     * @return ArrayBuffer 读取到的原始数据
     */
    JSValue Logger::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        size_t start_pos = 0;
        size_t read_length = 1024; // 默认1k

        // 解析第一个参数：起点位置
        if (argc >= 1 && !JS_IsUndefined(argv[0])) {
            int64_t pos;
            if (JS_ToInt64(ctx, &pos, argv[0]) < 0) {
                JSTHROW("start position must be a number");
            }
            if (pos < 0) {
                JSTHROW("start position must be non-negative");
            }
            start_pos = (size_t)pos;
        }

        // 解析第二个参数：读取长度
        if (argc >= 2 && !JS_IsUndefined(argv[1])) {
            int64_t length;
            if (JS_ToInt64(ctx, &length, argv[1]) < 0) {
                JSTHROW("read length must be a number");
            }
            if (length <= 0) {
                JSTHROW("read length must be positive");
            }
            read_length = (size_t)length;
        }

        uint8_t* buffer = (uint8_t*)malloc(read_length);
        if(!buffer) {
            JSTHROW("Failed to allocate read buffer");
        }
        size_t actual_length = read(start_pos, read_length, buffer);
        if(!actual_length) {
            free(buffer) ;
            buffer = nullptr ;
            JSTHROW("Failed to read from partition or start position exceeds partition size");
        }
        
        return JS_NewArrayBuffer(ctx, buffer, actual_length, freeArrayBuffer, nullptr, false);
    }

    /**
     * 向日志分区写入数据
     * 
     * 将指定的数据直接写入到日志分区中，支持字符串和ArrayBuffer两种数据类型。
     * 写入的数据可以选择是否添加时间戳前缀。
     * 
     * 使用示例：
     * ```js
     * // 写入字符串（默认添加时间戳前缀）
     * logger.write("Hello, World!");
     * 
     * // 写入字符串（不添加时间戳前缀）
     * logger.write("Hello, World!", false);
     * 
     * // 写入ArrayBuffer（添加时间戳前缀）
     * const buffer = new ArrayBuffer(10);
     * const view = new Uint8Array(buffer);
     * view.fill(65); // 填充字母'A'
     * logger.write(buffer, true);
     * 
     * // 写入格式化字符串（通过模板字符串）
     * const value = 42;
     * logger.write(`Value is: ${value}`);
     * ```
     * 
     * @module logger
     * @function write
     * @param data:string|ArrayBuffer 要写入的数据
     * @param timestampPrefix boolean=true 是否添加时间戳前缀
     * @return number 实际写入的字节数
     */
    JSValue Logger::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if (argc < 1) {
            JSTHROW("write requires data argument");
        }

        JSValue data = argv[0];
        bool timestampPrefix = true; // 默认值
        size_t written_bytes = 0;

        // 解析第二个参数：timestampPrefix
        if (argc >= 2 && !JS_IsUndefined(argv[1])) {
            timestampPrefix = JS_ToBool(ctx, argv[1]);
        }

        // 处理字符串类型
        if (JS_IsString(data)) {
            size_t str_len;
            const char* str = JS_ToCStringLen(ctx, &str_len, data);
            if (!str) {
                JSTHROW("Failed to convert string data");
            }
            
            if (str_len > 0) {
                written_bytes = write(str, str_len, timestampPrefix);
            }
            
            JS_FreeCString(ctx, str);
        }
        // 处理ArrayBuffer类型
        else if (JS_IsArrayBuffer(data)) {
            size_t buffer_size;
            uint8_t* buffer_data = JS_GetArrayBuffer(ctx, &buffer_size, data);
            
            if (!buffer_data) {
                JSTHROW("Failed to get ArrayBuffer data");
            }
            
            if (buffer_size > 0) {
                written_bytes = write(buffer_data, buffer_size, timestampPrefix);
            }
        }
        // 处理TypedArray类型（如Uint8Array等）
        else {
            // 尝试获取ArrayBuffer视图
            JSValue buffer_val = JS_GetPropertyStr(ctx, data, "buffer");
            if (!JS_IsException(buffer_val) && JS_IsArrayBuffer(buffer_val)) {
                // 获取byteOffset和byteLength
                JSValue offset_val = JS_GetPropertyStr(ctx, data, "byteOffset");
                JSValue length_val = JS_GetPropertyStr(ctx, data, "byteLength");
                
                if (!JS_IsException(offset_val) && !JS_IsException(length_val)) {
                    int64_t offset = 0, length = 0;
                    if (JS_ToInt64(ctx, &offset, offset_val) >= 0 && 
                        JS_ToInt64(ctx, &length, length_val) >= 0 && 
                        offset >= 0 && length > 0) {
                        
                        size_t buffer_size;
                        uint8_t* buffer_data = JS_GetArrayBuffer(ctx, &buffer_size, buffer_val);
                        
                        if (buffer_data && (size_t)(offset + length) <= buffer_size) {
                            written_bytes = write(buffer_data + offset, (size_t)length, timestampPrefix);
                        }
                    }
                }
                
                JS_FreeValue(ctx, offset_val);
                JS_FreeValue(ctx, length_val);
            }
            JS_FreeValue(ctx, buffer_val);
            
            // 如果上面的处理都失败了，尝试将参数转换为字符串
            if (written_bytes == 0) {
                size_t str_len;
                const char* str = JS_ToCStringLen(ctx, &str_len, data);
                if (str) {
                    if (str_len > 0) {
                        written_bytes = write(str, str_len, timestampPrefix);
                    }
                    JS_FreeCString(ctx, str);
                } else {
                    JSTHROW("Unsupported data type - must be string or ArrayBuffer");
                }
            }
        }

        return JS_NewInt64(ctx, (int64_t)written_bytes);
    }
    
    /**
     * 将缓冲区中的数据立即写入到flash分区
     * 
     * 调用此函数将强制把当前缓冲区中的所有数据写入到flash分区中，无论缓冲区是否已满。
     * 这在需要确保日志立即持久化的情况下非常有用，如在关键操作前后或系统即将重启时。
     * 
     * @module logger
     * @function flush
     * @return number 实际写入的字节数
     */
    JSValue Logger::flush(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        int written = flush();
        return JS_NewInt32(ctx, written);
    }
    /**
     * 清除日志分区中的所有数据
     * 
     * 调用此函数将清除日志分区中的所有数据，并重置写入位置到起始状态。
     * 注意，此操作不可逆，所有日志数据将被永久删除。
     * 
     * @module logger
     * @function erase
     * @return undefined
     */
    JSValue Logger::erase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        Logger::erase();
        return JS_UNDEFINED;
    }


    class ScopedCapture {
        bool wasCapturing ;
    public:
        ScopedCapture() { 
            wasCapturing = Logger::isCapturing() ;
            if (wasCapturing) {
                Logger::pause();
            }
        }
        ~ScopedCapture() {
            if (wasCapturing) Logger::resume();
        }
    };
    
    /**
     * 从日志区头部分页显示日志内容
     * 
     * 从日志分区的开头开始，按行分页显示日志内容。每行最多1024字符，超出按新行计算。
     * 直接输出到控制台，不返回JavaScript变量。
     * 
     * 使用示例：
     * ```js
     * // 显示第一页，每页20行
     * logger.top();
     * 
     * // 显示第2页，每页10行
     * logger.top(1, 10);
     * ```
     * 
     * @module logger
     * @function top
     * @param page:number=0 页码（从0开始）
     * @param linesPerPage number=20 每页显示的行数
     * @return undefined
     */
    JSValue Logger::top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        int64_t page = 0;
        int64_t lines_per_page = 20;

        // 解析第一个参数：页码
        if (argc >= 1 && !JS_IsUndefined(argv[0])) {
            if (JS_ToInt64(ctx, &page, argv[0]) < 0) {
                JSTHROW("page must be a number");
            }
            if (page < 0) {
                JSTHROW("page must be non-negative");
            }
        }

        // 解析第二个参数：每页行数
        if (argc >= 2 && !JS_IsUndefined(argv[1])) {
            if (JS_ToInt64(ctx, &lines_per_page, argv[1]) < 0) {
                JSTHROW("linesPerPage must be a number");
            }
            if (lines_per_page <= 0) {
                JSTHROW("linesPerPage must be positive");
            }
        }
        
        ScopedCapture capture; // 自动暂停和恢复日志捕获

        // 获取日志总长度
        size_t total_length = length();
        if (total_length == 0) {
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
            __real_printf("No log data available\n");
#else
            printf("No log data available\n");
#endif
            return JS_UNDEFINED;
        }

        // 从头开始读取，寻找指定页的内容
        size_t read_pos = 0;
        size_t lines_found = 0;
        size_t chars_in_current_line = 0;
        size_t target_start_line = (size_t)(page * lines_per_page);
        size_t target_end_line = target_start_line + (size_t)lines_per_page;
        
        uint8_t buffer[4096];
        size_t lines_output = 0;
        bool in_target_range = false;
        
        while (read_pos < total_length && lines_output < (size_t)lines_per_page) {
            size_t chunk_size = (total_length - read_pos > sizeof(buffer)) ? sizeof(buffer) : (total_length - read_pos);
            size_t actual_read = log_read(read_pos, chunk_size, buffer);
            
            if (actual_read == 0) {
                break;
            }
            
            for (size_t i = 0; i < actual_read; i++) {
                // 检查是否需要换行（遇到换行符或达到1024字符）
                bool should_newline = false;
                if (buffer[i] == '\n') {
                    should_newline = true;
                    chars_in_current_line = 0;
                } else {
                    chars_in_current_line++;
                    if (chars_in_current_line >= 1024) {
                        should_newline = true;
                        chars_in_current_line = 0;
                    }
                }
                
                if (should_newline) {
                    lines_found++;
                    
                    if (lines_found > target_start_line && lines_found <= target_end_line) {
                        if (!in_target_range) {
                            in_target_range = true;
                        }
                        lines_output++;
                        if (lines_output >= (size_t)lines_per_page) {
                            break;
                        }
                    }
                }
                
                // 输出字符（如果在目标范围内）
                if (in_target_range && lines_found > target_start_line && lines_found <= target_end_line) {
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
                    __real_printf("%c", buffer[i]);
#else
                    printf("%c", buffer[i]);
#endif
                }
            }
            
            read_pos += actual_read;
        }
        
        // 如果读取完所有数据但没有遇到换行符，且有内容，算作一行
        if (read_pos >= total_length && chars_in_current_line > 0) {
            lines_found++;
            if (lines_found > target_start_line && lines_found <= target_end_line) {
                lines_output++;
            }
        }
        
        // 如果没有找到任何内容，输出提示
        if (lines_output == 0) {
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
            __real_printf("Page not found\n");
#else
            printf("Page not found\n");
#endif
        }

        return JS_UNDEFINED;
    }

    /**
     * 从日志区尾部分页显示日志内容
     * 
     * 从日志分区的末尾开始，按行分页显示日志内容。每行最多1024字符，超出按新行计算。
     * 直接输出到控制台，不返回JavaScript变量。
     * 
     * 使用示例：
     * ```js
     * // 显示最后一页，每页20行
     * logger.tail();
     * 
     * // 显示倒数第2页，每页10行
     * logger.tail(1, 10);
     * ```
     * 
     * @module logger
     * @function tail
     * @param page:number=0 页码（从0开始，0表示最后一页）
     * @param linesPerPage number=20 每页显示的行数
     * @return undefined
     */
    JSValue Logger::tail(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        int64_t page = 0;
        int64_t lines_per_page = 20;

#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
        ScopedCapture capture; // 自动暂停和恢复日志捕获
#endif
        // 解析第一个参数：页码
        if (argc >= 1 && !JS_IsUndefined(argv[0])) {
            if (JS_ToInt64(ctx, &page, argv[0]) < 0) {
                JSTHROW("page must be a number");
            }
            if (page < 0) {
                JSTHROW("page must be non-negative");
            }
        }

        // 解析第二个参数：每页行数
        if (argc >= 2 && !JS_IsUndefined(argv[1])) {
            if (JS_ToInt64(ctx, &lines_per_page, argv[1]) < 0) {
                JSTHROW("linesPerPage must be a number");
            }
            if (lines_per_page <= 0) {
                JSTHROW("linesPerPage must be positive");
            }
        }

        // 获取日志总长度
        size_t total_length = length();
        if (total_length == 0) {
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
            __real_printf("No log data available\n");
#endif
            return JS_UNDEFINED;
        }

        // 先从头扫描，计算总行数
        size_t total_lines = 0;
        size_t scan_pos = 0;
        size_t chars_in_current_line = 0;
        uint8_t buffer[4096];
        
        // 扫描整个日志，计算总行数
        while (scan_pos < total_length) {
            size_t chunk_size = (total_length - scan_pos > sizeof(buffer)) ? sizeof(buffer) : (total_length - scan_pos);
            size_t actual_read = log_read(scan_pos, chunk_size, buffer);
            
            if (actual_read == 0) {
                break;
            }
            
            for (size_t i = 0; i < actual_read; i++) {
                if (buffer[i] == '\n') {
                    total_lines++;
                    chars_in_current_line = 0;
                } else {
                    chars_in_current_line++;
                    if (chars_in_current_line >= 1024) {
                        total_lines++;
                        chars_in_current_line = 0;
                    }
                }
            }
            scan_pos += actual_read;
        }
        
        // 如果最后一行有内容但没有换行符，算作一行
        if (chars_in_current_line > 0) {
            total_lines++;
        }
        
        if (total_lines == 0) {
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
            __real_printf("No lines found in log\n");
            return JS_UNDEFINED ;
#else
            printf("No lines found in log\n");
            return JS_UNDEFINED;
#endif
        }
        
        // 计算要显示的行范围（从后往前）
        size_t target_lines = (size_t)lines_per_page;
        size_t skip_pages = (size_t)page;
        
        if (skip_pages * target_lines >= total_lines) {
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
            __real_printf("Page not found\n");
            return JS_UNDEFINED ;
#else
            printf("Page not found\n");
            return JS_UNDEFINED;
#endif
        }
        
        // 计算目标行的起始和结束位置
        size_t lines_to_skip_from_end = skip_pages * target_lines;
        size_t target_end_line = total_lines - lines_to_skip_from_end;
        size_t target_start_line = (target_end_line > target_lines) ? (target_end_line - target_lines) : 0;
        
        // 重新扫描，找到目标行并输出
        size_t current_line = 0;
        size_t read_pos = 0;
        chars_in_current_line = 0;
        bool in_target_range = false;
        
        while (read_pos < total_length) {
            size_t chunk_size = (total_length - read_pos > sizeof(buffer)) ? sizeof(buffer) : (total_length - read_pos);
            size_t actual_read = log_read(read_pos, chunk_size, buffer);
            
            if (actual_read == 0) {
                break;
            }
            
            for (size_t i = 0; i < actual_read; i++) {
                // 检查是否在目标范围内
                if (current_line >= target_start_line && current_line < target_end_line) {
                    if (!in_target_range) {
                        in_target_range = true;
                    }
#ifdef CONFIG_BESHELL_LOGGER_ENABLE_WRAP
                    __real_printf("%c", buffer[i]);
#else
                    printf("%c", buffer[i]);
#endif
                }
                
                // 检查是否需要换行
                bool should_newline = false;
                if (buffer[i] == '\n') {
                    should_newline = true;
                    chars_in_current_line = 0;
                } else {
                    chars_in_current_line++;
                    if (chars_in_current_line >= 1024) {
                        should_newline = true;
                        chars_in_current_line = 0;
                    }
                }
                
                if (should_newline) {
                    current_line++;
                    if (current_line >= target_end_line) {
                        return JS_UNDEFINED ;
                    }
                }
            }
            read_pos += actual_read;
        }
        
        // 如果读取完所有数据但没有遇到换行符，且有内容，算作一行
        if (read_pos >= total_length && chars_in_current_line > 0) {
            current_line++;
        }
        return JS_UNDEFINED;
    }

    
    /**
     * 设置日志分区的写入偏移位置
     * 
     * 设置日志数据在分区中的写入位置偏移。这可以用于从指定位置开始写入日志数据。
     * 注意：不当的偏移设置可能会覆盖现有的日志数据或破坏日志格式。
     * 
     * 使用示例：
     * ```js
     * // 设置写入偏移到1024字节位置
     * logger.setWriteOffset(1024);
     * 
     * // 从头开始写入
     * logger.setWriteOffset(0);
     * ```
     * 
     * @module logger
     * @function setWriteOffset
     * @param offset:number 写入偏移位置（字节数）
     * @return undefined
     */
        JSValue Logger::setWriteOffset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            if (argc < 1) {
                JSTHROW("setWriteOffset requires an offset argument");
            }

            // 解析offset参数
            int64_t offset;
            if (JS_ToInt64(ctx, &offset, argv[0]) < 0) {
                JSTHROW("offset must be a number");
            }

            if (offset < 0) {
                JSTHROW("offset must be non-negative");
            }

            // 调用底层函数设置写入偏移
            log_set_write_offset((size_t)offset);
            
            return JS_UNDEFINED;
        }

    /**
     * 获取当前的写入偏移位置
     * 
     * 返回当前日志数据在分区中的写入位置偏移。这可以用于了解当前的写入位置。
     * 由于底层实现限制，此函数返回当前日志数据的总长度作为近似的写入偏移。
     * 
     * 使用示例：
     * ```js
     * // 获取当前写入偏移
     * const offset = logger.getWriteOffset();
     * console.log(`Current write offset: ${offset}`);
     * ```
     * 
     * @module logger
     * @function getWriteOffset
     * @return number 当前写入偏移位置（字节数）
     */
        JSValue Logger::getWriteOffset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            // 由于底层没有提供获取偏移的函数，我们需要添加一个
            // 这里暂时返回当前长度作为近似的写入偏移
            size_t current_length = length();
            return JS_NewInt64(ctx, (int64_t)current_length);
        }

    /**
     * 使用指定的数据填充日志分区
     * 
     * 使用指定的字符或字节值填充整个日志分区。这通常用于初始化或清除分区数据。
     * 支持字符串（使用第一个字符）或数字（取低8位作为字节值）作为填充数据。
     * 
     * 使用示例：
     * ```js
     * // 使用0填充分区
     * logger.fill(0);
     * 
     * // 使用字符'A'填充分区
     * logger.fill('A');
     * 
     * // 使用字符串的首字符填充分区
     * logger.fill("Hello"); // 使用'H'填充
     * 
     * // 使用0xFF填充分区
     * logger.fill(255);
     * ```
     * 
     * @module logger
     * @function fill
     * @param data:string|number 填充数据，字符串取首字符，数字取低8位
     * @return undefined
     */
        JSValue Logger::fill(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
            if (argc < 1) {
                JSTHROW("fill requires a data argument");
            }

            uint8_t fill_data = 0x00; // 默认填充0

            // 处理不同类型的输入
            if (JS_IsString(argv[0])) {
                size_t str_len;
                const char* str = JS_ToCStringLen(ctx, &str_len, argv[0]);
                if (!str) {
                    JSTHROW("Failed to convert string data");
                }
                
                if (str_len > 0) {
                    fill_data = (uint8_t)str[0]; // 使用字符串的第一个字符
                }
                
                JS_FreeCString(ctx, str);
            }
            else if (JS_IsNumber(argv[0])) {
                int64_t num;
                if (JS_ToInt64(ctx, &num, argv[0]) >= 0) {
                    fill_data = (uint8_t)(num & 0xFF); // 取低8位
                }
            }
            else {
                JSTHROW("fill data must be a string or number");
            }

            // 调用底层函数进行填充
            log_fill((char)fill_data);
            
            return JS_UNDEFINED;
        }
}

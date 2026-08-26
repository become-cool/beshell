#include "sdkconfig.h"
#include "soc/soc_caps.h"

#if SOC_USB_SERIAL_JTAG_SUPPORTED

#include "CDC.hpp"
#include "../../ModuleLoader.hpp"
#include "driver/usb_serial_jtag.h"
#include "esp_err.h"

namespace be {

    /**
     * CDC (USB Serial JTAG) 类
     * 
     * 用于通过 USB Serial JTAG 接口进行通信。CDC 是 ESP32 芯片内置的 USB 串口功能，
     * 无需额外的 USB 转串口芯片，直接通过 USB 线连接电脑即可进行通信。
     * 
     * **注意**：CDC 是一个独立的模块，不是通过 serial 模块访问的。
     * 
     * 示例：
     * ```javascript
     * import * as cdc from "cdc"
     * 
     * // 初始化 CDC
     * cdc.setup()
     * 
     * // 或者指定缓冲区大小
     * cdc.setup(256, 256)  // rx_buffer_size, tx_buffer_size
     * 
     * // 监听接收数据
     * cdc.on("data", (data) => {
     *     console.log("Received:", data)
     *     // 将 ArrayBuffer 转换为字符串
     *     const text = new TextDecoder().decode(data)
     *     console.log("Text:", text)
     * })
     * 
     * // 发送数据
     * cdc.write("Hello CDC!")
     * 
     * // 发送二进制数据
     * const buffer = new Uint8Array([0x01, 0x02, 0x03]).buffer
     * cdc.write(buffer)
     * ```
     * 
     * @module serial
     * @class CDC
     * @event data 当接收到数据时触发
     * @param data:ArrayBuffer 接收到的数据
     */

    char const * const CDC::name = "cdc" ;
    bool CDC::setuped = false ;

    typedef struct {
        int len ;
        uint8_t * buf ;
    } cdc_event_t ;

    CDC::CDC(JSContext * ctx, const char * name)
        : EventModule(ctx, name, 0)
    {
        exportFunction("setup",setup,0) ;
        exportFunction("write",write,0) ;
    }

    void CDC::exports(JSContext *ctx) {
        EventModule::exports(ctx) ;
        enableNativeEvent(ctx, sizeof(cdc_event_t), 10) ;
    }

    void CDC::taskListen(CDC* cdc) {

        assert(cdc) ;

        uint8_t buf[128];
        cdc_event_t event ;
        while (1) {
            event.len = usb_serial_jtag_read_bytes(buf, sizeof(buf), pdMS_TO_TICKS(1000));
            if (event.len > 0) {
                event.buf = (uint8_t*)malloc(event.len);
                if (event.buf) {
                    // Copy data after the length
                    memcpy((char*)event.buf, buf, event.len);
                    
                    // Emit event with the data - free memory if emission fails
                    if (!cdc->emitNativeEvent((void *)&event)) {
                        free(event.buf);
                    }
                }
            } else if(event.len<0) {
                // 读取错误（如驱动未初始化）
                printf("Read error: %d\n", event.len);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
        }
    }

    void CDC::onNativeEvent(JSContext *ctx, void * param) {
        if(!param) {
            return;
        }
        emitSyncFree("data", {JS_NewArrayBuffer(ctx, ((cdc_event_t*)param)->buf, ((cdc_event_t*)param)->len, freeArrayBuffer, NULL, false)}) ;
    }

    /**
     * 初始化并配置 USB Serial JTAG CDC 驱动
     * 
     * 初始化 CDC 驱动，配置接收和发送缓冲区大小。
     * 
     * 示例：
     * ```javascript
     * import * as cdc from "cdc"
     * 
     * // 使用默认缓冲区大小（256字节）
     * cdc.setup()
     * 
     * // 指定缓冲区大小
     * cdc.setup(512, 512)  // 接收缓冲区 512 字节，发送缓冲区 512 字节
     * ```
     *
     * @module serial
     * @class CDC
     * @function setup
     * @param rx_buffer_size:number=256 接收缓冲区大小（可选，默认 256）
     * @param tx_buffer_size:number=256 发送缓冲区大小（可选，默认 256）
     * @return undefined
     * @throws CDC 已初始化
     * @throws 驱动安装失败
     */
    JSValue CDC::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

        if(setuped) {
            JSTHROW("CDC already setup") ;
        }

        NativeModule * nmodule = ModuleLoader::moduleByName(ctx, CDC::name) ;
        if(!nmodule) {
            JSTHROW("CDC module not found") ;
        }

        // Default parameters
        uint32_t rx_buffer_size = 256;
        uint32_t tx_buffer_size = 256;
        
        // Parse optional parameters
        if (argc >= 1 && JS_IsNumber(argv[0])) {
            JS_ToUint32(ctx, &rx_buffer_size, argv[0]);
        }
        
        if (argc >= 2 && JS_IsNumber(argv[1])) {
            JS_ToUint32(ctx, &tx_buffer_size, argv[1]);
        }
        
        // Create configuration structure
        usb_serial_jtag_driver_config_t config = {
            .tx_buffer_size = tx_buffer_size,
            .rx_buffer_size = rx_buffer_size
        };
        
        // Call the driver installation function with config struct
        esp_err_t err = usb_serial_jtag_driver_install(&config);
        
        // Throw exception if error occurred
        if (err != ESP_OK) {
            JSTHROW("USB Serial JTAG driver installation failed: %d", err);
        }
    
        // 创建持续运行的 USB 任务
        xTaskCreatePinnedToCore((TaskFunction_t)taskListen, "usb_cdc_task", 4096, (void *)nmodule, 5, NULL, 1);
        
        setuped = true ;
        return JS_UNDEFINED;
    }
    
    /**
     * 向 USB Serial JTAG 写入数据
     * 
     * 发送字符串或二进制数据到 USB Serial JTAG 接口。
     * 
     * 示例：
     * ```javascript
     * import * as cdc from "cdc"
     * 
     * // 发送字符串
     * cdc.write("Hello World!")
     * 
     * // 发送二进制数据
     * const buffer = new Uint8Array([0x01, 0x02, 0x03, 0x04]).buffer
     * cdc.write(buffer)
     * 
     * // 检查实际写入的字节数
     * const bytesWritten = cdc.write("Test data")
     * console.log("Bytes written:", bytesWritten)
     * ```
     *
     * @module serial
     * @class CDC
     * @function write
     * @param data:string|ArrayBuffer 要写入的数据
     * @return number 实际写入的字节数
     * @throws 参数类型无效
     * @throws 写入失败
     */
    JSValue CDC::write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        if (argc < 1) {
            return JS_ThrowTypeError(ctx, "Missing argument: data (string or ArrayBuffer)");
        }
        
        size_t data_len = 0;
        const void *data_ptr = NULL;
        bool need_free = false;
        
        // Handle string input
        if (JS_IsString(argv[0])) {
            data_ptr = JS_ToCStringLen(ctx, &data_len, argv[0]);
            need_free = true;
        } 
        // Handle ArrayBuffer input
        else if (JS_IsArrayBuffer(argv[0])) {
            JSValue byte_length_val = JS_GetPropertyStr(ctx, argv[0], "byteLength");
            uint64_t length64 = 0;  // Use uint64_t to match JS_ToIndex signature
            
            JS_ToIndex(ctx, &length64, byte_length_val);
            data_len = (size_t)length64;  // Convert to size_t
            JS_FreeValue(ctx, byte_length_val);
            
            // Get the array buffer data - updated to match API
            data_ptr = JS_GetArrayBuffer(ctx, &data_len, argv[0]);
        } else {
            return JS_ThrowTypeError(ctx, "Expected string or ArrayBuffer");
        }
        
        if (!data_ptr || data_len == 0) {
            if (need_free) {
                JS_FreeCString(ctx, (const char *)data_ptr);
            }
            return JS_NewInt32(ctx, 0);
        }
        
        // Write data to USB Serial JTAG
        int bytes_written = usb_serial_jtag_write_bytes(data_ptr, data_len, 0);
        
        if (need_free) {
            JS_FreeCString(ctx, (const char *)data_ptr);
        }
        
        if (bytes_written < 0) {
            return JS_ThrowInternalError(ctx, "Failed to write to USB Serial JTAG: %d", bytes_written);
        }
        
        return JS_NewInt32(ctx, bytes_written);
    }
}
#endif

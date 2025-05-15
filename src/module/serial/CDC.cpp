#include "CDC.hpp"
#include "driver/usb_serial_jtag.h"
#include "esp_err.h"

namespace be {

    char const * const CDC::name = "cdc" ;

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

    JSValue CDC::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

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
        
        return JS_UNDEFINED;
    }
    
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
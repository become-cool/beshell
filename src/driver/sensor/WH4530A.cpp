#include "WH4530A.hpp"
#include <iostream>

// System registers
//---------------------------------------------------------	
#define WH450A_SYSM_CTRL               0x00
#define WH450A_INT_FLAG                0x02
#define WH450A_WAIT_TIME               0x03
//---------------------------------------------------------
// PS registers
//---------------------------------------------------------	
#define WH450A_PS_LED                  0x06
#define WH450A_PS_GAIN                 0x07
#define WH450A_PS_PULSE                0x08
#define WH450A_PS_TIME                 0x09
#define WH450A_PS_FILTER               0x0A
#define WH450A_PS_PERSIS               0x0B
#define WH450A_PS_THRE_LL              0x10
#define WH450A_PS_THRE_LH              0x11
#define WH450A_PS_THRE_HL              0x12
#define WH450A_PS_THRE_HH              0x13
#define WH450A_PS_DATA_L               0x18
#define WH450A_PS_DATA_H               0x19

using namespace std;
using namespace be;

namespace be::driver::sensor {
    DEFINE_NCLASS_META(WH4530A, I2CDevice)

    std::vector<JSCFunctionListEntry> WH4530A::methods = {
            JS_CFUNC_DEF("read", 0, WH4530A::read),
    };

    WH4530A::WH4530A(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj), 0x38) {
    }

    JSValue WH4530A::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new WH4530A(ctx);
        obj->shared() ;
        return obj->jsobj;
    }

    int WH4530A::setup() {
        if(addr==0){
            return -1 ;
        }

        i2c->write<uint8_t,uint8_t>(addr, WH450A_SYSM_CTRL,      0x80);             // soft reset
        vTaskDelay(5 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_WAIT_TIME,      0x0F);             // PS wait time
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_LED,         0xFF);             // LED driving current, pulse width
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_GAIN,        0x03);             // PS gain
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_PULSE,       0x00);             // PS pulse count
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_TIME,        0x0F);             // PS integration time
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_FILTER,      0x0F);             // PS low pass filter
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_PERSIS,      0x0);             // PS persistance
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_THRE_LL,     0xFF);                // PS low threshold low byte
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_THRE_LH,     0x0F);                // PS low threshold high byte
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_THRE_HL,     0xF4);              // PS high threshold low byte
        i2c->write<uint8_t,uint8_t>(addr, WH450A_PS_THRE_HH,     0x01);              // PS high threshold high byte
        vTaskDelay(1 / portTICK_PERIOD_MS);
        i2c->write<uint8_t,uint8_t>(addr, WH450A_SYSM_CTRL,      0x02);             // PS enable
        vTaskDelay(5 / portTICK_PERIOD_MS);
    
        return 0 ;
    }

    int WH4530A::read() {
        if(!i2c || !addr) {
            return -1 ;
        }
        uint8_t byteH = 0 ;
        uint8_t byteL = 0 ;
        i2c->read<uint8_t,uint8_t>(addr, WH450A_PS_DATA_L, byteL) ;
        i2c->read<uint8_t,uint8_t>(addr, WH450A_PS_DATA_H, byteH) ;

        return (byteH << 8) + byteL;
    }

    JSValue WH4530A::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(WH4530A, thisobj)
        int ret = thisobj->read() ;
        if( ret<0 ) {
            JSTHROW("%s.%s() failed, error: %d", "WH4530A", "read", ret)
        }
        return JS_NewUint32(ctx,(uint32_t)ret) ;
    }
}
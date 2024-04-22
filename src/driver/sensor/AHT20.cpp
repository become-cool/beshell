#include "AHT20.hpp"
#include <iostream>

#ifndef BIT
#define BIT(nr)                 (1UL << (nr))
#endif
#define AT581X_STATUS_Calibration_Enable    (3)             /* 1 --Calibration enable; 0 --Calibration disable */
#define AT581X_STATUS_CRC_FLAG              (4)
#define AT581X_STATUS_BUSY_INDICATION       (7)
using namespace std;
using namespace be;

namespace be::driver::sensor {
    DEFINE_NCLASS_META(AHT20, I2CDevice)

    std::vector<JSCFunctionListEntry> AHT20::methods = {
            JS_CFUNC_DEF("setup", 0, I2CDevice::setup),
            JS_CFUNC_DEF("read", 0, AHT20::read),
            JS_CFUNC_DEF("measure", 0, AHT20::measure),
    };

    AHT20::AHT20(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj)) {
    }

    JSValue AHT20::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new AHT20(ctx);
        return obj->jsobj;
    }

    bool AHT20::measure() {
        if(!i2c || !addr) {
            return false ;
        }
        uint8_t buf[] = {0xAC, 0x33, 0x00};
        return i2c->send(addr, buf, sizeof(buf));
    }

    int AHT20::read(float *humidity,float *temperature) {
        if(!i2c || !addr) {
            return -1 ;
        }
        
        uint8_t data[7];
        if (!i2c->recv(addr, data,7 )) {
            return -2 ;
        }
        
        if (!(data[0] & BIT(AT581X_STATUS_Calibration_Enable)) ||
            !(data[0] & BIT(AT581X_STATUS_CRC_FLAG)) ||
            ((data[0] & BIT(AT581X_STATUS_BUSY_INDICATION)) != 0))
        {
            return -3 ;
        }
        
        // if (aht20_calc_crc(data, 6) != buf[6]) {
        //  return -4 ;
        // }

        uint32_t rh = ( ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | (data[3]) ) >> 4 ;
        uint32_t temp = ((uint32_t)(data[3]&0x0F) << 16) | ((uint32_t)data[4] << 8) | (uint32_t)data[5] ;

        *temperature = temp*0.00019073F-50;
        *humidity = rh*0.0000953674316F;

        return 0;
    }

    uint8_t AHT20::aht20_calc_crc(uint8_t *data, uint8_t len) {
        uint8_t i;
        uint8_t byte;
        uint8_t crc = 0xFF;

        for (byte = 0; byte < len; byte++) {
            crc ^= data[byte];
            for (i = 8; i > 0; --i) {
                if ((crc & 0x80) != 0) {
                    crc = (crc << 1) ^ 0x31;
                } else {
                    crc = crc << 1;
                }
            }
        }

        return crc;
    }
    
    JSValue AHT20::measure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AHT20, thisobj)
        return thisobj->measure()? JS_TRUE: JS_FALSE ;
    }

    JSValue AHT20::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(AHT20, thisobj)
        float temperature, humidity ;
        int ret = thisobj->read(&humidity,&temperature) ;
        if( ret!=0 ) {
            JSTHROW("%s.%s() failed, error: %d", "AHT20", "read", ret)
        }
        JSValue value = JS_NewArray(ctx) ;
        JS_SetPropertyUint32(ctx,value,0,JS_NewFloat64(ctx,humidity));
        JS_SetPropertyUint32(ctx,value,1,JS_NewFloat64(ctx,temperature));
        return value ;
    }
}
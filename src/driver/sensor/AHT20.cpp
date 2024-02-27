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

namespace be::driver {
    DEFINE_NCLASS_META(AHT20, NativeClass)

    std::vector<JSCFunctionListEntry> AHT20::methods = {
            JS_CFUNC_DEF("begin", 0, AHT20::begin),
            JS_CFUNC_DEF("read", 0, AHT20::read),
    };

    AHT20::AHT20(JSContext *ctx, JSValue _jsobj)
            : NativeClass(ctx, build(ctx, _jsobj)) {
    }

    JSValue AHT20::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new AHT20(ctx);
        return obj->jsobj;
    }
    
    int AHT20::begin(I2C * _i2c, uint8_t _addr) {
        if(!_i2c) {
            return -1 ;
        }
        i2c = _i2c ;
        addr = _addr ;
        return 0 ;
    }

    bool AHT20::triggerMeasurement() {
        if(!i2c || !addr) {
            return false ;
        }
        uint8_t buf[] = {0x33, 0x00};
        return i2c->write<u_int8_t, u_int8_t *>(addr, 0xAC, buf);
    }

    int AHT20::read(float *humidity,float *temperature) {
        if(!i2c || !addr) {
            return -1 ;
        }
        
        uint8_t data[7];
        if (!i2c->recv(addr, data,7 )) {
            return -2;
        }
        if (!(data[0] & BIT(AT581X_STATUS_Calibration_Enable)) ||
            !(data[0] & BIT(AT581X_STATUS_CRC_FLAG)) ||
            ((data[0] & BIT(AT581X_STATUS_BUSY_INDICATION)) != 0))
        {
            return -3 ;
        }
        
        // if (aht20_calc_crc(data, 6) != buf[6]) {
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
    
    JSValue AHT20::begin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(AHT20, thisobj)
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, busnum)
        ARGV_TO_UINT8_OPT(1, addr, 0x38)
        I2C * i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
        if(!i2c) {
            JSTHROW("invalid i2c port number:%d", busnum)
        }
        int ret = thisobj->begin(i2c, addr) ;
        if( ret!=0 ){
            JSTHROW("%s.%s() failed, error: %d", "AHT20", "begin", ret)
        }
        return JS_UNDEFINED ;
    }

    JSValue AHT20:: read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(AHT20, thisobj)
        float temperature, humidity ;
        int ret = thisobj->read(&temperature,&humidity) ;
        if( ret!=0 ) {
            JSTHROW("%s.%s() failed, error: %d", "AHT20", "read", ret)
        }
        JSValue value = JS_NewArray(ctx) ;
        JS_SetPropertyUint32(ctx,value,0,JS_NewFloat64(ctx,temperature));
        JS_SetPropertyUint32(ctx,value,1,JS_NewFloat64(ctx,humidity));
        return value ;
    }
    

    void AHT20::provider(DriverModule *dm) {
        dm->exportClass<AHT20>();
    }
    void AHT20::use() {
        DriverModule::providers.push_back(provider);
    }
}
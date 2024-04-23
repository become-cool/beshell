#include "DS1307.hpp"
#include <iostream>


// DS1307 I2C address
#define EEPROM_I2C_ADDR   uint8_t(0x50)   ///< EEPROM I2C address
#define DS1307_I2C_ADDR   uint8_t(0x68)   ///< DS1307 I2C address
#define DS1307_BASE_YR            2000    ///< Base year is 2000

// DS1307 register address
#define DS1307_SEC_REG    uint8_t(0x00)   ///< DS1307 Seconds register address
#define DS1307_MIN_REG    uint8_t(0x01)   ///< DS1307 Minutes register address
#define DS1307_HR_REG     uint8_t(0x02)   ///< DS1307 Hour register address
#define DS1307_DOW_REG    uint8_t(0x03)   ///< DS1307 DAY register address
#define DS1307_DATE_REG   uint8_t(0x04)   ///< DS1307 Date register address
#define DS1307_MTH_REG    uint8_t(0x05)   ///< DS1307 Month register address
#define DS1307_YR_REG     uint8_t(0x06)   ///< DS1307 Year register address
#define DS1307_CTL_REG    uint8_t(0x07)   ///< DS1307 Control register address
#define DS1307_RAM_REG    uint8_t(0x08)   ///< DS1307 RAM register start address(08h–3Fh), size: 56 x 8bits

// Define register bit masks
#define DS1307_CLOCKHALT B10000000

#define DS1307_LO_BCD    B00001111
#define DS1307_HI_BCD    B11110000

#define DS1307_SEC_MASK    B01111111
#define DS1307_MIN_MASK    B01111111
#define DS1307_HR_MASK     B00111111
#define DS1307_DOW_MASK    B00000111
#define DS1307_DATE_MASK   B00111111
#define DS1307_MTH_MASK    B00011111
#define DS1307_YR_MASK     B11111111



using namespace std;
using namespace be;

namespace be::driver::sys {
    DEFINE_NCLASS_META(DS1307, I2CDevice)

    std::vector<JSCFunctionListEntry> DS1307::methods = {
            JS_CFUNC_DEF("read", 0, DS1307::read),
            JS_CFUNC_DEF("set", 0, DS1307::set),
    };

    DS1307::DS1307(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj)) {
    }

    JSValue DS1307::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new DS1307(ctx);
        obj->shared() ;
        return obj->jsobj;
    }

    static int tonumber(uint8_t & bcd) {
        bcd = (bcd & 0x0F) + ((bcd >> 4) * 10);
        return bcd ;
    }
    static uint8_t tobcd(int num) {
        return (num / 10) << 4 | (num % 10);
    }
    
    bool DS1307::read(int & sec, int & min, int & hour, int & day, int & date, int & mth, int & year) {
        if(!i2c) {
            return false ;
        }
        uint8_t _sec = 0 ;
        uint8_t _min = 0 ;
        uint8_t _hr = 0 ;
        uint8_t _dow = 0 ;
        uint8_t _date = 0 ;
        uint8_t _mth = 0 ;
        uint8_t _yr = 0 ;
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_SEC_REG, _sec)){
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_MIN_REG, _min)){
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_HR_REG, _hr)){
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_DOW_REG, _dow)){
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_DATE_REG, _date)){
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_MTH_REG, _mth)){
            return false ;
        }
        if(!i2c->read<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_YR_REG, _yr)){
            return false ;
        }

        sec = tonumber(_sec);
        min = tonumber(_min);
        hour = tonumber(_hr);
        day = tonumber(_dow);
        date = tonumber(_date);
        mth = tonumber(_mth);
        year = tonumber(_yr) + DS1307_BASE_YR;
        return true ;
    }

    JSValue DS1307::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(DS1307, thisobj)
        int sec = 0 ;
        int min = 0 ;
        int hr = 0 ;
        int dow = 0 ;    
        int date = 0 ;
        int mth = 0 ;
        int yr = 0 ;
        if( !thisobj->read(sec, min, hr, dow, date, mth, yr) ) {
            return JS_NULL ;
        }
        JSValue array = JS_NewArray(ctx);
        JS_SetPropertyUint32(ctx, array, 0, JS_NewInt32(ctx, sec));
        JS_SetPropertyUint32(ctx, array, 1, JS_NewInt32(ctx, min));
        JS_SetPropertyUint32(ctx, array, 2, JS_NewInt32(ctx, hr));
        JS_SetPropertyUint32(ctx, array, 3, JS_NewInt32(ctx, dow));
        JS_SetPropertyUint32(ctx, array, 4, JS_NewInt32(ctx, date));
        JS_SetPropertyUint32(ctx, array, 5, JS_NewInt32(ctx, mth));
        JS_SetPropertyUint32(ctx, array, 6, JS_NewInt32(ctx, yr));
        return array ;
    }

    bool DS1307::set(int sec, int min, int hour, int day, int date, int mth, int year) {
        if(!i2c) {
            return false ;
        }
        if(sec>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_SEC_REG, tobcd(sec)) ) {
                return false ;
            }
        }
        if(min>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_MIN_REG, tobcd(min)) ) {
                return false ;
            }
        }
        if(hour>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_HR_REG, tobcd(hour)) ) {
                return false ;
            }
        }
        if(day>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_DOW_REG, tobcd(day)) ) {
                return false ;
            }
        }
        if(date>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_DATE_REG, tobcd(date)) ) {
                return false ;
            }
        }
        if(mth>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_MTH_REG, tobcd(mth)) ) {
                return false ;
            }
        }
        if(year>=0){
            if( !i2c->write<uint8_t,uint8_t>(DS1307_I2C_ADDR, DS1307_YR_REG, tobcd(year-DS1307_BASE_YR)) ) {
                return false ;
            }
        }
        return true ;
    }

    
    JSValue DS1307::set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(DS1307, thisobj)
        ARGV_TO_INT8_OPT(0, sec, -1)
        ARGV_TO_INT8_OPT(1, min, -1)
        ARGV_TO_INT8_OPT(2, hr, -1)
        ARGV_TO_INT8_OPT(3, dow, -1)
        ARGV_TO_INT8_OPT(4, date, -1)
        ARGV_TO_INT8_OPT(5, mth, -1)
        ARGV_TO_INT8_OPT(6, yr, -1)
        return thisobj->set(sec, min, hr, dow, date, mth, yr) ? JS_TRUE : JS_FALSE ;
    }
}
#include "BMP280.hpp"

using namespace std ;
using namespace be ;

namespace be::driver {


    DEFINE_NCLASS_META(BMP280, NativeClass)
    std::vector<JSCFunctionListEntry> BMP280::methods = {
        JS_CFUNC_DEF("begin", 0, BMP280::begin),
        JS_CFUNC_DEF("readTemperature", 0, BMP280::readTemperature),
        JS_CFUNC_DEF("readPressure", 0, BMP280::readPressure),
        JS_CFUNC_DEF("readAltitude", 0, BMP280::readAltitude),
    } ;

    BMP280::BMP280(JSContext * ctx, JSValue _jsobj)
        : NativeClass(ctx,build(ctx,_jsobj))
    {}

    JSValue BMP280::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new BMP280(ctx) ;
        obj->shared() ;
        return obj->jsobj ;
    }
    
    #define READ_REG(reg, var, vartype, initvalue, errorcode)   \
        var = initvalue ;                                       \
        if(i2c->read<uint8_t,vartype>(addr, reg, var)) {        \
            return errorcode ;                                  \
        }
    #define READ_CAL(reg, var)                                  \
        var = 0 ;                                               \
        if(!i2c->read<uint8_t,uint16_t>(addr, reg, var)) {      \
            return -7 ;                                         \
        }

    int BMP280::begin(I2C * _i2c, uint8_t _addr) {
        if(!_i2c) {
            return -1 ;
        }
        i2c = _i2c ;
        addr = _addr ;

        uint8_t id = 0 ;
        if( !i2c->read<uint8_t,uint8_t>(addr, 0xd0, id) ){
            return -2 ;
        }
        if( id!=0x58 ) {
            return -3 ;
        }

        // clear all status
        if( !i2c->write(addr, 0xe0, 0xb6) ){
            return -4 ;
        }

        // Normal Mode(20Bit)
        if( !i2c->write<uint8_t,uint8_t>(addr, 0xf4, 0xff) ){
            return -5 ;
        }

        // Filter (0.5ms)
        if( !i2c->write<uint8_t,uint8_t>(addr, 0xf5, 0x00) ){
            return -6 ;
        }

        READ_CAL(0x88, dig_T1)
        READ_CAL(0x8A, dig_T2)
        READ_CAL(0x8C, dig_T3)
        READ_CAL(0x8E, dig_P1)
        READ_CAL(0x90, dig_P2)
        READ_CAL(0x92, dig_P3)
        READ_CAL(0x94, dig_P4)
        READ_CAL(0x96, dig_P5)
        READ_CAL(0x98, dig_P6)
        READ_CAL(0x9A, dig_P7)
        READ_CAL(0x9C, dig_P8)
        READ_CAL(0x9E, dig_P9)

        // dn3(dig_T1, dig_T2, dig_T3)
        // dn3(dig_P1, dig_P2, dig_P3)
        // dn3(dig_P4, dig_P5, dig_P6)
        // dn3(dig_P7, dig_P8, dig_P9)

        return 0 ;
    }
    
    int BMP280::readTemperature(double & value) {
        if(!i2c){
            return -1 ;
        }
        uint8_t buff[3] ;
        if(!i2c->read<uint8_t>(addr,0xFA,buff,sizeof(buff))) {
            return -2 ;
        }
        uint32_t tmp = (((uint32_t)buff[0] << 12)|((uint32_t)buff[1] << 4)|((uint32_t)buff[2] >> 4));

        //Temperature
        double var1 = (((double)tmp)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
        double var2 = ((((double)tmp)/131072.0-((double)dig_T1)/8192.0)*(((double)tmp)
                    /131072.0-((double)dig_T1)/8192.0))*((double)dig_T3);

        uint32_t t_fine = (unsigned long)(var1+var2);

        value = (var1+var2)/5120.0;

        return 0 ;
    }
    int BMP280::readPressure(double & value) {
        if(!i2c){
            return -1 ;
        }
        uint8_t buff[3] ;
        if(!i2c->read<uint8_t>(addr,0xFA,buff,sizeof(buff))) {
            return -2 ;
        }
        uint32_t tmp = (((uint32_t)buff[0] << 12)|((uint32_t)buff[1] << 4)|((uint32_t)buff[2] >> 4));

        if(!i2c->read<uint8_t>(addr,0xF7,buff,sizeof(buff))) {
            return -3 ;
        }
        uint32_t prs = (((uint32_t)buff[0] << 12)|((uint32_t)buff[1] << 4)|((uint32_t)buff[2] >> 4));

        //Temperature
        double var1 = (((double)tmp)/16384.0-((double)dig_T1)/1024.0)*((double)dig_T2);
        double var2 = ((((double)tmp)/131072.0-((double)dig_T1)/8192.0)*(((double)tmp)
                    /131072.0-((double)dig_T1)/8192.0))*((double)dig_T3);

        uint32_t t_fine = (unsigned long)(var1+var2);

        var1 = ((double)t_fine/2.0)-64000.0;
        var2 = var1*var1*((double)dig_P6)/32768.0;
        var2 = var2 +var1*((double)dig_P5)*2.0;
        var2 = (var2/4.0)+(((double)dig_P4)*65536.0);
        var1 = (((double)dig_P3)*var1*var1/524288.0+((double)dig_P2)*var1)/524288.0;
        var1 = (1.0+var1/32768.0)*((double)dig_P1);
        value = 1048576.0-(double)prs;
        value = (value-(var2/4096.0))*6250.0/var1;
        var1 = ((double)dig_P9)*value*value/2147483648.0;
        var2 = value*((double)dig_P8)/32768.0;
        value = value+(var1+var2+((double)dig_P7))/16.0;

        return 0 ;
    }

    JSValue BMP280::begin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(BMP280, thisobj)
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, busnum)
        ARGV_TO_UINT8_OPT(1, addr, 0x76)
        I2C * i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
        if(!i2c) {
            JSTHROW("invalid i2c port number:%d", busnum)
        }
        int ret = thisobj->begin(i2c, addr) ;
        if( ret!=0 ){
            JSTHROW("%s.%s() failed, error: %d", "BMP280", "begin", ret)
        }
        return JS_UNDEFINED ;
    }
    JSValue BMP280::readTemperature(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(BMP280, thisobj)
        double tmp = 0 ;
        int ret = thisobj->readTemperature(tmp) ;
        if( ret!=0 ) {
            JSTHROW("%s.%s failed(), error: %d", "BMP280", "readTemperature", ret)
        }
        return JS_NewFloat64(ctx,tmp) ;
    }
    JSValue BMP280::readPressure(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(BMP280, thisobj)
        double prs = 0.0 ;
        int ret = thisobj->readPressure(prs) ;
        if( ret!=0 ) {
            JSTHROW("%s.%s failed(), error: %d", "BMP280", "readPressure", ret)
        }
        return JS_NewFloat64(ctx,prs) ;
    }
    JSValue BMP280::readAltitude(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        JSTHROW("not implements")
    }

    // float BMP280::getTemperature()
    // {
    //   int32_t   raw = getTemperatureRaw();
    //   int32_t   v1, v2, T;
    //   if(lastOperateStatus == eStatusOK) {
    //     v1 = ((((raw >> 3) - ((int32_t) _sCalib.t1 << 1))) * ((int32_t) _sCalib.t2)) >> 11;
    //     v2 = (((((raw >> 4) - ((int32_t) _sCalib.t1)) * ((raw >> 4) - ((int32_t) _sCalib.t1))) >> 12) * ((int32_t) _sCalib.t3)) >> 14;
    //     _t_fine = v1 + v2;
    //     __DBG_CODE(Serial.print("t_fine: "); Serial.print(_t_fine));
    //     T = (_t_fine * 5 + 128) >> 8;
    //     return (float)(T)/100.0;
    //   }
    //   return 0;
    // }

    // uint32_t BMP280::getPressure()
    // {
    //   getTemperature();   // update _t_fine
    //   int32_t   raw = getPressureRaw();
    //   int64_t   rslt = 0;
    //   int64_t   v1, v2;
    //   if(lastOperateStatus == eStatusOK) {
    //     v1 = ((int64_t) _t_fine) - 128000;
    //     v2 = v1 * v1 * (int64_t) _sCalib.p6;
    //     v2 = v2 + ((v1 * (int64_t) _sCalib.p5) << 17);
    //     v2 = v2 + (((int64_t) _sCalib.p4) << 35);
    //     v1 = ((v1 * v1 * (int64_t) _sCalib.p3) >> 8) + ((v1 * (int64_t) _sCalib.p2) << 12);
    //     v1 = (((((int64_t) 1) << 47) + v1)) * ((int64_t) _sCalib.p1) >> 33;
    //     if(v1 == 0)
    //       return 0;
    //     rslt = 1048576 - raw;
    //     rslt = (((rslt << 31) - v2) * 3125) / v1;
    //     v1 = (((int64_t) _sCalib.p9) * (rslt >> 13) * (rslt >> 13)) >> 25;
    //     v2 = (((int64_t) _sCalib.p8) * rslt) >> 19;
    //     rslt = ((rslt + v1 + v2) >> 8) + (((int64_t) _sCalib.p7) << 4);
    //     return (uint32_t) (rslt / 256);
    //   }
    //   return 0;
    // }

    
    void BMP280::provider(DriverModule * dm) {
        dm->exportClass<BMP280>() ;
    }
    void BMP280::use() {
        DriverModule::providers.push_back(provider) ;
    }
}
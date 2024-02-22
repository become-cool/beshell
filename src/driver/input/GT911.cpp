#include "GT911.hpp"
#include <cassert>

using namespace std ;


#define GT_CTRL_REG 	(0x8040) 
#define GT_CFGS_REG 	(0x8047)  
#define GT_CHECK_REG 	(0x80FF)  

#define GT_PID_REG 		(0x8140)   
#define GT_GSTID_REG 	(0x814E)  
#define GT_TP1_REG 		(0x8150)  
#define GT_TP2_REG 		(0x8158)   
#define GT_TP3_REG 		(0x8160) 
#define GT_TP4_REG 		(0x8168)   
#define GT_TP5_REG 		(0x8170)   

namespace be::driver {

    DEFINE_NCLASS_META(GT911, NativeClass)

    std::vector<JSCFunctionListEntry> GT911::methods = {
        JS_CFUNC_DEF("setup", 0, GT911::setup),
    } ;

    GT911::GT911(JSContext * ctx, be::I2C * _i2c, uint8_t _addr)
        : NativeClass(ctx,build(ctx))
        , addr(_addr)
        , i2c(_i2c)
    {}

    JSValue GT911::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
        ARGV_TO_UINT8(0, busnum)
        ARGV_TO_UINT8_OPT(1, addr, 0x5D)
        I2C * i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
        if(!i2c) {
            JSTHROW("invalid i2c port number:%d", busnum)
        }
        auto obj = new GT911(ctx,i2c,addr) ;
        obj->self = std::shared_ptr<GT911> (obj) ;
        return obj->jsobj ;
    }

    bool GT911::reset() {
        assert(i2c) ;
        uint8_t data = 0X02 ;
        return i2c->write<uint16_t, uint8_t>(addr, GT_CTRL_REG, 0X02) ;
    }

    uint8_t GT911::readConfigVersion() {
        assert(i2c) ;
        uint8_t data = 0 ;
        i2c->read<uint16_t, uint8_t>(addr, GT_CFGS_REG, data) ;
        return data ;
    }

    JSValue GT911::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(GT911, thisobj)
        thisobj->reset() ;
        dn(thisobj->readConfigVersion()) ;
        return JS_UNDEFINED ;
    }

    void GT911::provider(DriverModule * dm) {
        dm->exportClass<GT911>() ;
    }
    void GT911::use() {
        DriverModule::providers.push_back(provider) ;
    }
}

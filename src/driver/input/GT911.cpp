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


namespace be::driver::input {

    DEFINE_NCLASS_META(GT911, InDevPointer)

    std::vector<JSCFunctionListEntry> GT911::methods = {
        JS_CFUNC_DEF("setup", 0, GT911::setup),
    } ;

    GT911::GT911(JSContext * ctx, be::I2C * i2c, uint8_t addr)
        : InDevPointer(ctx,build(ctx),5)
        , addr(addr)
        , i2c(i2c)
    {
    }

    JSValue GT911::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        ARGV_TO_UINT8_OPT(0, busnum, 255)
        ARGV_TO_UINT8_OPT(1, addr, 0x5D)
        I2C * i2c = nullptr ;
        if(busnum<255) {
            i2c = be::I2C::flyweight(ctx, (i2c_port_t)busnum) ;
            if(!i2c) {
                JSTHROW("invalid i2c port number:%d", busnum)
            }
        }
        auto obj = new GT911(ctx,i2c,addr) ;
        obj->shared() ;
        return obj->jsobj ;
    }

    bool GT911::reset() {
        if(!i2c) {
            return false ;
        }
        uint8_t data = 0X02 ;
        return i2c->write<uint16_t, uint8_t>(addr, GT_CTRL_REG, 0X02) ;
    }

    uint8_t GT911::readConfigVersion() {
        if(!i2c) {
            return 0 ;
        }
        uint8_t data = 0 ;
        i2c->read<uint16_t, uint8_t>(addr, GT_CFGS_REG, data) ;
        return data ;
    }
    
    bool GT911::dataReady() {
        if(!i2c) {
            return false ;
        }
        uint8_t data = 0;
        if(!i2c->read<uint16_t,uint8_t>(addr,GT_GSTID_REG,data)){
            return false ;
        }
        return data & 0x80 ;
    }

    bool GT911::readPos(uint8_t i, int16_t &x, int16_t &y) {
        if(!i2c) {
            return false ;
        }
        if(!this->dataReady()) {
            return false ;
        }
        uint16_t arrTpReg[] = {
            GT_TP1_REG,
            GT_TP2_REG,
            GT_TP3_REG,
            GT_TP4_REG,
            GT_TP5_REG
        } ;
        if(i>=sizeof(arrTpReg)/sizeof(uint16_t)) {
            printf("GT911::readPos() invalid arg i = %d\n",i) ;
            return false ;
        }
        uint8_t data[4] = {0} ;
        if( !i2c->read<uint16_t>(addr, arrTpReg[i], data, 4) ){
            return false ;
        }
        x = ((data[1] & 0x0f) << 8) + data[0];
        y = ((data[3] & 0x0f) << 8) + data[2];
        data[0] = 0 ;
        return i2c->write<uint16_t,uint8_t>(addr, GT_GSTID_REG, data[0]);
    }

    uint8_t GT911::readPointCount() {
        if(!i2c) {
            return 0 ;
        }
        uint8_t data = 0;
        if(!i2c->read<uint16_t,uint8_t>(addr,GT_GSTID_REG,data)){
            return 0 ;
        }
        uint8_t touchs = 0 ;
        if (data & 0x80) {
            touchs = data & 0x0F;
        }
        
        if(!touchs) {
            uint8_t temp = 0;
            i2c->write<uint16_t,uint8_t>(addr, GT_GSTID_REG, temp);
        }

        return touchs ;
    }

    JSValue GT911::setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(GT911, thisobj)
        ASSERT_ARGC(1)

        int GET_INT32_PROP_OPT(argv[0], "i2c", i2cnum, 0)
        GET_INT32_PROP_OPT(argv[1], "addr", thisobj->addr, -1)

        thisobj->i2c = be::I2C::flyweight(ctx, (i2c_port_t)i2cnum) ;
        if(!thisobj->i2c) {
            JSTHROW("invalid i2c port number:%d", i2cnum)
        }

        if(thisobj->addr<0) {
            if(!thisobj->i2c->isInstalled()) {
                JSTHROW("i2c driver not installed, cannot detect address of GT911")
            }
            if( thisobj->i2c->ping(0x5D) ){
                thisobj->addr = 0x5D ;
            }
            else if( thisobj->i2c->ping(0x14) ){
                thisobj->addr = 0x14 ;
            }
            else {
                JSTHROW("GT911 not found (address 0x5D or 0x14)")
            }
        }
        else if(thisobj->addr!= 0x5D && thisobj->addr!= 0x14) {
            JSTHROW("invalid GT911 address:0x%02X (must be 0x5D or 0x14)", thisobj->addr)
        }

        // @todo read from setup options
        thisobj->pointsSize = 1 ;
        if(thisobj->pointsSize>INDEVPOINTER_MAX_NUM) {
            thisobj->pointsSize = INDEVPOINTER_MAX_NUM ;
        }

        thisobj->reset() ;

        return InDevPointer::setup(ctx, this_val, argc, argv) ;
    }
}

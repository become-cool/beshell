#include "VL53L0X.hpp"

using namespace std ;


#define VL53L0X_REG_IDENTIFICATION_MODEL_ID      		    0x00c0
#define VL53L0X_REG_IDENTIFICATION_REVISION_ID      		0x00c2
#define VL53L0X_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD   		0x0050
#define VL53L0X_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 		0x0070
#define VL53L0X_REG_SYSRANGE_START                 		    0x0000
#define VL53L0X_REG_RESULT_INTERRUPT_STATUS        		    0x0013
#define VL53L0X_REG_RESULT_RANGE_STATUS            		    0x0014
#define VL53L0X_REG_I2C_SLAVE_DEVICE_ADDRESS        		0x008a
#define VL53L0X_REG_SYSTEM_RANGE_CONFIG			            0x0009
#define VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV       0x0089
#define VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT                0x0000
#define VL53L0X_REG_SYSRANGE_MODE_START_STOP                0x0001
#define VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK                0x0002
#define VL53L0X_REG_SYSRANGE_MODE_TIMED                     0x0004

#define VL53L0X_DEVICEMODE_SINGLE_RANGING	               ((uint8_t)  0)
#define VL53L0X_DEVICEMODE_CONTINUOUS_RANGING	           ((uint8_t)  1)
#define VL53L0X_DEVICEMODE_CONTINUOUS_TIMED_RANGING        ((uint8_t)  3)
#define VL53L0X_DEFAULT_MAX_LOOP  200

#define ESD_2V8


#define WRITE_REG(reg,val)                                      \
        if( !i2c->write<uint8_t, uint8_t>(addr, reg, val) ){    \
            return false ;                                      \
        }

#define READ_REG(reg,var)                                       \
        if( !i2c->read<uint8_t,uint8_t>(addr, reg, var) ) {     \
            return false ;                                      \
        }

namespace be::driver {
    DEFINE_NCLASS_META(VL53L0X, I2CDevice)
    std::vector<JSCFunctionListEntry> VL53L0X::methods = {
        JS_CFUNC_DEF("begin", 2, I2CDevice::begin),
        JS_CFUNC_DEF("start", 0, VL53L0X::start),
        JS_CFUNC_DEF("stop", 0, VL53L0X::stop),
        JS_CFUNC_DEF("readDistance", 0, VL53L0X::readDistance),
    } ;

    VL53L0X::VL53L0X(JSContext * ctx, JSValue _jsobj)
        : I2CDevice(ctx,build(ctx,_jsobj))
    {}
    JSValue VL53L0X::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new VL53L0X(ctx) ;
        return obj->jsobj ;
    }

    int VL53L0X::begin(be::I2C * i2c, uint8_t addr) {
        int ret = I2CDevice::begin(i2c,addr) ;
        if( ret<0 ) {
            return ret ;
        }

        if( dataInit()<=0 ){
            return -11 ;
        }
        
        uint8_t ver, id ;
        i2c->read<uint8_t,uint8_t>(addr, VL53L0X_REG_IDENTIFICATION_REVISION_ID, ver) ;
        dn(ver)
        i2c->read<uint8_t,uint8_t>(addr, VL53L0X_REG_IDENTIFICATION_MODEL_ID, id) ;
        dn(id)

        return 0 ;
    }

    bool VL53L0X::dataInit(){
        if(!i2c) {
            return false;
        }
    // #ifdef ESD_2V8
    //     uint8_t data;
    //     data = readByteData(VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV);
    //     data = (data & 0xFE) | 0x01;
    //     i2c->write<uint8_t, uint8_t>(addr, VL53L0X_REG_VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV, data);
    // #endif

        WRITE_REG(0x88, 0x00)
        WRITE_REG(0x80, 0x01)
        WRITE_REG(0xFF, 0x01)
        WRITE_REG(0x00, 0x00)

        uint8_t val ;
        READ_REG(0x91, val)

        WRITE_REG(0x91, 0x3c)
        WRITE_REG(0x00, 0x01)
        WRITE_REG(0xFF, 0x00)
        WRITE_REG(0x80, 0x00)
        
        return true ;
    }

    bool VL53L0X::setMode(ModeState _mode, PrecisionState _precision) {
        mode = _mode ;

        if(precision == High){
	        WRITE_REG(VL53L0X_REG_SYSTEM_RANGE_CONFIG, 1);
        }
        else{
	        WRITE_REG(VL53L0X_REG_SYSTEM_RANGE_CONFIG, 0);
        }
        precision = _precision ;

        return true ;
    }
    
    bool VL53L0X::start() {

        // uint8_t DeviceMode;
        uint8_t Byte;
        uint8_t StartStopByte = VL53L0X_REG_SYSRANGE_MODE_START_STOP;
        uint32_t LoopNb;
        
        // DeviceMode = _detailedData.mode;
        
        WRITE_REG(0x80, 0x01);
        WRITE_REG(0xFF, 0x01);
        WRITE_REG(0x00, 0x00);
        WRITE_REG(0x91, 0x3c);
        WRITE_REG(0x00, 0x01);
        WRITE_REG(0xFF, 0x00);
        WRITE_REG(0x80, 0x00);
        
        switch(mode){
            case Single:
                WRITE_REG(VL53L0X_REG_SYSRANGE_START, 0x01);
                Byte = VL53L0X_REG_SYSRANGE_MODE_START_STOP; 
                /* Wait until start bit has been cleared */
                LoopNb = 0;
                do {
                    if (LoopNb > 0) { 
                        READ_REG(VL53L0X_REG_SYSRANGE_START, Byte);
                    }
                    LoopNb = LoopNb + 1;
                } while (((Byte & StartStopByte) == StartStopByte) && 
                            (LoopNb < VL53L0X_DEFAULT_MAX_LOOP));
                break;
            case Continuous:
                /* Back-to-back mode */
                /* Check if need to apply interrupt settings */
                //VL53L0X_CheckAndLoadInterruptSettings(Dev, 1);中断检查?
                WRITE_REG(VL53L0X_REG_SYSRANGE_START, VL53L0X_REG_SYSRANGE_MODE_BACKTOBACK);
                break;
            default:
                /* Selected mode not supported */
                return false ;	
        }

        return true ;
    }
    bool VL53L0X::stop() {
        
        WRITE_REG(VL53L0X_REG_SYSRANGE_START, VL53L0X_REG_SYSRANGE_MODE_SINGLESHOT);
        WRITE_REG(0xFF, 0x01);
        WRITE_REG(0x00, 0x00);
        WRITE_REG(0x91, 0x00);
        WRITE_REG(0x00, 0x01);
        WRITE_REG(0xFF, 0x00);
    
        return true ;
    }

    bool VL53L0X::readDistance(uint16_t & dist) {
        
        uint8_t buff[12] ;
        if( !i2c->read<uint8_t>(addr, VL53L0X_REG_RESULT_RANGE_STATUS, buff, sizeof(buff)) ){
            return false ;
        }

        // uint16_t status = ((buff[0] & 0x78) >> 3);
        // uint16_t ambientCount = ((buff[6] & 0xFF) << 8) | (buff[7] & 0xFF);
        // uint16_t signalCount = ((buff[8] & 0xFF) << 8) | (buff[9] & 0xFF);
        uint16_t distance = ((buff[10] & 0xFF) << 8) | (buff[11] & 0xFF);

        if(distance != 20) {
            lastDistance = distance ;
        }
        
        if(precision == High) {
            lastDistance/= 4 ;
        }
        dist = lastDistance ;

        return true ;
    }


    JSValue VL53L0X::start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(VL53L0X, thisobj)
        return thisobj->start()? JS_TRUE: JS_FALSE ;
    }
    JSValue VL53L0X::stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(VL53L0X, thisobj)
        return thisobj->stop()? JS_TRUE: JS_FALSE ;
    }
    JSValue VL53L0X::readDistance(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(VL53L0X, thisobj)
        uint16_t dist = 0 ;
        if(!thisobj->readDistance(dist)){
            return JS_NULL ;
        }
        return JS_NewUint32(ctx, dist);
    }
}
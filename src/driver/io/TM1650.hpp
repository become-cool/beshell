#pragma once
#include "../I2CDevice.hpp"


#define TM1650_NUM_DIGITS   16 // max number of digits
#define TM1650_MAX_STRING   128 // number of digits
#define TM1650_MIN_BRIGHT	0
#define TM1650_MAX_BRIGHT	7

namespace be::driver::io {
    class TM1650: public I2CDevice {
    DECLARE_NCLASS_META
    public:
        TM1650(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        static JSValue clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
        static JSValue setDot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        int setup();
        void	clear();
        void	on();
        void	off();
        void	displayState(bool aState);
        void	string(const char *aString);
        int 	displayRunning(char *aString);
        int 	displayRunningShift();
        void	setBrightness(unsigned int aValue = TM1650_MAX_BRIGHT);
        void	setBrightnessGradually(unsigned int aValue = TM1650_MAX_BRIGHT);
        inline unsigned int getBrightness() { return brightness; };

        void	controlPosition(unsigned int aPos, uint8_t aValue);
        void	setPosition(unsigned int pos, uint8_t value);
        void	setDot(unsigned int pos, bool state);
        uint8_t	getPosition(unsigned int pos) { return buffer[pos]; };
        inline unsigned int	getNumPositions() { return numDigits; };

    private:
        static std::vector<JSCFunctionListEntry> methods ;
        
        char *iPosition;
        bool exists = false;
        unsigned int numDigits = 4;
        unsigned int brightness;
        char iString[TM1650_MAX_STRING+1];
        uint8_t buffer[TM1650_NUM_DIGITS+1];
        uint8_t ctrl[TM1650_NUM_DIGITS];
    } ;
}
#include "TM1650.hpp"
#include <iostream>

using namespace std ;



#define TM1650_DISPLAY_BASE 0x34 // Address of the left-most digit 
#define TM1650_DCTRL_BASE   0x24 // Address of the control register of the left-most digit

#define TM1650_BIT_ONOFF	0b00000001
#define TM1650_MSK_ONOFF	0b11111110
#define TM1650_BIT_DOT		0b00000001
#define TM1650_MSK_DOT		0b11110111
#define TM1650_BRIGHT_SHIFT	4
#define TM1650_MSK_BRIGHT	0b10001111

const uint8_t TM1650_CDigits[128] {
//0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10
  0x00, 0x82, 0x21, 0x00, 0x00, 0x00, 0x00, 0x02, 0x39, 0x0F, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00, // 0x20
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x6f, 0x00, 0x00, 0x00, 0x48, 0x00, 0x53, // 0x30
  0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x06, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x3F, // 0x40
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x00, 0x0F, 0x00, 0x08, // 0x50 
  0x63, 0x5F, 0x7C, 0x58, 0x5E, 0x7B, 0x71, 0x6F, 0x74, 0x02, 0x1E, 0x00, 0x06, 0x00, 0x54, 0x5C, // 0x60
  0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x30, 0x0F, 0x00, 0x00  // 0x70
};


namespace be::driver::io {
    DEFINE_NCLASS_META(TM1650, I2CDevice)

    std::vector<JSCFunctionListEntry> TM1650::methods = {
            JS_CFUNC_DEF("secleartup", 0, TM1650::clear),
            JS_CFUNC_DEF("on", 0, TM1650::on),
            JS_CFUNC_DEF("off", 0, TM1650::off),
            JS_CFUNC_DEF("string", 0, TM1650::string),
            JS_CFUNC_DEF("setDot", 0, TM1650::setDot),
    };

    TM1650::TM1650(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj)) {
    }

    JSValue TM1650::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new TM1650(ctx);
        obj->shared() ;
        return obj->jsobj;
    }
    

    JSValue TM1650::clear(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TM1650, that)
        that->clear() ;
        return JS_UNDEFINED;
    }
    JSValue TM1650::on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TM1650, that)
        that->on() ;
        return JS_UNDEFINED;
    }
    JSValue TM1650::off(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TM1650, that)
        that->off() ;
        return JS_UNDEFINED;
    }
    JSValue TM1650::string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TM1650, that)
        CHECK_ARGC(1)
        std::string ARGV_TO_STRING(0, str)
        that->string(str.c_str()) ;
        return JS_UNDEFINED;
    }

    JSValue TM1650::setDot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        THIS_NCLASS(TM1650, that)
        CHECK_ARGC(2)
        ARGV_TO_UINT8(0, pos)
        bool state = JS_ToBool(ctx, argv[1]) ;
        that->setDot(pos, state) ;
        return JS_UNDEFINED;
    }
        
    int TM1650::setup() {
        iPosition = NULL;
        for (int i=0; i<numDigits; i++) {
            buffer[i] = 0;
            ctrl[i] = 0;
        }
        // Wire.beginTransmission(TM1650_DISPLAY_BASE);
        // exists = (Wire.endTransmission() == 0);
        exists = i2c->ping(TM1650_DISPLAY_BASE) ;
        dn(exists)

        clear();
        on();
        return 0 ;
    }

    /** Set brightness of all digits equally
     * aValue - brightness value with 1 being the lowest, and 7 being the brightest
     */
    void TM1650::setBrightness(unsigned int aValue) {
        if (!exists) return;

        brightness = (aValue > TM1650_MAX_BRIGHT) ? TM1650_MAX_BRIGHT : aValue;

        for (int i=0; i<numDigits; i++) {
            // Wire.beginTransmission(TM1650_DCTRL_BASE+i);
            // ctrl[i] = (ctrl[i] & TM1650_MSK_BRIGHT) | ( brightness << TM1650_BRIGHT_SHIFT );
            // Wire.write((uint8_t) ctrl[i]);
            // Wire.endTransmission();
        }
    }

    /** Set brightness of all digits equally
     * aValue - brightness value with 1 being the lowest, and 7 being the brightest
     */
    void TM1650::setBrightnessGradually(unsigned int aValue) {
        if (!exists || aValue == brightness) return;

        if (aValue > TM1650_MAX_BRIGHT) aValue = TM1650_MAX_BRIGHT;
        int step = (aValue < brightness) ? -1 : 1;
        unsigned int i = brightness;
        do {
            setBrightness(i);
            // delay(50);
            vTaskDelay(50 / portTICK_PERIOD_MS);
            i += step;
        } while (i!=aValue);
    }

    /** Turns display on or off according to aState
     */
    void TM1650::displayState (bool aState)
    {
    if (aState) on ();
    else off();
    }

    /** Turns the display on
     */
    void TM1650::on ()
    // turn all digits on
    {
        if (!exists) return;
        for (int i=0; i<numDigits; i++) {
            // Wire.beginTransmission(TM1650_DCTRL_BASE+i);
            ctrl[i] = (ctrl[i] & TM1650_MSK_ONOFF) | TM1650_BIT_DOT;
            // Wire.write((uint8_t) ctrl[i]);
            // Wire.endTransmission();
            i2c->send(TM1650_DCTRL_BASE+i, ctrl+i, 1) ;
        }
    }
    /** Turns the display off
     */
    void TM1650::off ()
    // turn all digits off
    {
        if (!exists) return;
        for (int i=0; i<numDigits; i++) {
            // Wire.beginTransmission(TM1650_DCTRL_BASE+i);
            ctrl[i] = (ctrl[i] & TM1650_MSK_ONOFF);
            // Wire.write((uint8_t) ctrl[i]);
            // Wire.endTransmission();
            i2c->send(TM1650_DCTRL_BASE+i, ctrl+i, 1) ;
        }
    }

    /** Directly write to the CONTROL register of the digital position
     * aPos = position to set the control register for
     * aValue = value to write to the position
     *
     * Internal control buffer is updated as well
     */
    void TM1650::controlPosition(unsigned int aPos, uint8_t aValue) {
        if (!exists) return;
        if (aPos < numDigits) {
            // Wire.beginTransmission(TM1650_DCTRL_BASE + (int) aPos);
            // ctrl[aPos] = aValue;
            // Wire.write(aValue);
            // Wire.endTransmission();
        }
    }

    /** Directly write to the digit register of the digital position
     * aPos = position to set the digit register for
     * aValue = value to write to the position
     *
     * Internal position buffer is updated as well
     */
    void TM1650::setPosition(unsigned int pos, uint8_t aValue) {
        if (!exists) return;
        if (pos < numDigits) {
            // Wire.beginTransmission(TM1650_DISPLAY_BASE + (int) aPos);
            buffer[pos] = aValue;
            // Wire.write(aValue);
            // Wire.endTransmission();
            i2c->send(TM1650_DISPLAY_BASE + (int) pos, buffer+pos, 1) ;
        }
    }

    /** Directly set/clear a 'dot' next to a specific position
     * aPos = position to set/clear the dot for
     * aState = display the dot if true, clear if false
     *
     * Internal buffer is updated as well
     */
    void	TM1650::setDot(unsigned int aPos, bool aState) {
        buffer[aPos] = (buffer[aPos] & 0x7F) | (aState ? 0x80 : 0);
        setPosition(aPos, buffer[aPos]);
    }

    /** Clear all digits. Keep the display on.
     */
    void TM1650::clear()
    // clears all digits
    {
    if (!exists) return;
    for (int i=0; i<numDigits; i++) {
        // Wire.beginTransmission(TM1650_DISPLAY_BASE+i);
        // buffer[i] = 0;
        // Wire.write((uint8_t) 0);
        // Wire.endTransmission();
    }
    }

    /** Display string on the display 
     * aString = character array to be displayed
     *
     * Internal buffer is updated as well
     * Only first N positions of the string are displayed if
     *  the string is longer than the number of digits
     */
    void TM1650::string(const char *aString)
    {
        if (!exists) return;
        for (int i=0; i<numDigits; i++) {
            uint8_t a = aString[i] & 0x7F;
            buffer[i] = (buffer[i]&0x80) | TM1650_CDigits[a];
            if (a) {
                i2c->send(TM1650_DISPLAY_BASE+i, buffer+i, 1) ;
                
                // Wire.beginTransmission(TM1650_DISPLAY_BASE+i);
                // Wire.write(buffer[i] | dot);
                // Wire.endTransmission();
            }
            else
                break;
        }
    }

    /** Display string on the display in a running fashion
     * aString = character array to be displayed
     *
     * Starts with first N positions of the string.
     * Subsequent characters are displayed with 1 char shift each time displayRunningShift() is called
     *
     * returns: number of iterations remaining to display the whole string
     */
    int TM1650::displayRunning(char *aString) {

        strncpy(iString, aString, TM1650_MAX_STRING+1);
        iPosition = iString;
        iString[TM1650_MAX_STRING] = '\0'; //just in case.
            string(iPosition);

        int l = strlen(iPosition);
        if (l <= numDigits) return 0;
        return (l - numDigits);
    }

    /** Display next segment (shifting to the left) of the string set by displayRunning()
     * Starts with first N positions of the string.
     * Subsequent characters are displayed with 1 char shift each time displayRunningShift is called
     *
     * returns: number of iterations remaining to display the whole string
     */
    int TM1650::displayRunningShift() {
            if (strlen(iPosition) <= numDigits) return 0;
            string(++iPosition);
        return (strlen(iPosition) - numDigits);
    }
}
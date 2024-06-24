#pragma once

#include "driver/I2CDevice.hpp"

namespace be::driver::sensor {
    class TMP117: public I2CDevice {
        DECLARE_NCLASS_META
    private:
        static std::vector<JSCFunctionListEntry> methods ;
    public:

        TMP117(JSContext * ctx, JSValue _jsobj=JS_NULL) ;
        static JSValue constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

        uint8_t getAddress();												// Returns the address of the device
        double readTempC();													// Returns the temperature in degrees C
        double readTempF();													// Converts readTempC result to degrees F
        void softReset();													// Performs a software reset on the Configuration Register Field bits
        float getTemperatureOffset();										// Reads the temperature offset
        void setTemperatureOffset(float offset);							// Writes to the temperature offset
        float getLowLimit();												// Returns the low limit register
        void setLowLimit(float lowLimit);									// Sets the low limit temperature for the low limit register
        float getHighLimit();												// Returns the high limit register
        void setHighLimit(float highLimit);									// Sets the low limit temperature for the low limit register
        uint16_t getConfigurationRegister();								// Get Configuration Register
        uint8_t getHighLowAlert();											// Reads in Alert mode for high and low alert flags
        bool getHighAlert();												// Reads in Alert mode for a high alert flag
        bool getLowAlert();													// Reads in Alert mode for a low alert flag
        void setAlertFunctionMode(uint8_t setAlertMode);					// Set alert or therm mode
        uint8_t getAlertFunctionMode();										// Check to see if in alert or therm mode
        uint8_t getConversionMode();										// Checks to see the Conversion Mode the device is currently in
        void setContinuousConversionMode();									// Sets the Conversion Mode of the Device to be Continuous
        void setOneShotMode();												// Sets the Conversion Mode of the Device to be One Shot
        void setShutdownMode();												// Sets the Conversion Mode of the Device to be Shutdown
        void setConversionAverageMode(uint8_t convMode);					// Sets the conversion averaging mode
        uint8_t getConversionAverageMode();									// Returns the Conversion Averaging Mode
        void setConversionCycleBit(uint8_t convTime);						// Sets the conversion cycle time bit
        uint8_t getConversionCycleBit();									// Returns the conversion cycle time bit value
        bool dataReady();													// Checks to see if there is data ready from the device

        
        static JSValue read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

    private:
        bool readRegister(uint8_t reg, uint16_t & out) ;
        uint16_t readRegister(uint8_t reg);		// Reads 2 register bytes from sensor
        bool writeRegister(uint8_t reg, uint16_t data); // Wires single byte of data to the sensor
    } ;
}
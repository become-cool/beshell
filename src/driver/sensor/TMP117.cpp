#include "TMP117.hpp"
#include <iostream>

#define bitRead(data, n)  ((data>>n)&1)
#define bitWrite(data, n, v) ((v) ? (data |= (1 << n)) : (data &= ~(1 << n)))
#define bitClear(data, n)  bitWrite(data,n,1)

#define highByte(x) ((uint8_t)(((x) >> 8) & 0xFF))
#define lowByte(x) ((uint8_t)((x) & 0xFF))

/*
  TMP117 Registers as defined in Table 3 from datasheet (pg 24)
  Features of the TMP117:
   - ±0.1°C (Maximum) From –20°C to +50°C
   - ±0.15°C (Maximum) From –40°C to +70°C
   - ±0.2°C (Maximum) From –40°C to +100°C
   - ±0.25°C (Maximum) From –55°C to +125°C
   - ±0.3°C (Maximum) From –55°C to +150°C
   - Low Power Consumption 3.5-µA, 1-Hz Conversion Cycle
*/

// Address of the registers. This can be found on page 23 of the datasheet
enum TMP117_Register
{
  TMP117_TEMP_RESULT = 0X00,
  TMP117_CONFIGURATION = 0x01,
  TMP117_T_HIGH_LIMIT = 0X02,
  TMP117_T_LOW_LIMIT = 0X03,
  TMP117_EEPROM_UL = 0X04,
  TMP117_EEPROM1 = 0X05,
  TMP117_EEPROM2 = 0X06,
  TMP117_TEMP_OFFSET = 0X07,
  TMP117_EEPROM3 = 0X08,
  TMP117_DEVICE_ID = 0X0F
};

#define DEVICE_ID_VALUE 0x0117			// Value found in the device ID register on reset (page 24 Table 3 of datasheet)
#define TMP117_RESOLUTION 0.0078125f	// Resolution of the device, found on (page 1 of datasheet)
#define CONTINUOUS_CONVERSION_MODE 0b00 // Continuous Conversion Mode
#define ONE_SHOT_MODE 0b11				// One Shot Conversion Mode
#define SHUTDOWN_MODE 0b01				// Shutdown Conversion Mode

// Configuration register found on page 25 Figure 26 and Table 6
typedef union {
	struct
	{
		uint8_t EMPTY : 1;			// Empty bit in register
		uint8_t TMP_SOFT_RESET : 1; // Software reset bit
		uint8_t DR_ALERT : 1;		// ALERT pin select bit
		uint8_t POL : 1;			// ALERT pin polarity bit
		uint8_t T_NA : 1;			// Therm/alert mode select
		uint8_t AVG : 2;			// Conversion averaging modes
		uint8_t CONV : 3;			// Conversion cycle bit
		uint8_t MOD : 2;			// Set conversion mode
		uint8_t EEPROM_BUSY : 1;	// EEPROM busy flag
		uint8_t DATA_READY : 1;		// Data ready flag
		uint8_t LOW_ALERT : 1;		// Low Alert flag
		uint8_t HIGH_ALERT : 1;		// High Alert flag
	} CONFIGURATION_FIELDS;
	uint16_t CONFIGURATION_COMBINED;
} CONFIGURATION_REG;

// Device ID Register used for checking if the device ID is the same as declared
// This register is found on Page 30 of the datasheet in Table 15 and Figure 34
typedef union {
	struct
	{
		uint16_t DID : 12; // Indicates the device ID
		uint8_t REV : 4;   // Indicates the revision number
	} DEVICE_ID_FIELDS;
	uint16_t DEVICE_ID_COMBINED;
} DEVICE_ID_REG;

using namespace std;
using namespace be;

namespace be::driver::sensor {
    DEFINE_NCLASS_META(TMP117, I2CDevice)

    std::vector<JSCFunctionListEntry> TMP117::methods = {
            JS_CFUNC_DEF("read", 0, TMP117::read),
    };

    TMP117::TMP117(JSContext *ctx, JSValue _jsobj)
            : I2CDevice(ctx, build(ctx, _jsobj), 0x38) {
    }

    JSValue TMP117::constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        auto obj = new TMP117(ctx);
        obj->shared() ;
        return obj->jsobj;
    }

    // int TMP117::read() {
    //     if(!i2c || !addr) {
    //         return -1 ;
    //     }

    //     // return (byteH << 8) + byteL;
    //     return 0 ;
    // }

    JSValue TMP117::read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
        THIS_NCLASS(TMP117, thisobj)
        double data = thisobj->readTempC() ;
        return JS_NewFloat64(ctx,data) ;
    }

    


    /* READ REGISTER
        This function reads the register bytes from the sensor when called upon.
        This reads 2 bytes of information from the 16-bit registers. 
    */
    bool TMP117::readRegister(uint8_t reg, uint16_t & out) {
        if(!i2c || !addr) {
            return false ;
        }
        
        if(!i2c->read<uint8_t, uint16_t>(addr, reg, out)){
            return 0 ;
        }

        uint8_t byte1 = (out>>8) & 0xff ;
        uint8_t byte2 = out&0xff ;
        out = (byte2<<8) | byte1 ;

        return true ;
    }

    uint16_t TMP117::readRegister(uint8_t reg){
        uint16_t value = 0 ;
        readRegister(reg, value) ;
        return value ;
    }


    /* WRITE REGISTER
        Wire data to a TMP117 register
    */
    bool TMP117::writeRegister(uint8_t reg, uint16_t data) // originally TMP117_Register reg
    {
        if(!i2c || !addr) {
            return false ;
        }
        data = ((data>>8)&0xff) | ((data&0xff)<<8) ;
        return i2c->write<uint8_t, uint16_t>(addr, reg, data) ;
    }

    /* READ TEMPERATURE CELSIUS
        This function reads the temperature reading from the sensor
        and returns the value in degrees celsius.

        NOTE: The data type of digitalTemp is a signed integer, meaning that the 
        value of the binary number being read will be negative if the MSB is 1,
        and positive if the bit is 0. 
    */
    double TMP117::readTempC()
    {
        int16_t digitalTempC = readRegister(TMP117_TEMP_RESULT); // Calls to read registers to pull all the bits to store in an array

        float finalTempC = digitalTempC * TMP117_RESOLUTION; // Multiplies by the resolution for digital to final temp

        return finalTempC;
    }

    /* READ TEMPERATURE FAHRENHEIT
        This function calculates the fahrenheit reading from the
        celsius reading initially found.
        The device reads in celsius unless this function is called.
    */
    double TMP117::readTempF()
    {
        return readTempC() * 9.0 / 5.0 + 32.0; // Conversion from °C to °F
    }

    /* GET TEMPERATURE OFFSET
        This function reads the temperature offset. This reads from the register
        value 0x07 (TMP117_TEMP_OFFSET). This can be found on page 23 of the 
        datasheet. 
    */
    float TMP117::getTemperatureOffset()
    {
        int16_t _offset = 0;
        _offset = readRegister(TMP117_TEMP_OFFSET);				// Reads from the temperature offset register
        float finalOffset = (float)_offset * TMP117_RESOLUTION; // Multiplies by the resolution for correct offset temperature
        return finalOffset;
    }

    /* SET TEMPERATURE OFFSET
        This function sets the offset temperature of the device. The user
        can write to this to set any desired offset within the temperature range.
        This writes to the register value 0x07 (TMP117_TEMP_OFFSET)
    */
    void TMP117::setTemperatureOffset(float offset)
    {
        int16_t resolutionOffset = offset / TMP117_RESOLUTION; // Divides by the resolution to send the correct value to the sensor
        writeRegister(TMP117_TEMP_OFFSET, resolutionOffset);   // Writes to the offset temperature register with the new offset value
    }

    /* GET LOW LIMIT
        This function reads the low limit register that is set by the user.
        The values are signed integers since they can be negative.
    */
    float TMP117::getLowLimit()
    {
        int16_t lowLimit = 0;
        lowLimit = readRegister(TMP117_T_LOW_LIMIT);			// Calls to read register to pull all the bits to store in a variable
        float finalLimit = (float)lowLimit * TMP117_RESOLUTION; // Multiplies by the resolution for correct offset temperature
        return finalLimit;
    }

    /* SET LOW LIMIT
        This function allows the user to set the low limit register to whatever
        specified value, as long as in the range for the temperature sensor. This
        function can be used as a threshold for Therm mode and or Alert mode.
        The values are signed integers since they can be negative.
    */
    void TMP117::setLowLimit(float lowLimit)
    {
        int16_t finalLimit = lowLimit / TMP117_RESOLUTION; // Divides by the resolution to send the correct value to the sensor
        writeRegister(TMP117_T_LOW_LIMIT, finalLimit);	 // Writes to the low limit temperature register with the new limit value
    }

    /* GET HIGH LIMIT
        This function reads the high limit register that is set by the user.
        The values are signed integers since they can be negative.
    */
    float TMP117::getHighLimit()
    {
        int16_t highLimit = 0;
        highLimit = readRegister(TMP117_T_HIGH_LIMIT);			 // Calls to read registers to pull all the bits to store in an array
        float finalLimit = (float)highLimit * TMP117_RESOLUTION; // Multiplies by the resolution for correct offset temperature
        return finalLimit;
    }

    /* SET HIGH LIMIT
        This function allows the user to set the high limit register to whatever
        specified value, as long as in the range for the temperature sensor. This
        function can be used as a threshold for Therm mode and or Alert mode
        The values are signed integers since they can be negative.
    */
    void TMP117::setHighLimit(float highLimit)
    {
        int16_t finalLimit = highLimit / TMP117_RESOLUTION; // Divides by the resolution to send the correct value to the sensor
        writeRegister(TMP117_T_HIGH_LIMIT, finalLimit);		// Writes to the high limit temperature register with the new limit value
    }

    /*GET CONFIGURATION REGISTER
        This function reads configuration register. Use this function if you need to read
        certain flags before they are cleared. This can be found on page 25 of the
        datasheet.
    */
    uint16_t TMP117::getConfigurationRegister()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION);

        return configReg;
    }

    /*GET HIGH AND LOW ALERT
        This function reads configuration register and saves the high and low alert flags. 
        Use this function if you need to read the alert flags before they are cleared.
        This can be found on page 25 of the datasheet.
    */
    uint8_t TMP117::getHighLowAlert()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); //Grab what is in the configuration register

        uint8_t highlowAlert = 0; //temporary variable to hold high and low alert flags
        //Get high alert flag from 15th bit of the configuration register,
        //    then write bit to highlowAlert's 2nd position from the right
        bitWrite(highlowAlert, 1, bitRead(configReg, 15));

        //Get high alert flag from 14th bit of the configuration register,
        //    then write bit to highlowAlert's 1st position from the right
        bitWrite(highlowAlert, 0, bitRead(configReg, 14));

        return highlowAlert; //return high and low alert flags
    }

    /*GET HIGH ALERT
        This function reads the 15th bit of the configuration register to
        tell if the conversion result is higher than the high limit. This
        is set as a High Alert flag. This can be found on page 25 of the 
        datasheet.
    */
    bool TMP117::getHighAlert()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); //Grab what is in the configuration register

        uint8_t highAlert = bitRead(configReg, 15); // Fills with the 15th bit of the configuration register

        if (highAlert == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*GET LOW ALERT
        This function reads the 14th bit of the configuration register to 
        tell if the conversion result is lower than the low limit. This
        is set as a Low Alert flag. This can be found on page 25 of the 
        datasheet.
    */
    bool TMP117::getLowAlert()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); //Grab what is in the configuration register

        uint8_t lowAlert = bitRead(configReg, 14); // Fills with the 14th bit of the configuration register

        if (lowAlert == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*SET ALERT FUNCTION MODE
        This function sets the alert function mode to either "alert" or
        "therm" mode. This can be found on page 25 of the datasheet.
    */
    void TMP117::setAlertFunctionMode(uint8_t setAlertMode)
    {
        uint16_t AlertFunctionMode = 0;
        AlertFunctionMode = readRegister(TMP117_CONFIGURATION); // Fills mode to be the config register

        if (setAlertMode == 1) // 1: Therm mode
        {
            bitWrite(AlertFunctionMode, 4, 1); //set register bit to be 1
            writeRegister(TMP117_CONFIGURATION, AlertFunctionMode);
        }
        else // 0: alert mode
        {
            bitClear(AlertFunctionMode, 4); //set register bit to be 0
            writeRegister(TMP117_CONFIGURATION, AlertFunctionMode);
        }
    }

    /*GET ALERT FUNCTION MODE
        This function gets the alert function mode to either "alert" or 
        "therm" mode. This can be found on page 25 of the datasheet.
    */
    uint8_t TMP117::getAlertFunctionMode()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); // Fill configReg with the configuration register

        uint8_t currentAlertMode = bitRead(configReg, 4); //Get the value from the Therm/alert mode ("T/nA") select field

        if (currentAlertMode == 1) //if "therm" mode
        {
            return 1;
        }
        else //if "alert" mode
        {
            return 0;
        }
    }

    /* SOFTWARE RESET
        This function performs a software reset, loading all the default
        values into the configuration register. A table of the configuration
        register can be found on page 25 of the datasheet.
    */
    void TMP117::softReset()
    {
        uint16_t reset = 0;
        reset = readRegister(TMP117_CONFIGURATION); // Fills mode to be the configuration register
        bitWrite(reset, 1, 1);
        writeRegister(TMP117_CONFIGURATION, reset);
    }

    /* SET CONTINUOUS CONVERSION MODE
        This function sets the conversion mode of the sensor to be 
        continuous. This can be found in the datasheet on Page 25 Table 6.
        The TMP117 defaults to Continuous Conversion Mode on reset.
    */
    void TMP117::setContinuousConversionMode()
    {
        uint16_t mode = 0;
        mode = readRegister(TMP117_CONFIGURATION); // Fills mode to be the configuration register

        bitClear(mode, 10); // Clears bit 10
        bitClear(mode, 11); // Clears bit 11
        writeRegister(TMP117_CONFIGURATION, mode);
    }

    /* SET SHUTDOWN MODE
        This function sets the conversion mode of the sensor to be 
        in shutdown mode. This can be found in the datasheet on Page 
        25 Table 6. The TMP117 defaults to Continuous Conversion Mode 
        on reset.
    */
    void TMP117::setShutdownMode()
    {
        uint16_t mode = 0;
        mode = readRegister(TMP117_CONFIGURATION); // Fills mode to be the configuration register

        bitClear(mode, 11);	// Clears bit 11
        bitWrite(mode, 10, 1); // Sets bit 10 to 1
        writeRegister(TMP117_CONFIGURATION, mode);
    }

    /* SET ONE SHOT MODE
        This function sets the conversion mode of the sensor to be 
        in one shot mode. This can be found in the datasheet on Page 
        25 Table 6. The TMP117 defaults to Continuous Conversion Mode 
        on reset.
    */
    void TMP117::setOneShotMode()
    {
        uint16_t mode = 0;
        mode = readRegister(TMP117_CONFIGURATION); // Fills mode to be the configuration register

        bitWrite(mode, 10, 1); // Sets bit 10 to 1
        bitWrite(mode, 11, 1); // Sets bit 11 to 1
        writeRegister(TMP117_CONFIGURATION, mode);
    }

    /* GET CONVERSION MODE
        This function reads the mode for the conversions, then
        prints it to the Serial Monitor in the Arduino IDE.
        This can be found in the datasheet on Page 25 Table 6. 
        This function can return Continuous Conversion Mode (1),
        Shutdown Mode (2), and One-Shot Mode (3). This should never 
        return 4 (the other continuous conversion) since there is 
        no function for writing to the value. 
    */
    uint8_t TMP117::getConversionMode()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); // Fill configReg with the configuration register

        uint8_t currentMode1 = bitRead(configReg, 11); // Left most conversion bit
        uint8_t currentMode2 = bitRead(configReg, 10); // Right most conversion bit

        if ((currentMode1 == 0) && (currentMode2 == 0)) // 0b00, Continuous Conversion Mode
        {
            return 1; // Actually 0, but return 1 for the example sketch
        }
        else if ((currentMode1 == 1) && (currentMode2 == 0)) // 0b10, should not be set this
        {
            return 4; // This value should never be returned
        }
        else if ((currentMode1 == 0) && (currentMode2 == 1)) // 0b01, Shutdown Mode
        {
            return 2; // Actually 0b01, but return 2 for the example sketch
        }
        else if ((currentMode1 == 1) && (currentMode2 == 1)) // 0b11, One-Shot Mode
        {
            return 3;
        }
        else
        {
            return 1; //Default
        }
    }

    /*SET CONVERSION AVERAGE MODE 
        This function sets the conversion averaging mode of the device
        when in Continuous Conversion Mode. This can be found on page 
        25 of the TMP117 datasheet. 
    */
    void TMP117::setConversionAverageMode(uint8_t convMode)
    {
        uint16_t mode = 0;
        mode = readRegister(TMP117_CONFIGURATION); // Fills to be the config register

        if (convMode == 0) // No Averaging, 0b00
        {
            bitClear(mode, 5); // Clears bit 5
            bitClear(mode, 6); // Clears bit 6
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convMode == 1) // 8 Averaged Conversions, 0b01
        {
            bitClear(mode, 6);	// Clears bit 6
            bitWrite(mode, 5, 1); // Sets bit 5 to be 1
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convMode == 2) // 32 Averaged Conversions, 0b10
        {
            bitWrite(mode, 6, 1); // Sets bit 6 to be 1
            bitClear(mode, 5);	// Clears bit 5
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convMode == 3) // 64 Averaged Conversions, 0b11
        {
            bitWrite(mode, 6, 1); // Sets bit 6 to be 1
            bitWrite(mode, 5, 1); // Sets bit 5 to be 1
            writeRegister(TMP117_CONFIGURATION, mode);
        }
    }

    /*GET CONVERSION AVERAGE MODE
        This function reads for the averaging mode of the conversions, 
        then prints it to the Serial Monitor in the Arduino IDE.
        This can be found in the datasheet on Page 25 Table 6.
    */
    uint8_t TMP117::getConversionAverageMode()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); // Fills configReg with config register values

        uint8_t currentMode6 = bitRead(configReg, 6); // Left most averaging bit
        uint8_t currentMode5 = bitRead(configReg, 5); // Right most averaging bit

        if ((currentMode6 == 0) && (currentMode5 == 0)) // 0b00, no averaging
        {
            return 0b00;
        }
        else if ((currentMode6 == 0) && (currentMode5 == 1)) // 0b01, 8 averaged conversions
        {
            return 0b01;
        }
        else if ((currentMode6 == 1) && (currentMode5 == 0)) // 0b10, 32 averaged conversions
        {
            return 0b10;
        }
        else if ((currentMode6 == 1) && (currentMode5 == 1)) // 0b11, 64 averaged conversions
        {
            return 0b11;
        }
        else
        {
            return 0b00; //No averaging
        }
    }

    /* SET CONVERSION CYCLE BIT
        This function sets the conversion cycle time bit in 
        Continuous Conversion mode. The times for the conversions
        can be found below or in the datasheet on Page 25 Table
        6 or on Page 27 Table 7. The user puts in 0-7 and it will
        return the cycle time accoring to the values in the chart.

        Conversion Cycle Time in CC Mode (found on the datasheet page 26 table 6)
                AVG       0       1       2       3
        CONV  averaging  (0)     (8)     (32)   (64)
            0             15.5ms  125ms   500ms    1s     
            1             125ms   125ms   500ms    1s     
            2             250ms   250ms   500ms    1s     
            3             500ms   500ms   500ms    1s     
            4             1s      1s      1s       1s     
            5             4s      4s      4s       4s     
            6             8s      8s      8s       8s     
            7             16s     16s     16s      16s    
    */
    void TMP117::setConversionCycleBit(uint8_t convTime)
    {
        uint16_t mode = 0;
        mode = readRegister(TMP117_CONFIGURATION); // Fills in time to be the config register

        if (convTime == 0) // 0b000
        {
            bitClear(mode, 9); // Clears bit 9
            bitClear(mode, 8); // Clears bit 8
            bitClear(mode, 7); // Clears bit 7
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 1) // 0b001
        {
            bitClear(mode, 9);	// Clears bit 9
            bitClear(mode, 8);	// Clears bit 8
            bitWrite(mode, 7, 1); // Sets bit 7 to be 1
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 2) // 0b010
        {
            bitClear(mode, 9);	// Clears bit 9
            bitWrite(mode, 8, 1); // Sets bit 8 to be 1
            bitClear(mode, 7);	// Clears bit 7
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 3) // 0b011
        {
            bitClear(mode, 9);	// Clears bit 9
            bitWrite(mode, 8, 1); // Sets bit 8 to be 1
            bitWrite(mode, 7, 1); // Sets bit 7 to be 1
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 4) // 0b100
        {
            bitWrite(mode, 9, 1); // Sets bit 9 to be 1
            bitClear(mode, 8);	// Clears bit 8
            bitClear(mode, 7);	// Clears bit 7
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 5) // 0b101
        {
            bitWrite(mode, 9, 1); // Sets bit 9 to be 1
            bitClear(mode, 8);	// Clears bit 8
            bitWrite(mode, 7, 1); // Sets bit 7 to be 1
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 6) // 0b110
        {
            bitWrite(mode, 9, 1); // Sets bit 9 to be 1
            bitWrite(mode, 8, 1); // Sets bit 8 to be 1
            bitClear(mode, 7);	// Clears bit 7
            writeRegister(TMP117_CONFIGURATION, mode);
        }
        else if (convTime == 7) // 0b111
        {
            bitWrite(mode, 9, 1); // Sets bit 9 to be 1
            bitWrite(mode, 8, 1); // Sets bit 8 to be 1
            bitWrite(mode, 7, 1); // Sets bit 7 to be 1
            writeRegister(TMP117_CONFIGURATION, mode);
        }
    }

    /* GET CONVERSION CYCLE BIT
        This function returns the Conversion Cycle Bit value that the 
        device is currently in at the time. This bit can help determine
        the conversion cycle time that the device is in while being in
        continuous conversion mode. 

        Conversion Cycle Time in CC Mode (found on the datasheet page 26 table 6)
                AVG       0       1       2       3
        CONV  averaging  (0)     (8)     (32)   (64)
            0             15.5ms  125ms   500ms    1s     
            1             125ms   125ms   500ms    1s     
            2             250ms   250ms   500ms    1s     
            3             500ms   500ms   500ms    1s     
            4             1s      1s      1s       1s     
            5             4s      4s      4s       4s     
            6             8s      8s      8s       8s     
            7             16s     16s     16s      16s    
    */
    uint8_t TMP117::getConversionCycleBit()
    {
        uint16_t configReg = 0;
        configReg = readRegister(TMP117_CONFIGURATION); // Fills configReg with config register values

        uint8_t currentTime9 = bitRead(configReg, 9); // Left most bit (9)
        uint8_t currentTime8 = bitRead(configReg, 8); // Middle bit (8)
        uint8_t currentTime7 = bitRead(configReg, 7); // Right most bit (7)

        if ((currentTime9 == 0) && (currentTime8 == 0) && (currentTime7 == 0)) // 0b000
        {
            return 0b000;
        }
        else if ((currentTime9 == 0) && (currentTime8 == 0) && (currentTime7 == 1)) // 0b001
        {
            return 0b001;
        }
        else if ((currentTime9 == 0) && (currentTime8 == 1) && (currentTime7 == 0)) // 0b010
        {
            return 0b010;
        }
        else if ((currentTime9 == 0) && (currentTime8 == 1) && (currentTime7 == 1)) // 0b011
        {
            return 0b011;
        }
        else if ((currentTime9 == 1) && (currentTime8 == 0) && (currentTime7 == 0)) // 0b100
        {
            return 0b100;
        }
        else if ((currentTime9 == 1) && (currentTime8 == 0) && (currentTime7 == 1)) // 0b101
        {
            return 0b101;
        }
        else if ((currentTime9 == 1) && (currentTime8 == 1) && (currentTime7 == 0)) // 0b110
        {
            return 0b110;
        }
        else if ((currentTime9 == 1) && (currentTime8 == 1) && (currentTime7 == 1)) // 0b111
        {
            return 0b111;
        }
        else
        {
            return 0b000; //Default
        }
    }

    /* DATA READY
        This function checks to see if there is data ready to be sent
        from the TMP117. This can be found in Page 25 Table 6 of the 
        data sheet.
    */
    bool TMP117::dataReady()
    {
        uint16_t response = readRegister(TMP117_CONFIGURATION);

        // If statement to see if the 13th bit of the register is 1 or not
        if (response & 1 << 13)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}
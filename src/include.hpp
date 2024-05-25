#include "debug.h"

// core
#include "NativeClass.hpp"
#include "NativeModule.hpp"
#include "ModuleLoader.hpp"
#include "JSEngine.hpp"
#include "EventEmitter.hpp"
#include "JSTimer.hpp"

//fs
#include "fs/FSPartition.hpp"
#include "fs/LittleFS.hpp"
#include "fs/FatFS.hpp"
#include "fs/RawFS.hpp"
#include "fs/FS.hpp"

// telnet/repl
#include "telnet/Protocal.hpp"
#include "telnet/TelnetChannel.hpp"
#include "telnet/TelnetBLE.hpp"
#include "telnet/TelnetSerial.hpp"
#include "telnet/TelnetStdIO.hpp"
#include "telnet/TelnetWS.hpp"
#include "telnet/Telnet.hpp"
#include "repl/REPL.hpp"

// module
#include "module/DeviceTree.hpp"
#include "module/GPIO.hpp"
#include "module/NVS.hpp"
#include "module/Path.hpp"
#include "module/Process.hpp"
#include "module/WiFi.hpp"

// serial
#include "module/serial/Serial.hpp"
#include "module/serial/UART.hpp"
#include "module/serial/I2C.hpp"
#include "module/serial/SPI.hpp"
#include "module/serial/I2S.hpp"

// mg
#include "mg/Response.hpp"
#include "mg/HTTPRequest.hpp"
#include "mg/Client.hpp"
#include "mg/Server.hpp"
#include "mg/Mg.hpp"

// sqlite3
#include "module/sqlite3/SQLite3.hpp"
#include "module/sqlite3/DB.hpp"

//lvgl
#ifndef ARDUINO
#include "lv/LV.hpp"
#endif

// driver
#include "driver/input/GT911.hpp"
#include "driver/sensor/BMP280.hpp"
#include "driver/sensor/AHT20.hpp"
#include "driver/sensor/WH4530A.hpp"
#include "driver/sensor/DS18B20.hpp"
#include "driver/sensor/MLX90614/MLX90614.hpp"
#include "driver/sensor/VL53L0X.hpp"
#include "driver/io/PCA9557.hpp"
#include "driver/io/TM1650.hpp"
#include "driver/sys/DS1307.hpp"
#include "driver/sys/SDCard.hpp"
#include "driver/disp/ST7701.hpp"
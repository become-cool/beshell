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
#include "fs/RawFS.hpp"
#include "fs/FSModule.hpp"
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
#include "module/DeviceModule.hpp"
#include "module/GPIOModule.hpp"
#include "module/NVSModule.hpp"
#include "module/PathModule.hpp"
#include "module/ProcessModule.hpp"
#include "module/WiFiModule.hpp"

// serial
#include "module/serial/SerialModule.hpp"
#include "module/serial/UART.hpp"
#include "module/serial/I2C.hpp"
#include "module/serial/SPI.hpp"
#include "module/serial/I2S.hpp"

// mg
#include "mg/Response.hpp"
#include "mg/HTTPRequest.hpp"
#include "mg/Client.hpp"
#include "mg/Server.hpp"
#include "mg/MgModule.hpp"

// sqlite3
#include "module/sqlite3/SQLite3Module.hpp"
#include "module/sqlite3/DB.hpp"


// driver
#include "driver/DriverModule.hpp"
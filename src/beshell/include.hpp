// #include "debug.h"
#include "sdkconfig.h"

// core
#include "./NativeClass.hpp"
#include "./NativeModule.hpp"
#include "./JSEngine.hpp"
#include "./EventEmitter.hpp"
#include "./JSTimer.hpp"
#include "./ModuleLoader.hpp"

//fs
#include "./fs/FSPartition.hpp"
#include "./fs/FatFS.hpp"
#include "./fs/RawFS.hpp"
#include "./fs/FS.hpp"
#include "./fs/LittleFS.hpp"

// repl/repl
#include "./repl/Protocal.hpp"
#include "./repl/REPLChannel.hpp"
#include "./repl/REPLSerial.hpp"
#include "./repl/REPLStdIO.hpp"
#include "./repl/REPLCDC.hpp"
#include "./repl/REPL.hpp"
#include "./cammonds/Cammonds.hpp"

// module
#include "./module/gpio/GPIO.hpp"
#include "./module/NVS.hpp"
#include "./module/Path.hpp"
#include "./module/Process.hpp"
#include "./module/WiFi.hpp"
#include "./module/Flash.hpp"
#include "./module/logger/Logger.hpp"
#include "./module/animator/Animator.hpp"
// serial
#include "./module/serial/Serial.hpp"
#include "./module/serial/UART.hpp"
#include "./module/serial/I2C-legacy.hpp"
#include "./module/serial/SPI.hpp"
#include "./module/serial/CDC.hpp"


// driver
#include "./driver/DriverModule.hpp"

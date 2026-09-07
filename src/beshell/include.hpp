// #include "debug.h"
#ifdef ESP_PLATFORM
#include "sdkconfig.h"
#endif

// core
#include "./NativeClass.hpp"
#include "./NativeModule.hpp"
#include "./JSEngine.hpp"
#include "./EventEmitter.hpp"
#include "./JSTimer.hpp"
#include "./ModuleLoader.hpp"

//fs
#include "./fs/FSPartition.hpp"
#include "./fs/FS.hpp"
#ifdef ESP_PLATFORM
#include "./fs/FatFS.hpp"
#include "./fs/RawFS.hpp"
#include "./fs/LittleFS.hpp"
#endif

// repl/repl
#include "./repl/Protocal.hpp"
#include "./repl/REPLChannel.hpp"
#include "./repl/REPLStdIO.hpp"
#ifdef ESP_PLATFORM
#include "./repl/REPLSerial.hpp"
#include "./repl/REPLCDC.hpp"
#endif
#include "./repl/REPL.hpp"
#include "./cammonds/Cammonds.hpp"

// module
#include "./module/NVS.hpp"
#include "./module/Path.hpp"
#include "./module/Process.hpp"
#include "./module/logger/Logger.hpp"
#include "./module/animator/Animator.hpp"
#ifdef ESP_PLATFORM
#include "./module/gpio/GPIO.hpp"
#include "./module/WiFi.hpp"
#include "./module/Flash.hpp"
// serial
#include "./module/serial/Serial.hpp"
#include "./module/serial/UART.hpp"
#include "./module/serial/I2C-legacy.hpp"
#include "./module/serial/SPI.hpp"
#include "./module/serial/CDC.hpp"
#endif


// driver
#include "./driver/DriverModule.hpp"

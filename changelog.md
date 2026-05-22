# v1.0.13

New Feature:

* + InDevPointer
* spi.setup() 支持4线SPI


# v1.0.11 2026/5/11

New Feature:

* example 可从 CMakeLists.private.txt 文件中加载 components 目录
* 完善了 JSDoc 文档
* EventEmitter::emitNativeEvent() 增加参数 bool fromISR=false 支持从 ISR 中触发

# v1.0.10 

New Feature:

* + ModuleLoader::addModuleSource()

Fix:

* js-to-c.js 生成的 *.c 文件，字符数组由 static 类型改为 const

# v1.0.9 2026/3/4

Fix:

* repl cdc not work
* gpio adc/pwm issue

# v1.0.8 2026/3/3

Fix:

* 其他 beshell-xxx 下游components的依赖问题

# v1.0.7 2026/2/8

New Feature:

* + FS::unmount()/FS::remount()
* + BeShell::evalMain()
* + GPIO 连续模式

Reflect:

* - 移除 BT 到独立的 component:  beshell-bt
* 将 gpio 的 pwm 和 adc 系列api函数放到 gpio.pwm 和 gpio.adc 对象中
* telnet 改名为 repl

# v1.0.6 2026/2/3

New Feature:

* + serial.uart.unsetup()
* + telnet sync, ping, pong 协议
* + ota.start() 参数 opt.downloader
* 支持在 sdkconfig 中取消 CONFIG_ESP_WIFI_SOFTAP_SUPPORT 以节省 sram

Fix:

* mg.download() catch the exceptions of the callback function
* fixed bug for bt.central.connect()
* fixed: js-to-c.js 路径
* fixed: telnet cdc delay
* fixed: "bt queue full" 刷屏
* fixed: telnet cdc output
* uart.unsetup() 清除所有资源
* quickjs/VERSION 文件中多余的换行符会造成项目生成错误


# v1.0.5 2025-11-25

Fix:

* vscode 环境下从 exmaples/beshell-app 自动创建工程，第一次编译会因为找不到 mklittlefs 导致失败

# v1.0.4 2025-11-24

Refact:

* 移除 gpio adc legacy 接口
* 实现了 ng 驱动的 i2c 接口


# v1.0.3 2025-10-07

Add:

* examples
* gpio.adcReadChannel()

Fix:

* build issues on ESP32

Refact:

* 按 ESP Componenty Registry 规则打包
* 移除了 mg、lvgl、sqlite 等依赖第三方库的代码，作为独立的 component 发布
* 移除了大量驱动代码、ota 等非基础功能，作为独立的 component 发布

New Feature:

* + sync, ping, pong 协议
* + telnet MQTT 协议
* + ota.start() 参数 opt.downloader

Fixed: 

* telnet cdc delay
* "bt queue full" 刷屏


# v0.3.10   2025-12-29

New Feature:

* add serial.uart.unsetup()


# v0.3.9   2025-09-30

New Feature:

* mg.addDNSCache()
* mg.removeDNSCache()
* mg.clearDNSCache()
* ota module compatible with Flash encryption mode
* gpio adc functions


Fixed:

* mg.download() catch the exceptions of the callback function


# v0.3.8   2025-09-18

* bt.setPower() add arg `type`
* bt.getMTU()
* bt.setRandomMac()
* bt.setCharValue()
* reboot 命令增加可选参数 path 
* source 命令可以使用参数
* telnet.ble
* telnet 接收数据对称加密
* telnet.startLog()
* mg.https/mqtts support client cert/key
* cdc module
* cdc telnet
* process.getTimerCallback()
* process.getTimerCount()
* process.getRunningTime()
* process.getChipTemperature()
* process.resetReason()
* process.top()
* process.getMac()
* process.setMac()
* process.tasks()
* process.cpuCount()
* login / logout cmd
* driver wb2812b
* W5500.setup() arg "options" add property: "mac"
* W5500.setIP()
* wifi.setStaIP()
* wifi.dhcpStaStart()
* wifi.dhcpStaStop()
* wifi.setMAC()
* wifi.getMAC()
* gpio pwm function
* logger module

Fix issure:

* 解决了在 setInterval 的 callback 中调用 clearTimeout() 导致事件循环错乱
* gpio.watch() 打印 callback 抛出的异常

## v0.3.7   2025-04-26

Added: 

* bt module
* wifi.startStaDeamon()
* wifi.stopStaDeamon()
* wifi.isStaDeamonRunning()
* fs.tell()
* mg.startCaptivePortal()
* mg.Request.isCaptivePortalRequest()
* mg.sntpRequest()
* mg.post()
* mongoose 支持 mqtt
* NativeClass::emitSyncFree()
* spi eth ic driver W5500
* lcd ic driver ST7789
* adc driver CS1237
* 在主循环中自动调用 gc
* process.gc()
* process.ref()
* export NativeClass to global ns
* NativeClass.setDestructor()
* global function delay
* JS_IsArrayBuffer()
* console.block()
* flash module

Fixed:

* i2c read() leaks memory

Removed:

* wifi.autoReconnect()

Refactored:

* BeShell::main() 如果发生崩溃，等待30秒后再执行 main 脚本
* wifi.conenct()  auto call startStaDeamon() when retry==-1
* EventEmitter::emitNativeEvent() 返回 bool
* 修改了 dt.device 中驱动对象的存放结构
* rename process.delay() to process.sleep()
* 升级 mongoose 库到 7.17

## v0.3.6   2024-11-27

Added:

* EventEmitter 新的 native事件触发机制:  emitNativeEvent()/onNativeEvent()
* AudioPlayer stop 事件
* AudioPlayer.playPCM()
* add uart.setup() option properties: stopbits, parity
* add driver.ST7701.initReg()
* add driver.WS2812B
* add i2c.isInstalled()
* driver TM1652
* add process.delay(ms)
* + EventModule

Refactored:

* 后台任务方式读取 pointer input dev, 避免低帧率状态下丢失事件
* dt.load() 可以接收对象参数


## v0.3.5   2024-10-05

Added:

* uart.unsetup()
* uart.isInstalled()
* sensor driver TMP117
* bt telnet
* BeShell::useTelnet()
* process.readMac()
* mg.setDNS()
* mg.getDNSTimeout()
* mg.setDNSTimeout()
* mg.get()
* mg.download()
* repl command: compile
* loader.compile()
* AudioPlayer
* NativeClass::printOnDestruct
* 增加了 JS 写设备驱动的机制
* add lv.Obj.show()/hide()/setVisble()

Fixed:

Refactored:

* uart.write() 可以接受数组参数
* ArrayBuffer.prototype.toArray() 增加参数 signed
* BeShell::setup() 和 BeShell::main() 增加参数 mainScriptPath
* telnet 做为 NativeModule 的子类，可在 js 中调用
* serial.spiX "X"从0开始计数
* mg.connect() crash
* process.setTime 影响 setInterval()
* 重构了 JSEngine 的循环机制
* 重构了 I2C 的读/写接口

## v0.3.4   2024-6-4

Added:

* 完善 WiFi 模块的 API
* 完善 Mg 模块的 API

Fixed:

* 低于 idf 5.2 无法正确创建串口对象

Refactored:

* EventEmitter 的主要方法不再返回 this

## v0.3.2   2024-5-26

Refactored:

* 预设的 partitions.cvs 文件中分区名称由 fshome 改为 fsroot
* JSEngine::evalScript() 增加参数指定是否输出异常


## v0.3.1   2024-5-25

Added:
* driver WH4530A
* driver DS1307
* driver TM1650
* driver MLX90614
* nvs.readString() / nvs.writeString()
* process.readEFUSE()
* process.setTimezoneOffset()

Fixed:

Refactored:

* 全面重构了 BeShell 的接口
* I2CDevice::begin() rename to I2CDevice::setup()
* DeviceTree 在 js 中调用
* 加入了 be::driver:input 名称空间
* lv.Animation 从 EventEmitter 继承，增加了 stop 事件

### v0.3.0   2024-4-22

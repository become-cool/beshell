
New Feature:

* bt.setPower() add arg `type`
* bt.getMTU()
* reboot 命令增加可选参数 path 
* source 命令可以使用参数
* telnet.ble
* telnet 接收数据对称加密
* mg.https/mqtts support client cert/key
* cdc module
* cdc telnet
* process.getTimerCallback(), process.getTimerCount()
* process.getRunningTime()
* login / logout cmd
* driver wb2812b

Fix issure:


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
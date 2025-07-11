## dev

Added:

* EventEmitter 新的 native事件触发机制:  emitNativeEvent()/onNativeEvent()
* AudioPlayer stop 事件
* AudioPlayer.playPCM()
* add uart.setup() option properties: stopbits, parity
* add driver.ST7701.initReg()
* add i2c.isInstalled()
* driver TM1652

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
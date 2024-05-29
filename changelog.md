## dev


Added:

Refactored:

* EventEmitter 的主要方法不再返回 this
* 完善 WiFi 模块的 API

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

### v0.3.0   2024-4-22
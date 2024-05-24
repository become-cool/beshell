## master/dev

Added:
* driver WH4530A
* driver DS1307
* nvs.readString() / nvs.writeString()
* process.readEFUSE()
* process.setTimezoneOffset()

Fixed:

Refactored:

* 全面重构了 BeShell 的接口
* I2CDevice::begin() rename to I2CDevice::setup()
* DeviceTree 在 js 中调用


### v0.3.0   2024-4-22
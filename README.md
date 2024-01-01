BeShell 是一个为类似 ESP32 这样的32位单片机平台设计的 **JavaScript **运行环境。



BeShell 的 JavaScript 采用 QuickJS ，它几乎完整地实现了 ES2020 标准。



> BeShell 的定位是 FreeRTOS 的 shell ，JavaScript 是“第一公民”

---

BeShell 将大量常用的 ESP-IDF API 封装为 JavaScript 接口，方便使用者基于 JavaScript 开发单片机应用。BeShell 在 ESP32 平台上提供了以下 JS 层面的特性：

- GPIO 和 外设 (I2C/SPI/UART等)
- 很多常用硬件设备和IC的驱动 (电机、屏幕、摄像头、传感器、控制器等)
* 文件系统 (LittleFS / Fat32 / RawFS / SDCard)

* 在多种通讯方式(串口/WiFi/标准IO)上提供了 Telnet 协议，可以用于执行交互式JS命令 (REPL) 和 文件传输

* GUI (LVGL for JavaScript)

* MP3/JPEG/MIDI音色合成 等媒体功能

* 通讯协议 (HTTP/HTTPS/WebSocket/MQTT等)



所有这些特性都是可以在编译时自由剪裁，使用者只引用必要的功能，以便保持较小编译尺寸。最小的 BeShell 仅仅是一个 QuickJS for ESP32 。



---

BeShell 以 `ESP-IDF components` 和 `Arduino库` 的形式发布。使用者可以根据自己的能力和喜好，自由地在 ESP-IDF 和 Arduino 两种框架下开发 BeShell 应用。



BeShell 提供了友好且优雅的 C/C++/JavaScript 接口，支持使用者客户`混合编程`：计算密集或高频的任务用 C/C++ 写，业务复制的任务用 JavaScript 写。



由于 BeShell 提供了完备的 JS 特性，大部分简单的应用，是可以 纯JS 完成的。



---

BeShell 具备良好的跨平台性，同时也能在 Linux/MacOS/Windows 上编译运行。BeShell 还提供了 WebAssemble 的构建共享，你甚至还可以在浏览器里运行 BeShell 。
BeShell 是一个为类似 ESP32 这样的32位单片机平台设计的 **JavaScript **运行环境。

BeShell 的 JavaScript 采用 QuickJS ，基本完整的实现了 ES2020 标准。

---

BeShell 将常用的 ESP-IDF API 封装为 JavaScript 接口，让你以完全使用 JavaScript 开发单片机应用。BeShell 在 ESP32 平台上提供了以下 JS 层面的特性：

- GPIO 和 外设 (I2C/SPI/UART等)
- 很多常用硬件设备和IC的驱动 (电机、屏幕、摄像头、传感器、控制器等)
* 多种文件系统 (LittleFS / Fat32 / RawFS / SDCard)

* 在多种通讯方式(串口/WiFi/标准IO)上提供了协议，可以用于执行交互式JS命令 (REPL) 和 文件传输

* GUI (LVGL for JavaScript)

* MP3/JPEG/MIDI音色合成 等媒体功能

* 通讯协议 (HTTP/HTTPS/WebSocket/MQTT等)



所有特性都是可以在编译前自由剪裁，根据需要引用功能，以便保持较小的编译尺寸。最小的 BeShell 仅仅是一个 JS for ESP32 。



---

ESP32 平台通有两种主要的开发环境：`ESP-IDF框架` 和 `Arduino` 。

BeShell 以 `ESP-IDF components` 和 `Arduino库` 两种形式发布。使用者可以根据自己的喜好、能力选择应用开发环境。



BeShell 提供了友好且优雅的 C/C++/JavaScript 接口，支持使用者客户`混合编程`：计算密集或高频的任务用 C/C++ 写，业务复杂的任务用 JavaScript 写。由于 BeShell 提供了完备的 JS 特性，大部分简单的应用，是可以 纯JS 完成的。



---

BeShell 具备良好的跨平台性，同时也能在 Linux/MacOS/Windows 上编译运行。BeShell 还提供了 WebAssemble 的构建共享，可以在浏览器里运行 BeShell 。


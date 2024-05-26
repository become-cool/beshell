## BeShell 是一个为 ESP32 平台设计的 JavaScript 开发框架。

BeShell 的 JavaScript 引擎采用 [QuickJS](https://bellard.org/quickjs/) ，完整的实现了 ES2020 标准。BeShell 将很多常用的 ESP-IDF API 封装为 JavaScript 接口，开发者可以完全使用 **JavaScript** 开发单片机应用。

---
## 主页和文档

完整的文档请访问 BeShell 主页：[https://beshell.become.cool](https://beshell.become.cool)

---
## 特性

BeShell 在 ESP32 平台上提供了以下 JS 层面的特性：

* GPIO 操作 (数字/模拟输入输出，PWM输出等)
  
* 单片机功能外设 (I2C/SPI/UART/WiFi/蓝牙 等)

* 一些硬件设备和IC的驱动 (电机、屏幕、摄像头、传感器、控制器等)

* 多种文件系统 (LittleFS / Fat32 / RawFS / SDCard)

* 在多种通讯方式(串口/WiFi/标准IO)上提供了协议，可以用于执行交互式JS命令 (REPL) 和 文件传输

* GUI (LVGL for JavaScript)

* MP3/JPEG/MIDI音色合成 等媒体功能

* 通讯协议 (HTTP/HTTPS/WebSocket/MQTT等)

* 符合流行的 JS 开发习惯，尽可能接近 NodeJS 等生态

* 其他 

所有特性都是可以在编译前自由剪裁，根据需要引用功能，以便保持较小的编译尺寸。

> 最小的 BeShell 仅仅是一个 JS for ESP32 。

---
## 环境 / IDE

BeShell 很好的支持两种开发环境：`Arduino` 和 `ESP-IDF框架`。

BeShell 以 `Arduino库` 和 `ESP-IDF components` 两种形式发布。使用者可以根据自己的喜好、能力选择应用开发环境。

  * [在 Arduino IDE 中安装 BeShell 库](https://beshell.become.cool/guide/install_arduino.html)

  * [在 ESP IDF 工程中安装 BeShell 组件](https://beshell.become.cool/guide/install_esp-idf.html)
---
## 混合编程

BeShell 提供了友好且优雅的 C/C++/JavaScript 接口，支持使用者客户`混合编程`：

  * 计算密集或高频的任务用 C/C++ 写
  
  * 业务复杂的任务用 JavaScript 写。

由于 BeShell 提供了完备的 JS 特性，大部分简单的应用，是可以 纯JS 完成的。

---
## 跨平台

BeShell 具备良好的跨平台性，同时也能在 Linux/MacOS/Windows 上编译运行。BeShell 还提供了 WebAssemble 的构建共享，可以在浏览器里运行 BeShell 。

---
## 工具

[BeConsole](https://beconsole.become.cool) 是一个 WEB 在线 IDE，可以通过 串口、WiFi 连接到运行 BeShell 应用的 ESP32 设备，提供了命令终端、文件管理、代码编辑器等功能。



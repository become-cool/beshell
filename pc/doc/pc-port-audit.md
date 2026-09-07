# BeShell PC/WebAssembly 移植审计报告（阶段 1 产出）

> 审计范围：`src/beshell/` 全部源文件（含 deps/ 快扫）。审计日期：2026-09-06。
> 约定：ESP 侧守卫宏 `ESP_PLATFORM`（继续用 FreeRTOS API）；PC 侧宏 `LINUX_PLATFORM` → 改名 `BESHELL_PLATFORM_PC`；PC 侧同步原语用 std（≤C++14）或 no-op。

## 一、总览

| 分类 | 文件 | 处理策略 |
|------|------|---------|
| **核心层——有遗漏需修复** | include.hpp、BeShell.class.hpp、BeShell.cpp、JSEngine.hpp/cpp、EventEmitter.cpp、NativeModule.cpp、qjs_utils.h、debug.h、fs/FS.cpp、fs/FS-JS.cpp、repl/REPL.hpp/cpp、cammonds/Cammonds.cpp、module/NVS.cpp、module/Process.cpp、module/Sleep.cpp(L4) | 就地守卫 + 收敛抽象 |
| **ESP 专属——整文件排除（CMake）** | platform.esp32.cpp、thread-esp32.cpp、psram.c、malloc_funcs.c、stubs.c、fs/FatFS.cpp、fs/LittleFS.cpp、fs/RawFS.cpp/hpp、repl/REPLSerial.*、repl/REPLCDC.*、driver/input/InDevPointer.*、module/WiFi.cpp、module/Flash.hpp/cpp、module/gpio/*.cpp、module/serial/*.cpp 及 UART/I2C/I2S/SPI/CDC 头、module/logger/logger-esp.cpp、deps/littlefs/src/esp_littlefs.*、littlefs_api.* | PC 构建不编译 |
| **PC 侧需新增实现** | thread-pc（mutex/队列/sleep/时基 4 原语）、logger-pc.cpp（log_* C 接口）、REPLStdIO Windows 分支 | 阶段 2/3 |
| **已正确守卫的范本** | JSTimer.hpp/cpp、REPLChannel.hpp/cpp、logger-esp.cpp、Sleep.cpp（除 L4）、uuid.c、NativeModule.cpp（队列部分）、Cammonds.cpp（大部分） | 照抄模式 |
| **完全可移植（零改动）** | NativeClass、ModuleLoader、path、string_utils、mallocf、quickjs.h/private.h、fs/FSPartition.*、repl/Protocal.*、repl/REPLModule.*、repl/REPLChannelNClass.*、repl/REPLStdIO.*（POSIX）、driver/DriverModule.*、basic/Console.*、module/Path.*、module/animator/ 全部 8 文件、deps/quickjs/*、deps/littlefs/src/littlefs/lfs_*、deps/Options.hpp | 无 |

## 二、一级阻塞点：头文件传染链（优先修）

PC 编译时最先爆炸的三条链：

1. **include.hpp**（被 BeShell.h/.hpp → 几乎全工程引用）
   - L2: `#include "sdkconfig.h"` 无守卫
   - L39-42: 无守卫 include ESP 专属头 `UART.hpp`（含 freertos 头）、`SPI.hpp`（含 driver/spi_master.h）、`CDC.hpp`（含 sdkconfig.h + soc/soc_caps.h）
   - 处理：ESP 专属 include 段整体包 `#ifdef ESP_PLATFORM`
2. **JSEngine.hpp**（被 NativeModule/NativeClass/EventEmitter/JSTimer/ModuleLoader 全部核心 cpp 引用）
   - L10-11: `freertos/FreeRTOS.h`、`freertos/semphr.h` 无守卫
   - L58: `SemaphoreHandle_t xSemaphore;` 成员无守卫
   - L107: `take(int timeout=portMAX_DELAY)` 默认参数无守卫
   - 处理：收敛到 thread 抽象（`be::Mutex` 类型 + 毫秒超时表达）
3. **repl/REPL.hpp**（被 REPL 全族引用）
   - L41: `QueueHandle_t pkg_queue;` 无守卫（include 有守卫、成员没有）
   - L58: 内联 `loop()` 中 `xQueueReceive` 无守卫
   - 处理：收敛到 thread 抽象（消息队列封装）

## 三、核心层遗漏清单（就地守卫 / 收敛抽象）

### 就地加 ESP_PLATFORM 守卫

| 文件 | 位置 | 内容 |
|------|------|------|
| BeShell.class.hpp | L12 | `#define ESPIDF_VERSION IDF_VER`（PC 兜底 `"-"`） |
| BeShell.cpp | L59, L64 | banner 中 ESPIDF_VERSION；`vTaskPrioritySet(NULL,10)` |
| EventEmitter.cpp | L24-31, L233-268 | **「hpp 已守卫、cpp 未守卫」失配**：析构 vQueueDelete/free、enableNativeEvent、emitNativeEvent（含 IRAM_ATTR、xQueueSendFromISR）、nativeEventLoop、onNativeEvent 定义 —— 照 NativeModule.cpp 模式补齐 |
| NativeModule.cpp | L173/L206 | **反向失配**：`EventModule::emitSync` 定义被多余守卫包住，hpp 声明无守卫 → PC 链接失败。**删除这对守卫**（函数体无平台依赖） |
| qjs_utils.h | L105-108, L202-208, L333, L363 | 宏定义体内的 `gpio_num_t`/`GPIO_NUM_MAX`/`esp_err_t`/`ESP_OK`（潜伏型，展开点才炸） |
| debug.h | L10, L61-66 | `esp_get_free_heap_size()`/`heap_caps_get_free_size`（潜伏型调试宏，PC 分支 no-op） |
| fs/FS.cpp | L35-36 | `FS::partitions`/`FS::pwd` 定义无守卫（hpp 声明已守卫，失配） |
| fs/FS-JS.cpp | L144, L956-963 | `esp_littlefs.h` include（全文件未用其符号，可删）；`FS::usage()` 用 ESP 专属 partitions（PC 侧抛不支持） |
| repl/REPL.cpp | L8, L62-80 | `mbedtls/aes.h` + AES 解密（见「外部依赖」节） |
| cammonds/Cammonds.cpp | L6, L19, L45-63 | NVS.hpp 防御性守卫；删无用 `utime.h`；三个 `__attribute__((weak))` FreeRTOS 桩（TaskStatus_t 等类型 PC 未定义） |
| module/NVS.cpp | L144-211 | 3 个 C++ helper（erase/readString/writeString）整段未守卫且无 PC 存根 |
| module/Process.cpp | L4, L204-230, L367-369, L725, L802-857, L925, L957-958 | esp_err.h include；esp_mac_type_t/ESP_MAC_* 常量；esp_base_mac_addr_set；SOC_TEMP_SENSOR_SUPPORTED 外层补守卫；esp_reset_reason 整段；configNUMBER_OF_CORES 两处 |
| module/Sleep.cpp | L4 | `esp_err.h` 移入 L6 守卫块（该文件其余部分已是范本） |
| module/serial/UART.hpp / SPI.hpp / CDC.hpp / I2S.hpp / I2S-legacy.hpp | 各文件头 | 被 include.hpp/Serial.cpp 引用，文件头 include 漏在守卫外（整文件排除后只需处理 include.hpp 侧） |
| module/serial/i2s_chan_share.h | L3 | `hal/i2s_types.h` 加守卫（供外部卫星组件） |

### 收敛到 platform/thread 抽象（接口保留，PC 侧 no-op 或极简实现）

> 审计后修正（2026-09-06 讨论结论）：PC 单线程模型下**所有同步原语均无真实生产者/竞争者**
> （REPLStdIO 在主循环直接调 onReceived 不经队列；原生事件生产者全是 ESP ISR/外设线程；
> mutex 调用方 I2C/I2S 全是外设）。因此 PC 侧不需要任何同步机制实现：

| 原语 | 使用点 | PC 实现 |
|------|--------|---------|
| Mutex（JSEngine take/give、REPLChannel mutex） | JSEngine L482/485 等 | **no-op**（take 恒 true），接口保留 |
| `REPL::pkg_queue` | REPL.hpp L41/L58、REPL.cpp L39/L47 | **整体消除**：execPackage 的 PC 实现直接转调 onReceived（当前即在主线程，语义等价） |
| 原生事件队列（EventEmitter/NativeModule nevent_queue） | EventEmitter.cpp、NativeModule.cpp | **no-op**：enableNativeEvent 不创建队列，emitNativeEvent 返回 false；接口完整保留供卫星组件编译 |
| `platform::sleepMs(ms)` | BeShell.cpp vTaskDelay（已守卫）、Process.cpp L544 | `std::this_thread::sleep_for`（C++11） |
| 时基（复用已有 `gettime()/gettime_us()`） | Process.cpp L693 `esp_timer_get_time` | 已有 POSIX 实现，直接复用 |
| 核数（纯信息查询，非调度参数） | Process.cpp L925/L957 `configNUMBER_OF_CORES` | `std::thread::hardware_concurrency()`（不创建线程） |

**core id 结论**：`run_wait_on_core`/`xTaskCreatePinnedToCore`/`portNUM_PROCESSORS` 的 core 参数全部服务外设驱动调度（I2C/UART/ADC/CDC/InDevPointer），随外设整体裁剪，PC 无残留。
**thread.hpp 最终形态**：PC 侧仅需 sleepMs/uptime 两个时间函数（建议归入 platform），同步原语全为 no-op 桩；`thread-pc.cpp`/`run_wait_on_core` 不实现，随 I2C 一起排除。

## 四、ESP 专属整文件排除清单（PC CMake 不编译）

platform.esp32.cpp、thread-esp32.cpp、psram.c、malloc_funcs.c、stubs.c、
fs/FatFS.cpp、fs/LittleFS.cpp、fs/RawFS.cpp(+hpp 的 dirent 耦合）、
repl/REPLSerial.hpp/cpp、repl/REPLCDC.hpp/cpp、
driver/input/InDevPointer.hpp/cpp、
module/WiFi.cpp、module/Flash.hpp/cpp、module/Sleep.cpp（或保留其"PC 抛异常"混合形态，仅需修 L4）、
module/gpio/GPIO.cpp/GPIO.adc.cpp/GPIO.pwm.cpp、
module/serial/Serial.cpp、UART.cpp、I2C.cpp、I2C-legacy.cpp、I2S.cpp、I2S-legacy.cpp、SPI.cpp、CDC.cpp（对应 .hpp 见三）、
module/logger/logger-esp.cpp（已守卫，CMake 排除或保留均可）、
deps/littlefs/src/esp_littlefs.c/h、littlefs_api.c/h（保留纯 lfs.c/lfs_util.c）

## 五、PC 侧需新增/调整

1. **thread-pc 不需要**：无 std::thread、无队列实现（见三）；**platform.pc.cpp** 仅含 sleepMs/uptime/空 init
2. **logger-pc.cpp**：`log_*` C 接口（logger-imp.hpp）的 PC 实现，否则 Logger.cpp/Logger.JS.cpp 链接缺符号
3. **REPLStdIO**：`LINUX_PLATFORM` → `BESHELL_PLATFORM_PC`；POSIX 头（unistd.h/sys/select.h）需移入非 Windows 分支，Windows 用 `_kbhit/_read` 或 `WaitForSingleObject` 实现
4. **mbedtls**（repl/REPL.cpp AES-CTR）：PC 构建显式引入，或加特性宏 `BESHELL_REPL_CRYPTO` 隔离整段
5. **`CONFIG_QUICKJS_VERSION`**：由组件 CMakeLists `add_definitions` 提供（BeShell.cpp L58、Process.cpp L72 使用），PC CMake 必须复刻
6. **`CONFIG_BIGNUM`**（JSEngine.cpp L123）：QuickJS 构建宏，PC 构建同样定义

## 六、顺带发现的既有 bug（与移植无关，记录待用户决定）

1. **Process.cpp L84 `LiNUX_PLATFORM` 拼写错误**（第 2 字母大写 i）→ 现有 LINUX 分支从未生效；改名 BESHELL_PLATFORM_PC 时顺带修复
2. **serial 宏缺 CONFIG_ 前缀**：I2C.cpp L2 / I2C-legacy.hpp L2 / I2C-legacy.cpp L1 用 `#if BESHELL_SERIAL_I2C_USE_LEGACY`（无前缀），Kconfig 只生成 `CONFIG_BESHELL_SERIAL_I2C_USE_LEGACY` → legacy I2C 路径是死代码（I2S 侧写法正确）
3. GPIO.adc.cpp L5-6 与 L11-12 重复 include；fs/FSPartition.hpp L22 `findPartitionByLabel()` 有声明无定义（死代码）；deps/cmdline.hpp 全库无人引用（死文件，且 cxxabi.h 不兼容 MSVC）
4. **反向陷阱**：`#if !CONFIG_BESHELL_SERIAL_I2S_USE_LEGACY` 在 PC 上因宏未定义而为真，ESP 内容会展开——不能靠这类功能宏防 PC，必须 CMake 排除或 ESP_PLATFORM 守卫

## 七、非 ESP 的 Windows 移植阻塞点（阶段 3 处理）

- `qjs_utils.c` gettime 用 `clock_gettime(CLOCK_REALTIME)`（MSVC 无，需 QueryPerformanceCounter 分支）
- `fs/FS.cpp` L246 双参 `mkdir`/`ACCESSPERMS`（参照同文件 L62-81 的 WIN32 分支补）
- `fs/FS-JS.cpp` L1203-1204 `fileno()/fsync()`（MSVC 需 `_fileno/_commit`）
- Logger.cpp L6 `unistd.h`、Cammonds.cpp `__attribute__((weak))`（MSVC 无）
- RawFS.hpp 的 `dirent.h`（MSVC 无；该文件已整文件排除，暂不阻塞）

# BeShell PC / WASM 构建指南

> 适用范围：`beshell/pc/` 独立 CMake 工程（不经由 ESP-IDF 组件构建）。
> 目标平台：Windows（MinGW）、Linux、macOS、WebAssembly（Emscripten）。

## 概述

`pc/` 工程把 BeShell 核心（QuickJS 引擎、JS 模块系统、REPL、FS、animator 等）
构建为本机可执行文件或 WASM 模块，用于：

- 在 PC 上开发/调试纯 JS 逻辑（无需烧录设备）
- 验证核心引擎与平台抽象层的可移植性
- 在浏览器/Node 中运行 BeShell（WASM）

ESP 专属内容（WiFi、GPIO、串口、Flash、LittleFS/FatFS 后端、REPLSerial/CDC 等）
不参与构建，排除清单见 [doc/pc-port-audit.md](pc-port-audit.md) 第四节。

## 构建依赖

| 依赖 | 要求 |
|------|------|
| CMake | ≥ 3.16 |
| C 编译器 | C99 |
| C++ 编译器 | C++14（**GCC 需 ≥ 8**，见「工具链注意事项」） |
| Emscripten | 仅 WASM 构建需要（emsdk） |

## 快速开始

### Windows（MinGW）

```powershell
cd pc
cmake -B build -G Ninja `
    -DCMAKE_MAKE_PROGRAM=C:/path/to/ninja.exe `
    -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe `
    -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe
cmake --build build
.\build\beshell-pc.exe
```

### Linux / macOS

```bash
cd pc
cmake -B build
cmake --build build
./build/beshell-pc
```

### WASM（Emscripten）

```bash
# 先激活 emsdk 环境（emcmake/emcc 在 PATH 中）
cd pc
emcmake cmake -B build-wasm
cmake --build build-wasm
# 产物：build-wasm/beshell-wasm.js + beshell-wasm.wasm（main.js 已打包进 MEMFS）

# Node 运行
node build-wasm/beshell-wasm.js
```

## 运行说明

### PC 本机

```
beshell-pc [脚本路径]
```

- 默认执行 `./fs/main.js`（`FS::setPrefix("./fs")` 把 VFS 根映射到该目录），随后进入 REPL。
- REPL 走标准输入输出：Windows 控制台按行输入；stdin 重定向为管道时也可工作
  （`PeekNamedPipe`/`ReadFile` 非阻塞轮询）；Linux/macOS 使用 `select()`。
- 文件系统经标准 C 文件 API 直接访问本地目录（`fs.readFileSync("/a.txt")`
  实际读写 `./fs/a.txt`）。

### WASM

- `main.js` 在构建时通过 `--preload-file` 打包进 MEMFS 根（`/`），启动时自动执行。
- 无 stdin 控制台，REPLStdIO 不参与实例化；JS 侧通过导出的 C 接口执行代码：

```javascript
// 浏览器 console 或 Node 中
Module.ccall('beshell_eval', null, ['string'], ['console.log(1+1)'])
```

- C 侧 `printf`/`std::cout` 输出由 Emscripten 自动映射到 `console.log`。

## 构建配置细节

`pc/CMakeLists.txt` 自动完成：

| 项 | 内容 |
|----|------|
| 平台宏 | `BESHELL_PLATFORM_PC` + 子平台宏 `BESHELL_PLATFORM_PC_WINDOWS` / `_LINUX` / `_MACOS` / `_WASM` |
| `CONFIG_QUICKJS_VERSION` | 读取 `src/beshell/deps/quickjs/VERSION`（与 ESP 侧一致） |
| `_WIN32_WINNT=0x0601` | 仅 Windows（MinGW 默认目标系统版本过低） |
| 源文件 | glob `src/beshell/**` 后按正则排除 ESP 专属文件；QuickJS 5 个 C 文件单独列出 |
| 告警抑制 | 沿用组件根 CMakeLists 的 `-Wno-*` / `-fpermissive` 集合（仅 GCC/Clang） |
| WASM | `-Oz`、`ALLOW_MEMORY_GROWTH`、`EXPORTED_FUNCTIONS=_main,_beshell_eval`、`--preload-file fs/main.js@/main.js` |

### 工具链注意事项

- **GCC < 8（如 MinGW.org 6.3.0）会在 `-O1` 及以上误编译 quickjs.c 等多个源文件**
  （启动即堆损坏崩溃）。检测到此类编译器时 CMake 强制 `-O0` 并输出警告。
  需要优化构建请使用 MinGW-w64 GCC ≥ 8 / Clang / MSVC / Emscripten。
- REPL 加密（mbedtls AES-CTR）默认关闭；PC 上如需启用，自行提供 mbedtls 并定义
  `BESHELL_REPL_CRYPTO`（见 `src/beshell/repl/REPL.cpp` 头部注释）。

## 目录结构

```
pc/
├── CMakeLists.txt        # 构建脚本
├── main.cpp              # 入口：PC 交互式 REPL / WASM emscripten 主循环
├── fs/main.js            # 冒烟测试脚本（banner、Promise、setTimeout 验证）
├── doc/
│   ├── build.md          # 本文档
│   └── pc-port-audit.md  # 移植审计报告（源码排除清单、宏体系设计）
└── build*/               # 构建产物（gitignore）
```

## 相关文档

- [pc-port-audit.md](pc-port-audit.md)：移植审计报告（传染链、守卫失配、排除清单）
- 组件根 `quickjs-modify.md`：QuickJS 本地补丁记录（含 PC 适配的两处）
- 工作区根 `AGENTS.md`：JS 模块开发约定、NativeModule/NativeClass 框架说明

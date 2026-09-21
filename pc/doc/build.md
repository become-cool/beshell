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

> **binaryen 版本失配（conda-forge 打包缺陷）**：conda 的 `emscripten` 4.0.9
> 依赖约束为 `binaryen >=117,<118`，但 emcc 4.0.9 实际需要 binaryen 123（旧 117 在
> 链接期 `wasm-opt` 会因不识别 `--no-stack-ir` / `--enable-call-indirect-overlong`
> 而失败）。临时解法：从镜像手动下载 binaryen 121 的 `.conda` 包解压到本地目录，
> 构建时设置环境变量 `BINARYEN=/path/to/binaryen121/Library` 覆盖默认路径。

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
- 无 stdin 控制台，REPLStdIO 不参与实例化；JS 侧通过导出的 C 接口执行代码。
- **已注册模块**（`main.cpp` 中 `beshell.use<...>()`）：`fs`、`nvs`、`animator`，以及
  内建 `loader`/`process`/`repl`/`sleep`。`import animator` / `import nvs` 均可用
  （`nvs` 在 PC/WASM 侧为存根，读/写返回 `false`/`null`）：

```javascript
// 浏览器 console 或 Node 中
Module.ccall('beshell_eval', null, ['string'], ['console.log(1+1)'])
```

- C 侧 `printf`/`std::cout` 输出由 Emscripten 自动映射到 `console.log`。

### 网页测试页

`pc/web/index.html` 是一个终端风格的浏览器 REPL 测试页，加载 `build-wasm/` 产物后在
页面上输入 JS 表达式，由 WASM 里的 QuickJS 引擎执行并回显结果/异常：

```bash
# 把 WASM 产物拷贝到 web/ 目录（index.html 与 beshell-wasm.js 同目录引用）
cp build-wasm/beshell-wasm.js build-wasm/beshell-wasm.wasm build-wasm/beshell-wasm.data web/

# 起一个静态服务器（file:// 协议下 Emscripten 无法加载 .wasm/.data）
cd web && python -m http.server 8123
# 浏览器打开 http://127.0.0.1:8123/index.html
```

页面通过 `createBeShell({ print, printErr, onRuntimeInitialized })` 工厂函数（由
`-sMODULARIZE=1 -sEXPORT_NAME=createBeShell` 生成）实例化模块，把 WASM 的 stdout/stderr
转发到输出区，再经 `ccall('beshell_eval', ...)` 执行输入框中的代码。

`beshell_eval` 与原生 REPL 走同一条路径（`Cammonds::input`）：先匹配 shell 命令
（`ls`/`cd`/`pwd`/`help` 等），否则作为 JS 求值；输出经一个 WASM 专用 `REPLChannel`
（`WasmChannel`，见 `main.cpp`）转发到 stdout。

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
├── web/index.html        # 浏览器 REPL 测试页（加载 build-wasm 产物）
├── doc/
│   ├── build.md          # 本文档
│   └── pc-port-audit.md  # 移植审计报告（源码排除清单、宏体系设计）
└── build*/               # 构建产物（gitignore）
```

## 相关文档

- [pc-port-audit.md](pc-port-audit.md)：移植审计报告（传染链、守卫失配、排除清单）
- 组件根 `quickjs-modify.md`：QuickJS 本地补丁记录（含 PC 适配的两处）
- 工作区根 `AGENTS.md`：JS 模块开发约定、NativeModule/NativeClass 框架说明

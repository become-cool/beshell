cmake_minimum_required(VERSION 3.16)

# BeShell PC 平台构建工程（独立 CMake 工程，不经由 ESP-IDF 组件构建）
# 支持 Windows(MinGW/MSVC)、Linux、macOS，以及 Emscripten(WASM)
#
# 用法（MinGW 示例）：
#   cd pc
#   cmake -B build -G "MinGW Makefiles"
#   cmake --build build
#   ./build/beshell-pc.exe          # 运行 ./fs/main.js 并进入 REPL

project(beshell-pc C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(BESHELL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ---- 子平台宏 ----
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(BESHELL_PC_SUB "WASM")
elseif(WIN32)
    set(BESHELL_PC_SUB "WINDOWS")
elseif(APPLE)
    set(BESHELL_PC_SUB "MACOS")
else()
    set(BESHELL_PC_SUB "LINUX")
endif()
message(STATUS "BeShell PC sub-platform: ${BESHELL_PC_SUB}")

# ---- QuickJS 版本号（与组件根 CMakeLists.txt 一致）----
file(READ "${BESHELL_DIR}/src/beshell/deps/quickjs/VERSION" QJS_VERSION_CONTENT)
string(STRIP "${QJS_VERSION_CONTENT}" QJS_VERSION_CONTENT)

# ---- BeShell 源文件 ----
file(GLOB_RECURSE src_beshell
    ${BESHELL_DIR}/src/beshell/*.c
    ${BESHELL_DIR}/src/beshell/*.cpp
)
# 内置依赖不随 glob（quickjs 单独列出）
list(FILTER src_beshell EXCLUDE REGEX "/deps/")

# ESP 专属源文件排除（清单见 doc/pc-port-audit.md 第四节）
list(FILTER src_beshell EXCLUDE REGEX "/platform\\.esp32\\.cpp$")
list(FILTER src_beshell EXCLUDE REGEX "/thread-esp32\\.cpp$")
list(FILTER src_beshell EXCLUDE REGEX "/(psram|malloc_funcs|stubs)\\.c$")
list(FILTER src_beshell EXCLUDE REGEX "/fs/(FatFS|LittleFS|RawFS)\\.cpp$")
list(FILTER src_beshell EXCLUDE REGEX "/repl/REPL(Serial|CDC)\\.cpp$")
list(FILTER src_beshell EXCLUDE REGEX "/driver/input/InDevPointer\\.cpp$")
list(FILTER src_beshell EXCLUDE REGEX "/module/(WiFi|Flash)\\.cpp$")
list(FILTER src_beshell EXCLUDE REGEX "/module/gpio/")
list(FILTER src_beshell EXCLUDE REGEX "/module/serial/")
list(FILTER src_beshell EXCLUDE REGEX "/module/logger/logger-esp\\.cpp$")

# ---- QuickJS 源文件 ----
set(src_quickjs
    ${BESHELL_DIR}/src/beshell/deps/quickjs/quickjs-libc.c
    ${BESHELL_DIR}/src/beshell/deps/quickjs/quickjs.c
    ${BESHELL_DIR}/src/beshell/deps/quickjs/libregexp.c
    ${BESHELL_DIR}/src/beshell/deps/quickjs/cutils.c
    ${BESHELL_DIR}/src/beshell/deps/quickjs/libunicode.c
)

add_executable(beshell-pc
    main.cpp
    ${src_beshell}
    ${src_quickjs}
)

# 老旧 GCC（如 MinGW.org 6.3，2016 年）在 -O1+ 会误编译 quickjs.c 等多个 TU
# （表现为启动即堆损坏崩溃）。检测到 GCC<8 时强制 -O0；优化构建请使用
# MinGW-w64 GCC>=8 / Clang / MSVC / Emscripten。
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 8)
    message(WARNING "GCC ${CMAKE_C_COMPILER_VERSION} 过旧，-O1+ 会误编译本工程，已强制 -O0。"
                    " 如需优化构建请使用 MinGW-w64 GCC>=8 或更高版本工具链。")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS_RELEASE "-O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O0")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O0 -g")
    set(CMAKE_C_FLAGS_MINSIZEREL "-O0")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-O0")
endif()

target_include_directories(beshell-pc PRIVATE
    ${BESHELL_DIR}/src
    ${BESHELL_DIR}/src/beshell
)

target_compile_definitions(beshell-pc PRIVATE
    BESHELL_PLATFORM_PC
    BESHELL_PLATFORM_PC_${BESHELL_PC_SUB}
    CONFIG_QUICKJS_VERSION="${QJS_VERSION_CONTENT}"
)

# MinGW 默认目标系统版本过低（缺少 GetTickCount64 等），提升到 Windows 7
if(WIN32)
    target_compile_definitions(beshell-pc PRIVATE _WIN32_WINNT=0x0601)
endif()

# ---- Emscripten (WASM) ----
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    # 体积优先
    target_compile_options(beshell-pc PRIVATE -Oz)
    target_link_options(beshell-pc PRIVATE
        -Oz
        # 导出 C 接口供 JS 调用（beshell_eval 见 main.cpp）
        "-sEXPORTED_FUNCTIONS=_main,_beshell_eval"
        "-sEXPORTED_RUNTIME_METHODS=ccall,cwrap"
        # 允许内存增长（QuickJS 堆按需扩张）
        "-sALLOW_MEMORY_GROWTH=1"
        # main.js 打包进 MEMFS 根目录
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/fs/main.js@/main.js"
        # 同时支持浏览器与 node 运行
        "-sENVIRONMENT=web,node"
    )
    # 产物为 beshell-wasm.js + beshell-wasm.wasm
    set_target_properties(beshell-pc PROPERTIES OUTPUT_NAME beshell-wasm SUFFIX ".js")
endif()

# ---- 编译选项（沿用组件根 CMakeLists.txt 的告警抑制风格，仅 GCC/Clang）----
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(beshell-pc PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-Wno-discarded-qualifiers>
        $<$<COMPILE_LANGUAGE:C>:-Wno-int-conversion>
        $<$<COMPILE_LANGUAGE:C>:-Wno-incompatible-pointer-types>
        $<$<COMPILE_LANGUAGE:C>:-Wno-implicit-function-declaration>
        $<$<COMPILE_LANGUAGE:C>:-Wno-missing-brace>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-deprecated-enum-enum-conversion>
        -Wno-missing-field-initializers
        -Wno-char-subscripts
        -Wno-pointer-arith
        -Wno-cast-function-type
        -Wno-format
        -Wno-implicit-fallthrough
        -Wno-return-type
        -Wno-sign-compare
        -Wno-conversion
        -Wno-unused-parameter
        -Wno-unused-label
        -Wno-unused-variable
        -Wno-unused-value
        -Wno-unused-function
        -Wno-unused-but-set-variable
        -Wno-deprecated-declarations
        -Wno-comment
        -Wno-type-limits
        -fno-stack-protector
        -fpermissive
    )
endif()

# BeShell PC 构建公共片段（供 beshell/pc 与各延伸组件的 pc/ 工程 include）
#
# 提供 beshell_pc_setup(TARGET)：把 beshell 便携源码 + quickjs + 编译选项 + 平台宏
# 直接并入调用方 target（不生成静态库）。使用前提：在 project() 之后 include 本文件。
#
# 本片段只服务 beshell 核心源码；延伸组件各自的 ESP/PC 分层、排除清单、PC 后端
# 由延伸自己的 pc/ 工程维护，beshell 不感知任何具体延伸。

# 捕获本文件所在目录（CMAKE_CURRENT_LIST_DIR 在函数内会变成调用方目录，故在 include 时定死）
set(_BESHELL_PC_DIR "${CMAKE_CURRENT_LIST_DIR}")
get_filename_component(_BESHELL_DIR "${_BESHELL_PC_DIR}/.." ABSOLUTE)

# ---- 子平台宏 ----
if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(_BESHELL_PC_SUB "WASM")
elseif(WIN32)
    set(_BESHELL_PC_SUB "WINDOWS")
elseif(APPLE)
    set(_BESHELL_PC_SUB "MACOS")
else()
    set(_BESHELL_PC_SUB "LINUX")
endif()
message(STATUS "BeShell PC sub-platform: ${_BESHELL_PC_SUB}")

# ---- QuickJS 版本号（与组件根 CMakeLists.txt 一致）----
file(READ "${_BESHELL_DIR}/src/beshell/deps/quickjs/VERSION" _BESHELL_QJS_VERSION)
string(STRIP "${_BESHELL_QJS_VERSION}" _BESHELL_QJS_VERSION)

# ---- 编译标准（beshell 源码要求 C99/C++14）----
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- 老旧 GCC（如 MinGW.org 6.3，2016 年）在 -O1+ 会误编译 quickjs.c 等多个 TU ----
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

# ---- 主函数：把 beshell 便携源码并入 target ----
function(beshell_pc_setup TARGET)

    # BeShell 源文件
    file(GLOB_RECURSE _beshell_src
        ${_BESHELL_DIR}/src/beshell/*.c
        ${_BESHELL_DIR}/src/beshell/*.cpp
    )
    # 内置依赖不随 glob（quickjs 单独列出）
    list(FILTER _beshell_src EXCLUDE REGEX "/deps/")

    # ESP 专属源文件排除（清单见 doc/pc-port-audit.md 第四节）
    list(FILTER _beshell_src EXCLUDE REGEX "/platform\\.esp32\\.cpp$")
    list(FILTER _beshell_src EXCLUDE REGEX "/thread-esp32\\.cpp$")
    list(FILTER _beshell_src EXCLUDE REGEX "/(psram|malloc_funcs|stubs)\\.c$")
    list(FILTER _beshell_src EXCLUDE REGEX "/fs/(FatFS|LittleFS|RawFS)\\.cpp$")
    list(FILTER _beshell_src EXCLUDE REGEX "/repl/REPL(Serial|CDC)\\.cpp$")
    list(FILTER _beshell_src EXCLUDE REGEX "/driver/input/InDevPointer\\.cpp$")
    list(FILTER _beshell_src EXCLUDE REGEX "/module/(WiFi|Flash)\\.cpp$")
    list(FILTER _beshell_src EXCLUDE REGEX "/module/gpio/")
    list(FILTER _beshell_src EXCLUDE REGEX "/module/serial/")
    list(FILTER _beshell_src EXCLUDE REGEX "/module/logger/logger-esp\\.cpp$")

    # QuickJS 源文件
    set(_quickjs_src
        ${_BESHELL_DIR}/src/beshell/deps/quickjs/quickjs-libc.c
        ${_BESHELL_DIR}/src/beshell/deps/quickjs/quickjs.c
        ${_BESHELL_DIR}/src/beshell/deps/quickjs/libregexp.c
        ${_BESHELL_DIR}/src/beshell/deps/quickjs/cutils.c
        # deps/quickjs/dtoa.c
        ${_BESHELL_DIR}/src/beshell/deps/quickjs/libunicode.c
    )

    target_sources(${TARGET} PRIVATE
        ${_beshell_src}
        ${_quickjs_src}
    )

    target_include_directories(${TARGET} PRIVATE
        ${_BESHELL_DIR}/src
        ${_BESHELL_DIR}/src/beshell
    )

    target_compile_definitions(${TARGET} PRIVATE
        BESHELL_PLATFORM_PC
        BESHELL_PLATFORM_PC_${_BESHELL_PC_SUB}
        CONFIG_QUICKJS_VERSION="${_BESHELL_QJS_VERSION}"
    )

    # MinGW 默认目标系统版本过低（缺少 GetTickCount64 等），提升到 Windows 7
    if(WIN32)
        target_compile_definitions(${TARGET} PRIVATE _WIN32_WINNT=0x0601)
    endif()

    # 编译选项（沿用组件根 CMakeLists.txt 的告警抑制风格，仅 GCC/Clang）
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET} PRIVATE
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
endfunction()

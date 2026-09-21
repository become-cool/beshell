cmake_minimum_required(VERSION 3.16)

# BeShell PC 平台构建工程（独立 CMake 工程，不经由 ESP-IDF 组件构建）
# 支持 Windows(MinGW/MSVC)、Linux、macOS，以及 Emscripten(WASM)
#
# 用法（MinGW 示例）：
#   cd pc
#   cmake -B build -G "MinGW Makefiles"
#   cmake --build build
#   ./build/beshell-pc.exe          # 运行 ./fs/main.js 并进入 REPL
#
# 延伸组件的 pc/ 工程只需 include 同目录的 beshell-pc-common.cmake 并调用
# beshell_pc_setup(<target>)，即可把 beshell 核心源码并入自身 target。

project(beshell-pc C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 公共构建片段：定义 beshell_pc_setup(TARGET)
include(${CMAKE_CURRENT_LIST_DIR}/beshell-pc-common.cmake)

add_executable(beshell-pc
    main.cpp
)

beshell_pc_setup(beshell-pc)

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
        # 模块化：导出 createBeShell 工厂函数，便于在网页/Node 中显式实例化并注入 print 等回调
        "-sMODULARIZE=1"
        "-sEXPORT_NAME=createBeShell"
    )
    # 产物为 beshell-wasm.js + beshell-wasm.wasm
    set_target_properties(beshell-pc PROPERTIES OUTPUT_NAME beshell-wasm SUFFIX ".js")
endif()

cmake_minimum_required(VERSION 3.12)
project(beshell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")


if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "-O0 -Wall -g -ggdb -std=c++17")
    set(CMAKE_C_FLAGS "-O0 -Wall -g -ggdb -std=gnu99")
elseif (${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_CXX_FLAGS "-O0 -Wall -std=c++17")
    set(CMAKE_C_FLAGS "-O0 -Wall -std=gnu99")
endif ()


LINK_LIBRARIES(m)
LINK_LIBRARIES(pthread)

add_definitions(-g)
add_definitions(-DLINUX_PLATFORM)
add_definitions(-DMG_ENABLE_OPENSSL)
# add_definitions(-DWIN32)
# add_definitions(-DMG_ENABLE_LINES)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_C_FLAGS_DEBUG "-O0")


set(SRC_LIST

        ../src/BeShell.cpp
        ../src/JSTimer.cpp
        ../src/JSEngine.cpp
        ../src/repl/REPL.cpp
        ../src/path.cpp
        ../src/string_utils.cpp
        ../src/qjs_utils.c
        ../src/mallocf.c

        ../src/NativeClass.cpp
        ../src/EventEmitter.cpp
        ../src/NativeModule.cpp
        ../src/ModuleLoader.cpp

        ../src/module/PathModule.cpp
        ../src/module/ProcessModule.cpp
        ../src/module/NVSModule.cpp

        ../src/basic/Console.cpp
        # ../src/driver/display/DisplayModule.cpp
        # ../src/driver/display/Display.cpp
        # ../src/driver/display/Bus8080.cpp
        # ../src/driver/display/Display8080.cpp

        ../src/telnet/Protocal.cpp
        ../src/telnet/Telnet.cpp
        ../src/telnet/TelnetChannel.cpp
        ../src/telnet/TelnetStdIO.cpp
        ../src/telnet/TelnetWS.cpp

        ../src/fs/FS.cpp
        ../src/fs/FSModule.cpp

        demo/Demo.cpp
        demo/DemoModule.cpp

        )

aux_source_directory(../deps/quickjs SRC_LIST)


include_directories(.)
include_directories(../src)
include_directories(../deps)
include_directories(../deps/littlefs/src/littlefs)


add_definitions(-ldl)


function(embed_js_src varname jspath hpath)

    execute_process(COMMAND stat -c %Y ${FILE1_PATH} OUTPUT_VARIABLE FILE1_TIMESTAMP)

    if(js_time STRGREATER h_time)
        message("===================")
    endif()

    # file(WRITE ${hpath} "#@pragma once\n\n")
    # file(APPEND ${hpath} "const char* ${varname} = R\"(")
    # file(APPEND ${hpath} "${FILE_CONTENT}")
    # file(APPEND ${hpath} ")\";\n")
endfunction()

embed_js_src(jsSrcWifi "${CMAKE_CURRENT_LIST_DIR}/../src/js/wifi.js", "${CMAKE_CURRENT_LIST_DIR}/../src/js/wifi.js.h")



# 添加可执行文件
add_executable(${PROJECT_NAME}  main.cpp ${SRC_LIST})

add_executable(mklfs  mklfs/mklfs.c ../deps/littlefs/src/littlefs/lfs.c ../deps/littlefs/src/littlefs/lfs_util.c ${SRC_LIST})
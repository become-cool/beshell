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

        ${CMAKE_SOURCE_DIR}/src/BeShell.cpp
        ${CMAKE_SOURCE_DIR}/src/JSTimer.cpp
        ${CMAKE_SOURCE_DIR}/src/JSEngine.cpp
        ${CMAKE_SOURCE_DIR}/src/repl/REPL.cpp
        ${CMAKE_SOURCE_DIR}/src/path.cpp
        ${CMAKE_SOURCE_DIR}/src/string_utils.cpp
        ${CMAKE_SOURCE_DIR}/src/qjs_utils.c
        ${CMAKE_SOURCE_DIR}/src/mallocf.c

        ${CMAKE_SOURCE_DIR}/src/NativeClass.cpp
        ${CMAKE_SOURCE_DIR}/src/NativeModule.cpp
        ${CMAKE_SOURCE_DIR}/src/ModuleLoader.cpp

        ${CMAKE_SOURCE_DIR}/src/module/PathModule.cpp
        ${CMAKE_SOURCE_DIR}/src/module/ProcessModule.cpp
        ${CMAKE_SOURCE_DIR}/src/module/NVSModule.cpp

        ${CMAKE_SOURCE_DIR}/src/basic/Console.cpp
        # ${CMAKE_SOURCE_DIR}/src/driver/display/DisplayModule.cpp
        # ${CMAKE_SOURCE_DIR}/src/driver/display/Display.cpp
        # ${CMAKE_SOURCE_DIR}/src/driver/display/Bus8080.cpp
        # ${CMAKE_SOURCE_DIR}/src/driver/display/Display8080.cpp

        ${CMAKE_SOURCE_DIR}/src/telnet/Protocal.cpp
        ${CMAKE_SOURCE_DIR}/src/telnet/Telnet.cpp
        ${CMAKE_SOURCE_DIR}/src/telnet/TelnetChannel.cpp
        ${CMAKE_SOURCE_DIR}/src/telnet/TelnetStdIO.cpp
        ${CMAKE_SOURCE_DIR}/src/telnet/TelnetWS.cpp

        ${CMAKE_SOURCE_DIR}/src/fs/FS.cpp
        ${CMAKE_SOURCE_DIR}/src/fs/FSModule.cpp

        demo/Demo.cpp
        demo/DemoModule.cpp

        )

aux_source_directory(${CMAKE_SOURCE_DIR}/deps/quickjs SRC_LIST)


include_directories(.)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/deps/littlefs/src/littlefs)




add_definitions(-ldl)


# 添加可执行文件
add_executable(${PROJECT_NAME}  main.cpp ${SRC_LIST})

add_executable(mklfs  mklfs/mklfs.c ${CMAKE_SOURCE_DIR}/deps/littlefs/src/littlefs/lfs.c ${CMAKE_SOURCE_DIR}/deps/littlefs/src/littlefs/lfs_util.c ${SRC_LIST})
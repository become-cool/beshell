cmake_minimum_required(VERSION 3.12)
project(beshell)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_CXX_FLAGS "-O0 -Wall -g -ggdb -std=c++11")
    set(CMAKE_C_FLAGS "-O0 -Wall -g -ggdb -std=gnu99")
elseif (${CMAKE_BUILD_TYPE} MATCHES Release)
    set(CMAKE_CXX_FLAGS "-O0 -Wall -std=c++11")
    set(CMAKE_C_FLAGS "-O0 -Wall -std=gnu99")
endif()


LINK_LIBRARIES(m)
LINK_LIBRARIES(pthread)

add_definitions(-g)
add_definitions(-DPLATFORM_LINUX)
add_definitions(-DMG_ENABLE_OPENSSL)
# add_definitions(-DWIN32)
# add_definitions(-DMG_ENABLE_LINES)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_C_FLAGS_DEBUG "-O0")


set(SRC_LIST
    main.cpp
    src/BeShell.cpp
    src/JSEngine.cpp
    src/REPL.cpp
    src/NVS.cpp
    src/path.c
    src/utils.c

    src/module/NativeModule.cpp
    src/module/ModuleLoader.cpp
    src/module/FSModule.cpp
    src/module/Path.cpp
    src/module/Console.cpp
    src/module/Process.cpp

    src/telnet/Protocal.cpp
    src/telnet/Telnet.cpp
    src/telnet/TelnetChannel.cpp
    src/telnet/TelnetStdIO.cpp
    src/telnet/TelnetWS.cpp

    src/fs/FS.cpp


)

aux_source_directory(src/deps/quickjs SRC_LIST)


include_directories(.)
include_directories(src)


add_definitions(-ldl)


# 添加可执行文件
add_executable(${PROJECT_NAME} ${SRC_LIST})
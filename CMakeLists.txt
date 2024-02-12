# project(beshll-component)
cmake_minimum_required(VERSION 3.24)
# 头文件目录
set(include_dirs ./src)

# 源码目录
set(src_dirs src)
list(APPEND src_dirs src/fs)
list(APPEND src_dirs src/repl)
list(APPEND src_dirs src/telnet)
list(APPEND src_dirs src/basic)
list(APPEND src_dirs src/module)
list(APPEND src_dirs src/module/serial)
list(APPEND src_dirs src/driver/display)
list(APPEND src_dirs src/lv)

# # little fs
list(APPEND src_dirs src/deps/littlefs/src)
list(APPEND src_dirs src/deps/littlefs/src/littlefs)

# quickjs
list(APPEND src_dirs src/deps/quickjs)

# # TinyUntar
# list(APPEND include_dirs deps/TinyUntar/tinyuntar)
# list(APPEND src_dirs deps/TinyUntar/tinyuntar)

# # mongoose
# list(APPEND include_dirs deps/mongoose)
# list(APPEND src_dirs deps/mongoose)

# # helix
# list(APPEND include_dirs deps/helix/include)
# list(APPEND src_dirs deps/helix/src)

# # esp32-perfmon
# list(APPEND include_dirs deps/esp32-perfmon/include)
# list(APPEND src_dirs deps/esp32-perfmon)


if (IDF_VERSION_MAJOR LESS 5)
    set(requires esp_adc_cal)
else ()
    set(requires esp_adc)
endif ()

idf_component_register(
        SRC_DIRS
            ${src_dirs}
        INCLUDE_DIRS
            ${include_dirs}
        REQUIRES 
            driver
            mbedtls
            vfs
            fatfs
            spi_flash
            esp_timer
            nvs_flash
            esp_wifi
            efuse
            esp_netif
            esp_ringbuf
            esp_lcd
            lvgl
            ${requires}
        EMBED_FILES fs-root.img
)

# # for mongoose tls/ssl
add_definitions(-DMG_ENABLE_MBEDTLS)
# add_definitions(-DMG_ENABLE_OPENSSL)
# add_definitions(-DMG_ENABLE_LINES)
# add_definitions(-DMBEDTLS_X509_CA_CHAIN_ON_DISK)

add_definitions(-DGNUBOY_NO_MINIZIP)
add_definitions(-DGNUBOY_NO_SCREENSHOT)
add_definitions(-DIS_LITTLE_ENDIAN)
set(CMAKE_CXX_STANDARD 11)

# for c
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-discarded-qualifiers")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-conversion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=maybe-uninitialized")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-pointer-arith")

# for c++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-permissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-brace")

if (COMMAND component_compile_options)
    # both c and c++
    component_compile_options(-Wno-missing-braces)
    component_compile_options(-Wno-error=format=)
    component_compile_options(-Wno-format)
    component_compile_options(-Wno-unused-function)
    component_compile_options(-Wno-unused-variable)
    component_compile_options(-Wno-unused-but-set-variable)
    component_compile_options(-Wno-cast-function-type)
    component_compile_options(-Wno-implicit-fallthrough)
    component_compile_options(-Wno-cpp)
    component_compile_options(-Wno-type-limits)
    component_compile_options(-Wno-int-to-pointer-cast)
    component_compile_options(-Wno-error=switch)
    component_compile_options(-Wno-switch)
    component_compile_options(-Wno-missing-field-initializers)
endif ()

# string(REPLACE "-fno-rtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#clang
if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_definitions(-D__STDC_NO_ATOMICS__=1)
endif ()
message("Operating System: ${CMAKE_SYSTEM_NAME}")
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_subdirectory(pc)
endif ()
# project(beshll-component)
cmake_minimum_required(VERSION 3.24)
# 头文件目录
set(include_dirs
        ./src
        ./deps
    )

# 源码目录
file(GLOB_RECURSE src_beshell
        src/*.c
        src/*.cpp
    )

# little fs
set(src_littlefs
        deps/littlefs/src/esp_littlefs.c
        deps/littlefs/src/littlefs_api.c
        deps/littlefs/src/littlefs/lfs.c
        deps/littlefs/src/littlefs/lfs_util.c
    )

# quickjs
file(GLOB_RECURSE src_quickjs deps/quickjs/*.c)

# lvgl
file(GLOB_RECURSE src_lvgl deps/lvgl/src/*.c)
list(APPEND include_dirs deps/lvgl)
list(APPEND include_dirs deps/lvgl/src)

# # TinyUntar
# list(APPEND include_dirs deps/TinyUntar/tinyuntar)
# list(APPEND src_dirs deps/TinyUntar/tinyuntar)

# mongoose
file(GLOB_RECURSE src_mg deps/mongoose/*.c)
# list(APPEND include_dirs deps/mongoose)
# list(APPEND src_dirs deps/mongoose)

# # helix
# list(APPEND include_dirs deps/helix/include)
# list(APPEND src_dirs deps/helix/src)

# # esp32-perfmon
# list(APPEND include_dirs deps/esp32-perfmon/include)
# list(APPEND src_dirs deps/esp32-perfmon)

# sqlite3
file(GLOB_RECURSE src_sqlite3
    deps/sqlite3/*.c
    deps/sqlite3/*.cpp
)



if (IDF_VERSION_MAJOR LESS 5)
    set(requires esp_adc_cal)
else ()
    set(requires esp_adc)
endif ()


# function(embed_js_src varname jspath hpath)
#     file(WRITE ${hpath} "#@pragma once\n\n")
#     file(APPEND ${hpath} "const char* ${varname} = R\"(")
#     file(APPEND ${hpath} "${FILE_CONTENT}")
#     file(APPEND ${hpath} ")\";\n")
# endfunction()

# # embed_js_src(jsSrcWifi "${CMAKE_CURRENT_LIST_DIR}/src/js/wifi.js", "${CMAKE_CURRENT_LIST_DIR}/src/js/wifi.js.h")


# file(READ ${CMAKE_CURRENT_LIST_DIR}/src/js/wifi.js WIFI_SRC)
# set(HEADER_TEMPLATE ${CMAKE_CURRENT_LIST_DIR}/src/js/wifi.js.tpl.h)
# set(HEADER_FILE ${CMAKE_CURRENT_LIST_DIR}/src/js/wifi.js.h)

# configure_file(${HEADER_TEMPLATE} ${HEADER_FILE} @ONLY)


idf_component_register(
        SRCS
            ${src_beshell}
            ${src_quickjs}
            ${src_littlefs}
            ${src_lvgl}
            ${src_mg}
            ${src_sqlite3}
        # SRC_DIRS
        #     ${src_dirs}
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
            ${requires}
        # EMBED_FILES src/js/wifi.js
)

# for lvgl
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)


# # for mongoose tls/ssl
add_definitions(-DMG_ENABLE_MBEDTLS)
add_definitions(-DMG_ARCH_ESP32)
# add_definitions(-DMG_ENABLE_OPENSSL)
# add_definitions(-DMG_ENABLE_LINES)
# add_definitions(-DMBEDTLS_X509_CA_CHAIN_ON_DISK)

# for sqlite3
# add_definitions(-DSQLITE_COVERAGE_TEST)
# add_definitions(-DSQLITE_DEBUG)

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

target_compile_options(${COMPONENT_LIB} PRIVATE -fno-stack-protector -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -Wpointer-arith -Wno-error=unused-value -Wno-error=unused-label -Wno-error=unused-function -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wno-error=char-subscripts -Wno-error=maybe-uninitialized -Wno-unused-parameter -Wno-sign-compare -Wno-error=misleading-indentation -Wno-error=stringop-truncation)

# for c++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-permissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-brace")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-enum-enum-conversion")

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
# c 头文件目录
set(include_dirs ./src)
list(APPEND include_dirs src/telnet)
list(APPEND include_dirs src/libb64)
list(APPEND include_dirs src/media)
list(APPEND include_dirs src/media/TinySoundFont)
list(APPEND include_dirs src/media/audio_stream)
list(APPEND include_dirs src/media/tjpgd)
list(APPEND include_dirs src/encoding)
list(APPEND include_dirs src/led)
list(APPEND include_dirs src/led/rainbow)
list(APPEND include_dirs src/modules)
# list(APPEND include_dirs src/modules/http)
list(APPEND include_dirs src/modules/serial)
list(APPEND include_dirs src/modules/mg)
list(APPEND include_dirs src/modules/driver)
list(APPEND include_dirs src/modules/media)

# c 源码目录
set(src_dirs src)
list(APPEND src_dirs src/telnet)
list(APPEND src_dirs src/libb64)


# list(APPEND src_dirs src/media)
list(APPEND src_dirs src/media/TinySoundFont)
list(APPEND src_dirs src/media/audio_stream)
list(APPEND src_dirs src/media/tjpgd)
list(APPEND src_dirs src/encoding)
list(APPEND src_dirs src/led)
list(APPEND src_dirs src/led/rainbow)
list(APPEND src_dirs src/modules)
# list(APPEND src_dirs src/modules/http)
list(APPEND src_dirs src/modules/mg)
list(APPEND src_dirs src/modules/driver)
list(APPEND src_dirs src/modules/media)
list(APPEND src_dirs src/modules/serial)

# little fs
list(APPEND include_dirs deps/littlefs/src)
list(APPEND include_dirs deps/littlefs/src/littlefs)
list(APPEND src_dirs deps/littlefs/src)
list(APPEND src_dirs deps/littlefs/src/littlefs)

# quickjs
list(APPEND include_dirs deps/quickjs)
list(APPEND src_dirs deps/quickjs)

# TinyUntar
list(APPEND include_dirs deps/TinyUntar/tinyuntar)
list(APPEND src_dirs deps/TinyUntar/tinyuntar)

# mongoose
list(APPEND include_dirs deps/mongoose)
list(APPEND src_dirs deps/mongoose)

# helix
list(APPEND include_dirs deps/helix/include)
list(APPEND src_dirs deps/helix/src)

# esp32-perfmon
list(APPEND include_dirs deps/esp32-perfmon/include)
list(APPEND src_dirs deps/esp32-perfmon)


if(IDF_VERSION_MAJOR LESS 5)
    set(requires driver mbedtls vfs fatfs spi_flash esp_timer nvs_flash esp_wifi efuse esp_netif esp_ringbuf esp_adc_cal)
else()
    set(requires driver mbedtls vfs fatfs spi_flash esp_timer nvs_flash esp_wifi efuse esp_netif esp_ringbuf esp_adc)
endif()


idf_component_register(
    SRC_DIRS ${src_dirs}
    INCLUDE_DIRS ${include_dirs}
    REQUIRES ${requires}
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
add_definitions(-DPLATFORM_ESP32)

component_compile_options(-Wno-error=format=)
component_compile_options(-Wno-format)
component_compile_options(-Wno-incompatible-pointer-types)
component_compile_options(-Wno-unused-function)
component_compile_options(-Wno-unused-variable)
component_compile_options(-Wno-unused-but-set-variable)
component_compile_options(-Wno-cast-function-type)
component_compile_options(-Wno-implicit-fallthrough)
component_compile_options(-Wno-discarded-qualifiers)
component_compile_options(-Wno-int-conversion)
component_compile_options(-Wno-cpp)
component_compile_options(-Wno-type-limits)
component_compile_options(-Wno-int-to-pointer-cast)
component_compile_options(-Wno-implicit-function-declaration)
# component_compile_options(-Wno-error=maybe-uninitialized)

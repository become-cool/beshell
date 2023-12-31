# c 头文件目录
set(include_dirs .)
list(APPEND include_dirs telnet)
list(APPEND include_dirs libb64)
list(APPEND include_dirs gameplayer)
list(APPEND include_dirs gameplayer/nofrendo)
list(APPEND include_dirs gameplayer/nofrendo/cpu)
list(APPEND include_dirs gameplayer/nofrendo/libsnss)
list(APPEND include_dirs gameplayer/nofrendo/nes)
list(APPEND include_dirs gameplayer/nofrendo/sndhrdw)
list(APPEND include_dirs media)
list(APPEND include_dirs media/TinySoundFont)
list(APPEND include_dirs media/audio_stream)
list(APPEND include_dirs media/tjpgd)
list(APPEND include_dirs encoding)
list(APPEND include_dirs led)
list(APPEND include_dirs led/rainbow)
list(APPEND include_dirs modules)
# list(APPEND include_dirs modules/http)
list(APPEND include_dirs modules/serial)
list(APPEND include_dirs modules/mg)
list(APPEND include_dirs modules/driver)
list(APPEND include_dirs modules/media)
list(APPEND include_dirs modules/lvgl)
list(APPEND include_dirs modules/lvgl/driver)
list(APPEND include_dirs modules/lvgl/font)

# c 源码目录
set(src_dirs .)
list(APPEND src_dirs telnet)
list(APPEND src_dirs libb64)

list(APPEND src_dirs gameplayer)
list(APPEND src_dirs gameplayer/nofrendo)
list(APPEND src_dirs gameplayer/nofrendo/cpu)
list(APPEND src_dirs gameplayer/nofrendo/libsnss)
list(APPEND src_dirs gameplayer/nofrendo/nes)
list(APPEND src_dirs gameplayer/nofrendo/sndhrdw)
list(APPEND src_dirs gameplayer/nofrendo/mappers)


# list(APPEND src_dirs media)
list(APPEND src_dirs media/TinySoundFont)
list(APPEND src_dirs media/audio_stream)
list(APPEND src_dirs media/tjpgd)
list(APPEND src_dirs encoding)
list(APPEND src_dirs led)
list(APPEND src_dirs led/rainbow)
list(APPEND src_dirs modules)
# list(APPEND src_dirs modules/http)
list(APPEND src_dirs modules/mg)
list(APPEND src_dirs modules/driver)
list(APPEND src_dirs modules/media)
list(APPEND src_dirs modules/serial)
list(APPEND src_dirs modules/lvgl)
list(APPEND src_dirs modules/lvgl/driver)
list(APPEND src_dirs modules/lvgl/font)

idf_component_register(
    SRC_DIRS ${src_dirs}
    INCLUDE_DIRS ${include_dirs}
    # EMBED_FILES root.raw
)

# # for mongoose tls/ssl
add_definitions(-DMG_ENABLE_MBEDTLS)
# add_definitions(-DMG_ENABLE_OPENSSL)

# # for nofrendo
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-error=char-subscripts -Wno-error=attributes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=char-subscripts -Wno-error=attributes")


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
component_compile_options(-Wno-error=char-subscripts)
component_compile_options(-Wno-error=misleading-indentation)
component_compile_options(-Wno-attributes)
component_compile_options(-Wno-char-subscripts)

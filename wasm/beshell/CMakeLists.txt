cmake_minimum_required(VERSION 3.10)

project(beshell)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")


LINK_LIBRARIES(m)
LINK_LIBRARIES(pthread)

add_definitions(-g)
add_definitions(-DPLATFORM_WASM)
# add_definitions(-DMG_ENABLE_OPENSSL)
# add_definitions(-DWIN32)
# add_definitions(-DMG_ENABLE_LINES)
# add_definitions(-DLV_THEME_DEFAULT_DARK)
# add_definitions(-DLV_CONF_INCLUDE_SIMPLE)


SET(CMAKE_CXX_FLAGS_DEBUG "-Oz")
SET(CMAKE_C_FLAGS_DEBUG "-Oz")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -D ASMJS -gsource-map")
set(LINK_FLAGS "${LINK_FLAGS} -s WASM=1")
set(LINK_FLAGS "${LINK_FLAGS} -s EXIT_RUNTIME")
set(LINK_FLAGS "${LINK_FLAGS} -s EXPORTED_RUNTIME_METHODS='[ccall, cwrap, callMain, writeStringToMemory, UTF8ToString, stringToUTF8, addOnInit]'")
set(LINK_FLAGS "${LINK_FLAGS} -lidbfs.js")
# set(LINK_FLAGS "${LINK_FLAGS} --preload-file fs-root")

# ssl
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)
# add_definitions(-lssl -lcrypto -ldl)


# lvgl
# add_definitions(-DLV_CONF_SKIP)
include_directories(../../components/lvgl/src)
file(GLOB_RECURSE SRC_LIST "../../components/lvgl/src/*.c")

# quickjs
aux_source_directory(../../components/quickjs SRC_LIST)
include_directories(../../components/quickjs)

# mongoose
aux_source_directory(../../components/mongoose SRC_LIST)
include_directories(../../components/mongoose)



# hexli
include_directories(../../components/helix/include)
aux_source_directory(../../components/helix/src SRC_LIST)


aux_source_directory(./src SRC_LIST)

include_directories(./src)
include_directories(../../main)
include_directories(../../main/telnet)
include_directories(../../main/media)
include_directories(../../main/media/audio_stream)
include_directories(../../main/media/TinySoundFont)
include_directories(../../main/modules)
include_directories(../../main/modules/media)
include_directories(../../main/modules/lvgl)
include_directories(../../main/modules/lvgl/font)

list(APPEND SRC_LIST ../../main/utils.c)
list(APPEND SRC_LIST ../../main/eventloop.c)
list(APPEND SRC_LIST ../../main/telnet/module_telnet.c)
list(APPEND SRC_LIST ../../main/telnet/telnet_protocal.c)
list(APPEND SRC_LIST ../../main/telnet/telnet_wasm.c)
list(APPEND SRC_LIST ../../main/js_main_loop.c)
list(APPEND SRC_LIST ../../main/mallocf.c)
list(APPEND SRC_LIST ../../main/stack.c)
list(APPEND SRC_LIST ../../main/be_list.c)
list(APPEND SRC_LIST ../../main/uuid.c)
list(APPEND SRC_LIST ../../main/font/s16.c)
list(APPEND SRC_LIST ../../main/rawfs.c)
list(APPEND SRC_LIST ../../main/debug.c)
list(APPEND SRC_LIST ../../main/media/TinySoundFont/tml.c)
list(APPEND SRC_LIST ../../main/media/TinySoundFont/tsf.c)
# list(APPEND SRC_LIST ../../main/media/tiny_jpeg.c)
list(APPEND SRC_LIST ../../main/modules/module_fs.c)
list(APPEND SRC_LIST ../../main/modules/module_utils.c)
list(APPEND SRC_LIST ../../main/modules/module_process.c)
list(APPEND SRC_LIST ../../main/modules/module_metadata.c)
list(APPEND SRC_LIST ../../main/modules/module_nvs.c)
list(APPEND SRC_LIST ../../main/modules/media/module_media.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/module_lvgl.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/display.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/indev.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/indev_pointer.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/indev_i2c.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/style.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/be_lv_struct_wrapper.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/be_lv_draggable.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/widgets.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/widgets_gen.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/be_gl.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/be_gl_js.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/font/font_msyh_3500.c)
list(APPEND SRC_LIST ../../main/modules/lvgl/lv_obj_init.js.c)


# game emulator
include_directories(../../main/gameplayer)
include_directories(../../main/gameplayer/nofrendo)
include_directories(../../main/gameplayer/nofrendo/cpu)
include_directories(../../main/gameplayer/nofrendo/libsnss)
include_directories(../../main/gameplayer/nofrendo/nes)
include_directories(../../main/gameplayer/nofrendo/sndhrdw)

file(GLOB_RECURSE src_gameplayer "../../main/gameplayer/nofrendo/*.c")
SET (SRC_LIST 
    ${SRC_LIST}
    ${src_gameplayer}
)

add_definitions(-DNOFRENDO_DEBUG)

include(wasm_wrapper.cmake)

add_executable(beshell ${SRC_LIST})
target_link_libraries(beshell libssl.a libcrypto.a dl)
target_link_libraries(beshell dl pthread)

set_property(TARGET beshell PROPERTY C_STANDARD 99)
set_target_properties(beshell PROPERTIES LINK_FLAGS "${LINK_FLAGS}")


# add_custom_command(OUTPUT beapi.simulate.c
#     PRE_BUILD
#     COMMAND xxd -i ../src/beapi.simulate.js beapi.simulate.c
#     COMMAND echo "xxd -i ../src/beapi.simulate.js beapi.simulate.c"
#     DEPENDS ../src/beapi.simulate.js
#     VERBATIM
# )
cmake_minimum_required(VERSION 3.10)

project(beshell)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")


LINK_LIBRARIES(m)
LINK_LIBRARIES(pthread)

add_definitions(-g)
add_definitions(-DPLATFORM_LINUX)
add_definitions(-DMG_ENABLE_OPENSSL)
# add_definitions(-DWIN32)
# add_definitions(-DMG_ENABLE_LINES)
add_definitions(-DLV_THEME_DEFAULT_DARK)
add_definitions(-DLV_CONF_INCLUDE_SIMPLE)


SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_C_FLAGS_DEBUG "-O0")


# ssl
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/)
add_definitions(-lssl -lcrypto -ldl)


# lvgl
# add_definitions(-DLV_CONF_SKIP)
# include_directories(../deps/lvgl/src)
# file(GLOB_RECURSE SRC_LIST "../deps/lvgl/src/*.c")

# quickjs
aux_source_directory(../deps/quickjs SRC_LIST)
include_directories(../deps/quickjs)

# mongoose
# aux_source_directory(../deps/mongoose SRC_LIST)
# include_directories(../deps/mongoose)



# hexli
include_directories(../deps/helix/include)
aux_source_directory(../deps/helix/src SRC_LIST)


aux_source_directory(./src SRC_LIST)

include_directories(./src)
include_directories(../src)
include_directories(../src/telnet)
include_directories(../src/anchor9)
include_directories(../src/media)
include_directories(../src/media/audio_stream)
include_directories(../src/media/TinySoundFont)
include_directories(../src/modules)
include_directories(../src/modules/mg)
# include_directories(../src/modules/lvgl)
# include_directories(../src/modules/lvgl/font)

list(APPEND SRC_LIST ../src/be_list.c)
list(APPEND SRC_LIST ../src/utils.c)
list(APPEND SRC_LIST ../src/runtime.c)
list(APPEND SRC_LIST ../src/eventloop.c)
list(APPEND SRC_LIST ../src/telnet/module_telnet.c)
# list(APPEND SRC_LIST ../src/telnet/telnet_ws.c)
list(APPEND SRC_LIST ../src/telnet/telnet_stdio.c)
list(APPEND SRC_LIST ../src/telnet/telnet_protocal.c)
list(APPEND SRC_LIST ../src/js_main_loop.c)
list(APPEND SRC_LIST ../src/mallocf.c)
list(APPEND SRC_LIST ../src/stack.c)
# list(APPEND SRC_LIST ../src/list.c)
list(APPEND SRC_LIST ../src/uuid.c)
# list(APPEND SRC_LIST ../src/font/s16.c)
# list(APPEND SRC_LIST ../src/rawfs.c)
list(APPEND SRC_LIST ../src/debug.c)
# list(APPEND SRC_LIST ../src/anchor9/anchor9.c)
# list(APPEND SRC_LIST ../src/anchor9/anchor9_lvgl.c)
list(APPEND SRC_LIST ../src/media/TinySoundFont/tml.c)
list(APPEND SRC_LIST ../src/media/TinySoundFont/tsf.c)
# list(APPEND SRC_LIST ../src/media/tiny_jpeg.c)
list(APPEND SRC_LIST ../src/modules/module_fs.c)
list(APPEND SRC_LIST ../src/modules/module_utils.c)
# list(APPEND SRC_LIST ../src/modules/mg/module_mg.c)
# list(APPEND SRC_LIST ../src/modules/mg/mg_request.c)
# list(APPEND SRC_LIST ../src/modules/mg/mg_response.c)
# list(APPEND SRC_LIST ../src/modules/mg/mg_client.c)
# list(APPEND SRC_LIST ../src/modules/mg/mg_server.c)
list(APPEND SRC_LIST ../src/modules/module_process.c)
list(APPEND SRC_LIST ../src/modules/module_metadata.c)
list(APPEND SRC_LIST ../src/modules/module_nvs.c)
list(APPEND SRC_LIST ../src/modules/media/module_media.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/module_lvgl.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/display.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/indev.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/indev_pointer.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/style.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/be_lv_struct_wrapper.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/be_lv_draggable.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/widgets.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/widgets_gen.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/be_gl.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/be_gl_js.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/font/font_msyh_3500.c)
# list(APPEND SRC_LIST ../src/modules/lvgl/lv_obj_init.js.c)
list(APPEND SRC_LIST beapi.simulate.c)


# game emulator
# include_directories(../src/gameplayer)
# include_directories(../src/gameplayer/nofrendo)
# include_directories(../src/gameplayer/nofrendo/cpu)
# include_directories(../src/gameplayer/nofrendo/libsnss)
# include_directories(../src/gameplayer/nofrendo/nes)
# include_directories(../src/gameplayer/nofrendo/sndhrdw)

# file(GLOB_RECURSE src_gameplayer "../src/gameplayer/nofrendo/*.c")
# SET (SRC_LIST 
#     ${SRC_LIST}
#     ${src_gameplayer}
# )

add_definitions(-DNOFRENDO_DEBUG)


add_executable(beshell ${SRC_LIST})
target_link_libraries(beshell libssl.a libcrypto.a dl)
target_link_libraries(beshell dl pthread)

set_property(TARGET beshell PROPERTY C_STANDARD 99)


add_custom_command(OUTPUT beapi.simulate.c
    PRE_BUILD
    COMMAND xxd -i ../src/beapi.simulate.js beapi.simulate.c
    COMMAND echo "xxd -i ../src/beapi.simulate.js beapi.simulate.c"
    DEPENDS ../src/beapi.simulate.js
    VERBATIM
)

add_definitions(-Wno-error=format=)
add_definitions(-Wno-format)
add_definitions(-Wno-incompatible-pointer-types)
add_definitions(-Wno-unused-function)
add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-but-set-variable)
add_definitions(-Wno-cast-function-type)
add_definitions(-Wno-implicit-fallthrough)
add_definitions(-Wno-discarded-qualifiers)
add_definitions(-Wno-int-conversion)
add_definitions(-Wno-cpp)
add_definitions(-Wno-type-limits)
add_definitions(-Wno-int-to-pointer-cast)
add_definitions(-Wno-endif-labels)
add_definitions(-Wno-implicit-function-declaration)

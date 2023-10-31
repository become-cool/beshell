cmake_minimum_required(VERSION 3.12)
project (lvgl_api)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -D ASMJS -DLV_USE_FLEX -gsource-map")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(../../components/lvgl/src)
include_directories(../../main/modules/lvgl)
include_directories(../../main/modules/lvgl/font)
include_directories(./src)

file(GLOB_RECURSE INCLUDES "./src/*.h")

file(GLOB_RECURSE LIB_SOURCES "../../components/lvgl/*.c")
file(GLOB MY_SOURCES "./src/*.c" "../../main/modules/lvgl/font/font_msyh.c")

set(SOURCES ${LIB_SOURCES} ${MY_SOURCES})

add_executable(lvgl_api ${SOURCES} ${INCLUDES})

set(CMAKE_EXECUTABLE_SUFFIX ".html")

# configure_file(./src/index.html ./ COPYONLY)
# configure_file(./src/test.js ./ COPYONLY)
configure_file(./src/lvgl.js ./ COPYONLY)

# set(LINK_FLAGS "${LINK_FLAGS} --shell-file ${PROJECT_SOURCE_DIR}/src/lvgl_shell.html")
set(LINK_FLAGS "${LINK_FLAGS} -s WASM=1")
set(LINK_FLAGS "${LINK_FLAGS} -s EXPORTED_RUNTIME_METHODS='[ccall, cwrap, writeStringToMemory, UTF8ToString, stringToUTF8, addOnInit]'")

include(wasm_wrapper.cmake)

set_target_properties(lvgl_api PROPERTIES LINK_FLAGS "${LINK_FLAGS}")

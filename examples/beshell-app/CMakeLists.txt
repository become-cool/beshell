
# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# js 源文件目录
set(JS_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/js")             

# js 镜像文件路径
set(JS_IMG_PATH "${CMAKE_CURRENT_LIST_DIR}/img/js.bin")


# 如果存在 CMakeLists.private.txt，则包含它（可用于覆盖 EXTRA_COMPONENT_DIRS 等配置）
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.private.txt")
    include("${CMAKE_CURRENT_LIST_DIR}/CMakeLists.private.txt")
endif()

# 自动搜索并设置外部 component 目录（如果 CMakeLists.private.txt 未定义）
if(NOT DEFINED EXTRA_COMPONENT_DIRS)
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../../components")
        set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../../components")
    elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../components")
        set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/../components")
    endif()
endif()

# 自动搜索 beshell/tools 目录（如果 CMakeLists.private.txt 未定义）
if(NOT DEFINED BESHELL_TOOLS_DIR)
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../../components/beshell/tools")
        set(BESHELL_TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}/../../components/beshell/tools")
    elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../components/beshell/tools")
        set(BESHELL_TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}/../components/beshell/tools")
    elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/components/beshell/tools")
        set(BESHELL_TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}/components/beshell/tools")
    else()
        set(BESHELL_TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}/managed_components/become-cool__beshell/tools")
    endif()
endif()

# 设置工具路径
set(MKLITTLEFS_PATH "${BESHELL_TOOLS_DIR}/mklittlefs.exe")
set(JS_TO_C_PATH "${BESHELL_TOOLS_DIR}/js-to-c.js")


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(beshell-app)


# 自动打包 js.bin
file(GLOB_RECURSE JS_SRC_FILES
    RELATIVE ${JS_SRC_DIR}
    ${JS_SRC_DIR}/*.js
)
list(TRANSFORM JS_SRC_FILES PREPEND ${JS_SRC_DIR}/)

add_custom_command(
    OUTPUT "${JS_IMG_PATH}"
    COMMAND echo "Packing javascipt img ..."
    COMMAND ${MKLITTLEFS_PATH} -c "${JS_SRC_DIR}" -s 1048576 "${JS_IMG_PATH}" -d 5
    DEPENDS ${JS_SRC_FILES}
    VERBATIM
)

# idf.py build 时，同时也会打包 js.bin
add_custom_target(pack-js ALL DEPENDS "${JS_IMG_PATH}")

# 烧录 js 文件系统镜像
add_custom_target(flash-js
    COMMAND echo "Flashing javascipt img ..."
    COMMAND python ${IDF_PATH}/components/esptool_py/esptool/esptool.py
        --chip ${IDF_TARGET}
        --baud 921600
        --after hard_reset
    write_flash
        0x610000
        "${JS_IMG_PATH}"
    DEPENDS ${CUSTOM_BIN_PATH}
    VERBATIM
)

# idf.py flash 时，同时也自动烧录 js 文件系统镜像
add_dependencies(flash flash-js)



# 自动包含 CMakeLists.update_exmaple.txt（如果存在）
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.update_example.txt")
    include("${CMAKE_CURRENT_LIST_DIR}/CMakeLists.update_example.txt")
endif()

# js-lib 目录
set(JS_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/js-lib")



# 查找 js-lib 中的所有 .js 文件
file(GLOB JS_LIB_FILES "${JS_LIB_DIR}/*.js")

# 为每个 .js 文件生成对应的 .c 文件
set(JS_C_FILES "")
foreach(JS_FILE ${JS_LIB_FILES})
    get_filename_component(JS_FILENAME ${JS_FILE} NAME)
    string(REPLACE ".js" ".c" JS_C_FILENAME ${JS_FILENAME})
    set(JS_C_FILE "${JS_LIB_DIR}/${JS_C_FILENAME}")
    list(APPEND JS_C_FILES ${JS_C_FILE})
    
    # 只有当 .js 文件比 .c 文件更新时才重新编译
    add_custom_command(
        OUTPUT ${JS_C_FILE}
        COMMAND echo "Compiling ${JS_FILENAME} to ${JS_C_FILENAME} ..."
        COMMAND node ${JS_TO_C_PATH} ${JS_FILE} ${JS_C_FILE}
        DEPENDS ${JS_FILE}
        VERBATIM
    )
endforeach()

# 添加 gen-c 目标，用于将 js-lib 中的 .js 文件编译为 .c
add_custom_target(gen-c DEPENDS ${JS_C_FILES})

# idf.py build 时自动调用 gen-c
add_dependencies(app gen-c)
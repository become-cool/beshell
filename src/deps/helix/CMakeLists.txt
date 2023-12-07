cmake_minimum_required(VERSION 3.10)


file(GLOB SOURCES src/*.c)
idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS include
)

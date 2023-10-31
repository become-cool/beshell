cmake_minimum_required(VERSION 3.5)

idf_component_register(
    SRC_DIRS .
    REQUIRES mbedtls
    INCLUDE_DIRS .)

component_compile_options(-DMG_ENABLE_MBEDTLS)
# component_compile_options(-DMG_ENABLE_LINES)
# component_compile_options(-DMBEDTLS_X509_CA_CHAIN_ON_DISK)
component_compile_options(-DMG_ARCH_ESP32)




cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(rp2040_nesemu)

pico_sdk_init()

add_subdirectory(main)
add_subdirectory(components)
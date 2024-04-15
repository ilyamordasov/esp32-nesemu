set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/duser/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(rp2040_nesemu C CXX ASM)

add_executable(rp2040_nesemu 
      main.c
      usb_descriptors.c )

pico_set_program_name(rp2040_nesemu "rp2040_nesemu")
pico_set_program_version(rp2040_nesemu "0.1")

pico_enable_stdio_uart(rp2040_nesemu 1)
pico_enable_stdio_usb(rp2040_nesemu 0)

# Add the standard library to the build
target_link_libraries(rp2040_nesemu PUBLIC
        pico_stdlib
        tinyusb_device 
        tinyusb_board
        w25qxx
        msc
)

# Add the standard include files to the build
target_include_directories(rp2040_nesemu PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/..  
  ${CMAKE_CURRENT_LIST_DIR}/main # for our common lwipopts or any other standard includes, if required
  $ENV{PICO_SDK_PATH}/lib/tinyusb/hw
  $ENV{PICO_SDK_PATH}/lib/tinyusb/src
)

pico_add_extra_outputs(rp2040_nesemu)
add_library(msc msc.c)

target_sources(msc INTERFACE
    ${CMAKE_SOURCE_DIR}/components/w25qxx/w25qxx.c
    ${CMAKE_SOURCE_DIR}/components/msc/msc.c
)

target_include_directories(msc PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $ENV{PICO_SDK_PATH}/lib/tinyusb/hw
    $ENV{PICO_SDK_PATH}/lib/tinyusb/src
    ../../main
    ../w25qxx
    ${CMAKE_CURRENT_LIST_DIR}/main
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(msc PUBLIC
        tinyusb_device
        tinyusb_board
        pico_stdlib
        w25qxx
        hardware_spi
        hardware_dma
        hardware_rtc
)
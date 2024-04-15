add_library(msc msc.c)

target_sources(msc INTERFACE
    ${CMAKE_SOURCE_DIR}/components/w25qxx/w25qxx.c
    ${CMAKE_SOURCE_DIR}/components/msc/msc.c
)

target_include_directories(msc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/main)

target_link_libraries(msc INTERFACE
        w25qxx
        hardware_spi
        hardware_dma
        hardware_rtc
        tinyusb_board
        pico_stdlib
)
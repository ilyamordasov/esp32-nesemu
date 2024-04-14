add_library(w25qxx
    w25qxx.c
)

target_include_directories(w25qxx PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(w25qxx INTERFACE
        hardware_spi
        hardware_dma
        hardware_rtc
        tinyusb_board
        pico_stdlib
)
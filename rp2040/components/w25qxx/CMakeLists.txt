add_library(w25qxx
    w25qxx.c
)

target_include_directories(w25qxx PUBLIC ${CMAKE_CURRENT_LIST_DIR}/main)

target_link_libraries(w25qxx PUBLIC
        pico_stdlib
        tinyusb_board
        hardware_spi
        hardware_dma
        hardware_rtc
)
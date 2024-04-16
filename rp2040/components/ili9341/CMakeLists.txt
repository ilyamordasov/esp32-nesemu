add_library(ili9341
    ili9341.c font.c
)

target_include_directories(ili9341 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/main
    ../w25qxx
)

target_link_libraries(ili9341 PUBLIC
        w25qxx
        pico_stdlib
        hardware_spi
        hardware_dma
        hardware_rtc
)
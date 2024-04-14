add_library(msc msc.c)

target_sources(msc INTERFACE
    ${CMAKE_SOURCE_DIR}/components/w25qxx/w25qxx.c
    ${CMAKE_SOURCE_DIR}/components/msc/msc.c
)

target_include_directories(mcs PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(msc w25qxx INTERFACE
        hardware_spi
        hardware_dma
        hardware_rtc
        pico_stdlib
)
add_library(msc INTERFACE)
target_sources(msc INTERFACE
    ${CMAKE_SOURCE_DIR}/components/w25qxx/w25qxx.c
    ${CMAKE_SOURCE_DIR}/components/msc/msc.c
)

target_include_directories(msc INTERFACE
    ${CMAKE_SOURCE_DIR}/components/w25qxx
    ${CMAKE_SOURCE_DIR}/components/msc
)

target_link_libraries(msc INTERFACE
        hardware_spi
        hardware_dma
        hardware_rtc
        pico_stdlib
)
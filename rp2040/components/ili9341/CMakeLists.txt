add_library(ili9341 ili9341.c font.c)

target_sources(ili9341 INTERFACE
    ${CMAKE_SOURCE_DIR}/components/w25qxx/w25qxx.c
    ${CMAKE_SOURCE_DIR}/components/msc/msc.c
)

target_include_directories(ili9341 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ../../main
    ../w25qxx
    ${CMAKE_CURRENT_LIST_DIR}/main
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ili9341 PUBLIC
        pico_stdlib
        w25qxx
        hardware_spi
        hardware_dma
)
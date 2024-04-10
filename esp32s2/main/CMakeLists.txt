set(priv_requires driver fatfs console)

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)

list(APPEND priv_requires w25qxx msc wear_levelling esp_partition spi_flash esp_event console nvs_flash esp_partition)

idf_component_register(
    SRCS ${app_sources}
    INCLUDE_DIRS .
    PRIV_REQUIRES "${priv_requires}"
)
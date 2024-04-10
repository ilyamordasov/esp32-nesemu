idf_component_register(
    SRCS msc.c
    PRIV_REQUIRES driver wear_levelling esp_partition spi_flash fatfs vfs console
    INCLUDE_DIRS "."
)
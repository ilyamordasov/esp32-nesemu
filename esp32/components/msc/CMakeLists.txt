idf_component_register(
    SRCS w25qxx.c
    PRIV_REQUIRES driver wear_levelling esp_partition spi_flash fatfs vfs
    INCLUDE_DIRS "."
)
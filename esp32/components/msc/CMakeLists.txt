idf_component_register(
    SRCS msc.c
    PRIV_REQUIRES w25qxx driver wear_levelling esp_partition spi_flash fatfs vfs console esp_tinyusb
    INCLUDE_DIRS "."
)
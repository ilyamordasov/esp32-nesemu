set(component_srcs "w25qxx.c")

idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver wear_levelling esp_partition spi_flash fatfs vfs
                       INCLUDE_DIRS ".")
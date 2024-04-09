set(priv_requires driver fatfs console)

list(APPEND priv_requires wear_levelling esp_partition spi_flash esp_event nvs_flash esp_partition)

idf_component_register(
    SRCS
    "main.c"
    INCLUDE_DIRS .
    PRIV_REQUIRES "${priv_requires}"
)
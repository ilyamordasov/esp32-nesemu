set(component_srcs "w25qxx.c")

idf_component_register(SRCS "${component_srcs}"
                       PRIV_REQUIRES driver spi_flash
                       INCLUDE_DIRS ".")
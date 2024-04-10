idf_component_register(
    SRCS serial.c
    PRIV_REQUIRES driver esp_event
    INCLUDE_DIRS "."
)
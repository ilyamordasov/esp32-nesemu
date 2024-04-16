idf_component_register(
    SRCS ili9341.c game_list.c
    PRIV_REQUIRES driver lvgl esp_lcd_ili9341
    INCLUDE_DIRS "."
)
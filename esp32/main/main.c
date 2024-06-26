#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
// #include "nvs_flash.h"
// #include "driver/gpio.h"
// #include "nofrendo.h"

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "msc.h"
#include "ili9341.h"

#define TAG "NES"

// char *osd_getromdata() {
// 	char* romdata;
// 	const esp_partition_t* part;
// 	spi_flash_mmap_handle_t hrom;
// 	esp_err_t err;
// 	nvs_flash_init();
// 	part=esp_partition_find_first(0x40, 1, NULL);
// 	if (part==0) ESP_LOGE(TAG, "Couldn't find rom part!\n");
// 	err=esp_partition_mmap(part, 0, 3*1024*1024, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom);
// 	if (err!=ESP_OK) ESP_LOGE(TAG, "Couldn't map rom part!\n");
// 	ESP_LOGI(TAG, "Initialized. ROM@%p\n", romdata);
//     return (char*)romdata;
// }


// esp_err_t event_handler(void *ctx, system_event_t *event)
// {
//     return ESP_OK;
// }


int app_main(void)
{
	ESP_LOGI(TAG, "NoFrendo start!\n");

	msc_init();
	ili9341_init();

	// nofrendo_main(0, NULL);
	// ESP_LOGI(TAG, "NoFrendo died? WtF?\n");
	// asm("break.n 1");
    return 0;
}


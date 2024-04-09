#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "esp_flash.h"
#include "esp_flash_spi_init.h"
#include "esp_partition.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include "soc/spi_pins.h"

#include "w25qxx.h"

// h2 and c2 will not support external flash
#define FLASH_FREQ_MHZ      40

#define PIN_NUM_MISO    CONFIG_W25QXX_PIN_MISO
#define PIN_NUM_MOSI    CONFIG_W25QXX_PIN_MOSI
#define PIN_NUM_CLK     CONFIG_W25QXX_PIN_CLK
#define PIN_NUM_CS      CONFIG_W25QXX_PIN_CS
#define HOST_ID         SPI2_HOST
#define SPI_DMA_CHAN    SPI_DMA_CH_AUTO

static const char *TAG = "W25QXX";

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

// Mount path for the partition
const char *base_path = "/roms";

static esp_flash_t* init_ext_flash(void);
static const esp_partition_t* add_partition(esp_flash_t* ext_flash, const char* partition_label);
static void list_data_partitions(void);
static bool mount_fatfs(const char* partition_label);

void w25qxx_init(void)
{
    // Set up SPI bus and initialize the external SPI Flash chip
    esp_flash_t* flash = init_ext_flash();
    if (flash == NULL) {
        return;
    }

    // Add the entire external flash chip as a partition
    const char *partition_label = "storage";
    add_partition(flash, partition_label);

    // List the available partitions
    list_data_partitions();

    // Initialize FAT FS in the partition
    if (!mount_fatfs(partition_label)) {
        return;
    }

    // Print FAT FS size information
    uint64_t bytes_total, bytes_free;
    esp_vfs_fat_info(base_path, &bytes_total, &bytes_free);
    ESP_LOGI(TAG, "FAT FS: %" PRIu64 " kB total, %" PRIu64 " kB free", bytes_total / 1024, bytes_free / 1024);

    // Create a file in FAT FS
    ESP_LOGI(TAG, "Opening file");
    FILE *f = fopen("/extflash/hello.txt", "wb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return;
    }
    fprintf(f, "Written using ESP-IDF %s\n", esp_get_idf_version());
    fclose(f);
    ESP_LOGI(TAG, "File written");

    // Open file for reading
    ESP_LOGI(TAG, "Reading file");
    f = fopen("/extflash/hello.txt", "rb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[128];
    fgets(line, sizeof(line), f);
    fclose(f);
    // strip newline
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
}

static esp_flash_t* init_ext_flash(void)
{
    const spi_bus_config_t bus_config = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
    };

    const esp_flash_spi_device_config_t device_config = {
        .host_id = SPI2_HOST,
        .cs_id = 0,
        .cs_io_num = PIN_NUM_CS,
        .io_mode = SPI_FLASH_DIO,
        .freq_mhz = FLASH_FREQ_MHZ,
    };

    ESP_LOGI(TAG, "Initializing external SPI Flash");
    ESP_LOGI(TAG, "Pin assignments:");
    ESP_LOGI(TAG, "MOSI: %2d   MISO: %2d   SCLK: %2d   CS: %2d",
        bus_config.mosi_io_num, bus_config.miso_io_num,
        bus_config.sclk_io_num, device_config.cs_io_num
    );

    // Initialize the SPI bus
    ESP_LOGI(TAG, "DMA CHANNEL: %d", SPI_DMA_CHAN);
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_CHAN));

    // Add device to the SPI bus
    esp_flash_t* ext_flash;
    ESP_ERROR_CHECK(spi_bus_add_flash_device(&ext_flash, &device_config));

    // Probe the Flash chip and initialize it
    esp_err_t err = esp_flash_init(ext_flash);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize external Flash: %s (0x%x)", esp_err_to_name(err), err);
        return NULL;
    }

    // Print out the ID and size
    uint32_t id;
    ESP_ERROR_CHECK(esp_flash_read_id(ext_flash, &id));
    ESP_LOGI(TAG, "Initialized external Flash, size=%" PRIu32 " KB, ID=0x%" PRIx32, ext_flash->size / 1024, id);

    return ext_flash;
}

static const esp_partition_t* add_partition(esp_flash_t* ext_flash, const char* partition_label)
{
    ESP_LOGI(TAG, "Adding external Flash as a partition, label=\"%s\", size=%" PRIu32 " KB", partition_label, ext_flash->size / 1024);
    const esp_partition_t* fat_partition;
    const size_t offset = 0;
    ESP_ERROR_CHECK(esp_partition_register_external(ext_flash, offset, ext_flash->size, partition_label, ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, &fat_partition));

    return fat_partition;
}

static void list_data_partitions(void)
{
    ESP_LOGI(TAG, "Listing data partitions:");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

    for (; it != NULL; it = esp_partition_next(it)) {
        const esp_partition_t *part = esp_partition_get(it);
        ESP_LOGI(TAG, "- partition '%s', subtype %d, offset 0x%" PRIx32 ", size %" PRIu32 " kB",
        part->label, part->subtype, part->address, part->size / 1024);
    }

    esp_partition_iterator_release(it);
}

static bool mount_fatfs(const char* partition_label)
{
    ESP_LOGI(TAG, "Mounting FAT filesystem");
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 100,
            .format_if_mount_failed = false,
            .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(base_path, partition_label, &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return false;
    }
    return true;
}

void w25qxx_listdir(void) {
    // Open directory
    ESP_LOGI(TAG, "Check /roms dir");
    DIR dir;
    FILINFO fno;
    FRESULT res = f_opendir(&dir, "/roms");
    if (res == FR_OK) {
        ESP_LOGI(TAG, "FR_OK");
        while (true) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) {
                break;  // No more files
            }
            if (fno.fattrib & AM_DIR) {
                ESP_LOGI(TAG, "It's a directory");
            } else {
                // It's a file
                printf("File: %s\n", fno.fname);
            }
        }
        f_closedir(&dir);
    }
    else {
        ESP_LOGE(TAG, "FR_NOT_OK");
    }
}
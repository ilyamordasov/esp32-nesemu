#ifndef MAIN_W25QXX_H_
#define MAIN_W25QXX_H_

esp_flash_t* init_ext_flash(void);
esp_partition_t* add_partition(esp_flash_t* ext_flash, const char* partition_label);
void list_data_partitions(void);

void w25qxx_init(void);
void w25qxx_listdir(const char* base_path);

#endif /* MAIN_W25QXX_H_ */
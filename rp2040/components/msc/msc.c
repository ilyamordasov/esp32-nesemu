#include "bsp/board.h"
#include "tusb.h"

#include "w25qxx.h"
#include "hardware/gpio.h"
#include "msc.h"

w25q_data_t *pW25Q=NULL;

void storage_driver_init() {
  // W25Q driver initialize
  pW25Q = (w25q_data_t*)malloc(sizeof(w25q_data_t));
  pW25Q->spiInit=false;
  w25q_disk_initialize(W25Q_SPI_PORT, W25Q_PIN_CS, pW25Q);

  // LED blinking when reading/writing
  gpio_init(LED_BLINKING_PIN);
  gpio_set_dir(LED_BLINKING_PIN, true);
}

#if CFG_TUD_MSC

// Invoked to determine max LUN
uint8_t tud_msc_get_maxlun_cb(void)
{
  return 2; // LUN 0: SDMMC, LUN 1: W25Q Flash
}

// Invoked when received SCSI_CMD_INQUIRY
// Application fill vendor id, product id and revision with string up to 8, 16, 4 characters respectively
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
    sprintf(vendor_id  , "Winbond");
    sprintf(product_id , "Mass Storage");
    sprintf(product_rev, "1.0");
 
}

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
  //if ( lun == 1 && board_button_read() ) return false;

  return true; // RAM disk is always ready
}

// Invoked when received SCSI_CMD_READ_CAPACITY_10 and SCSI_CMD_READ_FORMAT_CAPACITY to determine the disk size
// Application update block count and block size
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
{
    *block_count = pW25Q->sectorCount;
    *block_size  = pW25Q->sectorSize;
}

// Invoked when received Start Stop Unit command
// - Start = 0 : stopped power mode, if load_eject = 1 : unload disk storage
// - Start = 1 : active mode, if load_eject = 1 : load disk storage
bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject)
{
  (void) lun;
  (void) power_condition;

  if ( load_eject )
  {
    if (start)
    {
      // load disk storage
    }else
    {
      // unload disk storage
    }
  }

  return true;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and return number of copied bytes.
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    if (!w25q_read_sector((uint32_t)lba, offset, buffer, bufsize, pW25Q)) {
        return -1;
    }
    return (int32_t) bufsize;
}

bool tud_msc_is_writable_cb (uint8_t lun)
{
  (void) lun;

#ifdef CFG_EXAMPLE_MSC_READONLY
  return false;
#else
  return true;
#endif
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and return number of written bytes
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{
    if (offset >= pW25Q->sectorSize) return -1;
    #ifndef CFG_EXAMPLE_MSC_READONLY
        w25q_sector_erase(lba, pW25Q);
        w25q_write_sector(lba, offset, buffer, bufsize, pW25Q);
    #else
        (void) lun; (void) lba; (void) offset; (void) buffer;
    #endif
    
    return (int32_t) bufsize;
}

// Callback invoked when received an SCSI command not in built-in list below
// - READ_CAPACITY10, READ_FORMAT_CAPACITY, INQUIRY, MODE_SENSE6, REQUEST_SENSE
// - READ10 and WRITE10 has their own callbacks
int32_t tud_msc_scsi_cb (uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize)
{
  // read10 & write10 has their own callback and MUST not be handled here

  void const* response = NULL;
  int32_t resplen = 0;

  // most scsi handled is input
  bool in_xfer = true;

  switch (scsi_cmd[0])
  {
    default:
      // Set Sense = Invalid Command Operation
      tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

      // negative means error -> tinyusb could stall and/or response with failed status
      resplen = -1;
    break;
  }

  // return resplen must not larger than bufsize
  if ( resplen > bufsize ) resplen = bufsize;

  if ( response && (resplen > 0) )
  {
    if(in_xfer)
    {
      memcpy(buffer, response, (size_t) resplen);
    }else
    {
      // SCSI output
    }
  }

  return resplen;
}

#endif

void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < 50) return; // not enough time
  start_ms += 50;

  gpio_put(LED_BLINKING_PIN,led_state);
  led_state = 1 - led_state; // toggle
}

void led_blinking_task_off(void) {
  gpio_put(LED_BLINKING_PIN,false);
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "bsp/board.h"
#include "tusb.h"
#include "msc.h"
#include "ili9341.h"


/*------------- MAIN -------------*/
int main(void)
{
  stdio_init_all();

  storage_driver_init();

  board_init();

  //init ili9341
  ili9341_init();

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
 

  while (1)
  {
    tud_task(); // tinyusb device task
  }

  return 0;
}
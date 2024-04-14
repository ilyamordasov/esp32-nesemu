#ifndef MSC_H
#define MSC_H
/* used by my project */

#define SPI_BAUDRATE_LOW (1000*1000)
#define SPI_BAUDRATE_HIGH (40*1000*1000)

enum {
    W25Q_LUN=1,
};

/* =================================  */
#define  LED_BLINKING_PIN     25
void led_blinking_task(void);
void led_blinking_task_off(void);
/* =================================  */

void storage_driver_init(void);

#endif
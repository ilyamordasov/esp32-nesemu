#!/bin/bash
. ${IDF_PATH}/add_path.sh
esptool.py --chip esp32 --port $(ls /dev/tty.usbserial*) --baud $((230400*2)) write_flash -fs 4MB 0x100000 "$1"
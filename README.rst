|rp2040-build-status| |esp32-build-status|

Flashing
---------

.. code-block:: esp32

    esptool.py -p $(ls /dev/tty.usbserial*) --chip esp32 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin

.. code-block:: esp32s2

    esptool.py -p $(ls /dev/tty.usbmodem*) --chip esp32s2 -b 460800 --before default_reset --after no_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin

.. code-block:: esp32s3

    esptool.py $(ls /dev/tty.usbmodem*) --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x0 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin

SPI-FLASH
---------

    =====  =======  ========
    Pin    RP2040   ESP32SX
    =====  =======  ========
    MISO   4        37
    MOSI   3        35
    CLK    2        36
    CS     5        38
    =====  =======  ========



Display
-------

To display the NES output, please connect a 320x240 ili9341-based SPI display to the ESP32 in this way:

    =====  =======  ========
    Pin    RP2040   ESP32SX
    =====  =======  ========
    MISO   4        37
    MOSI   3        35
    CLK    2        36
    CS     9        38
    DC     8        39
    RST    7        40
    BCKL   5        34
    =====  =======  ========

(BCKL = backlight enable)

Controller
----------

To control the NES, connect a Playstation 1 or 2 controller as such:

    =====  =======  =========
    Pin    RP2040   ESP32SX
    =====  =======  =========
    CLK    14       1
    DAT    27       2
    ATT    16       3
    CMD    2        4
    =====  =======  =========

ROM
---
This NES emulator does not come with a ROM. Please supply your own and flash to address 0x00100000. You can use the flashrom.sh script as a template for doing so.

.. |rp2040-build-status| image:: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/rp2040-build.yml/badge.svg
    :alt: Build and Publish RP2040 Application
    :target: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/rp2040-build.yml

.. |esp32-build-status| image:: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/esp-build.yml/badge.svg
    :alt: Build and Publish ESP32 Application
    :target: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/esp-build.yml
|rp2040-build-status| |esp32-build-status|

Compiling
---------

This code is an esp-idf project. You will need esp-idf to compile it. Newer versions of esp-idf may introduce incompatibilities with this code;
for your reference, the code was tested against commit 12caaed28063e32d8b1fb13e13548b6fa52f87b3 of esp-idf.

Flashing
---------

.. code-block:: esp32

    esptool.py -p $(ls /dev/tty.usbserial*) --chip esp32 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin

.. code-block:: esp32s2

    esptool.py -p $(ls /dev/tty.usbmodem*) --chip esp32s2 -b 460800 --before default_reset --after no_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin

.. code-block:: esp32s3

    esptool.py $(ls /dev/tty.usbmodem*) --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x0 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin

Display
-------

To display the NES output, please connect a 320x240 ili9341-based SPI display to the ESP32 in this way:

    =====  =======================
    Pin    GPIO
    =====  =======================
    MISO   25
    MOSI   23
    CLK    19
    CS     22
    DC     21
    RST    18
    BCKL   5
    =====  =======================

(BCKL = backlight enable)

Also connect the power supply and ground. For now, the LCD is controlled using a SPI peripheral, fed using the 2nd CPU. This is less than ideal; feeding
the SPI controller using DMA is better, but was left out due to this being a proof of concept.


Controller
----------

To control the NES, connect a Playstation 1 or 2 controller as such:

    =====  =====
    Pin    GPIO
    =====  =====
    CLK    14
    DAT    27
    ATT    16
    CMD    2
    =====  =====

Also connect the power and ground lines. Most PS1/PS2 controllers work fine from a 3.3V power supply, if a 5V one is unavailable.

ROM
---
This NES emulator does not come with a ROM. Please supply your own and flash to address 0x00100000. You can use the flashrom.sh script as a template for doing so.

.. |rp2040-build-status| image:: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/rp2040-build.yml/badge.svg
    :alt: Build and Publish RP2040 Application
    :target: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/rp2040-build.yml

.. |esp32-build-status| image:: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/esp-build.yml/badge.svg
    :alt: Build and Publish ESP32 Application
    :target: https://github.com/ilyamordasov/esp32-nesemu/actions/workflows/esp-build.yml
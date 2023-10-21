# SpeedyBee F405 V3

> Notice: At the moment, DSHOT is not supported on SpeedyBe F405 V3. Use Multishot instead

> Notice: The analog OSD and the SD card (blackbox) share the same SPI bus, which can cause problems when trying to use analog OSD and blackbox at the same time.

## Flashing firmware

We often see reports of users having problems flashing new firmware with this baord. The following sugestions usually seem to fix the issues.

* Remove SD Card
* Disconnect devices from UART1 and UART3

Try removing the SD Card first, and if that still does not fix the issue, disconnect the devices connected to UART1 and UART3.





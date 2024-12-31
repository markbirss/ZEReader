# ZEReader
ZEReader is a Zephyr based firmware for an open hardware eBook reader approach.

## Basic Hardware Components
- Raspberry Pi Pico 1/2
- Waveshare ePaper Display 7,5" (Gooddisplay GDEW075T7, GDEY075T7) with UC8179 controller
- SD Card Reader (SPI)


## Setup for local development
TODO

## Build
### Build for Raspberry Pi Pico 1
```
west build -p always -b rpi_pico2/rp2350a/m33 app
```

### Build for Raspberry Pi Pico 2
```
west build -p always -b rpi_pico app
```

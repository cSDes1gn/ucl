# Documentation

## Installations
```bash
brew tap osx-cross/avr
brew install avr-gcc avrdude
```

## Compile and Flash
Using `CMake` toolchain:
```bash
mkdir build
cd build
cmake ..
make -j
make flash
```

Using `avrdude` and `avr-gcc` utilities
```bash
# compile
avr-gcc main.c usart.c -DF_CPU=16000000 -mmcu=atmega2560 -Wall -Werror -Wextra -Os
# translate to hex
avr-objcopy -j .text -j .data -O ihex a.out my.hex
# flash
avrdude -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:my.hex:i
```

## Important Links
1. https://www.nongnu.org/avrdude/user-manual/avrdude_toc.html#SEC_Contents
2. https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf
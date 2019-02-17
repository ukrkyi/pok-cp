#Application output name
GCC_STD ?= gnu99
MCU ?= attiny13a
CC := avr-gcc

FILE += main.c timer.c display.c shift.c

CFLAGS += -std=$(GCC_STD)
CFLAGS += -Wall
CFLAGS += -pedantic
CFLAGS += -Os
CFLAGS += -mmcu=$(MCU)
CFLAGS += -DMCU=$(MCU)
CFLAGS += -DF_CPU=9600000

#debug
CFLAGS += -g

#PROG := -P /dev/ttyACM0 -c stk500v1 -b 19200
PROG := -c usbasp -B4

all: clean build flash
	@echo "Done!"

build:
	@echo "Building file $(FILE)"
	$(CC) $(CFLAGS) $(FILE) -o a.out
	avr-objcopy -j .text -j .data -O ihex a.out main.hex
	avr-size --format=avr --mcu=$(MCU) a.out

flash: build
	avrdude -v -pt13 $(PROG) -U flash:w:main.hex:i

clean:
	@rm -f a.out main.hex

help:
	@cat README

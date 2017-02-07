CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = sudo avrdude

CFLAGS = -Wall -Os -Iusbdrv -I. -mmcu=attiny45 -DF_CPU=16500000
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p trinket -c usbtiny -v

OBJECTS = \
	main.o osc_calibrate.o \
	usbdrv/usbdrv.o usbdrv/usbdrvasm.o \
	Light_WS2812/light_ws2812.o

all: main.hex

$(OBJECTS): usbconfig.h ws2812_config.h

program: main.hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.c	
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

clean:
	$(RM) *.o *.hex *.elf usbdrv/*.o Light_WS2812/*.o


# Prereqs:
# - install cross CC, CXX, OBJCOPY, SIZE (apt get install arm-none-eabi-gcc)
# - https://github.com/PaulStoffregen/teensy_loader_cli install in PATH
# - install/configure udev 49-teensy.rules

MCU=MK20DX256

LOWER_MCU := $(shell echo $(MCU) | tr '[:upper:]' '[:lower:]')
MCU_LD = $(LOWER_MCU).ld

# The name of your project (used to name the compiled .hex file)
TARGET = main

SRCS_C = \
	teensy3/mk20dx128.c \
	teensy3/pins_teensy.c \
	teensy3/nonstd.c \
	teensy3/analog.c \
	teensy3/serial1.c \
	teensy3/serial2.c \
	teensy3/serial3.c \
	teensy3/usb_dev.c \
	teensy3/usb_mem.c \
	teensy3/usb_desc.c \
	teensy3/usb_rawhid.c \
	teensy3/usb_seremu.c
SRCS_CXX = \
	teensy3/yield.cpp \
	teensy3/HardwareSerial1.cpp \
	teensy3/HardwareSerial2.cpp \
	teensy3/HardwareSerial3.cpp \
	teensy3/usb_inst.cpp \
	Sprite/Sprite.cpp \
	Matrix/Matrix.cpp \
	main.cpp

# configurable options
OPTIONS = -DF_CPU=48000000 -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE
OPTIONS += -DUSB_RAWHID

# options needed by many Arduino libraries to configure for Teensy 3.0
OPTIONS += -D__$(MCU)__ -DARDUINO=10600 -DTEENSYDUINO=121

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -Os -mcpu=cortex-m4 -mthumb -MMD $(OPTIONS)
CPPFLAGS += -Iteensy3 -IMatrix -ISprite

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

# linker options
LDFLAGS = -Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 --specs=nano.specs -mcpu=cortex-m4 -mthumb -Tteensy3/$(MCU_LD)

# suppress warnings about wchar mismatch -jg
LDFLAGS += -Wl,--no-wchar-size-warning

# additional libraries to link
LIBS = -lm

# Install debian arm-none-eabi-gcc
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

OBJS := $(SRCS_C:.c=.o) $(SRCS_CXX:.cpp=.o)

all: $(TARGET).hex

$(TARGET).elf: $(OBJS) teensy3/$(MCU_LD)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

program: $(TARGET).hex
	teensy_loader_cli -mmcu=$(MCU) -v -w $<

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d
	rm -f Matrix/*.o Matrix/*.d
	rm -f Sprite/*.o Sprite/*.d
	rm -f teensy3/*.o teensy3/*.d
	rm -f $(TARGET).elf $(TARGET).hex

# Makefile for the rfboot bootloader

# Do not use this Makefile from the downloaded rfboot folder
# because you will get always the same fixed channel address and a
# useless key={0,0,0,0}
# use "rftool create ProjectName" instead. This copys this makefile and sets all the
# needed parameters.

# program name. Should not be changed.
PROGRAM    = rfboot

include compile_settings.mk

OBJ        = $(PROGRAM).o
OPTIMIZE   = -Os -fno-inline-small-functions -fno-split-wide-types
DEFS       = -g
LIBS       = -Icc1101 -Ixtea
CC         = avr-gcc

# Default frequency is 8Mhz
# This should be the normal for atmega328 @ 3.3V
ifeq ($(F_CPU),)
F_CPU := 8000000L
endif

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(F_CPU) $(DEFS)
override LDFLAGS       = -Wl,$(LDSECTION)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: atmega328p

atmega328p: clean
atmega328p: MCU_TARGET = atmega328p
atmega328p: CFLAGS += -std=gnu99 -Wall -ffunction-sections -fdata-sections -fshort-enums -g -Os -w -fno-exceptions -Wl,--gc-sections -Ixtea -Icc1101 -DCOMPILE_TIME=`date '+%s'`
#atmega328p: F_CPU = 8000000L
atmega328p: LDSECTION  = --section-start=.text=0x7000
atmega328p: $(PROGRAM)_atmega328p.elf
atmega328p: size

# 8MHz . Uses internal RC oscillator
noxtal: atmega328p
	avrdude -q -p atmega328p -c usbasp -v -e  -U lfuse:w:0xE2:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m

# Uses external crystal/resonator
xtal: atmega328p
	avrdude -q -p atmega328p -c usbasp -v -e  -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m

%.elf:
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o xtea.o xtea/xtea.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o cc1101.o cc1101/cc1101.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o spi.o cc1101/spi.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o rfboot.o rfboot.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ rfboot.o xtea.o spi.o cc1101.o

clean:
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

size:
	avr-size -C $(PROGRAM)_$(MCU_TARGET).elf

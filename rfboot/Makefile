# Makefile for the rfboot bootloader

# program name. Should not be changed.
PROGRAM    = rfboot

include hardware_settings.mk

OBJ        = $(PROGRAM).o
OPTIMIZE   = -Os -fno-inline-small-functions -fno-split-wide-types
DEFS       = -g
LIBS       = -Icc1101 -Ixtea
CC         = avr-gcc

# Default frequency is 8Mhz, unless you set one in "complile_settings.mk"
# This should be the normal for atmega328 @ 3.3V
ifeq ($(F_CPU),)
F_CPU := 8000000L
endif

# Default programmer is usbasp
ifeq ($(PROGRAMMER),)
PROGRAMMER := usbasp
endif

# Default is no crystal
ifeq ($(CRYSTAL),1)
LFUSE := 0xFF
else
LFUSE := 0xE2
endif

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(F_CPU) $(DEFS)
override LDFLAGS       = -Wl,$(LDSECTION)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: atmega328p

atmega328p: check
atmega328p: clean
atmega328p: MCU_TARGET = atmega328p
atmega328p: CFLAGS += -std=gnu99 -Wall -ffunction-sections -fdata-sections -fshort-enums -g -Os -w -fno-exceptions -Wl,--gc-sections -Ixtea -Icc1101 -DCOMPILE_TIME=`date '+%s'`
#atmega328p: F_CPU = 8000000L
atmega328p: LDSECTION  = --section-start=.text=0x7000
atmega328p: $(PROGRAM)_atmega328p.elf
atmega328p: size

# Uses internal RC oscillator at 8MHz
noxtal: atmega328p
	avrdude -q -p atmega328p -c $(PROGRAMMER) -v -e  -U lfuse:w:0xE2:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m

# Uses external crystal/resonator
xtal: atmega328p
	avrdude -q -p atmega328p -c $(PROGRAMMER) -v -e  -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m

burn: atmega328p
	avrdude -q -p atmega328p -c $(PROGRAMMER) -v -e  -U lfuse:w:$(LFUSE):m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m

%.elf:
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o xtea.o xtea/xtea.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o cc1101.o cc1101/cc1101.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o spi.o cc1101/spi.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o rfboot.o rfboot.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ rfboot.o xtea.o spi.o cc1101.o

check:
	@test -s rfboot_settings.h || { echo "rfb_settings.h does not exist ! Exiting..."; exit 1; }

clean:
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

size:
	avr-size --mcu=$(MCU_TARGET) -C $(PROGRAM)_$(MCU_TARGET).elf

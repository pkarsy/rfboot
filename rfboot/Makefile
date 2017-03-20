# Makefile for the rfboot bootloader
# adapted from the Makefile for atmegaboot
# do not use directly this makefile for generating production rfboot images
# because you will get always the same fixed channel address and a
# useless key={0,0,0,0}
# use the utility rfbootgen instead. This calls this makefile but sets all the
# needed variables first.


# program name. Should not be changed.
PROGRAM    = rfboot

#MCU_TARGET = atmega328p
#LDSECTION  = --section-start=.text=0x7800

# the efuse should really be 0xf8; since, however, only the lower
# three bits of that byte are used on the atmega168, avrdude gets
# confused if you specify 1's for the higher bits, see:
# http://tinker.it/now/2007/02/24/the-tale-of-avrdude-atmega168-and-extended-bits-fuses/
#
# similarly, the lock bits should be 0xff instead of 0x3f (to
# unlock the bootloader section) and 0xcf instead of 0x0f (to
# lock it), but since the high two bits of the lock byte are
# unused, avrdude would get confused.

#ISPFUSES    = avrdude -c $(ISPTOOL) -p $(MCU_TARGET) $(ISPPORT) $(ISPSPEED) \
#-e -u -U lock:w:0x3f:m -U efuse:w:0x$(EFUSE):m -U hfuse:w:0x$(HFUSE):m -U lfuse:w:0x$(LFUSE):m
#ISPFLASH    = avrdude -c $(ISPTOOL) -p $(MCU_TARGET) $(ISPPORT) $(ISPSPEED) \
#-U flash:w:$(PROGRAM)_$(TARGET).hex -U lock:w:0x0f:m


OBJ        = $(PROGRAM).o
OPTIMIZE   = -Os -fno-inline-small-functions -fno-split-wide-types
DEFS       = -g
LIBS       = -Icc1101 -Ixtea
#-Icc1101 -Ixtea
CC         = avr-gcc

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS)
override LDFLAGS       = -Wl,$(LDSECTION)
#-nostartfiles -nostdlib
#override LDFLAGS       = -Wl,-Map,$(PROGRAM).map,$(LDSECTION)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

all:
	#make clean
	make atmega328p


# Normally all theese values come from rfbootgen
# These defaults are for convenience when developing
# rfboot.c so a simple make can build it. The generated
# bootloader should not be used for a real project


#XTEAKEY ?= 0,0,0,0
#RFADDR ?= 00000
#RFCHANNEL ?= 0
#CE_PIN ?= B,1
#CSN_PIN ?= B,2


###########################################################################
### Auto to target einai gia ta gymna atmega328 pou xrisimopoiw           #
###########################################################################

#atmega328: clean
#atmega328: TARGET = atmega328_$(MHZ)MHz
#atmega328: MCU_TARGET = atmega328p
#atmega328: CFLAGS += -std=gnu99 -Wall -ffunction-sections -fdata-sections -fshort-enums -g -Os -w -fno-exceptions -Wl,--gc-sections -DXTEAKEY=$(XTEAKEY) -DRFADDR=$(RFADDR) -DRFCHANNEL=$(RFCHANNEL) -DCE_PIN=$(CE_PIN) -DCE_PORT=$(CE_PORT) -DCE_DDR=$(CE_DDR) -DCSN_PIN=$(CSN_PIN) -DCSN_PORT=$(CSN_PORT) -DCSN_DDR=$(CSN_DDR)
#atmega328: AVR_FREQ = $(MHZ)000000L
#atmega328: LDSECTION  = --section-start=.text=0x7800
#atmega328: $(PROGRAM)_atmega328_$(MHZ)MHz.elf
#atmega328: $(PROGRAM)_atmega328_$(MHZ)MHz.hex
#atmega328: size

atmega328p: clean
#atmega: TARGET = $(MCU_TARGET)_$(MHZ)MHz
atmega328p: MCU_TARGET = atmega328p
atmega328p: CFLAGS += -std=gnu99 -Wall -ffunction-sections -fdata-sections -fshort-enums -g -Os -w -fno-exceptions -Wl,--gc-sections -Ixtea -Icc1101
# -DCE_PIN=$(CE_PIN) -DCSN_PIN=$(CSN_PIN) -DRFBOOT_ADDRESS=$(RFBOOT_ADDRESS) -DRF_CHANNEL=$(RF_CHANNEL) -DXTEAKEY=$(XTEAKEY)
atmega328p: AVR_FREQ = 8000000L
atmega328p: LDSECTION  = --section-start=.text=0x7000
#atmega: $(PROGRAM)_$(MCU_TARGET)_$(MHZ)MHz.elf
atmega328p: $(PROGRAM)_atmega328p.elf
atmega328p: size

#atmega_isp: atmega328_8
#atmega_isp: TARGET = atmega328_8MHz
#atmega_isp: MCU_TARGET = atmega328p
#atmega_isp: HFUSE = DA
#atmega_isp: LFUSE = E2
#atmega_isp: EFUSE = 05
#atmega_isp: isp
noxtal: atmega328p
	avrdude -q -p atmega328p -c usbasp -v -e  -U lfuse:w:0xE2:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m
	
xtal: atmega328p
	avrdude -q -p atmega328p -c usbasp -v -e  -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0x05:m -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0C:m

promini: xtal

naked: noxtal

#usbtiny: atmega328p
#	avrdude -q -p atmega328p -c usbtiny -v -e  -U flash:w:rfboot_atmega328p.elf -U lock:w:0x0f:m
#	# -U lfuse:w:0xE2:m -U hfuse:w:0xDA:m -U efuse:w:0x05:m -U lock:w:0x0c:m


#isp: $(TARGET)
#	$(ISPFUSES)
#	$(ISPFLASH)

#isp-stk500: $(PROGRAM)_$(TARGET).hex
#	$(STK500-1)
#	$(STK500-2)

	

%.elf:
	#$(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o xtea.o xtea/xtea.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o cc1101.o cc1101/cc1101.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o spi.o cc1101/spi.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o rfboot.o rfboot.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ rfboot.o xtea.o spi.o cc1101.o
	# $(LIBS) 

clean:
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

#%.srec: %.elf
#	$(OBJCOPY) -j .text -j .data -O srec $< $@

#%.bin: %.elf
#	$(OBJCOPY) -j .text -j .data -O binary $< $@

size:
	#avr-size -C $(PROGRAM)_$(TARGET).elf
	avr-size -C $(PROGRAM)_$(MCU_TARGET).elf

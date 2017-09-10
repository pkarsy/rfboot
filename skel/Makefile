# This Makefile uses the arduino-makefile
# If you use another AVR build system you must take the necessary changes
# for the build to work

#for 8MHz projects choose pro328, even if the board is not o pro mini 3.3V
#for 16MHZ 	pro5v328.
BOARD_TAG     =  pro328
# It is ok, unless the enums are larger than 256 items
EXTRA_FLAGS += -fshort-enums

# This is the arduino-makefile engine
include /usr/share/arduino/Arduino.mk

send: all
	@# With this command the elf file is converted to binary format as rftool
	@# doesnt understand hex or elf files

#$(OBJCOPY) -j .text -j .data -O binary $(TARGET_ELF) $(OBJDIR)/$(TARGET).bin

	@# "rftool upload" tries to reset the remote module by sending a reset string
	@# Obviously the application code must support this, otherwise you have to
	@# manually reset the module
#rftool upload $(OBJDIR)/$(TARGET).bin
	rftool upload $(TARGET_ELF)

terminal:
	@# rftool will append the actual serial port after "-p"
	rftool monitor gtkterm -s 38400 -p

isp:
	@# This command writes the bootloader to the MCU
	make -C rfboot isp


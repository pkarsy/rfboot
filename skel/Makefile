# This Makefile uses the arduino-makefile
# If you use another AVR build system you must take the necessary changes
# for the build to work

#for 8MHz projects choose pro328, even if the board is not o pro mini 3.3V
#for 16MHZ 	pro5v328.
BOARD_TAG     =  pro328
# It is ok, unless the enums you use are larger than 256 items
EXTRA_FLAGS += -fshort-enums

# This is the arduino-makefile engine
include /usr/share/arduino/Arduino.mk

send:
	@# With this command the elf file is converted to binary format as rfboot
	@# doesnt understand other format

	$(OBJCOPY) -j .text -j .data -O binary $(TARGET_ELF) $(OBJDIR)/$(TARGET).bin

	@# "rftool upload" tries to reset the remote module by sending a @RESET character
	@# Obviously the application code must support this, otherwise you have to
	@# manually reset the module
	@# See https://rfboot.github.com about this topic
	@# rfupload must already know the "signature" of the USb-2-rf module
	@# see topic https://rfboot.github.com/topic/..

	rftool upload $(OBJDIR)/$(TARGET).bin

terminal:
	@# rftool will append the actual serial port after "-p"
	rftool monitor gtkterm -s 57600 -p

isp:
	make -C rfboot isp


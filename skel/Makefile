# This Makefile uses the arduino-makefile
# In order to use another AVR build system you must take the necessary changes
# for the build to work

# probably the project is NOT based on a promini, but the following
# setting keep the build system happy.
# for 8MHz projects choose pro328, even if the board is not o pro mini 3.3V
# for 16MHZ choose pro5v328.
BOARD_TAG     =  pro328

# It is ok, unless the enums are larger than 256 items
EXTRA_FLAGS += -fshort-enums

# We use arduino-makefile to compile the project
include /usr/share/arduino/Arduino.mk

send: all
	@# "rftool send" tries to reset the remote module by sending a reset string first
	@# The application code must support this however, otherwise you have to
	@# manually reset the module
	rftool send $(TARGET_ELF)

terminal:
	@# rftool will append the actual serial port after "-p"
	rftool terminal gtkterm -s 38400 -p

isp:
	@# This command writes the bootloader to the MCU
	make -C rfboot isp


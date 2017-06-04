BOARD_TAG = pro328
EXTRA_FLAGS = -fshort-enums -g
include /usr/share/arduino/Arduino.mk

MONITOR_PORT := $(shell rftool getport)

send: all
send: portcheck
	rftool resetLocal
	fuser -s -v -k -STOP $(MONITOR_PORT) || true
	$(MAKE) do_upload
	fuser -s -v -k -CONT $(MONITOR_PORT) || true

portcheck:
ifeq ($(MONITOR_PORT),)
	@$(error No usb2rf device is connected )
endif

hex: all
	cp -v build-pro328/usb2rf.hex .

sendHex:
	rftool resetLocal
	avrdude -q -V -D -p atmega328p -c arduino -b 57600 -P $(MONITOR_PORT) -U flash:w:usb2rf.hex:i

debug: portcheck
	# WARNING ! You need to put here the correct serial port device, for debug monitoring
	gtkterm -s 19200 -p /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_Z2CVM7J2-if00-port0

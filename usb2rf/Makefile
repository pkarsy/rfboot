BOARD_TAG = pro328
EXTRA_FLAGS = -fshort-enums
include /usr/share/arduino/Arduino.mk
 
MONITOR_PORT  =  /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_A800G6XK-if00-port0
# MONITOR_PORT  =  /dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AJV9MX1R-if00-port0
# isws
# cat ~/.usb2rf | grep -v "^#" | sort | tail -n 1

send: all
	make
	rftool resetLocal
	fuser -s -v -k -STOP $(MONITOR_PORT) || true
	$(MAKE) do_upload
	fuser -s -v -k -CONT $(MONITOR_PORT) || true

terminal:
	gtkterm	-p ${MONITOR_PORT} -s 57600

	

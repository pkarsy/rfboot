--- What the D3 to RST cable does in ProMini (usb2rf module)

This is no critical and is only useful if you modify the usb2rf sketch frequently. This is unlikely as you probably want to program the module only once.

D3-RST plays the same role as the DTR-RST connection, in arduinos, to support the autoreset feature. The difference is that the reset is triggered by the firmware and not by the Serial module. This is very importat because we do not want the module to reset every time the serial port is accessed by a program. 

However the usb2rf firmware asserts D3 LOW when we want to update (the firmware). The usb2rf/Makefile resets the module with this mechanism.

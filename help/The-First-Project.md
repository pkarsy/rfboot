This is an equivalent of the **Blink** example in the Arduino distribution
Before you start it is required that you have completed the
[Installation of rfboot](help/Installation)
which means that you have the software in your linux box and the usb2rf module ready


### Target (your electronics project)

### atmega328 MCU
This  includes a bare atmega328 with some caps etc. It also includes
many arduinos and clones. A very well suited arduino is the ProMini 3.3V. It has 3.3 regulator and can directly drive the CC1101 without level converters. It can powered directly from a Lithium cell, making it ideal for prototyping
Very Importand: You need an ISP programmer (usbasp, usbtiny etc) to burn the bootloader. After that no wires are required.

### CC1101 RF module
In order to communicate with the PC and upload code to MCU, the usb2rf module must be attached to the PC.

### Any other components your particular project needs.
This includes serial devices such as GSM modems, GPS modules witch can be attached to the hardware serial port, because rfboot does not use it.

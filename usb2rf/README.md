This folder contains code PCB files and tutorial to build, program and use the usb2rf module
The usb2rf module is a bridge allowing data from the PC to be transmitted as RF packets and in reverse
direction, allows rf packets to be received from the PC

```
+----------+
|          |         +---------+         +--------+
| Computer |   --->  | usb2rf  |  --->   | CC1101 |  ----> RF
|          |   USB   +---------+   SPI   +--------+
+----------+        usb2rf module
rftool
terminal emulator
etc
```

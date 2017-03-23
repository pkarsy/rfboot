This folder contains code PCB files and tutorial to build, program and use the usb2rf module
The usb2rf module is a bridge allowing data from the PC to be transmitted as RF packets and in reverse
direction, allows rf packets to be received from the PC

```
+----------+
|          |         +-----------------+         +--------+                               electronic project
| gtkterm  |  <--->  | usb2rf firmware |  <-->   | CC1101 |  <---> {{{ ΕΜ Waves }}} <-->       with a
| rftool   |   USB   +-----------------+   SPI   +--------+                                CC1101 module
+----------+           
Computer            |______________________________________|
                                 usb2rf module
```

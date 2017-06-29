### usb2rf module
Generally you need the following info only if you modify the usb2rf source code.

This folder contains firmware for the usb2rf module
The usb2rf module is a bridge allowing data from the PC to be transmitted as RF packets and in reverse
direction, allows rf packets to be received from the PC

### Upload the precompiled usb2rf.hex file
Normally you dont need/want to do that independently, as this is part of the [Installation](../help/Installation.md).
OK, If rftool knows the module, it is just a
```sh
> make sendHex
```

### build instructions
You need to install the Arduino libraries<br/>
[AltSoftSerial](https://github.com/PaulStoffregen/AltSoftSerial)<br/>
[digitalWriteFast](https://github.com/NicksonYap/digitalWriteFast)<br/>
and then compile as usual.
```sh
# To build the firmware
> make
# to upload the firmware
> make send
# to replace the precompiled usb2rf.hex
> make hex
```

### Debug port (only if you modify the usb2rf code)
By using another USB to UART module you can have debug messages, useful only if modifying the
usb2rf.ino file, as obviously the main port cannot be used for debug messages.
You need 3 jumper cables
ProMini D9 <---> SerialModule2 RX
D4 <---> DTR
GND <----> GND
and then:
```sh
> make debug
```
Now press "F7" (with gtkterm) to toggle DTR and enable/disable debug output
See the comments of usb2rf.ino for more info.

### Assemble the module
See [Installation](../help/Installation.md) for instructions to build it.

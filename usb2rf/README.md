### usb2rf module

This folder contains firmware (and a  PCB file if you want to buid a more robust module) for the usb2rf module
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
See the comments of usb2rf.ino

### Assemble the module
See [Installation](../help/Installation.md) for instructions to build it.

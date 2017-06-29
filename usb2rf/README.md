### usb2rf module

This folder contains firmware (and a  PCB file if you want to buid a more robust module) for the usb2rf module
The usb2rf module is a bridge allowing data from the PC to be transmitted as RF packets and in reverse
direction, allows rf packets to be received from the PC

### Upload the precompiled usb2rf.hex file
Normally you dont need/want to do that as this is part of the [Installation](../help/Installation.md).
It rftool knows the module, it is just a
```sh
> make sendHex
```

### build instructions
You need to install the Arduino libraries<br/>
[AltSoftSerial](https://github.com/PaulStoffregen/AltSoftSerial)<br/>
[digitalWriteFast](https://github.com/NicksonYap/digitalWriteFast)

### Assemble the module
See [Installation](../help/Installation.md) for instructions to build it.

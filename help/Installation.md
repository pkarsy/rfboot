## Installation

In order to use rfboot succesfully there are quite a few steps initially but after that
the process is simple.
You need to:
- Install the needed software in the PC
- Assemble and configure the usb2rf module (Connects to a USB port of the PC)
- Configure rftool to use the usb2rf module
- Upload the apropriate firmware to usb2rf (An Arduino sketch)

### Install the apropriate software
First of all a linux PC is needed, for the development. I don't use Windows neither MAC
and is very difficult for me to support another platform. You can use VirtualBox if
you really need to use rfboot from Windows or Mac.
I tried with Virtualbox (a Linux Mint 17.3 image) and it is working perfectly.
Of course if you are unfamiliar with linux and especially the command line, the problem
remains.

I do all development on a Linux Mint 17.3 box. Maybe you need to adapt the procedure
for your environment. Open a terminal:

```
> sudo apt-get update ; sudo apt-get upgrade # this is recommended before start

> sudo apt-get install arduino-core arduino-mk   # This also installs avr compiler and avr-libc

> sudo apt-get install geany # A programmers editor. You can use another if you like.
                             # The instructions here, use geany as text editor

> sudo apt-get install gtkterm # a serial terminal. I suggest to use this serial terminal
                               # before try your luck with another one.

> sudo apt-get install git   # To easily download rfboot.
```

Download the rfboot repository in your PC. Place it in some relatively safe place in your
PC like ~/programming/

```
> mkdir ~/programming
> cd ~/programming
> git clone https://github.com/pkarsy/rfboot.git
```
The "rftool" utility needs to be in the PATH.
```
# if the ~/bin does not exist "mkdir ~/bin" and then logout and login again
> cd ~/bin
> chmod +x ~/programming/rfboot/rftool/rftool
# Do NOT copy rftool, just symlink it
> ln -s ~/programming/rfboot/rftool/rftool
```
Now if you type
```
> rftool
```
Should give you a small usage message. This means rftool is in the PATH.

Now it is time to install mCC1101, a modified (and simplified) panStamp
CC1101 library. Change "~/sketchbook/libraries" with your actual sketchbook folder.

```
> cd ~/sketchbook/libraries
> git clone https://github.com/pkarsy/mCC1101.git
```

Linux by default does not give permission (to regular users) to
access the Serial ports, neither the ISP programmers. To fix it:

- Serial port
```
> sudo adduser myusername dialout
```
You need to logout and login again for the changes to take effect.
- ISP programmer

```
> sudo nano /etc/udev/rules.d/52-my.rules
```
Add the lines
```
# USBasp
ATTR{idVendor}=="16c0", ATTR{idProduct}=="05dc", MODE="666"

# USBtiny
ATTR{idVendor}=="1781", ATTR{idProduct}=="0c9f", MODE="666"
```

In the command line
```
> sudo service udev restart
```

***Software installation is done !***

### usb2rf module

***WARNING: The red FTDI modules in the picture (it seams they have a fake FTDI chip) are very unreliable. A lot of failed uploads, and mysterious CRC errors, dissapeared by using a CP2102 or Pl2303 USB-to-Serial module. I dont't have a genuine FTDI module to test it. In the next few days I will update this tutorial with the new instructions***

To build the usb2rf module you need :
- A CP2102 module ( A PL2303 is also OK)
- ProMini 3.3V. Do not use a 5V ProMini. CC1101 cannot tolerate 5V.
- CC1101 module (I use D-SUN modules)
- some female-female jumper wires (2.54mm spacing).

```
+----------+
|          |         +--------+          +----------+         +--------+
| gtkterm  |  <--->  | CP2102 |  <--->   | Pro mini |  <--->  | CC1101 |
| rftool   |   USB   +--------+  Serial  +----------+   SPI   +--------+
+----------+
   PC                |                   usb2rf module                 |
                     +-------------------------------------------------+
```

Here is a photo of the materials we need (It will change with CP2102)
![usb2rf](files/usb2rf1.jpg)

***Serial connection.***

4 cables are required :

CP2102 | Cable COLOR | Pro Mini
---- | ----- | --------
GND  | Black |GND
5V   | Red | RAW
TX   | Yellow | RX
RX   | Green |TX

ProMini regulates the 5V from RAW pin to 3.3V. CP2101 has also an internal 3.3 regulator and the 2 modules
can communicate without the need for logic level conversion.

***The SPI intreface***

7 cables are required :

CC1101 PIN | Cable COLOR | ProMini pin
------------- | ----------- | -----------
GND | Black | GND
VCC | Red | 3.3V
CSN | Yellow | 10
SCK | Green | 13
MOSI | Blue | 11
MISO | Violet | 12
GDO0 | Gray | 2
GDO2 |  | Not Connected

CC1101 is working fine with 3.3V, so again no level conversion is needed.

***Optional: connect with a Female-Female cable the proMini pins D3 and RST.***
See [Explanation](usb2rf-reset.md)

This is no critical and is only useful if you modify the usb2rf sketch frequently (unlikely as you probably you want to program the module only once)
Explanation: (skip this if you are not interested for the mechanism) D3-RST plays the same role as the DTR-RST
connection, in arduinos, to support the autoreset feature. The difference is that the reset is triggered by the firmware and not
by the Serial module. This is very importat because we do not want the module to reset every time the serial port is accessed by a
program. However the usb2rf firmware asserts D3 LOW when we want to update (the firmware). The usb2rf/Makefile resets the module with this mechanism.

![usb2rf2](files/usb2rf2.jpg)

And here is the final module

![usb2rf2](files/usb2rf3.jpg)

### Configure rftool to use the usb2rf module
```
> rftool addport
```
and insert the usb2rf module. rftool detects it and saves the Serial port device
to ~/.usb2rf file. You can have more than one usb2rf devices.

### Burn the apropriate firmware to usb2rf
You can build the sketch by yourself. (see readme in the usb2rf folder)
or upload the precompiled .hex
```
> cd usb2rf
> make sendHex
```
You may need to press proMini reset button as autoreset does not work.

***Hardware and software setup is done !***

Continue with
[The First Project](The-First-Project.md)


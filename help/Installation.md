## Installation

In order to use rfboot succesfully there are quite a few steps initially but after that
the process is simple.
You need to:
- Install the apropriate software
- Download and install rfboot from github
- Build the usb2rf module (Connects to a USB port of the PC)
- Configure rftool to use the usb2rf module
- Build your project with rfboot as bootloader

***

### Install the apropriate software
First of all a linux PC is needed, for the development. I don't use Windows neither MAC
and is very difficult for me to support another platform. You can use VirtualBox if
you really need to use rfboot from Windows or Mac. 
I tried with Virtualbox (a Linux Mint 17.3 image) and it is working perfectly.
Of course if you are unfamiliar
with linux and especially the command line the problem remains.

I did all development on a Linux Mint 17 box. Probably you need to adapt the procedure for your
environment.

```
> sudo apt-get update ; sudo apt-get upgrade # this is recommended before start

> sudo apt-get install arduino-core arduino-mk   # This also installs avr compiler and avr-libc

> sudo apt-get install geany # A programmers editor. You can use another if you like.
                             # The instructions use geany as text editor

> sudo apt-get install git # To easily download rfboot.
```

Download the rfboot repository in your PC. Place it in some relatively safe place in your
PC like ~/programming/

```
mkdir ~/programming
cd ~/programming
git clone https://github.com/pkarsy/rfboot.git
```
The "rftool" utility needs to be in the PATH.
```
> cd ~/bin # if the ~/bin does not exist "mkdir ~/bin" and then logout and login again
> chmod +x ~/programming/rfboot/rftool/rftool
> ln -s ~/programming/rfboot/rftool/rftool
```
***

### usb2rf module

To build the usb2rf module you need an FTDI module a ProMini 3.3V and a CC1101 module and some jumper wires.
More information on 
### [usb2rf module](Building-the-usb2rf-module.md)

***

### Target (your electronics project)

### atmega328 MCU
This  includes a bare atmega328 with some caps etc. It also includes
many arduinos and clones. A very well suited arduino is the ProMini 3.3V. It has 3.3 regulator and can directly drive the CC1101 without level converters. It can powered directly from a Lithium cell, making it ideal for prototyping
Very Importand: You need an ISP programmer (usbasp, usbtiny etc) to burn the bootloader. After that no wires are required.

### CC1101 RF module
In order to communicate with the PC and upload code to MCU, the usb2rf module must be attached to the PC.

### Any other components your particular project needs.
This includes serial devices such as GSM modems, GPS modules witch can be attached to the hardware serial port, because rfboot does not use it.


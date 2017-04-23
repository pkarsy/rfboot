As software outside of this repository is evolving rapidly, some of these comparisons may be outdated when you are reading this.

# differences with optiboot / atmegaboot

### size
The size of rfboot is about 3.5Kbytes witch means that eats 4Kb bootloader space. This is certainly big, especially
compared to optiboot(512 bytes). However what is important is the **free** space that can be used for application code.

- optiboot : 32-0.5 = 31.5 Kbytes

- atmegaboot (proMini) : 32-2 = 30 Kbytes

- rfboot : 32-4 = 28 Kbytes, wich is not as good as 31.5Kbytes but it is OK. If a project really needs more than 28Kb, then it probably needs another MCU anyway.

### MCU interface
optiboot/atmegaboot uses Serial connection, while rfboot uses SPI to communicate with the CC1101 chip. This can be a plus or minus, if other components of your project compete for these recourses. One noteworthy case is if your project uses a serial module like a GSM modem or a GPS module. They can attached to the fast hardware serial port without the need for a Software Serial library. If you ever used one, you know they are slow, especially at 8MHz clock rate. SPI on the other hand is easier to work with multiple devices, and there are also Software SPI
solutions (see Arduino shiftOut() )

### Easy of use

The daily use of rfboot is surprisingly simple. Edit the code click "send" and the code is sent to the correct
MCU even if you happen to have 10 open projects in the editor.
The first installation can be a problem however. Certainly a user should be familiar with basic concepts of MCU (and bootloaders) before try to use a different bootloader than the stock one witch is coming with the Arduinos for example.

### Arduino and rfboot
Generally speaking a bootloader is code agnostic. You can upload any firmware written in C or assembler or Arduino (Which uses the c++ compiler). Arduino is a first class citizen in this case. The arduino IDE is not
used however. Instead the excelllent arduino-makefile is used, witch gives us the power to use any editor. The examples in this site use the **geany** editor, but obvioulsy you can use another.

"rftool create ProjectName" creates a new arduino project with unique RF channel and SyncWord (To prevent RF collitions with other modules) and a unique XTEA key. All this customization is saved in the app_settings.h
file inside the project's folder.

A "make isp" burns the bootloader to the target MCU. This is done once per project.

From now on a "make send" sends the code to the module wirelesly. Most editors can be customized to execute such commands with a click.

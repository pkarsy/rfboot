### The First project

Before you start it is required that you have completed the
[Installation of rfboot](Installation)

There is no need to use any prebuild example.
"rftool create MyNewProject"
Creates a fully working rfboot based project, which will print the uptime on screen
every second. There is no need to configure RF channels XTEA etc. They are selected at
random and they are different at every project.

### Bill of materials (bare atmega)
- atmega328p-pu
- 2 ceramic caps 1uF (4 if you use a 3.3V regulator)
- A breadboard Pushbutton
- A cc1101 RF module with 2.54 pin spacing
- Male to female 2.54 jumper wires, and some simple striped wires
- A 3.3V stabilized power source. Alternatively A lithium-ion cell with a 3.3V regulator
. See photo
- ISP programmer with a 28pin ZIF socket. WARNING the isp programmer is needed only once
for each project, to write rfboot to the target MCU. It is NOT needed for the usb2rf module. 
- A breadboard. Note that a bad breadboard is the basic source of failures in a project.

or

### Bill of materials (pro mini target)
- proMini 3.3V
- A cc1101 RF module with 2.54 pin spacing.
- Female to female 2.54 jumper wires.
- ISP programmer with some Female-Female 2.54 cables.
WARNING: intil you reinstall the old proMini bootloader (atmegaBoot) the proMini
cannot be programmed with FTDI. Mark a "rfboot" in back side to be sure
TODO description and photos not included, but should be easy if you followed the bare
atmega option.

Here is how our project is look like. The LED is not needed but you probabaly want to
play a little with digitalWrite.
![The first project](https://github.com/pkarsy/rfboot/blob/master/help/files/FirstRfbootProject.png)

On the PC, probably in you sketchbook folder (not nesessarily however)
```
rftool create myProject
```
as you can see rftool generates random parameters (using /dev/urandom)
```
cd myProject
```
Here is a photo of the USBasp programmer together with a ZIF developer board. (search ebay)
If you have USBtiny
```
geany rfboot/compile_settings.mk
```
and uncomment the USBtiny line
```
make isp
```
If your project uses a crystal you can edit again "rfboot/compile_settings.mk" or
```
make xtal
```
Rfboot works perfectly with (uncalibrated 10% error) internal oscillator because the SPI
interface does not need a +-2% accurate clock source as Serial needs.
Now put the atmega and the other components in the breadboard.
Power the board and then
```
> make terminal # Opens gtkterm with all apropriate options
> make send # Sends the firmware
```
After the upload is finished
You will see uptime (in ms) to be printer in the gtkterm.

Add this line to setup():
```
PRINTLN("Hello world")l
```
All arduino machinery is working as expected.

***The first rfboot based project is finished !***
### The First project

Before you proceed, it is required that you have completed the
[Installation of rfboot](Installation.md)

Let's start :
```
> rftool create myProject
```
creates a fully working rfboot based project, which will print the uptime on screen
every second. There is no need to configure RF channels XTEA etc. They are selected at
random and they are different at every project.

### Bill of materials (bare atmega)
- atmega328p-pu
- 2 ceramic caps 1uF (Optional, for voltage stability near MCU)
- A momentary pushbutton (optional, for reset)
- A cc1101 RF module with 2.54 pin spacing. The photos show a D-SUN module
- A LED with a suitable resistor.
- Male to female 2.54 jumper wires, and some simple striped wires
- A 3.3V stabilized power source. For example A lithium-ion cell with a 3.3V regulator and
2 additional 1uF caps. LP2950-3.3 and especially HT7333 are very good for this purpose. See picture.
- ISP programmer with a 28pin ZIF socket. The isp programmer is used only once
for each project, to write rfboot to the target MCU.
- A breadboard. Note that a bad breadboard is the basic source of failures in a project.

Here is how our project looks like. It is not connected with anything, and we are going to program it wirelessly.
![The first project](https://github.com/pkarsy/rfboot/blob/master/help/files/FirstRfbootProject.jpg)

On the PC, possibly in you sketchbook folder
```
> rftool create myProject
```
as you can see rftool generates random parameters (using /dev/urandom), and of course
a "myProject" folder containing all information/data our project needs.
```
> cd myProject
```
Now it is time to burn the atmega with rfboot, using the ISP programmer.

Here is a photo of the USBasp programmer together with a ZIF developer board. (search ebay)
![USBasp](https://github.com/pkarsy/rfboot/blob/master/help/files/usbasp.jpg)
If you have USBtiny edit the file "rfboot/hardware_settings.mk" and uncomment
the USBtiny line. Put the atmega328p-pu chip on the ZIF socket and :

```
> make isp
```
To write th bootloader.
If your project uses a crystal you can edit "rfboot/hardware_settings.mk" before burn
or simply
```
> make xtal
```
Don't do this on this first project however, to keep things simple. 
Rfboot works perfectly with (uncalibrated up to 10% error) internal oscillator because the SPI
interface does not need a +-2% accurate clock source as Serial needs.

Now put the atmega and the other components in the breadboard.
Power the board and then
```
> make terminal # Opens gtkterm with all apropriate options
> make send # Sends the firmware
```
After the upload is finished, you will see uptime (in ms) in gtkterm.
If you are succesful, you can start playing with the code.

For example, add this line to setup():
```
PRINTLN("Hello world");
```
All arduino code is working as expected, with the exception of Serial.print(). The
PRINT and PRINTLN macros can be used instead which use sprintf internally.
```
i=2;
PRINT("i=%d",i);
j=7;
k=8;
PRINTLN("j=%d k=%d",j,k);
```
As you can see they look like the standard printf function.

Press '0' to '9' to see the response.

Now uncomment the lines related to the LED, inside setup()
and loop(). Please read the notes.

***The first rfboot based project is finished !***

Spend 2 minitues to configure geany (or your editor) to execute

"make terminal"

"make clean"

"make send"

with a menu entry, or keyboard shortcut

![Gtkterm menu](https://github.com/pkarsy/rfboot/blob/master/help/files/MenuEntry.png)

### The First project

Before you proceed it is required that you have completed the
[Installation of rfboot](Installation.md)

There is no need to use any prebuild example.
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
- A 3.3V stabilized power source. For example A lithium-ion cell with a 3.3V regulator and 2 additional 1uF caps
. See photo
- ISP programmer with a 28pin ZIF socket. The isp programmer is used only once
for each project, to write rfboot to the target MCU. After this no cables are necessary for upload.
- A breadboard. Note that a bad breadboard is the basic source of failures in a project.

Here is how our project looks like.
![The first project](https://github.com/pkarsy/rfboot/blob/master/help/files/FirstRfbootProject.jpg)

On the PC, possibly in you sketchbook folder
```
> rftool create myProject
```
as you can see rftool generates random parameters (using /dev/urandom)
```
> cd myProject
```
Here is a photo of the USBasp programmer together with a ZIF developer board. (search ebay)
![USBasp](https://github.com/pkarsy/rfboot/blob/master/help/files/usbasp.jpg)
If you have USBtiny edit the file "rfboot/hardware_settings.mk" and uncomment
the USBtiny line. Now:

```
> make isp
```
To write th bootloader.
If your project uses a crystal you can edit "rfboot/hardware_settings.mk" before burn
or simply
```
> make xtal
```
Rfboot works perfectly with (uncalibrated up to 10% error) internal oscillator because the SPI
interface does not need a +-2% accurate clock source as Serial needs.
Now put the atmega and the other components in the breadboard.
Power the board and then
```
> make terminal # Opens gtkterm with all apropriate options
> make send # Sends the firmware
```
After the upload is finished
You will see uptime (in ms) in gtkterm.

Add this line to setup():
```
PRINTLN("Hello world");
```
All arduino machinery is working as expected with the exception of Serial.print. The
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

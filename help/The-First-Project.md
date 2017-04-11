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
. See below for this option.
- ISP programmer with a 28pin ZIF socket
- A breadboard. Note that a bad breadboard is the basic source of failures in a project.

or

### Bill of materials (pro mini target)
- proMini 3.3V
- A cc1101 RF module with 2.54 pin spacing.
- Female to female 2.54 jumper wires.
- ISP programmer with some Female-Female 2.54 cables.
WARNING: intil you reinstall the old proMini bootloader (atmegaBoot) the proMini
cannot be programmed with FTDI. Mark a "rfboot" in back side to be sure

TODO ...

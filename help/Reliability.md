### Reliability

In the following section lets imagine a electronic project wich is problematic
to attach wires such as a **weathering station on the roof** a **quadcopter**
or a **model boat on a pool**. We use the cc1101 chip
to communicate with our  project, but we also want to update the code via the same RF link.
And most importantly, whatever failure happens while uploading, we should be able to
repeat the process until we succeed.

Here is explained what rfboot does to accomplish this job

At first, a bootloader runs only at power on or after a MCU Reset. The aproach we take here is
that the application must implement a mechanism to software reset the MCU, whenever
we need to update the firmware. A simple solution is to listen for incoming RF packets.
When an incoming packet contains exactly the string ie "Reset" then the application resets
the device giving control to rfboot.

The rftool utility, whenever creates a new empty project with **"rftool create ProjName"**
includes this mechanism in the .ino file.

As long as this mechanism works, we can upload code to the device literally thousands of
times, without any need to even touch the device. if however the device for some reason
(usually software bug)
stops responding to the reset request, then it is impossible to reprogram it remotely.
We need to access the reset button (we need to go to the roof for example).

rfboot avoids (not always as we will see, but close) this type of failure with the following precautions :

- If rfboot does not receive a packet for 20ms, then requests a retransmission, and of  course
rftool on PC side, resends the packet. This is the simplest defence but it is very
effective. Almost all uploads complete, even if some (or even a lot) retransmissions occur.

- If the upload process is interrupted (completly), rfboot detects it and waits for new firmware. Even
after a power cycle, rfboot refuses to start the application, and waits indefinitely for
new upload.

- At the event of a power loss or MCU reset while programming, when the power comes back rfboot
will detect the corrupted code and stays waiting for new firmware. When -eventually- the upload
process finishes correctly, only then rfboot gives control to the application.

- If the upload process is finished (seemingly) correctly but some packet arrived with some wrong bytes, rfboot
will detect that the flashed firmware does not have the correct CRC, and of course it does not start the application.
It is waiting for new firmware.

- rfboot enables the watchdog timer before the application starts. If for some reason the
application is malfunctioning, watchdog timer will eventually reset the device allowing to reprogram it.
**Note: If the code resets the watchdog periodically, but fails to respond to the reset string, then you are locked out. You will need to access the reset or power button.**

- Every time rftool creates a new empty project, gives unique channel, syncword, and XTEA
key to the project. This is in turn makes it impossible to send the code to the wrong device (essentially "bricking" it).
Even if we work in parallel with multiple modules, the code always go to the correct MCU.

**As we see, it is unfortunatelly possible to "brick" the remote target, by sending buggy code.**<br/>
Buggy code in this case means code who does not respond to a reset request (from the PC).
**Always test the firmware at the lab before uploading at the field.** <br/>
Generally it is a good idea to have the reset (or power) button somewhat accessible, for occasional use.


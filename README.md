# rfboot

Atmega328p Bootloader for wireless (OTA) code uploads, using the CC1101 RF chip.


Rfboot is very reliable. I use it for all my projects, even those without any need for RF connectivity, because I can work with dozens of projects in parallel without any need to swap serial cables. There are very few failed uploads (less than 1% in my tests), and fixed immediately with a second upload.</br>
Special care is taken (while rfboot is evolving) that all older rfboot installations are compatible with the newest uploading tool.</br>

Included :

- The atmega328 bootloader, intended to be installed to thhis MCU via an
ISP programmer.
- A suitable utility (rftool) to easily create new projects (the directory structure) using rfboot as bootloader.
The same tool is used for code upload. Every project is created with unique RF settings and a
preconfigured Makefile. While developing, a "make send" uploads the code wirelessly.
- Instructions to assemble a **usb2rf module**, witch allows us to upload code to the target and,
**equally important**, to use a Serial Terminal (like gtkterm) to send and receive text,
analogous to Serial.print()  and Serial.read().

This project's emphasis is on reliability, security and usability.

- [Installation.](help/Installation.md) Software and hardware you need, in order to upload code using rfboot.
- [The first project.](help/The-First-Project.md) Instructions to successfully complete your first Arduino project using a bare atmega328 as target, and rfboot as bootloader.
- [Reliability.](help/Reliability.md) Explains how rfboot does the job, under an inherently unreliable link (RF).
- [Encryption.](help/Encryption.md) Discuses how the firmware is encrypted over the air, and why this is useful.
- [Notes about the License.](help/Notes-about-the-License.md) Some clarifications of the License.

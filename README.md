# rfboot

# Importand note : As time passes, the limitations of rfboot become more and more obvious. Only atmega328 with cc1101 is supported and no itegration with platformio. If you are interested on OTA updates consider using another programmer like the excellent [esp-link](https://github.com/jeelabs/esp-link)

Atmega328p Bootloader for wireless (OTA) code uploads, using the CC1101 RF chip.

Included :

- The actual atmega328 bootloader, intended to be installed to this MCU via an ISP programmer.
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

# rfboot

Bootloader for Atmega328 over the air (OTA) using the TI CC1101 chip.

Included :

- The actual bootloader, intended to be installed to the project's MCU (atmega328), via an
ISP programmer.
- A powerful utility (rftool) to easily create new projects using rfboot as bootloader.
The same utility also sets RF channels etc. and uploads the code. Every new project has
preconfigured the Makefiles, and "make isp" and "make send" do all the job  needed.
- Instructions to build a usb2rf module, witch allows us to upload code to the target and,
**equally important**, to use a Serial Terminal (like gtkterm) to send and receive text
analogous to Serial.print()  and Serial.read().

This project aims at speeding up and simplifying work-flow, by allowing the program to be
uploaded via the RF link. The emphasis is on reliability, security and usability. Even
if the final project has a different bootloader, or no bootloader at all, at the phase of
development can be very useful.

- [Installation](help/Installation.md) A bootloader, and especially a wireless one, is a relatively complex topic. In this section instructions are given to make the process as straightforward as possible.
- [The first project](help/The-First-Project.md) Instructions to successfully complete your first Arduino project using a bare atmega328 as target, and rfboot as bootloader.
- [Reliability](help/Reliability.md) Explains how rfboot does the job, under an inherently unreliable link(RF).
- [Encryption](help/Encryption.md) Discuses how the firmware is encrypted over the air, and why this is useful.
- [Notes about the License](help/Notes-about-the-License.md) Some clarifications of the License.

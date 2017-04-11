# rfboot

**Notice: The bootloader is in a quite workable state. However the site is incomplete and a lot of modifications need to be done.
I hope in a few days (say until 15 April 2017) the code, help pages, etc will be added and this notice will be removed.**

Bootloader for Atmega328 over the air (OTA) using the TI CC1101 chip.
Included :

- The actual bootloader, designed to be installed to the projects MCU(atmega328) via an
ISP programmer.
- A powerful utility (rftool) to easily create new projects using rfboot as bootloader.
The same utility also sets RF channels etc. and uploads the code. Every new project has
preconfigured the Makefiles, and "make isp" and "make send" do all the job  needed
- An Arduino library for CC1101 (panStamp based)
- Instructions to build a usb2rf module, witch allows us to upload code to the target and
-**equally important**- to use a Serial Terminal (ie gtkterm) to send and receive text just
like Serial.print() and Serial.read()

A lot of electronic projects need to communicate with other components via an RF module.
This project aims at speeding up and simplifying work-flow, by allowing the program to be
uploaded via this RF link. The emphasis is on reliability, security and usability.Even
if the final project has different bootloader or no bootloader at all, at the phase of
development can be very useful.

- [Uploading code with rfboot](help/Uploading-code-with-rfboot.md) A demonstration of what rfboot can do. TODO
- [Installation](help/Installation.md) A bootloader, and especially a wireless one, is a relatively complex topic. In this section instructions are given to make the process as straightforward as possible.
- [The first project](help/The-First-Project.md) Instructions to successfully complete your first Arduino project using a ProMini 3.3V as target, and rfboot as bootloader. TODO
- [Reliability](help/Reliability.md) Explains how rfboot does the job, under an inherently unreliable link(RF)
- [Encryption](help/Encryption.md) Discuses how the firmware is encrypted over the air, and why this is useful
- [Notes about the License](help/Notes-about-the-License.md)

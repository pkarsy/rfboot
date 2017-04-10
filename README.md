# rfboot

**Notice: The bootloader is in a quite workable state. However the site is incomplete and a lot of modifications need to be done.
I hope in a few days (say until 10 April 2017) the code, help pages, etc will be added and this notice will be removed**

Bootloader for Atmega328 over the air (OTA) using the TI CC1101 chip.
Also included
- A powerful utility (rftool) to easily create new projects using rfboot as bootloader.
The same utility also sets RF channels etc. and uploads the code. 
- An Arduino library for CC1101 (panStamp based)
- Instructions to build a usb2rf module, wich allows us to upload code to the target and
-**equally important**- to use a Serial Terminal (ie gtkterm) to send and receive text just
like Serial.print() and Serial.read()

A lot of electronic projects need to communicate with other components via an RF module. This project aims at speeding up and simplifying workflow, by allowing the program to be uploaded via this RF link. The emphasis is on reliability, security and usability.

- [Uploading code with rfboot](https://github.com/pkarsy/rfboot/wiki/Rfboot-in-Action) A demostration of what rfboot can do. **VIDEOS NOT READY YET**

- [Installation](help/Installation.md) A bootloader, and especially a wireless one, is a relatively complex topic. In this section instructions are given to make the process as straightforward as possible.

- [The first project](https://github.com/pkarsy/rfboot/wiki/Fist-Project) Instructions to succesfully complete your first Arduino project using a ProMini 3.3V as target, and rfboot as bootloader. **NOT READY YET**

- [Reliability](https://github.com/pkarsy/rfboot/wiki/Reliability) Explains how rfboot does the job, under an inherently unreliable link(RF)

- [Encryption](https://github.com/pkarsy/rfboot/wiki/Encryption) Discuses how the firmware is encrypted over the air, and why this is useful

- [Notes about the License](https://github.com/pkarsy/rfboot/wiki/Notes-about-the-License)

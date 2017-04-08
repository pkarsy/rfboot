# rfboot

**Notice: The bootloader is in a quite workable state. However the site is incomplete and a lot of modifications need to be done.
I hope in a few days (say until 10 April 2017) the code, help pages, etc will be added and this notice will be removed**

Bootloader for Atmega328 over the air (OTA) using the TI CC1101 chip.
Also included
- A powerful utility (rftool) to easily create new projects using rfboot as bootloader
- An Arduino library for CC1101 (panStamp based)

A lot of electronic projects need to communicate with other components via an RF module. This project aims at speeding up and simplifying workflow, by allowing the program to be uploaded via this RF link. The emphasis is on reliability, security and usability.

- [Rfboot in Action](https://github.com/pkarsy/rfboot/wiki/Rfboot-in-Action) A good starting point. And gives us an idea of what tools we need to use rfboot. **VIDEOS NOT READY YET**

- [Installation](https://github.com/pkarsy/rfboot/wiki/Installation) A bootloader, and especially a wireless one, is a relatively complex topic. In this section instructions are given to make the process as straightforward as possible.

- [The first project](https://github.com/pkarsy/rfboot/wiki/Fist-Project) Instructions to succesfully complete your first Arduino project using a ProMini 3.3V as target, and rfboot as bootloader. **NOT READY YET**

- [Reliability](https://github.com/pkarsy/rfboot/wiki/Reliability) Explains how rfboot does the job, under an inherently unreliable link(RF)

- [Encryption](https://github.com/pkarsy/rfboot/wiki/Encryption) Discuses how the firmware is encrypted over the air, and why this is useful

- [Notes about the License](https://github.com/pkarsy/rfboot/wiki/Notes-about-the-License) This is a very important topic, but I Am Not A Lawyer. If you see some mistakes, pls inform me.

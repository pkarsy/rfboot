### Components and Porting

- RF<br>
For RF connectivity, the most common solution is to use 2.4GHz
transceivers like the very popular NRF24L01+ chip. I found however the distance range to be very short. Above 20m even the slightest obstacle can interrupt the signal. Also this
band seems overcrowed with all wifi routers/APs/MicrowaveOvens around. Signal theory also predicts that sub GHz signal has some advandages over 2.4GHz. Lower power to reach the same distance and more importantly much better penetration/obstacle tolerance. I tried a lot of sub-GHz tranceivers and I found the CC1101 chip to be the most flexible and also to have simple libraries with adueqate licence. The difference with the NRF24L01+ module when we speak about signal reception, is very big. Note however that the NRF24 modules are optimized for speed (minimum 250 Kpbs, probabbly the main target is keyboards, mice, and other wireless peripherals) so the comparison is not fair. The CC1101 also has support for WOR (wake on radio) wich is basically mandatory for battery powered projects listening for radio signals.

- MCU<br>
The use of atmega328p is very natural. It is a well supported chip with tons of online
information. It powers a lot of arduino boards, and can be programmed with standard arduino code. And the truth is that atmega328 has the right size (IO and RAM/Flash) for a lot of small projects.

- Porting<br>
I expect that porting rfboot to another avr MCU should not be hard, if such a need arises. The use of another RF chip can be a little harder however, with the exception of CC2500 wich is almost identical to CC1101 (It is a 2.4GHz transceiver) but I did't try this.

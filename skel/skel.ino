/*
    This file is part of rfboot
    https://github.com/pkarsy/rfboot
    and it is given to the Public Domain. This means you can do
    anything with it, including removing this notice.
*/

#include <avr/wdt.h>
#include "app_settings.h"
#include <mCC1101.h>
mCC1101 rf;

// These macros enables us to "print" messages via the RF
// link. They use the rf.print(..) which is implemented in mCC1101.cpp
#define PRINT(format, ...) rf.print( F(format), ##__VA_ARGS__)
#define PRINTLN(format, ...) rf.print( F(format "\r\n"), ##__VA_ARGS__)

// Interrupt from CC1101 gdo0 on PIN2(INT0)
void cc1101_interrupt(void) {
    // Becomes true when a packet is received
    // or after a packet is transmitted
    // "interrupt" variable is implemented inside mCC1101 class
    // getPacket and sendPacket can manipulate it if necessary
    rf.interrupt = true;
}

void setup() {
    // rfboot enables watchdog (2 sec timeout) automatically
    // we can disable it with wdt_disable(); but is not recommended
    // as a misbehaving code will lock the module and we will no be
    // able to resend code remotelly, ie without resetting the module

    // Althrough not needed by rfboot itself, almost all rfboot projects
    // also use the CC1101 module for connectivity witho othe modules/PC
    rf.init();
    // CFREQ_433 is the default with mCC1101
    // note that this is different than PanStamp library
    // rf.setCarrierFreq(CFREQ_433);
    // APP_CHANNEL and APP_SYNCWORD are defined in "app_settings.h"
    // and generated randomly by "rftool create ....."
    rf.setChannel(APP_CHANNEL);
    rf.setSyncWord(APP_SYNCWORD[0], APP_SYNCWORD[1]);
    rf.disableAddressCheck();

    // with the default register settings of the library
    // CC1101 chip asserts gdo0 LOW when a packet received
    // or after a packet is sent.
    // gdo1 is not used.
    attachInterrupt(0, cc1101_interrupt, FALLING);

    // Uncomment to use a LED in A5
    // pinMode(A5,OUTPUT);

    PRINTLN("Hello world, press some printable key");
}

void loop() {
    // As we said, rfboot has already enabled the watchdog timer.
    // This means we must reset it periodically, to avoid a reset.
    // If an operation/function inside loop blocks for more than 2sec,
    // the module will reset by the watchdog automatically.
    // This is good as it prevents a halted/non responding  module.
    wdt_reset();

    // this block of code is for demonstation purposes and disabled by default
    // It is a hint that at least cc1101 module
    // is correctly wired remove it to continue with your application.
    if (false) {
        // we print a message every 1000ms=1sec
        // the clock can be off by a few % because
        // RC oscillator is not very accurate
        const unsigned int DELAY = 1000;
        static uint32_t old_uptime_ms = 0;
        uint32_t current_uptime_ms = millis();
        if (current_uptime_ms - old_uptime_ms >= DELAY) {
            // Generally avoid to print a lot of messages, as the link is half duplex.
            // PRINT and PRINTLN use the F() operator, so the string literals do not
            // consume precious RAM. It is equivalent to
            // rf.print(F("formatString"), arg1, arg2, ..)
            PRINTLN("uptime = %lu sec", current_uptime_ms/DELAY);
            // we could just write
            // old_uptime_ms = current_uptime_ms;
            // but we want it to be multiple of 1000
            old_uptime_ms = current_uptime_ms / DELAY * DELAY;
        }
    }

    if (rf.interrupt) {
        byte packet[64];
        byte pkt_size = rf.getPacket(packet);
        rf.interrupt = false;
        if (pkt_size>0 and rf.crc_ok) { // We have a valid packet with some data
            // The following code resets the MCU when it gets the RESET_STRING (is defined in "app_settings.h")
            // This is for wireless firmware updates without physical contact
            const uint8_t RESET_LEN = strlen(RESET_STRING) ;
            if ( pkt_size==RESET_LEN and  memcmp( (char*)packet, RESET_STRING, RESET_LEN)==0 ) {
                // rftool on PC side, requires echo back
                PRINT("%s",RESET_STRING);
                wdt_enable( WDTO_15MS );
                // After 15ms -> reset
                while (1) {};
            }
            // here you can put code to check for any input
            if (pkt_size==1) {
                byte c = packet[0];
                if (c>=32 and c<=126)  {
                    PRINTLN("You pressed \"%c\"", packet[0]);
                }
                else {
                    PRINTLN("Non printable ascii char");
                }
            }
            else {
                PRINTLN("Got packet with size %d", pkt_size);
            }
            // if (pkt_size==4 ....
            // you will need strcmp/memcmp for this and VERY IMPORTAND
            // you cannot type 4 chars in the serial terminal. The fingers are
            // very slow and probably the 4 chars will arrive as 4 packets, 1
            // byte each. However you can paste text (up to 32 chars) and will
            // arrive as a single packet
        }
    }

    // Uncomment to make LED blink, Be sure to also uncomment the "pinMode" in setup()
    // We use a method that does not block the execution flow, we avoid delay(..) in
    // other words. Change the numbers to see what it happens. This is only an example
    // but all code you write must have this in mind: No delay(..) inside loop. If
    // the delay is small enough (some ms) to support some specific function/device,
    // then is OK i guess.

    // digitalWrite(A5, millis()%1000<500);
}


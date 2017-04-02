/*
	This file is part of rfboot
	https://github.com/pkarsy/rfboot
*/

#include <avr/wdt.h>
#include "app_settings.h"
#include <CC1101.h>
CC1101 rf;

// This function enables us to "print" messages via the RF
// link. It is based on printf and in fact is vastly more
// functional than Serial.print
bool rfprint(const __FlashStringHelper* fmt, ...) {
	byte packet[64];
	va_list args;
	va_start(args, fmt);
	byte pkt_len = vsnprintf_P( (char*)packet,60, (const char*)fmt, args );
	va_end(args);
	return rf.sendPacket(packet,pkt_len);
}

// Interrupts from CC1101 (INT0) gdo0 on PIN 2
void cc1101signalsInterrupt(void) {
	// Becomes true when a packet is received
	// or after a packet is transmitted
	rf.interrupt = true;
}

void setup() {
	rf.init();
	// CFREQ_433 is the default
	// rf.setCarrierFreq(CFREQ_433);
	rf.setChannel(APP_CHANNEL);
	rf.setSyncWord(APP_SYNCWORD[0],APP_SYNCWORD[1]);
	rf.disableAddressCheck();
	attachInterrupt(0, cc1101signalsInterrupt, FALLING);
}

void loop() {
	// rfboot bootloader already enables watchdog timer
	// we must to reset it periodically
	wdt_reset();

	{
		const int DELAY = 1000;
		static uint32_t timer = millis();
		if (millis()-timer>=DELAY) {
			rfprint(F("timer = %lu\r\n"),timer);
			timer+=DELAY;
		}
	}

	if (rf.interrupt) {
		byte packet[64];
		byte pkt_size = rf.getPacket(packet);
		rf.interrupt = false;
		if (pkt_size>0 and rf.crc_ok) //Serial.write(packet.data,pkt_size);
		{
			// The following code resets the MCU when it gets the RESET_STRING (is defined in "app_settings.h")
			// This is for wireless firmware updates without physical contact
			const uint8_t RESET_LEN = strlen(RESET_STRING) ;
			if ( pkt_size==RESET_LEN and  memcmp( (char*)packet, RESET_STRING, RESET_LEN)==0 ) {
				// rftool on PC side, requires echo back
				rfprint(F("%s"),RESET_STRING);
				wdt_enable( WDTO_15MS );
				while (1) {};
			}
		}
	}
}


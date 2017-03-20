/*
    2015 C Panagiotis Karagiannis
    * This software is distributed with the GPLv3+ Licence

*/


/* rfboot v0.6 wireless bootloader for atmega328p. Can use the nrf24L01 or the cc1101 chip
 * this file implements the bootloader part of rfboot
 *
 * - it is not based on optiboot or other bootloasers but is
 * written from scrach.
 * - Does not use EEPROM. Configurable settings can only
 * be changed at compile time.
 * Cannot be used to send code with serial port. In fact does not even touch the Rx Tx pins wich can be used
 * for other purposes (as GPIO pins) or to connect to another serial device (GPS for example)
 * RELIABILITY. This is where rfboot really shines.
 *
 * In the following note "brick" the device we mean that we cannot upload firmware
 * REMOTELLY. We can always upload code with physical access to the reset pin or by power cycle the atmega chip.
 *
 * And before all we need
 * to define what we want from rfboot
 * Here is the scenario:
 * We have a atmega328p powered device burried in a wall, in a roof, in a robot, or whatever
 * We have it running an application wich we can remotelly control (possibly via the RF chip but this is not necessary), and tell it
 * to reset in order to remotelly update the application code. This is the intended use
 * of rfboot. There are other bootloaders doing the same, free like rfboot and most are
 * compatible with avrdude which obviously is a plus (rfboot isnt) .
 * In order to update the firmware of our appliance, we need first to tell to the app we want to reset the chip
 * in order the bootloader to take control.
 * most bootloaders (wireless or not) is that even if the upload process fails, finally the control
 * passes to the half written app.
 * Of course the app will crash with probbability ~100% and then you cannot tell to
 * the application, PLEASE reboot again ! I cannot access the reset button !
 * optiboot-rf solves it by a very clever trick. Before bootloader gives control to the application
 * bottloader enables Watchdog 4 sec timer. If the application really crashes the watchdog reset MCU
 * and gives us the time to update the code. This approach requires of course that the app resets the watchdog timer periodically. Which is a good thing anyway.
 * At this stage you need hammers, screwdrivers etc to have
 * physical access to the device
 * rfboot also enables watchdog timer (see comment below)
 * rfboot however does it differently. The first info it receives from our side is the size
 * and the checksum of the application. (This is the reason I could not do rfboot to
 * be compatible with stk500/avrdude.)
 * if for some reason the upload fails, rfboot knows the app is not correctly
 * written and will never try to execute such an app. Instead it is waiting patiently
 * for you
 * to send new code until you succeed. Note that it not only withstands failures from
 * our side (bad RF signal for example, or a Laptop with the battery
 * failing at the wrong time) It also withstands failures from the avr side,
 * like power loss, brownout , hardware resets etc.
 * So the only way to brick the device is to upload a mulfunctioning app wich refuses
 * to reset the device due to a software bug. Given that you already tested the app in
 * your lab and you are not trying to send the wrong app to the wrong chip, this
 * possibility can be quite small.
 *
 * Bootloader for atmega328 mcu for wireless and optionally encrypted
 * code uploads using the inexpensive nrf24 module. It is very speedy
 * and according to my tests very reliable.
 * Even if you kill the upload process in the middle, rfboot detect it
 * and waits for a new upload session. Even if cut the power from the
 * atmega at the time of programming, when the power comes back rfboot
 * will detect it and will not try to start any corrupted code but
 * instead stays waiting for new code. When eventually the upload
 * process finishes, rfboot reads back the flashed program, calculates
 * the crc, and checks if it is equal with the crc came from the
 * programmer.
 * only then the program starts
 * if you like the idea of encryption, you have to also disable code
 * extraction  using an ISP programmer. The encryption key is saved
 * somewere in the bootloader area. Using OTA encryption and at the
 * same time allow code extraction from the chip doesnt make a lot of
 * sense except when developing-debugging. Start with Lock fuse
 * setting 0x0C. I read in many forums that the code can be readen with
 * the correct equipment and there are companies doing this for you for
 * a premium. So again the confidiality of the code is not guarandeed.
 * I mainly added encryption because it was very easy. I really have no
 * idea of the level of "security" it offers
 * this bootloader can be very useful when the mcu is hard to
 * disassemble from the installed location, but you still need to make code changes.
 * we send a signal to the loaded application to (possibly save any settings to eeprom and) reboot the mcu via watchdog
 *
 * Then the rfprogrammer (the eqivalent of avrdude) sends the code_size the CRC and finally the code in packets of 32 byte
 * checksum of the app. The bootloader writes the code to flash and in the end reads the writen flash
 * contents and recalculates the CRC. if is correct, jumps to the newly uploaded
 * app. The upload process is happening in reverse byte order and if for some reason is interrupted, then the first pages of flash have the known 0xFF pattern of an unprogrammed chip,
 * Of course if the flash is empty the bootloader stays waiting for code upload
 * if the mcu reset with the reset pin rfboot wait for code upload for a few seconds. This can disabled at compile time.
 * if mcu starts from power or brown out goes directly to app (if there is one)
 * uses nrf24l module for code upload
 * WARNING ! cannot initialize a reset, the duty for this is in the application. I consider this not a problem  because of the intended use of this bootloader.  if you are developing a non "hello world" program the hasle to add some code for this function is not an issue. Also -generally speaking- a real life program probably wont like uncoditional resets with the hardware reset pin because maybe some variables need to be saved in EEPROM. So a polite "please reset" request to the application has better results for a useful in real life,  but still possible to update application. For many purposes all these things are of course very frustrating and a bootloader like optiboot is preferred.
 *
 *
 * WARNING ! Don't install this bootloader to an arduino board. It does not use serial UART. This means, if you burn this bootoader to an arduino UNO for example, you will not be able to program it over USB any more. Only via nRF module. The same applies to a pro-mini which is programmed via a FTDI module etc.
 * rfboot is designed to be used on bare atmega328p chips. Of course you can still -as I do- develop Arduino applications, a bootloader is code agnostic.
 * WARNING ! rfboot cannot protect you from buggy code. You can upload anything to the MCU, a prog that freezes or fails to communicate with you or just ignores a reboot command from your side. Or -more dangerously- a code written for another station/project. Then you need physical access to the mcu to be able to reset it. If this is trouble, test your code first.

 * The hardware for rfboot is typical is atmega328p 8Mhz@3.3V with internal oscillator or external crystal/resonator. FUSES = E2 DA 05
 *  As nrf24 does not tolerate 5V on Vcc and atmega32 does not run 16Mhz@3.3V
 * The fact that rfboot does not initialize UART has an interesting effect.
 * You can use Hardware Serial to connect to a serial device (a GSM
 * modem for example) instead of relying on software serial. My tests show that software serial must run on slow speeds like 9600 or 4800 to be reliable, especially at 8Mhz or 1Mhz. Note however that high Baud rates may dictate to use an external crystal instead of internal oscillator.
 * not compatible unfortunatelly with avrdude. rfboot uses a companion program rfprogrammer instead of avrdude. Moreover it also needs a USB to RF adapter connected to the PC. This module is easy enough to build but adds to the effort to make this bootloader working in the first time.
 * Arduino users: You can continue to use arduino libraries and environment because a bootloader is code gnostic. You have to modify the upload settings of your environment however  //delete in order to upload code as you did with optiboot (UNO) or atmegaboot (ProMini) //delete
 * If you are using Arduino-Makefile (package arduino-mk on Debian Ubuntu Mint) the solution is simple:
 * add the following lines of code to the Makefile of interest. upload: .......
 *
 * WARNING ! It uses encryption to prevent code sniff, but it does not
 * implement any secure communication protocol. This would require a
 * much more complicated bootloader, without any reason. Unless you have
 * fear an evil Malory will perform a man in the middle attack on your
 * humble atmega or a reply attack etc. Again I implemented encryption
 * because it was easy and I liked to play with this option but I cant guess
 * the level of "security" offered.
 *
 * WARNING ! Do not rely on the default nrf24 settings when accessing the
 * nrf24 chip from within your app. rfboot starts before your application
 * and changes the default settings of the nrf24 chip.
 * if your application uses the nrf24 chip and works correctly with
 * another bootloader (optiboot, atmegaboot etc)
 * but not with rfboot the reason is simple but can drive you mad :
 * The application is not setting all the registers of nrf24
 * so some settings are the rfboot settings and not the default.
 * for a robust application you MUST set all these registers:
 * RFB library wich is distributed together with rfboot
 * has a convenience function radio_setup wich does exactly this.
 * DOC: Thing twice before change the CSN pin from the default arduino PIN 10 is
 * the SlaveSelect (SS) pin
 * According to avr dovumentation if this pin becomes INPUT and LOW the the
 * SPI hardware switches in slave mode causing your RF module to stop functioning
 * mesteriously ! Do this only if you need this particular PIN and use it only as
 * output. The ce pincan be changed freely from the default 9
 * in pro minis for example this is very convenient to  be the A0 pin
 * */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/boot.h>
#include <inttypes.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <avr/wdt.h>
#include <util/atomic.h>
#include <util/crc16.h>

#include "xtea.h"

//#include <stdbool.h>
#define byte uint8_t

// some simple macros for easy GPIO handling
// I found it from many sources and it is probably public domain
//#include "pin_macros.h"

// Seting specific to this rfboot instance
// channel address chip etc
// generated by "rftool"
#include "rfboot_settings.h"

#ifdef CHIP_NRF24L01
#include "spi/spi.h"
#include "spi/spi.c"
#include "mirf/mirf.h"
#include "mirf/mirf.c"
#endif


// The max payload a packet can contain. Limited by the hardware to 32 bytes on nrf24L01
// CC1101 has 64 bytes buffer but we restrict it to 32 bytes mainly because the code
// was first created for nrf24l01
#define PAYLOAD 32

// this is a random enough number contained in the first packet.
// rfprogrammer sends it and rfboot requires this number to be
// in the start of the header in order to continue. This basically protects
// the bootloader from using any random packet, if happens
// to be in the air, as a header
// TODO START_SIGNATURE in the settings.h file and RANDOM
#define START_SIGNATURE 0xd20f6cdf

// TODO remove
// when receives a valid header,
// rfboot sends back the protocol version it uses
// this is for future proofing. A newer rfupload will detect an earlier
// rfboot and will act accordingly. At the time of writing there is no other
// protocol version than 1
#define RFBOOT_PROTOCOL_VERSION 1

// This magic trick helps to convert a preprocessor variable to a string literal.
// we need it for RFADDR.
// #define _STRINGIFY(s) #s
// #define STRINGIFY(s) _STRINGIFY(s)
// RFADDR is passed as a variable from rfupload

// The preprocessor variable RFBOOT_ADDRESS is defined in the Makefile
//static uint8_t* RFBOOT_ADDRESS = RFBOOT_ADDRESS;

// xtea key value is not included here. the helper program rfboot-gen
// pass the value of the key at compile time.
// we have
// 4 comma separated integers (uint32_t) in the form ie 1,2,3,4
// so we enclose it in brackets to generate a valid C array.
// This is the format xtea algorithm needs
//static uint32_t const key[4] ={ XTEAKEY };


// this is the structure of the first packet and contains the header.
// Total is 11 bytes. the other 21 bytes are unused.
struct start_packet {
    uint32_t start_signature1; //prepei na einai START_SIGNATURE
    uint16_t app_size;
    uint16_t app_crc;
    uint16_t app_crc2;
    uint16_t round;
    uint32_t start_signature2;

    //uint8_t protocol_version;
    //uint8_t unused;
    //uint32_t start_signature2;
};

// The xtea algo uses blocks of 8 bytes
#define XTEA_BLOCK_SIZE 8

// This is from wikipedia xtea article
// A encipher function is not included
// as rfboot only decrypts packets and does not
// send any encrypted packet
/* void xtea_decipher( uint32_t v[2], uint32_t const key[4]) {
    static const uint8_t num_rounds=32;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (uint8_t i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
} */


// rfboot is designed to be a little smaller than this size
// FUSE for this bootloader size TODO
#define BOOTLOADER_SECTION_SIZE 4096

// This is the status codes rfboot is sending back //to the programmer
const uint8_t RFB_NO_SIGNATURE = 1;
const uint8_t RFB_INVALID_CODE_SIZE = 2;

const uint8_t RFB_ROUND_IS = 3;
const uint8_t RFB_SEND_PKT = 4;

const uint8_t RFB_WRONG_CRC=5;
const uint8_t RFB_SUCCESS=6;
//const uint8_t RFB_WRONG_PROTOCOL_VERSION=8;
//const uint8_t RFB_START_W_ENCRYPTION=3;
//const uint8_t RFB_START_CLEARTEXT=4;
//const uint8_t RFB_FLASH_INTERRUPTED=5;

// rfboot initializes the SPI bus so it is not wise to start directly the
// application code. Instead a MCU (with Watchdog) reset is triggered and after this the application
// code starts. With this mechanism we can be sure that the state of the MCU is correct
// This is also the method optiboot uses.
// However unlike optiboot, rfboot must know who is triggered the watchdog reset.
// If triggered from the app rfboot must wait for firmware update.
// If triggered from rfboot itself then the appliation code should start

// The solution is to use a global variable (witch basically means a specific location in the atmega RAM)
// the contents of which will inform rfboot who is triggered the reset.

// if this (unitialized on purpose) variable has the value RESET_BY_RFBOOT
// rfboot knows that ITSELF triggered a wdog reset with
// the intention to start the application and not load itself again.
// memory is preserved accross resets so appart from EEPROM -wich
// we dont want to touch- is the only way to send info from one instance
// of rfboot to the next (after a reset)
// no register !!!
const uint32_t RESET_BY_RFBOOT=0xd8317bc2;
volatile uint32_t reset_origin __attribute__ ((section (".noinit")));

// there is a very small possibillity this nechanism can fail: The app
// can trigger a wdog reset and happens the memory contents of this variable
// to be equal with RESET_BY_RFBOOT due to random manipulations in memory.
// if happens, the upload can always be done with the hardware reset.

// the next variable is also preserved between 2 continuous rfboot executions
// It is used to report correct reset causes EXTRF WDRF OWR BROWNOUT to the app
// (via the r2 register) as optiboot does
// Technically a watchdog reset is triggered from rfboot itself
// before the app starts. But we dont of coursze want to report this to the
// application code.

volatile uint8_t previous_reset_cause __attribute__ ((section (".noinit")));

// if we dont want to be a register then we MUST put a line
//  __asm__ __volatile__ ("mov r2, %0\n" :: "r" (mcusr_mirror));
// inside function reset_mcu();
// right before jmp
register uint8_t mcusr_mirror asm("r2") __attribute__ ((section (".noinit")));

// recommended code from avr-libc documentation
// MCUSR manipulation is VERY tricky so better leave it as is

void get_mcusr(void) __attribute__((naked)) __attribute__((section(".init3")));
void get_mcusr(void) {
    mcusr_mirror = MCUSR;
    MCUSR = 0;
    // rfboot does always enables a 2sec Watchdog timer. So even if we manage to
    // send buggy code to the target, watchdog timer will finally reset the
    // device allowing, to upload new code, WITHOUT the need of physicall contact
    // The appliction at normal operation will
    // have the duty to reset the WDOG timer periodiclly, witch is a good
    // practice anyway.
    wdt_enable(WDTO_2S);
}

//////////////////////////////////////////////////////////////////////
// This code generates the random data (entropy) needed for the IV
volatile byte sample = 0;
volatile bool sample_waiting = false;
byte current_bit = 0;
byte result = 0;

// Watchdog Timer Interrupt Service Routine
ISR(WDT_vect)
{
  sample = TCNT1L; // Ignore higher bits
  sample_waiting = true;
}

// Setup of the watchdog timer.
void wdtSetup() {
  cli();
  MCUSR = 0;
  // Start timed sequence
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  // Put WDT into interrupt mode
  // Set shortest prescaler(time-out) value = 2048 cycles (~16 ms)
  WDTCSR = _BV(WDIE);
  sei();
}

// Rotate bits to the left
// https://en.wikipedia.org/wiki/Circular_shift#Implementing_circular_shifts
byte rotl(const byte value, int shift) {
  if ((shift &= sizeof(value)*8 - 1) == 0)
    return value;
  return (value << shift) | (value >> (sizeof(value)*8 - shift));
}

bool gatherEntropy() {
      if (sample_waiting) {
        sample_waiting = false;
        result = rotl(result, 1); // Spread randomness around
        result ^= sample; // XOR preserves randomness
        current_bit++;
        if (current_bit > 7)
        {
          current_bit = 0;
          //rf.println(result);
          return true;
        }
      }
      return false;
}
///////////////////////////////////////////////////////////////////////

#ifdef CHIP_NRF24L01


// in this packet we store data coming from RF
uint8_t packet[PAYLOAD] __attribute__ ((section (".noinit")));

void radio_init(void) {
    mirf_init();
    _delay_ms(5); // xreiazetai ?

    mirf_set_raddr(RFBOOT_ADDRESS);
    mirf_set_taddr(RFBOOT_ADDRESS);
    mirf_config_register(SETUP_RETR, 0); // no retries
    mirf_config_register(RF_SETUP, (1 << RF_DR_LOW) | 1<<2 | 1<<1 ); //250kbps 0db


    mirf_config_register(EN_AA, 0); //disable ack

    //mirf_config_register(DYNPD, 0);
    //mirf_config_register(FEATURE, 0);

    mirf_config_register(DYNPD, 3); // incoming fifo 0 and 1 use dynamic payload
    mirf_config_register(FEATURE, _BV(EN_DPL)); // dynamic transmit payload

    mirf_config_register(SETUP_AW, 0x03);       // 5-byte addresses

    // 2 byte CRC
    // A lot of corrupted packets can pass the 1-byte CRC filter
    mirf_base_config = _BV(EN_CRC) | _BV(CRCO);

    mirf_config(); // sto tellos auto
}

void send_pkt( uint8_t msg, uint16_t data ) {

    LOW(CE_PIN);
    mirf_power_up_tx();       // Set to transmitter mode , Power up
    mirf_flush_tx();
    uint8_t pkt[3];
    pkt[0] = msg;
    pkt[1] = data & 0xff;
    pkt[2] = data >> 8 ;

    mirf_nrf_spi_write(W_TX_PAYLOAD, pkt false, 3);   // Send pkt_idx

    HIGH(CE_PIN);
    _delay_us(30); //nrf24 manual says at least 10us. The compilation is for
    // 8MHz. Lets say we run it in  a mcu at 20MHz the delay is 26*8/20 us (inside specs)
    LOW(CE_PIN);



    // TODO timeout me reset
    // isws na afiso to wdt timer ?
    // an kollisei sto loop o wdt tha to kanei reset
    while (1) {
        // if sending successful (TX_DS) or max retries exceded (MAX_RT).
        if( (mirf_get_status()  & ((1 << TX_DS)  | (1 << MAX_RT))) ){
            mirf_power_up_rx();
            break;
        }
    }
}

#define data_ready mirf_data_ready()
#define get_data() mirf_get_data(packet)

#endif

#ifdef CHIP_CC1101

#include "cc1101.h"

// a flag that a wireless packet has been received
//volatile  
register bool data_ready asm("r3") __attribute__ ((section (".noinit")));
//register uint8_t mcusr_mirror asm("r2") __attribute__ ((section (".noinit")));
// in this packet we store data coming from RF
CCPACKET ccpacket __attribute__ ((section (".noinit")));
uint8_t* packet = ccpacket.data;

// Generated by CC1101 when receives sync word (or sends a packet)
ISR (INT0_vect)
{
    /* interrupt code here */
    data_ready = true;
}

#define get_data() cc1101_receiveData(&ccpacket)

void radio_init(void) {
    cc1101_init();
    cc1101_setChannel(RFBOOT_CHANNEL);
    cc1101_setSyncWord(RFB_SYNCWORD[0],RFB_SYNCWORD[1]);
    disableAddressCheck();
    get_data();

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        EICRA |= (1 << ISC01);    // set INT0 to trigger on falling edge
        EIMSK |= (1 << INT0);     // Turns on INT0
    }

    __asm__("nop\n\t");
    __asm__("nop\n\t");
    data_ready = false;

}

CCPACKET outpkt;
void send_pkt(uint8_t msg, uint16_t data) {
    
    outpkt.length=3;
    outpkt.data[0]= msg ;
    outpkt.data[1]= data & 0xff ;
    outpkt.data[2]= data >> 8 ;
    cc1101_sendData(outpkt);
    while (! data_ready);
    data_ready = false;
}

void  send_iv(uint8_t* iv) {
    //CCPACKET outpkt;
    outpkt.length=8;
    memcpy(outpkt.data,iv,8);
    cc1101_sendData(outpkt);
    while (! data_ready);
    data_ready = false;
}



#endif

// Never returns, so naked and noreturn attributes dont hurt and reduce
// code size
void reset_mcu() __attribute__ ((naked))  __attribute__ ((__noreturn__));
void reset_mcu() {

    // rfboot will boot in a while
    // without remembering anything
    // But reset_origin variable will
    // contain the magic value RESET_BY_RFBOOT
    // so rfboot will know it is time to start the app
    reset_origin = RESET_BY_RFBOOT;

    // We save mcusr_mirror. after 15ms rfboot will put this value
    // to the r2 register before it gives control to the app
    previous_reset_cause=mcusr_mirror;

    // we enable watchdog at 15ms
    wdt_enable(WDTO_15MS);
    // we stay here until watchdog resets MCU
    while(1);
}

// we did it a function as we call the same thing a lot of times
void page_erase(uint16_t page) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        boot_spm_busy_wait();
        boot_page_erase(page);
    }
}

// the same
void flash_read_enable() {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        boot_spm_busy_wait();
        boot_rww_enable();
    }
}

int main(void) {

    // Disable interrupts.
    cli();

    // if the first 2 bytes of flash are 0xff, rfboot consider the flash
    // unprogrammed and stays waiting for code indefinitely
    // I searched the internet and as I understand it, there is no
    // 0xffff avr documented opcode at least for atmega328. So there is no
    // possibility an app will start with 0xffff by chance.

    if ( (pgm_read_word(0) != 0xffff) && (mcusr_mirror & _BV(WDRF) ) && (reset_origin == RESET_BY_RFBOOT)
    // check for hw reset or brownout
    // || (! mcusr_mirror & _BV(WDRF) ) prepei na sindiastei me PORF kalitera disabled ola auta
    )
    {
        // - We have an installed app
        // - We have watchdog reset
        // - reset_origin variable says that the reset triggered by the bootloader itself
        //   As a consequense : It is time to start the application code

        // it is absolutly necc to alter the reset_origin variable to something different
        // than RESET_BY_RFBOOT because there is a very high
        // probability that the app will not overwrite the memory
        // region reffering to this variable. If the application
        // triggers a WDOG reset, rfboot will find that
        // reset_origin == RESET_BY_RFBOOT and believing it itself
        // is the cause of the reset will jump directly to the app,
        // causing firmware updates to fail (when triggered from the app)
        reset_origin = 0;

        // mcusr_mirror is the register r2 which can be used by the
        // application to examine the cause of the reset. We set r2 to
        // be the reset cause of the previous reset, otherwise the app
        // will see as reset cause always WDOG
        mcusr_mirror = previous_reset_cause;


        // finally we start the app
        // note that we come from a WDOG reset
        // so the MCU registers I/O etc are in pristine state
        // but the registers of RF chip are NOT the default but
        // the settings set by rfboot
        // The app has responsibility of initialize the
        // rf module
        // the app must reset the watchdog every 4 secs at least
        // unless of course change the timeout or disables watchdog
        asm("jmp 0");
    }

    // The bootloader uses interrupts for 2 reasons:
    // to gather intropy (with WDIE)
    // to use CC1101
    //ATOMIC_BLOCK(ATOMIC_FORCEON) {
    /* Enable change of interrupt vectors */
    MCUCR = (1<<IVCE);
    /* Move interrupts to boot flash section */
    MCUCR = (1<<IVSEL);
    //}

    //#ifdef CHIP_NRF24L01

    //#endif

    // here we set rf channel, registers etc
    radio_init();
    {
        uint8_t i=250;
        while(1) {
            //#ifdef CHIP_CC1101
            if (data_ready) {
                if ( get_data() == 4 && ccpacket.crc_ok) {
                    data_ready = false;
                    break;
                }
                else i=250;
            }
            //#endif
            i--;
            if (!i) reset_mcu();
            _delay_us(1000);
            
        }
        uint32_t* p = packet;
        if (*p != START_SIGNATURE) reset_mcu();
    }
    
    //wdt_reset();
    uint16_t round = eeprom_read_word(E2END-1)+1;
    memset(packet,0,8);
    packet[0]=round & 0xff ;
    packet[1]=round >> 8;
    //xtea_decipher( (uint32_t*)packet , XTEAKEY );
    ccpacket.length = 8;
    
    cc1101_sendData(ccpacket);
    uint32_t iv[2];
    memcpy(iv,packet,8);
    while (! data_ready);
    data_ready = false;
    

    // the struct is used to extract upload parameters from the first packet
    struct start_packet *spacket = packet;

    {
        uint8_t i=250;

        while (true) {
            #ifdef CHIP_NRF24L01
            if (data_ready) {
                get_data();
                break;
            }
            #endif

            #ifdef CHIP_CC1101
            if (data_ready) {
                
                //cli();
                if ( get_data() == PAYLOAD && ccpacket.crc_ok) {
                    data_ready = false;
                    //sei();
                    break;
                 }
                 else {
                     i=250;
                     wdt_reset();
                 }
                //sei();
                

            }
            #endif

            i--;
            if (!i) reset_mcu();
            _delay_us(1000);
        }
    }

    // Ok we got the header
    wdt_reset();

    // We check if the first packet contains the correct signature
    // The signature is 32 bit, so is almost impossible for an unrelated
    // packet send by mistake (from an earlier upload for example),
    // to confuse the bootloader

    for (uint8_t i=0; i<=3; i++) {
        xtea_decipher_cbc( (uint32_t*)(packet+i*XTEA_BLOCK_SIZE) , XTEAKEY,iv );
    }

    if ( (spacket->start_signature1 == START_SIGNATURE) && (spacket->start_signature2 == START_SIGNATURE) ) {

        //if (spacket->round != round)  {
        //    send_pkt(RFB_WRONG_ROUND,round);
        //    reset_mcu();
        //}
    }
    else {
        // We were unable to find the signature so we give up.
        // Probbably rfupload used encryption with the wrong key or iv
        send_pkt(RFB_NO_SIGNATURE,0xffff);
        reset_mcu();
    }

    // We store here the size of the incoming app
    // this info is in the first packet
    // now using spacket pointer we extract info from the packet
    uint16_t app_size = spacket->app_size;

    // Here some basic checks for a valid app size. As you can see
    // app_size is always a multiple
    // of PAYLOAD. Trailing bytes are padded with 0xff
    // (rfprogrammer does this)
    if ( (app_size > (FLASHEND-BOOTLOADER_SECTION_SIZE+1) ) ||
    (app_size%PAYLOAD!=0) || (app_size==0) )
    {
        send_pkt(RFB_INVALID_CODE_SIZE,0xffff);
        reset_mcu();
    }

    // rfboot does not send back to the PC the flashed code for
    // verification
    // instead programmer sends 2 crcs (16bit each) so probabbly
    // effective crc length is 32 bits (see my research on this below)
    // wich rfboot uses after the flash to check if the code is OK.

    // this is the crc16 of the app
    uint16_t remote_crc = spacket->app_crc;

    // this is the crc16 calculated with the bytes in reverse order
    // again this is probbably an overkill but it is too
    // simple to implement and offers many orders of magnitude
    // better error detection
    uint16_t remote_crc2 = spacket->app_crc2;

    // This variable will point to the flash but the write process will
    // be in reverse
    uint16_t app_idx=app_size;

    // stelnei entoli gia to prwto paketo
    send_pkt(RFB_SEND_PKT, app_idx);

    // Before any write, we erase the first SPM page. If for some reason
    // the upload process fails, the first page will contain
    // 0xff and  rfboot will refuse to start the corrupted code.
    // see the start of main() how this is implemented
    eeprom_update_word(E2END-1, round);
    eeprom_busy_wait();
    page_erase(0);

    //one loop per SPM_PAGE in reverse order
    do
    {
        wdt_reset();
        // if i.e. app_idx == 256 we are going to burn the flash from 128-255
        // if app_idx == 128 we are going to burn the flash from 0-127 etc
        // We erase this SPM page unless it is the 0-127 page
        // which is erased before this loop starts
        if (app_idx>SPM_PAGESIZE) { // this means _not_ the first page
            uint16_t spm_page=(app_idx-1)/SPM_PAGESIZE*SPM_PAGESIZE;
            page_erase(spm_page);
        }
        // one loop per network packet 32 bytes == 4 xtea blocks == 2 AES blocks
        do
        {
            { // we send a request and expect a data packet
                uint16_t i=40*10-1;

                while (true) {

                    if ( (i%40)==0) send_pkt(RFB_SEND_PKT, app_idx);

                    // sto nrf24 chip data_ready einai function
                    // enw sto cc1101 variable

                    #ifdef CHIP_NRF24L01

                     if (data_ready) {
                         get_data();
                         break;
                     }
                    #endif

                    #ifdef CHIP_CC1101
                    if (data_ready) {
                        //cli();
                        if ( (get_data()==PAYLOAD) && ccpacket.crc_ok) {
                            //sei();
                            break;
                        }
                        else i=40*10+1;
                        wdt_reset();
                        //sei();
                    }
                    #endif
                    i--;
                    if (i==0) reset_mcu();
                    _delay_us(500);
                }
            }

            // edw stelnoume packeto poio page theloume
            // nwris nwris
            
            if (app_idx-PAYLOAD>0) send_pkt(RFB_SEND_PKT, app_idx-PAYLOAD);


            for (uint8_t i=0; i<=3; i++) {
                xtea_decipher_cbc( (uint32_t*)(packet+i*XTEA_BLOCK_SIZE) , XTEAKEY ,iv );
            }
            
            // at this point the packet is in cleartext

            // next thing is to wtite it in flash
            // the following code is basically what avr-gcc docunentation
            // suggests
            ATOMIC_BLOCK(ATOMIC_FORCEON) {
                boot_spm_busy_wait();
                uint8_t j=PAYLOAD;
                do {
                    app_idx-=2;
                    j-=2;
                    boot_page_fill(app_idx, *(uint16_t*)(packet+j));
                } while(j);
            }

        // we repeate the above 4 times until an SPM(128bytes) page fills
        } while (app_idx%SPM_PAGESIZE);
        //
        // Now we filled a full SPM page we have to burn it in flash
        //
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            boot_spm_busy_wait();
            boot_page_write(app_idx);
        }
    // We repeat writing SPM pages
    // if app_idx becomes 0 we have just written the SPM page 0-127
    // and the whole app is written to the flash
    } while (app_idx);

    // The fact that we are here, means the upload process is finished

    // Now we are going to check if the crc's of the written code are the same with
    // the CRC sent from the remote programmer. So we need to enable
    // flash read
    //ATOMIC_BLOCK(ATOMIC_FORCEON) {
    flash_read_enable();
    //}

    // as usual the crc's are initialized with zero
    uint16_t local_crc=0;
    uint16_t local_crc2=0;


    for (uint16_t i = 0; i < app_size ; i++) {

        // the first crc calculated reading the flash from start to end.
        local_crc = _crc16_update(local_crc,pgm_read_byte(i));

        // this crc is calculated reading the flash from end to start
        // The 2 crc's according to my (non sciendific) tests seem independend
        // offering an effective 32bit CRC
        // so offer a ~100% probbability that flash is really correctly
        // written. I am including a C program to test my hupothesis. If anyone has a
        // formal mathematical proof, I am very interested to know so
        // send me a note to  include a link here.
        // Note also that the use of 2 crc16 is
        // probably an overkill but it costs only 40-50 bytes in flash
        // and minimal MCU time, and offers vastly improved confidence,
        // that the code is correctly written.
        local_crc2 = _crc16_update(local_crc2,pgm_read_byte(app_size-1-i));
    }

    // both crc's are calculated, we do the test

    if ( (remote_crc != local_crc) || (remote_crc2 != local_crc2) ) {
        // if the crc's dont match, we erase the first SPM page again, so rfboot wont try
        // to start a corrupted code. Note that this should be rare, since
        // the network packets are already protected with CRC. rf chip
        // does this in hardware.

        page_erase(0);
        send_pkt(RFB_WRONG_CRC,0);

        //I am not sure if this is needed but it doesn't hurt either
        flash_read_enable();
    }
    else {
        send_pkt(RFB_SUCCESS,0);
    }

    // Reset MCU. If flash is correctly wtitten we go to app, not
    // directly but using a Watchdog reset. This has the great advandage
    // that the loaded app will see I/O pins etc are in their default state
    //
    // Of course as we saw earlier,if the process failed, which at
    // this point means the crc check failed, rfboot
    // will wait indefinitelly for new code
    reset_mcu();
}

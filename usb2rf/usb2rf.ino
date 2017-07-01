// WARNING: the USB-to-RF  module does NOT have rfboot as bootloader
// But the bootloader wich is preinstalled with the module
// Normally this is a ProMini 3.3V with ATmegaBOOT
// Again: Do not replace the bootloader of the USB-to-RF module

// WARNING:
// Some USB to serial adapters do not
// have a unique serial ID. If we only have one such module then is OK
// but if we have more than one connected to the PC at the same time
// it will be hard to choose the correct one.
// FTDI chips have unique serial IDs but at least the red 1-2$ modules (almost certainly fake chips)
// have some reliability problems.
// I prefet to use CP2102 which does not ship with unique ID, but you can program
// one easily
// http://cp210x-program.sourceforge.net/
// I have a CP2104 module and seems to have a unique ID from the factory.


#define PAYLOAD 32

#include <mCC1101.h>
mCC1101 rf;

// a flag that a wireless packet has been received
// Handle interrupt from CC1101 GDO0 <--> D2(INT0)
void cc1101signalsInterrupt(void) {
    rf.interrupt = true;
}

// Seems the AltSoftSerial does better than SoftSerial @ 8MHz
// Anything more than 19200 baud @ 8MHz seems unreliable
// Uses fixed pins
// To enable debug output you need to attach a second USB serial dongle "debug serial module"
// open with gtkterm 19200 baud. F7 enables/disables debug output
// D8(RX) <---> Debug SerialModule TX (no need to connect)
// D9(TX) <---> SerialModule2 RX
// D10 pwm is unusable (no problem, we dont need it, and it is used by SPI anyway)
// D4 <----> DTR (to enable disable debug with F7)
// GND <----> GND no need if both usb2serial are connected to the same PC. All USB ports share
// the same ground
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
// https://github.com/PaulStoffregen/AltSoftSerial
#include <AltSoftSerial.h>
AltSoftSerial debug_port;

// The digitalRead, digitalWrite function is slow
// and we use it extensivelly for the debug function
// so we prefer digitalReadFast.
// https://github.com/NicksonYap/digitalWriteFast
#include <digitalWriteFast.h>

#define RESET_TRIGGER_PIN 3

#define DEBUG_PIN 4

#define debug not digitalReadFast(DEBUG_PIN)

void(* resetFunc) (void) = 0;
uint32_t silence_timer ;

void drain_serial() {
    while ( Serial.read()!=-1 ) {};
}

void upload(uint16_t app_idx) {
    // Upload mode
    // Offloads some of the work rftool does
    // to improve latency
    // These are the status codes rfboot is sending back to the programmer
    const uint8_t RFB_SEND_PKT = 4;
    const byte USB_SEND_PACKET = 20;
    const byte USB_INFO_RESEND = 21;
    const byte USB_INFO_END = 22;
    //const byte USB_INFO_ABORT = 21;
    uint32_t timer = millis();

    byte outpacket[64];
    bool rfboot_waiting = true;
    bool outpacket_ready = false;
    Serial.write(USB_SEND_PACKET); // want 1 packets
    while (1) { // and (millis()-timer<1000) TODO

        if (millis()-timer>100) {
            if (debug) debug_port.print(F("upload: Timeout"));
            Serial.write(USB_INFO_END);
            return;
        }

        if ( (not outpacket_ready) and (Serial.available()>=PAYLOAD) ) {
            outpacket_ready = true;
            Serial.readBytes((char*)outpacket, PAYLOAD);
            if (app_idx>PAYLOAD) Serial.write(USB_SEND_PACKET); // TODO
            if (debug) {
                debug_port.print(F("Savail="));
                debug_port.println(Serial.available());
            }
        }

        if (rfboot_waiting and outpacket_ready) {
            rf.sendPacket(outpacket,PAYLOAD);
            // outpacket is not market as ready yet
            // it will when rfboot asks for next packet
            rfboot_waiting=false;
            if (debug) {
                debug_port.print(F("pkt out : idx="));
                debug_port.println(app_idx);
                //if (sending_header) debug_port.println(F("This was the header"));
            }
        }

        if (rf.interrupt) {
            byte inpacket[64];
            byte pkt_size = rf.getPacket(inpacket);
            rf.interrupt = false;
            if (pkt_size==3 and rf.crc_ok) {
                timer = millis(); // reset the timer
                // we just got a 3 byte packet from rfboot
                byte cmd = inpacket[0];

                if (cmd==RFB_SEND_PKT) {
                    uint16_t i=inpacket[1]+inpacket[2]*256;
                    if (i==app_idx) {
                        // rfboot needs the same packet
                        rf.sendPacket(outpacket,PAYLOAD);
                        rfboot_waiting = false;
                        Serial.write(USB_INFO_RESEND); // inform the resent
                        if (debug) {
                            debug_port.println(F("Resend"));
                        }
                    }
                    else if (i==app_idx-PAYLOAD) { // next packet

                        if (debug) debug_port.println(F("ok next pkt"));
                        rfboot_waiting = true;
                        app_idx = i;
                        outpacket_ready = false;
                    }
                    else {
                        if (debug) {
                            debug_port.print(__LINE__);
                            debug_port.print(F(": Protocol error. app_idx="));
                            debug_port.print(i);
                            debug_port.print(F(", but expected "));
                            debug_port.print(app_idx-PAYLOAD);
                            debug_port.print(F(" or "));
                            debug_port.print(app_idx);
                        }
                        drain_serial();
                        Serial.write(USB_INFO_END);
                        Serial.write(inpacket,3);
                        return; // ABORT
                    }
                }
                else {

                    drain_serial();
                    // Uncknown cmd
                    Serial.write(USB_INFO_END);
                    Serial.write(inpacket,3);

                    return; // ABORT
                }
                //}
            }
            else {
                if (debug) debug_port.print("Got unknown pkt"); // TODO pkt_size crc_ok

            }
        }
    }
    if (debug) {
        debug_port.print("app_idx="); debug_port.println(app_idx);
        debug_port.print(F("Serial.available()="));
        debug_port.println(Serial.available());
    }
}

void execCmd(uint8_t* cmd , uint8_t cmd_len ) {

    switch (cmd[0]) {

        case '0':
            if (cmd_len==1) {
                if (debug) {
                    debug_port.print(F("CC1101 register = "));
                    debug_port.println(rf.readConfigReg(CC1101_MDMCFG2),HEX);
                }
            }
            else {
                if (debug) {
                    debug_port.print(F("Custom command, bad length : "));
                    debug_port.println(cmd_len);
                }
            }
        break;

        case 'A':
            if (cmd_len==3) {
                rf.setSyncWord(cmd[1],cmd[2]);
                if (debug) {
                    debug_port.print(F("Syncword = "));
                    debug_port.print(cmd[1]) ;
                    debug_port.print(",");
                    debug_port.println(cmd[2]);
                }
            }
            else {
                if (debug) {
                    debug_port.print(F("cc1101 wrong cmd size "));
                    debug_port.println(cmd_len);
                }
            }
        break;

        case 'C':  // We set channel
            {
                if (cmd_len!=2) {
                    if (debug) {
                        debug_port.print(F("Channel command wrong size : "));
                        debug_port.println(cmd_len);
                    }
                }
                else {
                    uint8_t channel = cmd[1];

                    {
                        rf.setChannel(channel);
                        if (debug) {
                            debug_port.print(F("channel="));
                            debug_port.println(channel);
                        }
                    }
                }
            }
        break;

        case 'Q':
            if (cmd_len==1) {
                if (debug) {
                    debug_port.println(F("Silent for 50ms"));
                    silence_timer = millis();
                }
                resetFunc();
            }
            else {
                if (debug) {
                    debug_port.print(F("Silent command, bad length : "));
                    debug_port.println(cmd_len);
                }
            }
        break;

        case 'R': // MCU hardware reset
            {
                if (cmd_len!=1) {
                    if (debug) {
                        debug_port.print(F("MCU reset command wrong size : "));
                        debug_port.println(cmd_len);
                    }
                }
                else {
                    if (debug) {
                        debug_port.println(F("USB to RF Reset"));
                    }
                    rf.setSyncWord(0,0);

                    digitalWriteFast(RESET_TRIGGER_PIN,LOW);
                    pinModeFast(RESET_TRIGGER_PIN, OUTPUT); // reset the module because D4 is connected with RESET pin. See circuit diagram
                    // the following command will be executed only if the module fails to reset
                    debug_port.println(F("Usb2rf module failed to reset"));
                }
            }
        break;


        case 'U':
            if (cmd_len==3) {
                if (debug) {
                    debug_port.println(F("Switch to upload mode"));
                    //debug_port.flush();
                }
                // Perimeno size
                uint16_t app_idx=cmd[1]+cmd[2]*256;
                //debug_port.println( Serial.available() );
                upload(app_idx);
            }
            else {
                if (debug) {
                    debug_port.print(F("Upload code command, bad length : "));
                    debug_port.println(cmd_len);
                }
            }

            break;

        case 'W':
            // send wake up 1 sec pulse
            if (cmd_len==1) {
                if (debug) {
                    debug_port.println(F("Sending WakeUp burst 1050ms"));
                }
                const byte w='*';
                rf.sendBurstPacket(&w,1,1050);
                if (debug) {
                    debug_port.println(F("Done"));
                }
            }
        break;

        case 'Z':
            if (cmd_len==1) {
                if (debug) {
                    debug_port.println(F("Software reset"));
                    debug_port.flush();
                }
                rf.setSyncWord(0,0);
                resetFunc();
            }
            else {
                if (debug) {
                    debug_port.print(F("Software reset command, bad length : "));
                    debug_port.println(cmd_len);
                }
            }
        break;

        default:
            if (debug) {
                debug_port.print(F("Unknown command "));
                debug_port.write(cmd,cmd_len);
                debug_port.println();
            }
            break;
        }
}


int main() {

    init(); // mandatory, for arduino functions to work

    pinMode(DEBUG_PIN,INPUT_PULLUP);
    //Serial.begin(57600);
    Serial.begin(38400);

    debug_port.begin(19200);
    //delay(1);

    rf.init();
    //rf.setCarrierFreq(CFREQ_433);
    rf.disableAddressCheck();
    rf.setSyncWord(57,232);
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);

    rf.writeReg(CC1101_MDMCFG2, 0x97);

    //if (debug)
    //delay(8);
    debug_port.println(F("Usb2rf debug port at 19200 bps. Assert DTR "));
    debug_port.println(F("(F7 key with gtkterm) to enable/disable debug output"));

    uint8_t idx = 0;
    uint32_t timer = micros();
    bool cmdmode = false;
    delay(5);
    Serial.println(F("USB2RF"));

    //bool last_debug = not debug;
    bool last_debug = false;
    uint8_t packet[64];

    while (1) {
        if (debug != last_debug) {
            last_debug = debug;
            debug_port.print(F("debug messages "));
            if (debug) debug_port.println(F("enabled"));
            else debug_port.println(F("disabled"));
        }
        if (cmdmode) {
            if (Serial.available()) {

                uint8_t msg = Serial.read();
                packet[idx]=msg;
                idx++;
                if (idx==32) {
                    idx=0;
                    cmdmode=false;
                }
                timer = micros();
            }
            else if (micros()-timer>2000) {
                if (idx>0) {
                    execCmd(packet,idx);
                }
                cmdmode=false;
                idx=0;
                timer=micros();
            }
        }
        else {
            if (Serial.available()) {

                uint8_t msg = Serial.read();

                packet[idx]=msg;
                idx++;
                if (idx==32) {


                    if (debug) debug_port.write("out 32");

                    bool succ = rf.sendPacket(packet,32);

                    if ( debug ) {
                        if (succ)  debug_port.write("\r\n");
                        else debug_port.write(" F\r\n");
                    }
                    //while (! interrupt);
                    //interrupt = false;


                    if ( debug and (!succ) ) debug_port.println(F("Sending packet failed"));

                    idx=0;
                }
                else if (idx==5 and memcmp(packet,"COMMD",5)==0 ) {
                    cmdmode = true;
                    idx=0;
                }
                timer = micros();
            }

            else if (idx==0) {
                timer=micros();
            }

            else if (micros() - timer > 2000) {
                 {

                    if (debug) {
                        debug_port.write("out ");
                        debug_port.print(idx);
                    }

                    bool succ;

                    succ = rf.sendPacket(packet,idx);

                    if ( debug ) {
                        if (succ)  debug_port.write("\r\n");
                        else debug_port.write(" F\r\n");
                    }
                }
                timer = micros();
                idx=0;
            }
        }

        if (rf.interrupt) {
            byte pkt_size = rf.getPacket(packet);
            rf.interrupt = false;

            if (rf.crc_ok) {
                if ( pkt_size > 0) {
                    //else {
                    // Afti i grammi diorthonei to 5088 bug pou akoma den kserw pou ofeiletai
                    // pantos o bootloader stelnei olososto paketo kai o kwdikas to vlepei kanonika
                    // wstoso to 4,224,19 stanei sto PC mono san 4,224
                    // if (ccpacket.length==3 and ccpacket.data[0]==4 and ccpacket.data[2]<=19) ccpacket.data[2]+=128;

                    /* if (pkt_size==8) {
                        for (byte i=0;i<8;i++) {
                            if (packet[i]<16) Serial.write('0');
                            Serial.print(packet[i], HEX);
                            Serial.write(' ');
                        }
                        Serial.println();
                    }
                    else {
                        Serial.write(packet, pkt_size);
                    } */
                    Serial.write(packet, pkt_size);

                    if (debug) {
                        debug_port.write("in ");
                        //if (pkt_size != 3)
                        debug_port.println(pkt_size);
                    }
                    //}
                }
                else if (debug) {
                    debug_port.println("in 0 !");
                }
            }
            else {
                if (debug) {
                    debug_port.write("in CRC error. pkt_size=");
                    debug_port.println(pkt_size);
                }
            }
        }

    }
}


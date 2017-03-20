/*
Copyright (c) 2017 Panagiotis Karagiannis
Based on the panstamp library and modified to better suite the needs of
rfboot. The licence remains the same LGPLv3 or later
*/

/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */

#include "cc1101.h"
#include <avr/delay.h>
//#include "nvolat.h"

/**
 * Macros
 */
// Select (SPI) CC1101
//define cc1101_Select()  bitClear(PORT_SPI_SS, BIT_SPI_SS)
#define cc1101_Select()  LOW(SPI_SS)

// Deselect (SPI) CC1101
//#define cc1101_Deselect()  bitSet(PORT_SPI_SS, BIT_SPI_SS)
#define cc1101_Deselect()  HIGH(SPI_SS)

// Wait until SPI MISO line goes low
//#define wait_Miso()  while(bitRead(PORT_SPI_MISO, BIT_SPI_MISO))
#define wait_Miso()  while(READ(SPI_MISO))

// Get GDO0 pin state
//#define getGDO0state()  bitRead(PORT_GDO0, BIT_GDO0)
#define getGDO0state()  READ(GDO0)

// Wait until GDO0 line goes high
#define wait_GDO0_high()  while(!getGDO0state())

// Wait until GDO0 line goes low
#define wait_GDO0_low()  while(getGDO0state())

 /**
  * PATABLE
  */
//const byte paTable[8] = {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60};

/**
 * CC1101
 * 
 * Class constructor
 */
//void CC1101(void)
//{
//  cc1101_paTableByte = PA_LowPower;            // Priority = Low power
//}

/**
 * cc1101_wakeUp
 * 
 * Wake up CC1101 from Power Down state
 */
void cc1101_wakeUp(void)
{
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  cc1101_Deselect();                    // Deselect CC1101
}

/**
 * cc1101_writeReg
 * 
 * Write single register into the CC1101 IC via SPI
 * 
 * 'regAddr'    Register address
 * 'value'  Value to be writen
 */
void cc1101_writeReg(byte regAddr, byte value) 
{
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi_send(regAddr);                    // Send register address
  spi_send(value);                      // Send value
  cc1101_Deselect();                    // Deselect CC1101
}

/**
 * cc1101_writeBurstReg
 * 
 * Write multiple registers into the CC1101 IC via SPI
 * 
 * 'regAddr'    Register address
 * 'buffer' Data to be writen
 * 'len'    Data length
 */
void cc1101_writeBurstReg(byte regAddr, byte* buffer, byte len)
{
  byte addr, i;
  
  addr = regAddr | WRITE_BURST;         // Enable burst transfer
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait unt0il MISO goes low
  spi_send(addr);                       // Send register address
  
  for(i=0 ; i<len ; i++)
    spi_send(buffer[i]);                // Send value

  cc1101_Deselect();                    // Deselect CC1101  
}

/**
 * cc1101_cmdStrobe
 * 
 * Send command strobe to the CC1101 IC via SPI
 * 
 * 'cmd'    Command strobe
 */     
void cc1101_cmdStrobe(byte cmd) 
{
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi_send(cmd);                        // Send strobe command
  cc1101_Deselect();                    // Deselect CC1101
}

/**
 * cc1101_readReg
 * 
 * Read CC1101 register via SPI
 * 
 * 'regAddr'    Register address
 * 'regType'    Type of register: CC1101_CONFIG_REGISTER or CC1101_STATUS_REGISTER
 * 
 * Return:
 *  Data byte returned by the CC1101 IC
 */
byte cc1101_readReg(byte regAddr, byte regType) 
{
  byte addr, val;

  addr = regAddr | regType;
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi_send(addr);                       // Send register address
  val = spi_send(0x00);                 // Read result
  cc1101_Deselect();                    // Deselect CC1101

  return val;
}

/**
 * cc1101_readBurstReg
 * 
 * Read burst data from CC1101 via SPI
 * 
 * 'buffer' Buffer where to copy the result to
 * 'regAddr'    Register address
 * 'len'    Data length
 */
void cc1101_readBurstReg(byte * buffer, byte regAddr, byte len) 
{
  byte addr, i;
  
  addr = regAddr | READ_BURST;
  cc1101_Select();                      // Select CC1101
  wait_Miso();                          // Wait until MISO goes low
  spi_send(addr);                       // Send register address
  for(i=0 ; i<len ; i++)
    buffer[i] = spi_send(0x00);         // Read result byte by byte
  cc1101_Deselect();                    // Deselect CC1101
}

/**
 * cc1101_reset
 * 
 * Reset CC1101
 */
void cc1101_reset(void) 
{
  cc1101_Deselect();                    // Deselect CC1101
  //delayMicroseconds(5);
  _delay_us(5);
  cc1101_Select();                      // Select CC1101
  //delayMicroseconds(10);
  _delay_us(10);
  cc1101_Deselect();                    // Deselect CC1101
  //delayMicroseconds(41);
  _delay_us(41);
  cc1101_Select();                      // Select CC1101

  wait_Miso();                          // Wait until MISO goes low
  spi_send(CC1101_SRES);                // Send cc1101_reset command strobe
  wait_Miso();                          // Wait until MISO goes low

  cc1101_Deselect();                    // Deselect CC1101

  cc1101_setDefaultRegs();                     // Reconfigure CC1101
  //setRegsFromEeprom();                  // Take user settings from EEPROM
}

/**
 * cc1101_setDefaultRegs
 * 
 * Configure CC1101 registers
 */
void cc1101_setDefaultRegs(void) 
{
  cc1101_writeReg(CC1101_IOCFG2,  CC1101_DEFVAL_IOCFG2);
  cc1101_writeReg(CC1101_IOCFG1,  CC1101_DEFVAL_IOCFG1);
  cc1101_writeReg(CC1101_IOCFG0,  CC1101_DEFVAL_IOCFG0);
  cc1101_writeReg(CC1101_FIFOTHR,  CC1101_DEFVAL_FIFOTHR);
  cc1101_writeReg(CC1101_PKTLEN,  CC1101_DEFVAL_PKTLEN);
  cc1101_writeReg(CC1101_PKTCTRL1,  CC1101_DEFVAL_PKTCTRL1);
  cc1101_writeReg(CC1101_PKTCTRL0,  CC1101_DEFVAL_PKTCTRL0);

  // Set default synchronization word
  cc1101_setSyncWord(CC1101_DEFVAL_SYNC1, CC1101_DEFVAL_SYNC0);

  // Set default device address
  cc1101_setDevAddress(CC1101_DEFVAL_ADDR);
  // Set default frequency channel
  cc1101_setChannel(CC1101_DEFVAL_CHANNR);
  
  cc1101_writeReg(CC1101_FSCTRL1,  CC1101_DEFVAL_FSCTRL1);
  cc1101_writeReg(CC1101_FSCTRL0,  CC1101_DEFVAL_FSCTRL0);

  // Set default carrier frequency = 868 MHz
  // cc1101_setCarrierFreq(CFREQ_868);
  cc1101_setCarrierFreq(CFREQ_433);

  cc1101_writeReg(CC1101_MDMCFG4,  CC1101_DEFVAL_MDMCFG4);
  cc1101_writeReg(CC1101_MDMCFG3,  CC1101_DEFVAL_MDMCFG3);
  cc1101_writeReg(CC1101_MDMCFG2,  CC1101_DEFVAL_MDMCFG2);
  cc1101_writeReg(CC1101_MDMCFG1,  CC1101_DEFVAL_MDMCFG1);
  cc1101_writeReg(CC1101_MDMCFG0,  CC1101_DEFVAL_MDMCFG0);
  cc1101_writeReg(CC1101_DEVIATN,  CC1101_DEFVAL_DEVIATN);
  cc1101_writeReg(CC1101_MCSM2,  CC1101_DEFVAL_MCSM2);
  cc1101_writeReg(CC1101_MCSM1,  CC1101_DEFVAL_MCSM1);
  cc1101_writeReg(CC1101_MCSM0,  CC1101_DEFVAL_MCSM0);
  cc1101_writeReg(CC1101_FOCCFG,  CC1101_DEFVAL_FOCCFG);
  cc1101_writeReg(CC1101_BSCFG,  CC1101_DEFVAL_BSCFG);
  cc1101_writeReg(CC1101_AGCCTRL2,  CC1101_DEFVAL_AGCCTRL2);
  cc1101_writeReg(CC1101_AGCCTRL1,  CC1101_DEFVAL_AGCCTRL1);
  cc1101_writeReg(CC1101_AGCCTRL0,  CC1101_DEFVAL_AGCCTRL0);
  cc1101_writeReg(CC1101_WOREVT1,  CC1101_DEFVAL_WOREVT1);
  cc1101_writeReg(CC1101_WOREVT0,  CC1101_DEFVAL_WOREVT0);
  cc1101_writeReg(CC1101_WORCTRL,  CC1101_DEFVAL_WORCTRL);
  cc1101_writeReg(CC1101_FREND1,  CC1101_DEFVAL_FREND1);
  cc1101_writeReg(CC1101_FREND0,  CC1101_DEFVAL_FREND0);
  cc1101_writeReg(CC1101_FSCAL3,  CC1101_DEFVAL_FSCAL3);
  cc1101_writeReg(CC1101_FSCAL2,  CC1101_DEFVAL_FSCAL2);
  cc1101_writeReg(CC1101_FSCAL1,  CC1101_DEFVAL_FSCAL1);
  cc1101_writeReg(CC1101_FSCAL0,  CC1101_DEFVAL_FSCAL0);
  cc1101_writeReg(CC1101_RCCTRL1,  CC1101_DEFVAL_RCCTRL1);
  cc1101_writeReg(CC1101_RCCTRL0,  CC1101_DEFVAL_RCCTRL0);
  cc1101_writeReg(CC1101_FSTEST,  CC1101_DEFVAL_FSTEST);
  cc1101_writeReg(CC1101_PTEST,  CC1101_DEFVAL_PTEST);
  cc1101_writeReg(CC1101_AGCTEST,  CC1101_DEFVAL_AGCTEST);
  cc1101_writeReg(CC1101_TEST2,  CC1101_DEFVAL_TEST2);
  cc1101_writeReg(CC1101_TEST1,  CC1101_DEFVAL_TEST1);
  cc1101_writeReg(CC1101_TEST0,  CC1101_DEFVAL_TEST0);
}

/**
 * cc1101_init
 * 
 * Initialize CC1101
 */
void cc1101_init(void) 
{
  spi_init();                           // Initialize SPI interface
  //spi.setClockDivider(SPI_CLOCK_DIV16);
  //spi.setBitOrder(MSBFIRST);
  //TODO
  //INPUT(GDO0);                 // Config GDO0 as input

  cc1101_reset();                              // Reset CC1101

  // Configure PATABLE
  //cc1101_writeBurstReg(CC1101_PATABLE, (byte*)paTable, 8);
  cc1101_writeReg(CC1101_PATABLE, PA_LowPower);
}

/**
 * cc1101_setSyncWord
 * 
 * Set synchronization word
 * 
 * 'syncH'  Synchronization word - High byte
 * 'syncL'  Synchronization word - Low byte
 * 'save' If TRUE, save parameter in EEPROM
 */
 // TODO synWordMatrix ektos
void cc1101_setSyncWord(uint8_t syncH, uint8_t syncL) 
{
  //if ((cc1101_syncWord[0] != syncH) || (cc1101_syncWord[1] != syncL))
  //{
    cc1101_writeReg(CC1101_SYNC1, syncH);
    cc1101_writeReg(CC1101_SYNC0, syncL);
    //cc1101_syncWord[0] = syncH;
    //cc1101_syncWord[1] = syncL;
    // Save in EEPROM
    //if (save)
    //{
      //EEPROM.write(EEPROM_SYNC_WORD, syncH);
      //EEPROM.write(EEPROM_SYNC_WORD + 1, syncL);
    //}
  //}
}

/**
 * cc1101_setSyncWord (overriding method)
 * 
 * Set synchronization word
 * 
 * 'syncH'  Synchronization word - pointer to 2-byte array
 * 'save' If TRUE, save parameter in EEPROM
 */
//void cc1101_setSyncWord(byte *sync) 
//{
//  cc1101_setSyncWord(sync[0], sync[1], save);
//}

/**
 * cc1101_setDevAddress
 * 
 * Set device address
 * 
 * 'addr'   Device address
 * 'save' If TRUE, save parameter in EEPROM
 */
void cc1101_setDevAddress(byte addr) 
{
  //if (cc1101_devAddress != addr)
  //{
    cc1101_writeReg(CC1101_ADDR, addr);
  //  cc1101_devAddress = addr;
    // Save in EEPROM
    //if (save)
    //  //EEPROM.write(EEPROM_DEVICE_ADDR, addr)
    //  ;  
  //}
}

/**
 * cc1101_setChannel
 * 
 * Set frequency channel
 * 
 * 'chnl'   Frequency channel
 * 'save' If TRUE, save parameter in EEPROM
 */

//TODO define
void cc1101_setChannel(byte chnl) 
{
  //if (cc1101_channel != chnl)
  //{
    cc1101_writeReg(CC1101_CHANNR,  chnl);
  //  cc1101_channel = chnl;
    // Save in EEPROM
    //if (save)
    //  //EEPROM.write(EEPROM_FREQ_CHANNEL, chnl)
    //  ;
  //}
}

/**
 * cc1101_setCarrierFreq
 * 
 * Set carrier frequency
 * 
 * 'freq'   New carrier frequency
 */
void cc1101_setCarrierFreq(byte freq)
{
  switch(freq)
  {
    case CFREQ_915:
      cc1101_writeReg(CC1101_FREQ2,  CC1101_DEFVAL_FREQ2_915);
      cc1101_writeReg(CC1101_FREQ1,  CC1101_DEFVAL_FREQ1_915);
      cc1101_writeReg(CC1101_FREQ0,  CC1101_DEFVAL_FREQ0_915);
      break;
    case CFREQ_433:
      cc1101_writeReg(CC1101_FREQ2,  CC1101_DEFVAL_FREQ2_433);
      cc1101_writeReg(CC1101_FREQ1,  CC1101_DEFVAL_FREQ1_433);
      cc1101_writeReg(CC1101_FREQ0,  CC1101_DEFVAL_FREQ0_433);
      break;
    default:
      cc1101_writeReg(CC1101_FREQ2,  CC1101_DEFVAL_FREQ2_868);
      cc1101_writeReg(CC1101_FREQ1,  CC1101_DEFVAL_FREQ1_868);
      cc1101_writeReg(CC1101_FREQ0,  CC1101_DEFVAL_FREQ0_868);
      break;
  }
   
  //cc1101_carrierFreq = freq;  
}

/**
 * setRegsFromEeprom
 * 
 * Set registers from EEPROM
 */
//void setRegsFromEeprom(void)
//{

/*  byte bVal;
  byte arrV[2];

  // Read RF channel from EEPROM
  bVal = EEPROM.read(EEPROM_FREQ_CHANNEL);
  // Set RF channel
  if (bVal < NUMBER_OF_FCHANNELS )
    cc1101_setChannel(bVal, false);
  // Read Sync word from EEPROM
  arrV[0] = EEPROM.read(EEPROM_SYNC_WORD);
  arrV[1] = EEPROM.read(EEPROM_SYNC_WORD + 1);
  // Set Sync word. 0x00 and 0xFF values are not allowed
  if (((arrV[0] != 0x00) && (arrV[0] != 0xFF)) || ((arrV[1] != 0x00) && (arrV[1] != 0xFF)))
    cc1101_setSyncWord(arrV[0], arrV[1], false);
  // Read device address from EEPROM
  bVal = EEPROM.read(EEPROM_DEVICE_ADDR);
  // Set device address
  if (bVal > 0)
    cc1101_setDevAddress(bVal, false); */
//}

/**
 * cc1101_setPowerDownState
 * 
 * Put CC1101 into power-down state
 */
void cc1101_setPowerDownState() 
{
  // Comming from RX state, we need to enter the IDLE state first
  cc1101_cmdStrobe(CC1101_SIDLE);
  // Enter Power-down state
  cc1101_cmdStrobe(CC1101_SPWD);
}

/**
 * cc1101_sendData
 * 
 * Send data packet via RF
 * 
 * 'packet' Packet to be transmitted. First byte is the destination address
 *
 *  Return:
 *    True if the transmission succeeds
 *    False otherwise
 */
bool cc1101_sendData(CCPACKET packet)
{
  byte marcState;
  bool res = false;
 
  // Declare to be in Tx state. This will avoid receiving packets whilst
  // transmitting
  //cc1101_rfState = RFSTATE_TX;

  // Enter RX state
  setRxState();

  // Check that the RX state has been entered
  while (((marcState = readStatusReg(CC1101_MARCSTATE)) & 0x1F) != 0x0D)
  {
    if (marcState == 0x11)        // RX_OVERFLOW
      flushRxFifo();              // flush receive queue
  }

  //delayMicroseconds(500);
  _delay_us(500);

  // Set data length at the first position of the TX FIFO
  cc1101_writeReg(CC1101_TXFIFO,  packet.length);
  // Write data into the TX FIFO
  cc1101_writeBurstReg(CC1101_TXFIFO, packet.data, packet.length);

  // CCA enabled: will enter TX state only if the channel is clear
  setTxState();

  // Check that TX state is being entered (state = RXTX_SETTLING)
  marcState = readStatusReg(CC1101_MARCSTATE) & 0x1F;
  if((marcState != 0x13) && (marcState != 0x14) && (marcState != 0x15))
  {
    setIdleState();       // Enter IDLE state
    flushTxFifo();        // Flush Tx FIFO
    setRxState();         // Back to RX state

    // Declare to be in Rx state
    //cc1101_rfState = RFSTATE_RX;
    return false;
  }

  // Wait for the sync word to be transmitted
  wait_GDO0_high();

  // Wait until the end of the packet transmission
  wait_GDO0_low();

  // Check that the TX FIFO is empty
  if((readStatusReg(CC1101_TXBYTES) & 0x7F) == 0)
    res = true;

  setIdleState();       // Enter IDLE state
  flushTxFifo();        // Flush Tx FIFO

  // Enter back into RX state
  setRxState();

  // Declare to be in Rx state
  //cc1101_rfState = RFSTATE_RX;

  return res;
}

/**
 * cc1101_receiveData
 * 
 * Read data packet from RX FIFO
 *
 * 'packet' Container for the packet received
 * 
 * Return:
 *  Amount of bytes received
 */
byte cc1101_receiveData(CCPACKET * packet)
{
  byte val;
  byte rxBytes = readStatusReg(CC1101_RXBYTES);

  // Any byte waiting to be read and no overflow?
  if (rxBytes & 0x7F && !(rxBytes & 0x80))
  {
    // Read data length
    packet->length = readConfigReg(CC1101_RXFIFO);
    // If packet is too long
    if (packet->length > CC1101_DATA_LEN)
      packet->length = 0;   // Discard packet
    else
    {
      // Read data packet
      cc1101_readBurstReg(packet->data, CC1101_RXFIFO, packet->length);
      // Read RSSI
      packet->rssi = readConfigReg(CC1101_RXFIFO);
      // Read LQI and CRC_OK
      val = readConfigReg(CC1101_RXFIFO);
      packet->lqi = val & 0x7F;
      //packet->crc_ok = bitRead(val, 7);
      packet->crc_ok = (val>>7);
    }
  }
  else
    packet->length = 0;

  setIdleState();       // Enter IDLE state
  flushRxFifo();        // Flush Rx FIFO
  //cc1101_cmdStrobe(CC1101_SCAL);

  // Back to RX state
  setRxState();

  return packet->length;
}


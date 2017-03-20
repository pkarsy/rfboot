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

#include "spi.h"
#include "pin_macros.h"
/**
 * init
 * 
 * SPI initialization
 */
void spi_init() 
{
  //digitalWrite(SPI_SS, HIGH);
  HIGH(SPI_SS);
  
  // Configure SPI pins
  //pinMode(SPI_SS, OUTPUT);
  //pinMode(SPI_MOSI, OUTPUT);
  //pinMode(SPI_MISO, INPUT);
  //pinMode(SPI_SCK, OUTPUT);
  OUTPUT(SPI_SS);
  OUTPUT(SPI_MOSI);
  INPUT(SPI_MISO);
  OUTPUT(SPI_SCK);

  //digitalWrite(SPI_SCK, HIGH);
  //digitalWrite(SPI_MOSI, LOW);
  HIGH(SPI_SCK);
  LOW(SPI_MOSI);

  // SPI speed = clk/4
  SPCR = _BV(SPE) | _BV(MSTR);
}

/**
 * send
 * 
 * Send byte via SPI
 * 
 * 'value'  Value to be sent
 * 
 * Return:
 *  Response received from SPI slave
 */
byte spi_send(byte value) 
{
  SPDR = value;                          // Transfer byte via SPI
  wait_Spi();                            // Wait until SPI operation is terminated

  return SPDR;
}


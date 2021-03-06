 /*
 **************************************************************************
 *                                                                        *
 *                 ____                ___ _   _____                      *
 *                / __ \___  ___ ___  / _ | | / / _ \____                 *
 *               / /_/ / _ \/ -_) _ \/ __ | |/ / , _/ __/                 *
 *               \____/ .__/\__/_//_/_/ |_|___/_/|_|\__/                  *
 *                   /_/                                                  *
 *                                                                        *
 *              This file is part of the OpenAVRc project.                *
 *                                                                        *
 *                         Based on code(s) named :                       *
 *             OpenTx - https://github.com/opentx/opentx                  *
 *             Deviation - https://www.deviationtx.com/                   *
 *                                                                        *
 *                Only AVR code here for visibility ;-)                   *
 *                                                                        *
 *   OpenAVRc is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by *
 *   the Free Software Foundation, either version 2 of the License, or    *
 *   (at your option) any later version.                                  *
 *                                                                        *
 *   OpenAVRc is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *   GNU General Public License for more details.                         *
 *                                                                        *
 *       License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html          *
 *                                                                        *
 **************************************************************************
*/


// From https://github.com/g4lvanix/I2C-master-lib

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include "OpenAVRc.h"

#define I2C_READ 0x01
#define I2C_WRITE 0x00

#if defined(CPUM2560)
#define I2C_SPEED_100K()  { TWBR = (uint8_t) 72;}
#define I2C_SPEED_400K()  { TWBR = (uint8_t) 12;}
#define I2C_SPEED_888K()  { TWBR = (uint8_t) 1; }
#define I2C_SPEED_1M()    { TWBR = (uint8_t) 0; }
#endif

#if defined(CPUXMEGA)
#define I2C_SPEED_400K(port)  port.MASTER.BAUD = (uint8_t) 35; // 400KHz
#define I2C_SPEED_500K(port)  port.MASTER.BAUD = (uint8_t) 27; // 500KHz
#define I2C_SPEED_800K(port)  port.MASTER.BAUD = (uint8_t) 15; // 800KHz
#define I2C_SPEED_1M(port)    port.MASTER.BAUD = (uint8_t) 11; // 1MHz
#endif

void i2c_init(void);
uint8_t i2c_start(uint8_t address);
uint8_t i2c_write(uint8_t data);
FORCEINLINE void i2c_writeAndActiveISR(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
void i2c_stop(void);

#endif // I2C_MASTER_H
/*
##### void I2C_init()
This function needs to be called only once to set up the correct SCL frequency
for the bus

##### uint8_t I2C_start(uint8_t address)
This function needs to be called any time a connection to a new slave device should
be established. The function returns 1 if an error has occurred, otherwise it returns
0.

The syntax to start a operation write to a device is either:
`I2C_start(SLAVE_ADDRESS+I2C_WRITE);`
or
`I2C_start(SLAVE_WRITE_ADDRESS);`

The syntax to start a read operation from a device is either:
`I2C_start(SLAVE_ADDRESS+I2C_READ);`
or
`I2C_start(SLAVE_READ_ADDRESS);`

##### uint8_t I2C_write(uint8_t data)
This function is used to write data to the currently active device.
The only parameter this function takes is the 8 bit unsigned integer to be sent.
The function returns 1 if an error has occurred, otherwise it returns
0.

##### uint8_t I2C_read_ack()
This function is used to read one byte from a device and request another byte of data
after the transmission is complete by sending the acknowledge bit.
This function returns the received byte.

##### uint8_t I2C_read_nack()
This function is used to read one byte from a device an then not requesting another
byte and therefore stopping the current transmission.
This function returns the received byte.

##### uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length);
This function is used to transmit [length] number of bytes to an I2C device with the given
I2C address from [data].
The [address] passed to this function is the 7-bit slave address, left
shifted by one bit (i.e. 7-bit slave address is `0x2F` -> `(0x2F)<<1` = `0x5E`)

##### uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length);
This function is used to read [length] number of bytes from the I2C device with the given
I2C address into the [data].
The [address] passed to this function is the 7-bit slave address, left
shifted by one bit (i.e. 7-bit slave address is `0x2F` -> `(0x2F)<<1` = `0x5E`)

##### void I2C_stop()
This function disables the TWI peripheral completely
and therefore disconnects the device from the bus.
*/

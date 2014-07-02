/*
 * fram_i2c.h
 *
 * 铁电 I2C驱动
 *
 *  Created on: 2014年6月13日
 *      Author: ThinkPad User
 */

#ifndef FRAM_I2C_H_
#define FRAM_I2C_H_

/*
Read/Write Code (1 bit)
The 8th bit of the device address word is the R/W (read/write) code. When the R/W code is “0”, a
operation is enabled, and the R/W code is “1”, a read operation is enabled for the MB85RC1MT.
*/

#if 0
#define FRAM_I2C_ADDR	0xA0


void fram_Init(void);
uint8_t fram_ReadByte(uint16_t ReadAddr, uint8_t* pu8Val);

#endif

#endif /* FRAM_I2C_H_ */

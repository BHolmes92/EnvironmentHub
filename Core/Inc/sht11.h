/*
 * sht11.h
 *
 *  Created on: Oct 20, 2023
 *      Author: Ben
 */

#ifndef INC_SHT11_H_
#define INC_SHT11_H_

#define MEASURE_TEMP 0x05
#define READ_STATUS  0x07

#include <stdint.h>

uint8_t SHT11_Start(void); //Establish sensor

uint8_t SHT11_Write(uint8_t cmd);//Write Command to Sensor

void SHT11_Init_Transmission(void);

void SHT11_Write_Address(void);

uint16_t SHT11_Read(void);

void SHT11_Reset(void);

#endif /* INC_SHT11_H_ */

/*
 * ds18b20.h
 *
 *  Created on: Oct 18, 2023
 *      Author: Ben
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include <stdint.h>

uint8_t DS18B20_Start(void); //Establish sensor

void DS18B20_Write(uint8_t data); //Write Command to Sensor

uint8_t DS18B20_Read(void); //Read Sensor

#endif /* INC_DS18B20_H_ */

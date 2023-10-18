/*
 * utils.h
 *
 *  Created on: Oct 18, 2023
 *      Author: Ben
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdint.h>
#include "gpio.h"
#include "tim.h"

void delay_us(uint16_t us);

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* INC_UTILS_H_ */

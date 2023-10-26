/*
 * ds18b20.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Ben
 */

#include "ds18b20.h"
#include "utils.h"
#include "gpio.h"
/*
 * @brief  : Establish Connection
 * @retval : Confirmation or connection not set (-1)
 */
uint8_t DS18B20_Start(void){
	uint8_t response = 0;
	Set_Pin_Output(DS18B20_GPIO_Port, DS18B20_Pin);
	HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, 0); //Drive Line to 0
	delay_us(480);

	Set_Pin_Input(DS18B20_GPIO_Port, DS18B20_Pin); //Check for sensor response
	delay_us(80);

	if(!HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)){
		response = 1;
	}
	else{
		response = -1;
	}

	delay_us(400); //Total time for input 80+400= 480

	return response;
}

/*
 * @brief  : Write command to DS18B20
 */
void DS18B20_Write(uint8_t data){
	Set_Pin_Output(DS18B20_GPIO_Port, DS18B20_Pin);
	for(int i = 0; i < 8; i++){
		//Write 1
		if((data & (1 << i)) !=0){
			Set_Pin_Output(DS18B20_GPIO_Port, DS18B20_Pin);
			HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, 0); //Pull Line LOW to initiate Write
			delay_us(1);

			Set_Pin_Input(DS18B20_GPIO_Port, DS18B20_Pin); //Release line
			delay_us(60); //Sample Time
		}else{
		//Write 0
			Set_Pin_Output(DS18B20_GPIO_Port, DS18B20_Pin);
			HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, 0); //Pull Line LOW to write 1
			delay_us(60);
			Set_Pin_Input(DS18B20_GPIO_Port, DS18B20_Pin); //Release Line
		}
	}
}

/*
 * @brief  : Read value from DS18B20
 * @retval : Value from sensor ROM
 */
uint8_t DS18B20_Read(void){
	uint8_t value = 0;
	for(int i = 0; i < 8 ; i++){
		Set_Pin_Output(DS18B20_GPIO_Port, DS18B20_Pin);
		HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin,0); //Pull Line LOW to start read slot
		delay_us(2);
		Set_Pin_Input(DS18B20_GPIO_Port, DS18B20_Pin); //Wait for response
		if(HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)){
			value |= 1<<i;
		}
		delay_us(60); //Min read slot time
	}
	return value;
}

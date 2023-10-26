/*
 * sht11.c
 *
 *  Created on: Oct 20, 2023
 *      Author: Ben
 */


#include "sht11.h"
#include "utils.h"
#include "gpio.h"


/*
 * @brief   :   Reset sensor and delay to ensure ready for opperation
 * @retval  :   1 for success -1 for error
 */
uint8_t SHT11_Start(void){
	//Wait for sensor to enter sleep state (11ms)
	HAL_Delay(11);

}

/*
 * @brief  :   Initiates a transmission sequence start
 *             SDA is LOW for 2 SCK pulses
 */
void SHT11_Init_Transmission(void){
	Set_Pin_Output(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
	Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin); //SDA HIGH
	delay_us(10);
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 1); //Clock HIGH
	delay_us(10);
	Set_Pin_Output(SHT_DATA_GPIO_Port, SHT_DATA_Pin); //SDA LOW
	delay_us(10);
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 0); //Clock LOW
	delay_us(10);
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 1); //Clock HIGH
	delay_us(10);
	Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin); //Release SDA
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 0); //Clock LOW
	delay_us(10);
}

/*
 * @brief   :  Write the address (0x000) SDA is LOW for 3 SCK pulses
 */
void SHT11_Write_Address(void){
	Set_Pin_Output(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
	delay_us(1);
	for(int i = 0; i < 3; i++){
		HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 1);
		delay_us(10);
		HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 0);
	}
	Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
}

uint8_t SHT11_Write(uint8_t cmd){
	uint8_t response = 0;
	SHT11_Init_Transmission(); //Start Transmission
	SHT11_Write_Address();     //Send Address 3 CLK Cycles
	//Set_Pin_Output(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0); //Set Clock Low
	for(int i = 4; i >= 0; --i){ //Set DATA line for current bit 5 CLK Cycles
			//Write 1
		uint8_t temp_mask = ((cmd & (1 << i)) !=0);
			if(temp_mask){
				Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
				delay_us(10);
			}else{
			//Write 0
				Set_Pin_Output(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
				delay_us(10);
			}
			HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,1); //Send current MSG
			delay_us(10);
			HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0);
	}
	Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin); //Release Line
	//Check for sensor ACK on 9th clock cycle
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,1);
	delay_us(10);
	if(!HAL_GPIO_ReadPin(SHT_DATA_GPIO_Port, SHT_DATA_Pin)){
			response = 1;
		}
		else{
			response = -1;
		}
	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0);
	return response;
}

uint16_t SHT11_Read(void){
	uint16_t raw = 0;
	for(int i = 0; i < 16 ; i++){
		raw <<= 1; //Left shift to make room for current bit
		HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,1); //Send current MSG
		delay_us(10);
		HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0);
		delay_us(10);
		if(HAL_GPIO_ReadPin(SHT_DATA_GPIO_Port, SHT_DATA_Pin)){
			raw = raw | 0x0001; //Set current bit if high
		}
		if(i == 7){//ACK every byte received
			Set_Pin_Output(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
			delay_us(10);
			HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,1); //Send current MSG
			delay_us(10);
			HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0);
			delay_us(10);
			Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
		}
	}
//	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,1); //Send current MSG
//	delay_us(10);
//	HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0);
//	//Lower Byte
//	for(int i = 0; i < 7 ; i++){
//		HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,1); //Send current MSG
//		delay_us(10);
//		HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin,0);
//		delay_us(10);
//		if(HAL_GPIO_ReadPin(SHT_DATA_GPIO_Port, SHT_DATA_Pin)){
//			temp2 |= 1<<i;
//		}
//	}
	return(raw);
}

void SHT11_Reset(void){
	Set_Pin_Input(SHT_DATA_GPIO_Port, SHT_DATA_Pin);
	for(int i = 0; i < 10; i++){
			HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 1);
			delay_us(10);
			HAL_GPIO_WritePin(SHT_CLK_GPIO_Port, SHT_CLK_Pin, 0);
			delay_us(10);
		}
}

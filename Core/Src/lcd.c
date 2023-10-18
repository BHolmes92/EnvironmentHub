/*
 * header.c
 *
 *  Created on: Oct 18, 2023
 *      Author: Ben
 */

#include "lcd.h"
#include "gpio.h"
#include "main.h"
#include "utils.h"

void send_to_lcd(char data, int rs){
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, rs); //rs = 1 for data, 0 for command
	//Write data to each pin
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, ((data>>3) & 0x01));
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, ((data>>2) & 0x01));
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, ((data>>1) & 0x01));
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, ((data>>0) & 0x01));
	//Toggle EN Pin to transffer data
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, 1);
	delay_us(20);
	HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, 0);
	delay_us(20);
}

void send_cmd_lcd(char cmd){
	char databuf;
	databuf = ((cmd>>4) &0x0f);
	send_to_lcd(databuf, 0);
	databuf = ((cmd) &0x0f);
	send_to_lcd(databuf, 0);
}

void send_data_lcd(char data){
	char databuf;
	databuf = ((data >> 4)&0x0f);
	send_to_lcd(databuf, 1);
	databuf = ((data)&0x0f);
	send_to_lcd(databuf, 1);
}

void move_cur_lcd(int row, int col){
	switch(row){
	case (0):
			col |= 0x80;
			break;
	case(1):
			col |= 0xC0;
			break;
	}
	send_cmd_lcd(col);
}

void init_lcd(void){
	//Configure 4bit mode
	send_cmd_lcd(0x00);
	HAL_Delay(50);
	send_cmd_lcd(0x03);
	HAL_Delay(5);
	send_cmd_lcd(0x03);
	HAL_Delay(1);
	send_cmd_lcd(0x03);
	HAL_Delay(10);
	send_cmd_lcd(0x02);
	HAL_Delay(10);

	//Init Screen
	send_cmd_lcd(0x28);
	HAL_Delay(1);
	send_cmd_lcd(0x08);
	HAL_Delay(1);
	send_cmd_lcd(0x01);
	HAL_Delay(1);
	send_cmd_lcd(0x06);
	HAL_Delay(1);
	send_cmd_lcd(0x0C);
}

void clear_lcd(void){
	send_cmd_lcd(0x01);
}

void send_string_lcd(char* data){
	while(*data){
		send_data_lcd(*data++);
	}
}

/*
 * lcd.h
 *
 *  Created on: Oct 18, 2023
 *      Author: Ben
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

void send_to_lcd(char data, int rs);

void send_cmd_lcd(char cmd);

void move_cur_lcd(int row, int col);

void init_lcd(void);

void clear_lcd(void);

void send_string_lcd(char* data);

#endif /* INC_LCD_H_ */

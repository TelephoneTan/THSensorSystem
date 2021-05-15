//
// Created by 16244 on 2021/5/15.
//

#ifndef THSENSORSYSTEM_LCD1602_H
#define THSENSORSYSTEM_LCD1602_H

void lcd_init (void);   			// initialize lcd

void lcd_send_cmd (char cmd);  		// send command to the lcd

void lcd_send_data (char data); 	// send data to the lcd

void lcd_send_string (char *str);   // send string to the lcd

void lcd_put_cur(int row, int col); // set row  first row or secend row

void lcd_clear (void);  //clear lcd

void busy_wait(void);   // waiting for the lcd cpu free

#endif //THSENSORSYSTEM_LCD1602_H

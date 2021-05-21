//
// Created by 16244 on 2021/5/15.
//

#ifndef THSENSORSYSTEM_LCD1602_H
#define THSENSORSYSTEM_LCD1602_H

#include <stdint.h>

extern const uint16_t RS_Pin;
extern const uint16_t RW_Pin;
extern const uint16_t EN_Pin;
extern const uint16_t D0_Pin;
extern const uint16_t D1_Pin;
extern const uint16_t D2_Pin;
extern const uint16_t D3_Pin;
extern const uint16_t D4_Pin;
extern const uint16_t D5_Pin;
extern const uint16_t D6_Pin;
extern const uint16_t D7_Pin;

typedef enum LCD_PIN_VALUE {
    LCD_PIN_VALUE_LOW = 0, LCD_PIN_VALUE_HIGH = 1
} LCD_PIN_VALUE;

typedef enum LCD_PIN_MODE {
    LCD_PIN_MODE_INPUT, LCD_PIN_MODE_OUTPUT, LCD_PIN_MODE_INPUT_PULLUP
} LCD_PIN_MODE;

void LCD_pinMode(uint16_t pin, LCD_PIN_MODE mode);

void LCD_digitalWrite(uint16_t pin, LCD_PIN_VALUE value);

LCD_PIN_VALUE LCD_digitalRead(uint16_t pin);

void lcd_init (void);   			// initialize lcd

void lcd_send_cmd (char cmd);  		// send command to the lcd

void lcd_send_data (char data); 	// send data to the lcd

void lcd_send_string (char *str);   // send string to the lcd

void lcd_print(int row, int col, const char *format, ...);

void lcd_put_cur(int row, int col); // set row  first row or secend row

void lcd_clear (void);  //clear lcd

void busy_wait(void);   // waiting for the lcd cpu free

#endif //THSENSORSYSTEM_LCD1602_H
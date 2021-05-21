//
// Created by 16244 on 2021/5/15.
//

#include "LCD1602.h"

#include <stdio.h>
#include <stdarg.h>

/*
 * Waiting for the cpu free
 * There are two ways to make it acheive
 * 1.Set a delay time
 * 2.Read the D7's bit ,if its zero,cpu free,else busy.
 * 		You need to let the D7 pin as a input,after reading,set it back to output
 * 	I used the first way
 */
void busy_wait(){
    LCD_digitalWrite(RS_Pin, LCD_PIN_VALUE_LOW);
    LCD_digitalWrite(RW_Pin, LCD_PIN_VALUE_HIGH);
    LCD_digitalWrite(EN_Pin, LCD_PIN_VALUE_HIGH);

    LCD_pinMode(D7_Pin, LCD_PIN_MODE_INPUT);

    while(LCD_digitalRead(D7_Pin)==LCD_PIN_VALUE_HIGH){

    }

    LCD_pinMode(D7_Pin, LCD_PIN_MODE_OUTPUT);
}
/*
 * Read the data from D0~D7
 * Para data->data,rs->changge register(data or cmd)
 */
void send_to_lcd (char data, int rs)
{

    LCD_digitalWrite(RW_Pin, LCD_PIN_VALUE_LOW); //no read only write whether date or command
    LCD_digitalWrite(RS_Pin, rs ? LCD_PIN_VALUE_HIGH : LCD_PIN_VALUE_LOW);  // rs = 1 for data, rs=0 for command

    LCD_digitalWrite(EN_Pin, LCD_PIN_VALUE_HIGH);

    /* write the data to the respective pin */
    LCD_digitalWrite(D7_Pin, ((data>>7)&0x01));
    LCD_digitalWrite(D6_Pin, ((data>>6)&0x01));
    LCD_digitalWrite(D5_Pin, ((data>>5)&0x01));
    LCD_digitalWrite(D4_Pin, ((data>>4)&0x01));
    LCD_digitalWrite(D3_Pin, ((data>>3)&0x01));
    LCD_digitalWrite(D2_Pin, ((data>>2)&0x01));
    LCD_digitalWrite(D1_Pin, ((data>>1)&0x01));
    LCD_digitalWrite(D0_Pin, ((data>>0)&0x01));


    LCD_digitalWrite(EN_Pin, LCD_PIN_VALUE_LOW);
}
/*
 * write cmd
 */
void lcd_send_cmd (char cmd)
{
    busy_wait();

    send_to_lcd(cmd, 0);
}
/*
 * write data
 */
void lcd_send_data (char data)
{

    busy_wait();

    send_to_lcd(data, 1);
}
/*
 * clear lcd
 */
void lcd_clear (void)
{

    lcd_send_cmd(0x01);

}
/*
 * set data row
 */
void lcd_put_cur(int row, int col)
{
    busy_wait();
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}

void lcd_init (void)
{
    lcd_send_cmd(0x38);
    lcd_send_cmd(0x0c);
    lcd_send_cmd(0x06);
    lcd_send_cmd(0x01);

}
/*
 * send you string to lcd
 */
void lcd_send_string (char *str)
{
    while(*str!='\0'){
        lcd_send_data(*str++);
    }
}

void lcd_print(int row, int col, const char *format, ...){
    if (row < 0 || row > 1 || col < 0 || col > 15){
        return;
    }
    char str[17] = {0};
    va_list valist_list;
    va_start(valist_list, format);
    vsnprintf(str, sizeof(str)-col, format, valist_list);
    va_end(valist_list);
    lcd_put_cur(row, col);
    lcd_send_string(str);
}

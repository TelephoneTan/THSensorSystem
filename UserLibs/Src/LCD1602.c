//
// Created by 16244 on 2021/5/15.
//

#include "LCD1602.h"

#include "stm32f1xx_hal.h"

/*********** Define the LCD PINS below ****************/

#define RS_Pin GPIO_PIN_2  // register change
#define RS_GPIO_Port GPIOA //port A
#define RW_Pin GPIO_PIN_3  //R/W
#define RW_GPIO_Port GPIOA
#define EN_Pin GPIO_PIN_4	//ENABLE
#define EN_GPIO_Port GPIOA
#define D0_Pin GPIO_PIN_5   //DATA 0~7
#define D0_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_6
#define D1_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_7
#define D2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_4
#define D3_GPIO_Port GPIOC
#define D4_Pin GPIO_PIN_5
#define D4_GPIO_Port GPIOC
#define D5_Pin GPIO_PIN_0
#define D5_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_1
#define D6_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_2
#define D7_GPIO_Port GPIOC


GPIO_InitTypeDef GPIO_InitStruct = {0};

/*
 * Waiting for the cpu free
 * There are two ways to make it acheive
 * 1.Set a delay time
 * 2.Read the D7's bit ,if its zero,cpu free,else busy.
 * 		You need to let the D7 pin as a input,after reading,set it back to output
 * 	I used the first way
 */
void busy_wait(){

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	 __HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)==GPIO_PIN_SET){

	}

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/*
 * Read the data from D0~D7
 * Para data->data,rs->changge register(data or cmd)
 */
void send_to_lcd (char data, int rs)
{

    HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, 0); //no read only write whether date or command
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, rs);  // rs = 1 for data, rs=0 for command

    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 1);

    /* write the data to the respective pin */
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, ((data>>7)&0x01));
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, ((data>>6)&0x01));
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, ((data>>5)&0x01));
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, ((data>>4)&0x01));
    HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, ((data>>3)&0x01));
    HAL_GPIO_WritePin(D2_GPIO_Port, D2_Pin, ((data>>2)&0x01));
    HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, ((data>>1)&0x01));
    HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, ((data>>0)&0x01));


    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, 0);
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

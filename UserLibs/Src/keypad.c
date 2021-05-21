#include "keypad.h"

uint16_t read_keypad (void)
{
    /* Make ROW 1 LOW and all other ROWs HIGH */
    KEY_BOARD_digitalWrite(keyboard_R1_Pin__, KEY_BOARD_PIN_VALUE_LOW);  //Pull the R1 low
    KEY_BOARD_digitalWrite(keyboard_R2_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R2 High
    KEY_BOARD_digitalWrite(keyboard_R3_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R3 High
    KEY_BOARD_digitalWrite(keyboard_R4_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R4 High

    if (!(KEY_BOARD_digitalRead(keyboard_C1_Pin__)))   // if the Col 1 is low
    {
        while (!(KEY_BOARD_digitalRead(keyboard_C1_Pin__)));   // wait till the button is pressed
        return 1;
    }

    if (!(KEY_BOARD_digitalRead(keyboard_C2_Pin__)))   // if the Col 2 is low
    {
        while (!(KEY_BOARD_digitalRead(keyboard_C2_Pin__)));   // wait till the button is pressed
        return 2;
    }

    if (!(KEY_BOARD_digitalRead(keyboard_C3_Pin__)))   // if the Col 3 is low
    {
        while (!(KEY_BOARD_digitalRead(keyboard_C3_Pin__)));   // wait till the button is pressed
        return 3;
    }

    if (!(KEY_BOARD_digitalRead(keyboard_C4_Pin__)))   // if the Col 4 is low
    {
        while (!(KEY_BOARD_digitalRead(keyboard_C4_Pin__)));   // wait till the button is pressed
        return 4;
    }
//
//	/* Make ROW 2 LOW and all other ROWs HIGH */
//	KEY_BOARD_digitalWrite(keyboard_R1_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  //Pull the R1 low
//	KEY_BOARD_digitalWrite(keyboard_R2_Pin__, KEY_BOARD_PIN_VALUE_LOW);  // Pull the R2 High
//	KEY_BOARD_digitalWrite(keyboard_R3_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R3 High
//	KEY_BOARD_digitalWrite(keyboard_R4_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R4 High
//
//	//S5
//	if (!(KEY_BOARD_digitalRead (keyboard_C1_Pin__)))   // if the Col 1 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C1_Pin__)));   // wait till the button is pressed
//		return 5;
//	}
//
//	//S6
//	if (!(KEY_BOARD_digitalRead (keyboard_C2_Pin__)))   // if the Col 2 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C2_Pin__)));   // wait till the button is pressed
//		return 6;
//	}
//
//	//S7
//	if (!(KEY_BOARD_digitalRead (keyboard_C3_Pin__)))   // if the Col 3 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C3_Pin__)));   // wait till the button is pressed
//		return 7;
//	}
//
//	//S8
//	if (!(KEY_BOARD_digitalRead (keyboard_C4_Pin__)))   // if the Col 4 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C4_Pin__)));   // wait till the button is pressed
//		return 8;
//	}
//
//
//	/* Make ROW 3 LOW and all other ROWs HIGH */
//	KEY_BOARD_digitalWrite(keyboard_R1_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  //Pull the R1 low
//	KEY_BOARD_digitalWrite(keyboard_R2_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R2 High
//	KEY_BOARD_digitalWrite(keyboard_R3_Pin__, KEY_BOARD_PIN_VALUE_LOW);  // Pull the R3 High
//	KEY_BOARD_digitalWrite(keyboard_R4_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R4 High
//
//	//S9
//	if (!(KEY_BOARD_digitalRead (keyboard_C1_Pin__)))   // if the Col 1 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C1_Pin__)));   // wait till the button is pressed
//		return 9;
//	}
//
//	//S10
//	if (!(KEY_BOARD_digitalRead (keyboard_C2_Pin__)))   // if the Col 2 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C2_Pin__)));   // wait till the button is pressed
//		return 10;
//	}
//
//	//S11
//	if (!(KEY_BOARD_digitalRead (keyboard_C3_Pin__)))   // if the Col 3 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C3_Pin__)));   // wait till the button is pressed
//		return 11;
//	}
//
//	//S12
//	if (!(KEY_BOARD_digitalRead (keyboard_C4_Pin__)))   // if the Col 4 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C4_Pin__)));   // wait till the button is pressed
//		return 12;
//	}
//
//
//	/* Make ROW 4 LOW and all other ROWs HIGH */
//	KEY_BOARD_digitalWrite(keyboard_R1_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  //Pull the R1 low
//	KEY_BOARD_digitalWrite(keyboard_R2_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R2 High
//	KEY_BOARD_digitalWrite(keyboard_R3_Pin__, KEY_BOARD_PIN_VALUE_HIGH);  // Pull the R3 High
//	KEY_BOARD_digitalWrite(keyboard_R4_Pin__, KEY_BOARD_PIN_VALUE_LOW);  // Pull the R4 High
//	//S13
//	if (!(KEY_BOARD_digitalRead (keyboard_C1_Pin__)))   // if the Col 1 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C1_Pin__)));   // wait till the button is pressed
//		return 13;
//	}
//
//	//S14
//	if (!(KEY_BOARD_digitalRead (keyboard_C2_Pin__)))   // if the Col 2 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C2_Pin__)));   // wait till the button is pressed
//		return 14;
//	}
//
//	//S15
//	if (!(KEY_BOARD_digitalRead (keyboard_C3_Pin__)))   // if the Col 3 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C3_Pin__)));   // wait till the button is pressed
//		return 15;
//	}
//
//	//S16
//	if (!(KEY_BOARD_digitalRead (keyboard_C4_Pin__)))   // if the Col 4 is low
//	{
//		while (!(KEY_BOARD_digitalRead (keyboard_C4_Pin__)));   // wait till the button is pressed
//		return 16;
//	}

}

#ifndef KEYPAD_KEYPAD_H_
#define KEYPAD_KEYPAD_H_

#include <stdint.h>

/**
 * Return an integer according to the key pressed.
 * @note This function will block until any key is pressed.
 * @return 0~15
 */
uint16_t read_keypad (void);

/**
 * Return an integer.
 * @return 0~15 if any key is pressed, or else UINT16_MAX
 */
uint16_t test_read_keypad();

extern const uint16_t keyboard_C1_Pin__;
extern const uint16_t keyboard_C2_Pin__;
extern const uint16_t keyboard_C3_Pin__;
extern const uint16_t keyboard_C4_Pin__;
extern const uint16_t keyboard_R1_Pin__;
extern const uint16_t keyboard_R2_Pin__;
extern const uint16_t keyboard_R3_Pin__;
extern const uint16_t keyboard_R4_Pin__;

typedef enum KEY_BOARD_PIN_VALUE {
    KEY_BOARD_PIN_VALUE_LOW=0, KEY_BOARD_PIN_VALUE_HIGH=1
} KEY_BOARD_PIN_VALUE;

void KEY_BOARD_digitalWrite(uint16_t pin, KEY_BOARD_PIN_VALUE value);

KEY_BOARD_PIN_VALUE KEY_BOARD_digitalRead(uint16_t pin);

#endif

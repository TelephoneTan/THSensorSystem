#include "keypad.h"

uint16_t read_keypad (void)
{
    uint16_t btn_C[4] = {
            keyboard_C1_Pin__,
            keyboard_C2_Pin__,
            keyboard_C3_Pin__,
            keyboard_C4_Pin__
    };
    uint16_t btn_R[4] = {
            keyboard_R1_Pin__,
            keyboard_R2_Pin__,
            keyboard_R3_Pin__,
            keyboard_R4_Pin__
    };
    while (1) {
        for (int i = 0; i < sizeof(btn_C) / sizeof(uint16_t); ++i) {
            for (int set_index = 0; set_index < sizeof(btn_C) / sizeof(uint16_t); ++set_index) {
                KEY_BOARD_digitalWrite(btn_C[set_index],
                                       set_index == i ? KEY_BOARD_PIN_VALUE_HIGH : KEY_BOARD_PIN_VALUE_LOW);
            }
            for (int j = 0; j < sizeof(btn_R) / sizeof(uint16_t); ++j) {
                KEY_BOARD_PIN_VALUE key_value = KEY_BOARD_digitalRead(btn_R[j]);
                if (key_value) {
                    return i * (sizeof(btn_C) / sizeof(uint16_t)) + j;
                }
            }
        }
    }
}

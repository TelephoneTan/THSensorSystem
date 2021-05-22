#include "keypad.h"

static uint16_t read_keypad_in(int endless_loop, long long repeat_time){
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
    for(int r = 0; endless_loop || r < repeat_time; r++) {
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
    return UINT16_MAX;
}

uint16_t read_keypad (void)
{
    return read_keypad_in(1, 0);
}

uint16_t test_read_keypad(){
    return read_keypad_in(0, 1LL);
}

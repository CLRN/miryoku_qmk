// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"
#include "quantum/split_common/transactions.h"


// Additional Features double tap guard

enum {
    U_TD_BOOT,
#define MIRYOKU_X(LAYER, STRING) U_TD_U_##LAYER,
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    reset_keyboard();
  }
}

#define MIRYOKU_X(LAYER, STRING) \
void u_td_fn_U_##LAYER(tap_dance_state_t *state, void *user_data) { \
  if (state->count == 2) { \
    default_layer_set((layer_state_t)1 << U_##LAYER); \
  } \
}
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

tap_dance_action_t tap_dance_actions[] = {
    [U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};


// keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#define MIRYOKU_X(LAYER, STRING) [U_##LAYER] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_##LAYER, MIRYOKU_LAYER_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};


// shift functions

const key_override_t capsword_key_override = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);

const key_override_t **key_overrides = (const key_override_t *[]){
    &capsword_key_override,
    NULL
};


// thumb combos

#if defined (MIRYOKU_KLUDGE_THUMBCOMBOS)
const uint16_t PROGMEM thumbcombos_base_right[] = {LT(U_SYM, KC_ENT), LT(U_NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM thumbcombos_base_left[] = {LT(U_NAV, KC_SPC), LT(U_MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM thumbcombos_nav[] = {KC_ENT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM thumbcombos_mouse[] = {KC_BTN2, KC_BTN1, COMBO_END};
const uint16_t PROGMEM thumbcombos_media[] = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM thumbcombos_num[] = {KC_0, KC_MINS, COMBO_END};
  #if defined (MIRYOKU_LAYERS_FLIP)
const uint16_t PROGMEM thumbcombos_sym[] = {KC_UNDS, KC_LPRN, COMBO_END};
  #else
const uint16_t PROGMEM thumbcombos_sym[] = {KC_RPRN, KC_UNDS, COMBO_END};
  #endif
const uint16_t PROGMEM thumbcombos_fun[] = {KC_SPC, KC_TAB, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
  COMBO(thumbcombos_base_right, LT(U_FUN, KC_DEL)),
  COMBO(thumbcombos_base_left, LT(U_MEDIA, KC_ESC)),
  COMBO(thumbcombos_nav, KC_DEL),
  COMBO(thumbcombos_mouse, KC_BTN3),
  COMBO(thumbcombos_media, KC_MUTE),
  COMBO(thumbcombos_num, KC_DOT),
  #if defined (MIRYOKU_LAYERS_FLIP)
  COMBO(thumbcombos_sym, KC_RPRN),
  #else
  COMBO(thumbcombos_sym, KC_LPRN),
  #endif
  COMBO(thumbcombos_fun, KC_APP)
};
#endif

static char oled_buffer[32] = {};
#define COMMAND_COUNT 2

void handle_this_side_data(uint8_t *data, uint8_t length)
{
    uint8_t cmd = data[0] % COMMAND_COUNT;
    ++data;
    --length;
    switch (cmd) {
        case 0:
            memcpy(oled_buffer, data, length);
            break;
        case 1:
            rgblight_sethsv_noeeprom(data[0], data[1], data[2]);
            break;
    }
}

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    handle_this_side_data((uint8_t*)in_data, in_buflen);
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(0, 0, 0);
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
}

// enum layers { BASE, EXTRA, TAP, BUTTON, NAV, MOUSE, MEDIA, NUM, SYM, FUN };

// void oled_render_layer_state(void) {
//     oled_write("Layer: ", false);
//
//     switch (get_highest_layer(layer_state)) {
//         case BASE:
//             oled_write("Base", false);
//             break;
//         case EXTRA:
//             oled_write("Extra", false);
//             break;
//         case TAP:
//             oled_write("Tap", false);
//             break;
//         case BUTTON:
//             oled_write("Button", false);
//             break;
//         case NAV:
//             oled_write("Navigation", false);
//             break;
//         case MOUSE:
//             oled_write("Mouse", false);
//             break;
//         case MEDIA:
//             oled_write("Media", false);
//             break;
//         case NUM:
//             oled_write("Number", false);
//             break;
//         case SYM:
//             oled_write("Symbol", false);
//             break;
//         case FUN:
//             oled_write("Function", false);
//             break;
//     }
//
//     oled_write("\n", false);
//
//     char wpm_str[10];
//     sprintf(wpm_str, "WPM: %03d", get_current_wpm());
//     oled_write(wpm_str, false);
// }


// static void render_logo(void) {
//     static const char PROGMEM qmk_logo[] = {
//         0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
//         0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
//         0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
//     };
//
//     oled_write_P(qmk_logo, false);
// }

// void housekeeping_task_user(void) {
//     if (is_keyboard_master()) {
//         // Interact with slave every 500ms
//         static uint32_t last_sync = 0;
//         if (timer_elapsed32(last_sync) > 500) {
//             char test[32] = {};
//             sprintf(test, "WPM: %03d", get_current_wpm());
//             if (transaction_rpc_exec(USER_SYNC_A, sizeof(test), test, sizeof(test), test)) {
//                 last_sync = timer_read32();
//             }
//         }
//     }
// }

bool oled_task_user(void) {
    oled_write(oled_buffer, false);
    return false;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    uint8_t slave = data[0] / COMMAND_COUNT;
    if (slave) {
        transaction_rpc_exec(USER_SYNC_A, length, data, 0, NULL);
    } else {
        handle_this_side_data(data, length);
    }
}

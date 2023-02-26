/* Copyright 2021 djvs
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names { _BASE};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes { QMKBEST = SAFE_RANGE, QMKURL };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
      KC_ESC     ,KC_F1      ,KC_F3      ,KC_F5      ,KC_F7      ,KC_F9      ,KC_F11     ,KC_PAUSE              ,KC_AUDIO_VOL_UP    ,KC_PLUS ,\
      KC_GRAVE   ,KC_2       ,KC_4       ,KC_6       ,KC_8       ,KC_0       ,KC_EQUAL   ,KC_HOME               ,KC_NUMLOCK         ,KC_SLASH ,\
      KC_TAB     ,KC_W       ,KC_R       ,KC_Y       ,KC_I       ,KC_P       ,KC_RBRACKET,KC_END                ,KC_KP_7            ,KC_KP_9 ,\
      KC_CAPS    ,KC_S       ,KC_F       ,KC_H       ,KC_K       ,KC_SCOLON  ,KC_BSLASH  ,KC_UP                 ,KC_KP_4            ,KC_KP_PLUS ,\
      KC_LSHIFT  ,KC_X       ,KC_V       ,KC_N       ,KC_COMMA   ,KC_SLASH   ,KC_ENTER   ,KC_DOWN               ,KC_KP_2            ,KC_KP_ENTER ,\
      KC_LCTRL   ,KC_LALT    ,KC_SPACE   ,KC_M       ,KC_RALT    ,KC_RGUI    ,KC_RCTRL   ,KC_RIGHT              ,KC_KP_DOT          ,KC_AUDIO_VOL_UP ,\
      KC_LGUI    ,KC_C       ,KC_B       ,KC_J       ,KC_DOT     ,KC_RSHIFT  ,KC_LEFT    ,KC_KP_0               ,KC_KP_3            ,KC_PLUS ,\
      KC_Z       ,KC_D       ,KC_G       ,KC_U       ,KC_L       ,KC_QUOTE   ,KC_DELETE  ,KC_KP_1               ,KC_KP_6            ,KC_AUDIO_VOL_DOWN,\
      KC_A       ,KC_E       ,KC_T       ,KC_7       ,KC_O       ,KC_LBRACKET,KC_INSERT  ,KC_PGDOWN             ,KC_KP_5            ,KC_KP_MINUS ,\
      KC_Q       ,KC_3       ,KC_5       ,KC_PLUS    ,KC_9       ,KC_MINUS   ,KC_BSPACE  ,KC_PGUP               ,KC_KP_8            ,KC_KP_ASTERISK ,\
      KC_1       ,KC_F2      ,KC_F4      ,KC_F6      ,KC_F8      ,KC_F10     ,KC_F12     ,KC_AUDIO_VOL_DOWN     ,KC_KP_EQUAL        ,KC_MY_COMPUTER \
    ) 
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case KC_MY_COMPUTER:
      if ((get_mods() & MOD_MASK_GUI)) {
        reset_keyboard();
      /*} else if(record->event.pressed){
        tap_code16(KC_MY_COMPUTER);*/
      }
      break;
  }
  return true;
};

const rgblight_segment_t PROGMEM my_capslock_layer[] =
    RGBLIGHT_LAYER_SEGMENTS({1, 1, HSV_PURPLE});

const rgblight_segment_t PROGMEM my_numlock_layer[] =
    RGBLIGHT_LAYER_SEGMENTS({2, 1, HSV_BLUE});

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] =
    RGBLIGHT_LAYERS_LIST(my_capslock_layer, my_numlock_layer);

void keyboard_post_init_user(void) { rgblight_layers = my_rgb_layers; };

bool led_update_user(led_t led_state) {
  rgblight_set_layer_state(0, led_state.caps_lock);
  rgblight_set_layer_state(1, led_state.num_lock);
  return true;
};


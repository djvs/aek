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
      KC_ESC         ,KC_F1      ,KC_F3      ,KC_F5      ,KC_F7      ,KC_F9          ,KC_F11              ,KC_PAUSE              ,KC_AUDIO_VOL_UP    ,KC_PLUS ,\
      KC_GRAVE       ,KC_2       ,KC_4       ,KC_6       ,KC_8       ,KC_0           ,KC_EQUAL            ,KC_HOME               ,KC_NUM_LOCK         ,KC_SLASH ,\
      KC_TAB         ,KC_W       ,KC_R       ,KC_Y       ,KC_I       ,KC_P           ,KC_RIGHT_BRACKET    ,KC_END                ,KC_KP_7            ,KC_KP_9 ,\
      KC_CAPS        ,KC_S       ,KC_F       ,KC_H       ,KC_K       ,KC_SEMICOLON   ,KC_BACKSLASH        ,KC_UP                 ,KC_KP_4            ,KC_KP_PLUS ,\
      KC_LEFT_SHIFT  ,KC_X       ,KC_V       ,KC_N       ,KC_COMMA   ,KC_SLASH       ,KC_ENTER            ,KC_DOWN               ,KC_KP_2            ,KC_KP_ENTER ,\
      KC_LEFT_CTRL   ,KC_LALT    ,KC_SPACE   ,KC_M       ,KC_RALT    ,KC_RGUI        ,KC_RIGHT_CTRL       ,KC_RIGHT              ,KC_KP_DOT          ,KC_AUDIO_VOL_UP ,\
      KC_LGUI        ,KC_C       ,KC_B       ,KC_J       ,KC_DOT     ,KC_RIGHT_SHIFT ,KC_LEFT             ,KC_KP_0               ,KC_KP_3            ,KC_PLUS ,\
      KC_Z           ,KC_D       ,KC_G       ,KC_U       ,KC_L       ,KC_QUOTE       ,KC_DELETE           ,KC_KP_1               ,KC_KP_6            ,KC_AUDIO_VOL_DOWN,\
      KC_A           ,KC_E       ,KC_T       ,KC_7       ,KC_O       ,KC_LEFT_BRACKET,KC_INSERT           ,KC_PGDN               ,KC_KP_5            ,KC_KP_MINUS ,\
      KC_Q           ,KC_3       ,KC_5       ,KC_PLUS    ,KC_9       ,KC_MINUS       ,KC_BACKSPACE        ,KC_PGUP               ,KC_KP_8            ,KC_KP_ASTERISK ,\
      KC_1           ,KC_F2      ,KC_F4      ,KC_F6      ,KC_F8      ,KC_F10         ,KC_F12              ,KC_AUDIO_VOL_DOWN     ,KC_KP_EQUAL        ,KC_MY_COMPUTER \
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case KC_MY_COMPUTER:
      if ((get_mods() & MOD_MASK_GUI)) {
        eeconfig_init();
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


// custom LED animation
static const HSV custom_colors[] = {
    { 0,   192, 255 },   // red
    { 32,  192, 255 },   // orange
    { 170, 192, 255 },   // blue
};

#define CUSTOM_COLOR_COUNT (sizeof(custom_colors) / sizeof(custom_colors[0]))

#define TOTAL_CYCLE_SECONDS 5
#define TOTAL_CYCLE_MS (TOTAL_CYCLE_SECONDS * 1000UL) 
#define FRAME_INTERVAL_MS 16


// smooth hue transitions
static uint8_t hue_lerp(uint8_t a, uint8_t b, uint8_t t) {
    int16_t delta = b - a;
    if (delta > 127)  delta -= 256;
    if (delta < -127) delta += 256;
    return a + ((delta * t) >> 8); 
}

// smooth total hsv transitions (wraps hue_lerp)
static HSV hsv_lerp_smooth(HSV a, HSV b, uint8_t t_8bit) {
    HSV out;
    out.h = hue_lerp(a.h, b.h, t_8bit); 

    out.s = a.s + (((int32_t)b.s - a.s) * t_8bit >> 8);
    out.v = a.v + (((int32_t)b.v - a.v) * t_8bit >> 8);
    
    return out;
}


void rgblight_effect_custom_swirl_v2(void) {
    static uint32_t last_update_time = 0;

    // only update once per FRAME_INTERVAL_MS
    if (timer_elapsed32(last_update_time) < FRAME_INTERVAL_MS) {
        return;
    }
    last_update_time = timer_read32();

    // calculate the total elapsed time mod the cycle time
    uint32_t current_time_ms = timer_read32() % TOTAL_CYCLE_MS;

    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        // calculate a staggered delay for each LED (0ms for LED0, up to ~10s for the last LED)
        uint32_t led_offset_ms = (TOTAL_CYCLE_MS / RGBLED_NUM) * i;
        uint32_t led_time_ms = (current_time_ms + led_offset_ms) % TOTAL_CYCLE_MS;

        // determine progress through the 10-second cycle (0-255 range)
        uint8_t global_blend_percent = (led_time_ms * 255UL) / TOTAL_CYCLE_MS;
        
        // map the global percentage to the current color segment
        // how many steps between colors? (e.g. 2 for red->orange, orange->blue)
        uint32_t total_steps = 256UL * CUSTOM_COLOR_COUNT;
        uint32_t current_step = (global_blend_percent * total_steps) >> 8;

        uint8_t idx   = (current_step >> 8) % CUSTOM_COLOR_COUNT;          // Current color index
        uint8_t next  = (idx + 1) % CUSTOM_COLOR_COUNT;                    // Next color index
        uint8_t blend = (uint8_t)(current_step & 0xFF);                    // Blend factor (0-255)

        HSV hsv = hsv_lerp_smooth(
            custom_colors[idx],
            custom_colors[next],
            blend
        );

        hsv.v = rgblight_get_val(); 
        RGB rgb = hsv_to_rgb(hsv);
        rgblight_setrgb_at(rgb.r, rgb.g, rgb.b, i);
    }
}

void matrix_scan_user(void) {
    rgblight_effect_custom_swirl_v2();
}

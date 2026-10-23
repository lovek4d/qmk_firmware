/* Copyright 2015-2021 Jack Humbert
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

#ifdef AUDIO_ENABLE
#    include "muse.h"
#endif

enum planck_layers {
  _QWERTY,
  _COLEMAK,
  _LOWER,
  _RAISE,
  _ADJUST
};

#define QWERTY  PDF(_QWERTY)
#define COLEMAK PDF(_COLEMAK)

#define SFTENT RSFT_T(KC_ENT) // shift on hold, enter on tap
#define ESCCMD LGUI_T(KC_ESC) // gui on hold, escape on tap

enum planck_keycodes {
  GLOBE = SAFE_RANGE, // macOS Globe/Fn key (AC Next Keyboard Layout Select)
  LOWER,              // momentary _LOWER; tap then hold for _RAISE
  RAISE               // momentary _RAISE; tap then hold for _LOWER
};

#define LAYER_SWAP_TERM 200 // ms window for the tap and re-press that swap LOWER/RAISE

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |EscCmd|   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |SftEnt|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  Fn  | Ctrl |  Alt | Cmd  |LOWER |    Space    |RAISE | Left |Right | Down |  Up  |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    ESCCMD,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFTENT,
    GLOBE,   KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_RGHT, KC_DOWN, KC_UP
),

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |EscCmd|   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  '   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |SftEnt|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  Fn  | Ctrl |  Alt | Cmd  |LOWER |    Space    |RAISE | Left |Right | Down |  Up  |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,   KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC,
    ESCCMD,  KC_A,    KC_R,    KC_S,    KC_T,   KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFTENT,
    GLOBE,   KC_LCTL, KC_LALT, KC_LGUI, LOWER,  KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_RGHT, KC_DOWN, KC_UP
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Enter |MsnCtl| Home |  Up  | End  |PageUp|   *  |   4  |   5  |   6  |   +  |  =   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |PrtScn| Left | Down |Right |PageDn|   /  |   1  |   2  |   3  |   -  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Del  |      |      |      |------|             |      |   0  |   .  |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    _______,
    KC_ENT,  KC_MCTL, KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_ASTR, KC_4,    KC_5,    KC_6,   KC_PLUS, KC_EQL,
    _______, KC_PSCR, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_SLSH, KC_1,    KC_2,    KC_3,   KC_MINS, _______,
    KC_DEL,  _______, _______, _______, _______, _______, _______, _______, KC_0,    KC_DOT, _______, _______
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  Up  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | Down |   _  |   +  |   {  |   }  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |------|      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR,  KC_LPRN, KC_RPRN, KC_DEL,
    KC_F1,   KC_F2,    KC_F3,  KC_F4,   KC_F5,   KC_F6,   KC_UP,   KC_MINS, KC_EQL,   KC_LBRC, KC_RBRC, KC_BSLS,
    KC_F7,    KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,  KC_DOWN, KC_UNDS, KC_PLUS,  KC_LCBR, KC_RCBR, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______
),

/* Adjust (Lower + Raise), Media and RGB control
 * ,-----------------------------------------------------------------------------------.
 * |NK_TOG|      |      |      |R_BRT-|RGBTOG|RGBMOD|R_BRT+|      |      |Colmak|Qwerty|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |CapLck| Prev | Play | Next |Brgt- |R_HUE-|R_HUE+|Brgt+ | Vol- | Mute | Vol+ |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Audio-|Audio |Audio+|      |R_SAT-|R_SAT+|      |      |Music |Music+|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Sleep |      |      |      |------|             |------|      |      |      | DFU  |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    NK_TOGG, _______, _______, _______, UG_VALD, UG_TOGG, UG_NEXT, UG_VALU, _______, _______, COLEMAK, QWERTY,
    KC_CAPS, KC_MPRV, KC_MPLY, KC_MNXT, KC_BRID, UG_HUED, UG_HUEU, KC_BRIU, KC_VOLD, KC_MUTE, KC_VOLU, _______,
    _______, AU_PREV, AU_TOGG, AU_NEXT, _______, UG_SATD, UG_SATU, _______, _______, MU_TOGG, MU_NEXT, _______,
    KC_SLEP, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT
)

};


layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t tap_key = KC_NO; // LOWER/RAISE if it was just cleanly tapped
  static uint16_t tap_time;
  static uint8_t  lower_held = _LOWER, raise_held = _RAISE;

  switch (keycode) {
    case LOWER:
    case RAISE: {
      uint8_t *held = keycode == LOWER ? &lower_held : &raise_held;
      if (record->event.pressed) {
        bool swap = tap_key == keycode && timer_elapsed(tap_time) < LAYER_SWAP_TERM;
        *held = (keycode == LOWER) != swap ? _LOWER : _RAISE;
        layer_on(*held);
        tap_key = swap ? KC_NO : keycode;
        tap_time = timer_read();
      } else {
        layer_off(*held);
        if (tap_key == keycode && timer_elapsed(tap_time) < LAYER_SWAP_TERM) {
          tap_time = timer_read();
        } else {
          tap_key = KC_NO;
        }
      }
      return false;
    }
  }

  if (record->event.pressed) {
    tap_key = KC_NO; // any other key press means the last LOWER/RAISE wasn't a clean tap
  }

  switch (keycode) {
    case GLOBE:
      host_consumer_send(record->event.pressed ? AC_NEXT_KEYBOARD_LAYOUT_SELECT : 0);
      return false;
  }
  return true;
}

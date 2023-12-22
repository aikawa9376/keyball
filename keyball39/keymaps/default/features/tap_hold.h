// 基本はこれをオンにする
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(1, KC_A):
    case SFT_T(KC_Z):
      // Immediately select the hold action when another key is pressed.
      return false;
    default:
      // Do not select the hold action when another key is pressed.
      return true;
  }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(1, KC_A):
    case SFT_T(KC_Z):
      // Immediately select the hold action when another key is tapped.
      return true;
    default:
      // Do not select the hold action when another key is tapped.
      return false;
  }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LCTL_T(KC_ESC):
      return 0;
    default:
      return QUICK_TAP_TERM;
  }
}
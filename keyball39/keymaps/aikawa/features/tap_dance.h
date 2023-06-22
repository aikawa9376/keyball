// タップダンスの宣言
enum {
    TD_ESC_CAPS,
};

// タップダンスの定義
tap_dance_action_t tap_dance_actions[] = {
    // 1回タップすると Escape キー、2回タップすると Caps Lock。
    [TD_ESC_CAPS]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
};

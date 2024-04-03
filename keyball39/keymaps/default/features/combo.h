/* Copyright 2023 kamidai (@d_kamiichi)
 *
 * 参考ページ
 * 公式：https://docs.qmk.fm/#/ja/feature_combo?id=%e3%82%b3%e3%83%b3%e3%83%9c
 *
 * 注意： 以下の設定が必要です
 * rules.mk: COMBO_ENABLE = yes
 * config.h: #define COMBO_COUNT 2（=実際に設定するコンボ数）
 */

enum combos {
    CM_JK,
    CM_DF,
    CM_CV,
    CM_MV,
    CM_MS,
    CM_LC,
    CM_RC,
    CM_TG,
};

const uint16_t PROGMEM space_combo[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM enter_combo[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM imeon_combo[] = {KC_C, KC_V, COMBO_END};
// const uint16_t PROGMEM rctrl_combo[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM rctrl_combo[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM lctrl_combo[] = {KC_S, KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM spacem_combo[] = {KC_OG_BTN1, KC_OG_BTN2, COMBO_END};
const uint16_t PROGMEM imeonm_combo[] = {KC_OG_BTN4, KC_OG_BTN5, COMBO_END};

// test
const uint16_t PROGMEM stogg_combo[] = {SFT_T(KC_Z), SFT_T(KC_SLSH), COMBO_END};

combo_t key_combos[] = {
    [CM_JK] = COMBO(space_combo, KC_SPACE),
    [CM_DF] = COMBO(enter_combo, KC_ENTER),
    [CM_CV] = COMBO(imeon_combo, KC_RGUI),
    [CM_LC] = COMBO(rctrl_combo, MO(3)),
    [CM_RC] = COMBO(lctrl_combo, MO(3)),
    [CM_TG] = COMBO(stogg_combo, CW_TOGG),
    [CM_MV] = COMBO(imeonm_combo, KC_RGUI),
    [CM_MS] = COMBO(spacem_combo, KC_SPACE),
};

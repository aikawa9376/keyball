/* Copyright 2023 kamidai (@d_kamiichi)
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
 *
 * コード表：
 * - 公式ファームウェアのキーコード：
 *   KBC_RST(0x5DA5): Keyball 設定のリセット
 *   KBC_SAVE(0x5DA6): 現在の Keyball 設定を EEPROM に保存します
 *   CPI_I100(0x5DA7): CPI を 100 増加させます(最大:12000)
 *   CPI_D100(0x5DA8): CPI を 100 減少させます(最小:100)
 *   CPI_I1K(0x5DA9): CPI を 1000 増加させます(最大:12000)
 *   CPI_D1K(0x5DAA): CPI を 1000 減少させます(最小:100)
 *   SCRL_TO(0x5DAB): タップごとにスクロールモードの ON/OFF を切り替えます
 *   SCRL_MO(0x5DAC): キーを押している間、スクロールモードになります
 *   SCRL_DVI(0x5DAD): スクロール除数を１つ上げます(max D7 = 1/128)← 最もスクロール遅い
 *   SCRL_DVD(0x5DAE): スクロール除数を１つ下げます(min D0 = 1/1)← 最もスクロール速い
 */

enum custom_keycodes {
  KC_DBLB = KEYBALL_SAFE_RANGE,
  KC_TRPB,
  MC_TMUX,
  MC_TMCP,
  MC_ESC,
  SCRL_HO,
  SCRL_VR,
  SCRL_TB,
  SCRL_WD,
  ALT_TAB,
  AC_INS,
  AC_KEP,
};

extern uint16_t horizontal_flag;

bool hold_ctrl = false;
bool is_single_tap = true;
bool is_alt_tab_active = false; // ADD this near the beginning of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

// マクロキーの処理を行う関数
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  if (record->event.pressed) {
    is_single_tap = false;
    // ctrlキー押下時に他キーが押されたらクリックレイヤーを解除
    if (hold_ctrl) {
        disable_click_layer();
    }
  }

  switch (keycode) {
    // Tmuxのプレフィックス
    case MC_TMUX: {
      if (record->event.pressed) {
        tap_code16(RALT(KC_SPACE));
      }
      return false;  // キーのデフォルトの動作をスキップする
    }
    // Tmuxのコピーモード
    case MC_TMCP: {
      if (record->event.pressed) {
        tap_code16(RALT(KC_SPACE));
        tap_code16(KC_SPACE);
      }
      return false;  // キーのデフォルトの動作をスキップする
    }

    // windows切り替え用
    case ALT_TAB: {
      if (record->event.pressed) {
        if (!is_alt_tab_active) {
          is_alt_tab_active = true;
          register_code(KC_LALT);
        }
        alt_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
    }

    // 水平に固定してスクロール
    case SCRL_HO: {
      keyball_set_scroll_mode(record->event.pressed);

      if (record->event.pressed) {
          horizontal_flag = 1;
          state = CLICKING;
      } else {
          horizontal_flag = 0;
          state = CLICKED;
      }
      return false;
    }
    // 垂直に固定してスクロール
    case SCRL_VR: {
      keyball_set_scroll_mode(record->event.pressed);

      if (record->event.pressed) {
          horizontal_flag = 2;
          state = CLICKING;
      } else {
          horizontal_flag = 0;
          state = CLICKED;
      }
      return false;
    }

    // vivaldiのタブサイクルをワンキーで
    case SCRL_TB: {
      keyball_set_scroll_mode(record->event.pressed);

      if (record->event.pressed) {
          keyball_set_cpi(4);
          horizontal_flag = 1;
          register_code(KC_MS_BTN2);
          state = CLICKING;
      } else {
          keyball_set_cpi(KEYBALL_CPI_DEFAULT / 100);
          horizontal_flag = 0;
          unregister_code(KC_MS_BTN2);
          state = CLICKED;
      }
      return false;
    }

    // windowsでトラボでタスクスイッチするやつ
    case SCRL_WD: {
      keyball_set_scroll_mode(record->event.pressed);

      if (record->event.pressed) {
          scroll_convert_flag = true;
          horizontal_flag = 1;
          keyball_set_cpi(2);
          register_code(KC_LALT);
          state = CLICKING;
      } else {
          scroll_convert_flag = false;
          horizontal_flag = 0;
          keyball_set_cpi(KEYBALL_CPI_DEFAULT / 100);
          unregister_code(KC_LALT);
          state = CLICKED;
      }
      return false;
    }

    // 自動クリックレイヤーではESCは解除キーとして扱う
    case MC_ESC: {
      if (record->event.pressed) {
          is_single_tap = true;
          hold_ctrl = true;
          register_code(KC_LCTL);
      } else {
          hold_ctrl = false;
          unregister_code(KC_LCTL);
          if(is_single_tap) {
              if (click_layer && get_highest_layer(layer_state) == click_layer) {
                disable_click_layer();
              } else {
                tap_code16(KC_ESC);
                // 運用テスト
                tap_code16(KC_ESC);
              }
          }
      }

      return false;
    }

    // デフォルトのマウスキーを自動クリックレイヤーで使用可能にする
    case KC_MS_BTN1:
    case KC_MS_BTN2:
    case KC_MS_BTN3:
    case KC_MS_BTN4:
    case KC_MS_BTN5: {
      if (click_layer && get_highest_layer(layer_state) == click_layer) {
        if (record->event.pressed) {
          // キーダウン時: 状態をCLICKINGに設定
          state = CLICKING;
        } else {
          // キーアップ時: クリックレイヤーを有効にして、状態をCLICKEDに設定
          enable_click_layer();
          state = CLICKED;
        }
      }
      return true;
    }
    case KC_DBLB:
    case KC_TRPB: {
      if (click_layer && get_highest_layer(layer_state) == click_layer) {
        if (record->event.pressed) {
          // キーダウン時
          // `KC_DBLB`の場合
          if (keycode == KC_DBLB) {
            double_click_mouse_button1();  // マウスボタン1をダブルクリック
          }
          // `KC_TRPB`の場合
          if (keycode == KC_TRPB) {
            triple_click_mouse_button1();  // マウスボタン1をトリプルクリック
          }
        } else {
          if (click_layer && get_highest_layer(layer_state) == click_layer) {
            // キーアップ時: クリックレイヤーを有効にして、状態をCLICKEDに設定
            enable_click_layer();
            state = CLICKED;
          }
        }
        return false;  // キーのデフォルトの動作をスキップする
      } else {
        return true;
      }
    }
    case AC_KEP: {
      if (record->event.pressed) {
        keep_click_layer = true;
      }
      return false;
    }
  }

  disable_click_layer();
  return true;
}

void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

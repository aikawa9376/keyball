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
  SCRL_HO,
  SCRL_VR,
};

extern uint16_t horizontal_flag;

// マクロキーの処理を行う関数
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {
    case KC_DBLB:
    case KC_TRPB: {
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
    }
    // Tmuxのプレフィックス
    case MC_TMUX: {
      tap_code16(A(KC_SPACE));
      return false;  // キーのデフォルトの動作をスキップする
    }
    // Tmuxのコピーモード
    case MC_TMCP: {
      tap_code16(A(KC_SPACE));
      tap_code16(KC_SPACE);
      return false;  // キーのデフォルトの動作をスキップする
    }

    // 水平に固定してスクロール
    case SCRL_HO: {
      if (record->event.pressed) {
          horizontal_flag = 1;
          keyball_set_scroll_mode(record->event.pressed);
      } else {
          horizontal_flag = 0;
          keyball_set_scroll_mode(record->event.pressed);
      }
      return false;
    }
    // 垂直に固定してスクロール
    case SCRL_VR: {
      if (record->event.pressed) {
          horizontal_flag = 2;
          keyball_set_scroll_mode(record->event.pressed);
      } else {
          horizontal_flag = 0;
          keyball_set_scroll_mode(record->event.pressed);
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
  }

  return true;
}

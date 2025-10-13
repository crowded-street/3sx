/**
 * @file effc4.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effc4.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/menu/ex_data.h"

void Setup_Letter_C4(WORK_Other_CONN* ewk);

void effect_C4_move(WORK_Other_CONN* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (Menu_Cursor_Y[0] == ewk->wu.type) {
        ewk->wu.my_clear_level = 0;
    } else {
        ewk->wu.my_clear_level = 128;
    }

    if (ewk->master_id) {
        Setup_Letter_C4(ewk);
    }

    sort_push_request3(&ewk->wu);
}

s32 effect_C4_init(s16 id, s16 letter_type, s16 cursor_index, s16 master_player) {
    WORK_Other_CONN* ewk;
    s16 ix;
    s16 x;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.id = 124;
    ewk->wu.work_id = 16;
    ewk->master_id = id;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.my_family = 1;
    ewk->wu.char_index = letter_type;
    ewk->wu.type = cursor_index;
    ewk->master_player = master_player;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    Setup_Letter_C4(ewk);

    if (id == 0) {
        x = -160;
    } else {
        x = 72;
    }

    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + x;
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 172 - (ewk->wu.type * 13);
    ewk->wu.position_z = 68;
    return 0;
}

void Setup_Letter_C4(WORK_Other_CONN* ewk) {
    s16 x;
    s16 ix;
    u8* ptr;

    if (ewk->master_id) {
        ptr = (u8*)Ex_Letter_Data[Menu_Page_Buff][ewk->wu.char_index]
                                 [save_w[1].extra_option.contents[Menu_Page_Buff][ewk->wu.type]];
    } else {
        ptr = (u8*)Ex_Title_Data[Menu_Page_Buff][ewk->wu.char_index];
    }

    ix = 0;
    x = 0;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            x += 8;
            ptr++;
            continue;
        }

        if (*ptr == '/') {
            x += 4;
            ptr++;
            continue;
        }

        if (ewk->master_id) {
            if (save_w[1].extra_option.contents[Menu_Page_Buff][ewk->wu.type] !=
                save_w[0].extra_option.contents[Menu_Page_Buff][ewk->wu.type]) {
                ewk->conn[ix].col = 0x18;
            } else {
                ewk->conn[ix].col = 0;
            }
        }

        ewk->conn[ix].nx = x;
        ewk->conn[ix].ny = 0;
        ewk->conn[ix].chr = *ptr + 0x70A7;
        x += 8;
        ptr++;
        ix++;
    }

    ewk->num_of_conn = ix;
}

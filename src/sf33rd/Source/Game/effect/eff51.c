/**
 * @file eff51.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff51.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/texcash.h"

void Setup_Letter_51(WORK_Other_CONN* ewk);

void effect_51_move(WORK_Other_CONN* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    if (Menu_Cursor_Y[0] == ewk->wu.type) {
        ewk->wu.my_clear_level = 0;
    } else {
        ewk->wu.my_clear_level = 128;
    }

    Setup_Letter_51(ewk);
    sort_push_request3(&ewk->wu);
}

s32 effect_51_init(s16 letter_type, s16 cursor_index, s16 master_player) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.id = 51;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.my_family = 1;
    ewk->wu.char_index = letter_type;
    ewk->wu.type = cursor_index;
    ewk->master_player = master_player;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    Setup_Letter_51(ewk);
    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + 112;
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + 187 - (ewk->wu.type * 18);
    ewk->wu.position_z = 68;
    ewk->wu.position_y -= 3;
    return 0;
}

void Setup_Letter_51(WORK_Other_CONN* ewk) {
    u8* ptr =
        (u8*)Letter_Data_51[Menu_Page_Buff][ewk->wu.char_index][system_dir[1].contents[Menu_Page_Buff][ewk->wu.type]];
    s16 ix = 0;
    s16 x = 0;

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

        if (system_dir[1].contents[Menu_Page_Buff][ewk->wu.type] !=
            Dir_Default_Data.contents[Menu_Page_Buff][ewk->wu.type]) {
            ewk->conn[ix].col = 0x18;
        } else {
            ewk->conn[ix].col = 0;
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

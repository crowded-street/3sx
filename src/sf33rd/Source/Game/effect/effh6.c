/**
 * @file effh6.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effh6.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/staff.h"

s16 roll_rate_t;
s16 roll_rate;

static const s8 code_tab[128] = { -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                                  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                                  -1,  62, 124, 140, 139, 138, 136, 118, 132, 133, 144, 128, 119, 129, 120, 131,
                                  52,  53, 54,  55,  56,  57,  58,  59,  60,  61,  117, 116, 134, 130, 135, 63,
                                  137, 0,  1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
                                  15,  16, 17,  18,  19,  20,  21,  22,  23,  24,  25,  145, -1,  142, 149, 126,
                                  118, 26, 27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
                                  41,  42, 43,  44,  45,  46,  47,  48,  49,  50,  51,  147, 150, 150, 150, -1 };

void effect_H6_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        roll_rate = 1;
        roll_rate_t = 1;

        if (!ewk->wu.dir_step) {
            ewk->wu.old_cgnum = ewk->wu.cg_number = 0;
            ewk->wu.cg_number++;
            ewk->wu.cg_number &= 0x7FFF;
        }

        ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
        ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
        break;

    case 1:
        if (ewk->wu.dir_step) {
            switch (ewk->wu.dir_step) {
            default:
                ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos + roll_rate;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

                if (256 <= ewk->wu.xyz[1].disp.pos) {
                    ewk->wu.routine_no[0]++;
                }

                break;

            case 1:
                break;
            }

            if (Suicide[4]) {
                ewk->wu.routine_no[0] = 2;
            }

            sort_push_request4(&ewk->wu);
            break;
        }

        switch (ewk->wu.routine_no[1]) {
        case 0:
            switch (ewk->wu.routine_no[2]) {
            case 0:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;
                ewk->wu.xyz[0].disp.pos = ewk->wu.xyz[0].disp.pos - roll_rate;
                ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
                if (ewk->wu.routine_no[5] >= ewk->wu.xyz[0].disp.pos) {
                    ewk->wu.xyz[0].disp.pos = ewk->wu.routine_no[5];
                    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
                    ewk->wu.routine_no[2]++;
                    ewk->wu.dir_timer = ewk->wu.dir_timer - 48;
                }

                break;

            case 1:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;

                if (ewk->wu.dir_timer < 1) {
                    ewk->wu.routine_no[2]++;
                }

                break;

            case 2:
                ewk->wu.xyz[0].disp.pos = ewk->wu.xyz[0].disp.pos + roll_rate;
                ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;

                if (512 <= ewk->wu.xyz[0].disp.pos) {
                    ewk->wu.routine_no[0]++;
                }

                break;
            }

            break;

        case 1:
            switch (ewk->wu.routine_no[2]) {
            case 0:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;
                ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos + roll_rate;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

                if (ewk->wu.routine_no[6] <= ewk->wu.xyz[1].disp.pos) {
                    ewk->wu.xyz[1].disp.pos = ewk->wu.routine_no[6];
                    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
                    ewk->wu.routine_no[2]++;
                    ewk->wu.dir_timer = ewk->wu.dir_timer - 30;
                }

                break;

            case 1:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;

                if (ewk->wu.dir_timer < 1) {
                    ewk->wu.routine_no[2]++;
                }

                break;

            case 2:
                ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos + roll_rate;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
                if (256 <= ewk->wu.xyz[1].disp.pos) {
                    ewk->wu.routine_no[0]++;
                }

                break;
            }

            break;

        case 2:
            switch (ewk->wu.routine_no[2]) {
            case 0:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;
                ewk->wu.xyz[0].disp.pos = ewk->wu.xyz[0].disp.pos + roll_rate;
                ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;

                if (ewk->wu.routine_no[5] <= ewk->wu.xyz[0].disp.pos) {
                    ewk->wu.xyz[0].disp.pos = ewk->wu.routine_no[5];
                    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;
                    ewk->wu.routine_no[2]++;
                    ewk->wu.dir_timer = ewk->wu.dir_timer - 48;
                }

                break;

            case 1:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;

                if (ewk->wu.dir_timer < 1) {
                    ewk->wu.routine_no[2]++;
                }

                break;

            case 2:
                ewk->wu.xyz[0].disp.pos = ewk->wu.xyz[0].disp.pos - roll_rate;
                ewk->wu.position_x = ewk->wu.xyz[0].disp.pos & 0xFFFF;

                if (ewk->wu.xyz[0].disp.pos < -127) {
                    ewk->wu.routine_no[0]++;
                }

                break;
            }

            break;

        case 3:
            ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;

            if (ewk->wu.dir_timer < 1) {
                ewk->wu.routine_no[0]++;
            }

            break;

        case 4:
            switch (ewk->wu.routine_no[2]) {
            case 0:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;
                ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos + roll_rate;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

                if (ewk->wu.routine_no[6] <= ewk->wu.xyz[1].disp.pos) {
                    ewk->wu.xyz[1].disp.pos = ewk->wu.routine_no[6];
                    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;
                    ewk->wu.routine_no[2]++;
                    ewk->wu.dir_timer = ewk->wu.dir_timer - 60;
                }

                break;

            case 1:
                ewk->wu.dir_timer = ewk->wu.dir_timer - roll_rate_t;

                if (ewk->wu.dir_timer < 1) {
                    ewk->wu.routine_no[2]++;
                }

                break;

            case 2:
                ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos + roll_rate;
                ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

                if (256 <= ewk->wu.xyz[1].disp.pos) {
                    ewk->wu.routine_no[0]++;
                }

                break;
            }

            break;

        case 6:
            ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos + roll_rate;
            ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

            if (256 <= ewk->wu.xyz[1].disp.pos) {
                ewk->wu.routine_no[0]++;
            }

            break;

        case 7:
            ewk->wu.xyz[1].disp.pos = ewk->wu.xyz[1].disp.pos - roll_rate;
            ewk->wu.position_y = ewk->wu.xyz[1].disp.pos & 0xFFFF;

            if (0 < ewk->wu.xyz[1].disp.pos) {
                break;
            }

            ewk->wu.routine_no[0]++;
            break;
        }

        if (Suicide[4]) {
            ewk->wu.routine_no[0] = 2;
        }

        sort_push_request3(&ewk->wu);
        break;

    case 2:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0]++;
        break;

    case 3:
        ewk->wu.routine_no[0]++;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_H6_init(s16 timer, s8* str, s16 X, s16 Y, s16 Original_Color, s32 /* unused */) {
    WORK_Other_CONN* ewk;
    s16 i;
    s16 x;
    s16 c;
    s8* su = str;

    if ((x = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[x];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 176;
    ewk->wu.type = 0;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_family = 6;
    ewk->wu.my_priority = ewk->wu.position_z = 20;
    ewk->wu.my_mts = 8;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);

    if (Original_Color == 8) {
        Original_Color = 6;
        ewk->wu.my_bright_type = 2;
        ewk->wu.my_bright_level = 15;
    }

    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.dir_timer = timer;

    if (*su == '#') {
        switch (Original_Color) {
        case 0:
            ewk->wu.dir_step = 1;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = Y;
            ewk->wu.my_col_code = 0x202B;
            ewk->wu.cg_number = 0x794D;
            break;

        case 1:
            ewk->wu.dir_step = 2;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = -32;
            ewk->wu.my_col_code = 0x202B;
            ewk->wu.cg_number = 0x794A;
            break;

        case 2:
            ewk->wu.dir_step = 2;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = -32;
            ewk->wu.my_col_code = 0x202B;
            ewk->wu.cg_number = 0x794B;
            break;

        default:
            ewk->wu.dir_step = 2;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = -176;
            ewk->wu.my_col_code = 0x202B;
            ewk->wu.cg_number = 0x7949;
            break;
        }
    } else {
        ewk->wu.dir_step = 0;

        switch (Original_Color) {
        case 0:
            ewk->wu.routine_no[1] = 0;
            ewk->wu.routine_no[5] = X;
            ewk->wu.xyz[0].disp.pos = 384;
            ewk->wu.xyz[1].disp.pos = Y;
            break;

        case 1:
            ewk->wu.routine_no[1] = 1;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = 0;
            break;

        case 2:
            Original_Color = 0;
            ewk->wu.routine_no[1] = 2;
            ewk->wu.routine_no[5] = X;
            ewk->wu.xyz[0].disp.pos = -64;
            ewk->wu.xyz[1].disp.pos = Y;
            break;

        case 3:
            ewk->wu.routine_no[1] = 3;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = Y;
            break;

        case 4:
            Original_Color = 1;
            ewk->wu.routine_no[1] = 4;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = 0;
            break;

        case 5:
            Original_Color = 1;
            ewk->wu.routine_no[1] = 5;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = Y;
            break;

        case 6:
            ewk->wu.routine_no[1] = 6;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = -32;
            break;

        case 7:
            ewk->wu.routine_no[1] = 7;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = 256;
            break;

        case 8:
            ewk->wu.routine_no[1] = 6;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = -32;
            break;

        case 9:
            ewk->wu.dir_step = 2;
            ewk->wu.routine_no[1] = 0;
            ewk->wu.routine_no[6] = Y;
            ewk->wu.xyz[0].disp.pos = X;
            ewk->wu.xyz[1].disp.pos = 0;
            ewk->wu.my_col_code = 0x202B;
            break;
        }

        ewk->wu.my_col_code = 0;

        for (x = 0, i = 0; *su != '\0'; i += 9, su++) {
            if ((c = code_tab[*su]) == -1) {
                continue;
            }

            ewk->conn[x].nx = i;
            ewk->conn[x].ny = 0;

            switch (Original_Color) {
            case 8:
                ewk->conn[x].chr = c + 0x78F0;

                if (*su == '(') {
                    ewk->conn[x].chr = 0x794E;
                }

                if (*su == ')') {
                    ewk->conn[x].chr = 0x794F;
                }

                if (*su == '\'') {
                    ewk->conn[x].chr = 0x7950;
                }

                break;

            default:
                ewk->conn[x].chr = c + 0x78B0;

                if (*su == '{') {
                    ewk->conn[x].chr = 0x7951;
                }

                if (*su == '[') {
                    ewk->conn[x].chr = 0x7941;
                }

                if (*su == ']') {
                    ewk->conn[x].chr = 0x793E;
                }

                if (*su == '^') {
                    ewk->conn[x].chr = 0x7945;
                }

                if (*su == '}') {
                    ewk->conn[x].chr = 0x7946;
                }

                if (*su == '=') {
                    ewk->conn[x].chr = 0x7932;
                }

                if (*su == '&') {
                    ewk->conn[x].chr = 0x7938;
                }

                if (*su == '-') {
                    ewk->conn[x].chr = 0x7931;
                }

                if (*su == '\"') {
                    ewk->conn[x].chr = 0x792B;
                }

                if (*su == '%') {
                    ewk->conn[x].chr = 0x793A;
                }

                if (*su == '|') {
                    ewk->conn[x].chr = 0x7947;
                }

                if (*su == '(') {
                    ewk->conn[x].chr = 0x7934;
                }

                if (*su == ')') {
                    ewk->conn[x].chr = 0x7935;
                }

                if (*su == '*') {
                    ewk->conn[x].chr = 0x7940;
                }

                if (*su == '/') {
                    ewk->conn[x].chr = 0x7933;
                }

                if (*su == '@') {
                    ewk->conn[x].chr = 0x7939;
                }

                if (*su == '<') {
                    ewk->conn[x].chr = 0x7936;
                }

                if (*su == '>') {
                    ewk->conn[x].chr = 0x7937;
                }

                if (*su == '$') {
                    ewk->conn[x].chr = 0x793F;
                }

                if (*su == '+') {
                    ewk->conn[x].chr = 0x7929;
                }

                break;
            }

            ewk->conn[x].col = 0x202B;
            x++;
        }

        ewk->num_of_conn = x;
    }

    return 0;
}

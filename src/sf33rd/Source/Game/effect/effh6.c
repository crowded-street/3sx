/**
 * @file effh6.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effh6.h"
#include "sf33rd/Source/Game/effect/effh6_text.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/screen/staff.h"
#include "sf33rd/Source/Game/system/work_sys.h"

static const s8 code_tab[128] = { -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                                  -1,  -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
                                  -1,  62, 124, 140, 139, 138, 136, 118, 132, 133, 144, 128, 119, 129, 120, 131,
                                  52,  53, 54,  55,  56,  57,  58,  59,  60,  61,  117, 116, 134, 130, 135, 63,
                                  137, 0,  1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
                                  15,  16, 17,  18,  19,  20,  21,  22,  23,  24,  25,  145, -1,  142, 149, 126,
                                   118, 26, 27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
                                   41,  42, 43,  44,  45,  46,  47,  48,  49,  50,  51,  147, 150, 150, 150, -1 };

static void configure_h6_heading(WORK_Other_CONN* ewk, s16 X, s16 Y, s16 Original_Color) {
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
}

static s16 configure_h6_text_path(WORK_Other_CONN* ewk, s16 X, s16 Y, s16 Original_Color) {
    ewk->wu.dir_step = 0;

    if (Original_Color <= 4) {
        return effh6_configure_early_text_path(ewk, X, Y, Original_Color);
    }

    return effh6_configure_late_text_path(ewk, X, Y, Original_Color);
}

static s16 select_h6_alternate_character(s8 character, s16 c) {
    s16 chr = c + 0x78F0;

    if (character == '(') {
        chr = 0x794E;
    }

    if (character == ')') {
        chr = 0x794F;
    }

    if (character == '\'') {
        chr = 0x7950;
    }

    return chr;
}

static s16 select_h6_standard_character(s8 character, s16 c) {
    s16 chr = c + 0x78B0;

    chr = effh6_select_bracket_character(character, chr);
    chr = effh6_select_punctuation_character(character, chr);

    if (character == '/') {
        chr = 0x7933;
    }

    if (character == '@') {
        chr = 0x7939;
    }

    if (character == '<') {
        chr = 0x7936;
    }

    if (character == '>') {
        chr = 0x7937;
    }

    if (character == '$') {
        chr = 0x793F;
    }

    if (character == '+') {
        chr = 0x7929;
    }

    return chr;
}

static void populate_h6_connections(WORK_Other_CONN* ewk, const char* su, s16 Original_Color) {
    s16 i;
    s16 x;
    s16 c;

    ewk->wu.my_col_code = 0;

    for (x = 0, i = 0; *su != '\0'; i += 9, su++) {
        if ((c = code_tab[(int)*su]) == -1) {
            continue;
        }

        ewk->conn[x].nx = i;
        ewk->conn[x].ny = 0;

        switch (Original_Color) {
        case 8:
            ewk->conn[x].chr = select_h6_alternate_character(*su, c);
            break;

        default:
            ewk->conn[x].chr = select_h6_standard_character(*su, c);
            break;
        }

        ewk->conn[x].col = 0x202B;
        x++;
    }

    ewk->num_of_conn = x;
}

s32 effect_H6_init(const H6InitArgs* args) {
    WORK_Other_CONN* ewk;
    s16 x;
    s16 timer = args->timer;
    const char* su = args->str;
    s16 X = args->x;
    s16 Y = args->y;
    s16 Original_Color = args->original_color;

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
        configure_h6_heading(ewk, X, Y, Original_Color);
    } else {
        Original_Color = configure_h6_text_path(ewk, X, Y, Original_Color);
        populate_h6_connections(ewk, su, Original_Color);
    }

    return 0;
}

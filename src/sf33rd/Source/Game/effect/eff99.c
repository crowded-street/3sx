/**
 * @file eff99.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff99.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg.h"

void Setup_Letter_99(WORK_Other_CONN* ewk, s16 letter_index, s16 disp_index);

const s16 Pos_Data_99[5][3] = { { -144, 128, 23 }, { 48, 128, 23 }, { 8, 54, 23 }, { 0, 42, 23 }, { 0, 30, 23 } };

const s8* Letter_Data_99[5][21] = {
    { ";;;;;;;#", ";;;;;;##", ";;;;;###", ";;;;####", ";;;#####", ";;######", ";#######", "########" },
    { "#;;;;;;;", "##;;;;;;", "###;;;;;", "####;;;;", "#####;;;", "######;;", "#######;", "########" },
    { "UNKNOWN", "AMERICA", "JAPAN",  "HONG KONG", "ENGLAND", "RUSSIA", "GERMANY",
      "JAPAN",   "KENYA",   "BRAZIL", "HONG KONG", "AMERICA", "BRAZIL", "MEXICO",
      "JAPAN",   "CHINA",   "JAPAN",  "",          "RUSSIA",  "FRANCE", "RANDOM" },
    { "GILL STAGE",
      "SUBWAY STATION",
      "SUZAKU CASTLE ROOFTOP",
      "SHOPPING DISTRICT",
      "MAIN STREET",
      "MOSQUE",
      "HOME SWEET HOME",
      "A ROAD IN KYOTO",
      "SAVANNA",
      "SANTOS HARBOR",
      "SHOPPING DISTRICT",
      "SUBWAY STATION",
      "SANTOS HARBOR",
      "ORMECA RUINS",
      "KOUSYU STREET",
      "CHINESE RESTAURANT",
      "THE DOJO OF RINDO-KAN",
      "",
      "MOSQUE",
      "CLUB METRO",
      "" },
    { "",
      "ALEX STAGE",
      "RYU STAGE",
      "YUN STAGE",
      "DUDLEY STAGE",
      "NECRO STAGE",
      "HUGO STAGE",
      "IBUKI STAGE",
      "ELENA STAGE",
      "ORO STAGE",
      "YANG STAGE",
      "KEN STAGE",
      "SEAN STAGE",
      "URIEN STAGE",
      "AKUMA STAGE",
      "CHUN-LI STAGE",
      "MAKOTO STAGE",
      "",
      "TWELVE STAGE",
      "REMY STAGE",
      "" }
};

void effect_99_move(WORK_Other_CONN* ewk) {
    if (Menu_Suicide[ewk->master_player]) {
        push_effect_work(&ewk->wu);
        return;
    }

    switch (ewk->wu.type) {
    case 0:
        Setup_Letter_99(ewk, ewk->wu.dir_step, Vital_Handicap[Present_Mode][ewk->master_id]);
        break;

    case 1:
        Setup_Letter_99(ewk, ewk->wu.dir_step, VS_Stage);
        break;
    }

    sort_push_request3(&ewk->wu);
}

s32 effect_99_init(s16 id, s16 type, s16 char_offset, s16 letter_index, s16 pos_index, s16 master_player) {
    WORK_Other_CONN* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other_CONN*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.disp_flag = 1;
    ewk->wu.id = 99;
    ewk->wu.work_id = 16;
    ewk->wu.my_col_code = 0x1AC;
    ewk->wu.my_family = 3;
    ewk->master_id = id;
    ewk->wu.type = type;
    ewk->wu.old_cgnum = char_offset;
    ewk->wu.dir_step = letter_index;
    ewk->master_priority = pos_index;
    ewk->master_player = master_player;
    ewk->wu.my_mts = 13;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Pos_Data_99[pos_index][0];
    ewk->wu.position_y = bg_w.bgw[ewk->wu.my_family - 1].wxy[1].disp.pos + Pos_Data_99[pos_index][1];
    ewk->wu.position_z = Pos_Data_99[pos_index][2];
    return 0;
}

void Setup_Letter_99(WORK_Other_CONN* ewk, s16 letter_index, s16 disp_index) {
    s16 x;
    s16 ix;
    s16 offset_x;
    u8* ptr;
    u8 space;

    if (ewk->wu.old_cgnum == 0x70A7) {
        offset_x = 8;
    } else {
        offset_x = 14;
    }

    ptr = (u8*)Letter_Data_99[letter_index][disp_index];
    ix = 0;
    x = 0;
    space = 0;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            x += offset_x;
            ptr++;
            space++;
            continue;
        }

        ewk->conn[ix].nx = x;
        ewk->conn[ix].ny = 0;
        ewk->conn[ix].col = 0;
        ewk->conn[ix].chr = ewk->wu.old_cgnum + *ptr;
        x += offset_x;
        ptr++;
        ix++;
    }

    ewk->num_of_conn = ix;

    if (ewk->wu.type != 1) {
        return;
    }

    if (ewk->wu.old_cgnum == 0x7047) {
        x = 16;
    } else {
        x = 8;
    }

    x = x * (ix + space) / 2;

    ewk->wu.position_x = bg_w.bgw[ewk->wu.my_family - 1].wxy[0].disp.pos + Pos_Data_99[ewk->master_priority][0] - x;
}

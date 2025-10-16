/**
 * @file effk2.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effk2.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff13.h"
#include "sf33rd/Source/Game/effect/effc2.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sound/se_data.h"

void disp_effK2(WORK* wk, WORK* mk, DADD* hk);
void set_next_next_y(WORK* wk, u8 flag);

const DADD hahen_dummy[1] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

const DADD p_05_lv_02[3] = { { 1, 100, -64, 88, -2, -2, 0, 0, 1, 2, 1, 2, 1, 0, 0 },
                             { 6, 132, -60, 86, 0, 0, 17, 1, 44, 0, 0, 0, 1, 0, 0 },
                             { 6, 134, -62, 92, 0, 0, 17, 1, 45, 0, 0, 0, 1, 0, 0 } };

const DADD p_05_lv_03[3] = { { 1, 102, 0, 88, 42, 0, 255, 0, 2, 3, 0, 2, 1, 0, 0 },
                             { 6, 137, -2, 86, 0, 0, 17, 1, 46, 0, 0, 0, 1, 0, 0 },
                             { 6, 133, 2, 90, 0, 0, 17, 1, 47, 0, 0, 0, 1, 0, 0 } };

const DADD p_05_lv_04[4] = { { 8, 104, -88, 32, 2, 2, 17, 0, 3, 3, 1, 2, 1, 0, 0 },
                             { 8, 106, -96, 64, 2, 2, 17, 0, 4, 3, 1, 2, 1, 0, 0 },
                             { 6, 137, -72, 32, 0, 0, 17, 1, 48, 0, 0, 0, 1, 0, 0 },
                             { 6, 141, -80, 64, 0, 0, 17, 1, 49, 0, 0, 0, 1, 0, 0 } };

const DADD p_05_lv_06[8] = {
    { 1, 115, -32, 64, -2, -2, 17, 0, 5, 3, 1, 2, 1, 0, 0 },  { 1, 115, -32, 64, -2, -2, 17, 0, 6, 3, 1, 2, 1, 0, 0 },
    { 1, 116, -32, 64, -2, -2, 17, 0, 7, 3, 1, 2, 1, 0, 0 },  { 1, 115, -32, 64, -2, -2, 17, 0, 8, 3, 1, 2, 1, 0, 0 },
    { 1, 116, -32, 64, -2, -2, 17, 0, 9, 3, 1, 2, 1, 0, 0 },  { 1, 116, -32, 64, -2, -2, 17, 0, 10, 3, 1, 2, 1, 0, 0 },
    { 1, 115, -32, 64, -2, -2, 17, 0, 11, 3, 1, 2, 1, 0, 0 }, { 1, 116, -32, 64, -2, -2, 17, 0, 12, 3, 1, 2, 1, 0, 0 }
};

const DADD p_05_lv_07[3] = { { 8, 107, -32, 64, 2, 2, 17, 0, 13, 3, 1, 2, 1, 0, 0 },
                             { 6, 134, -32, 68, 0, 0, 17, 1, 50, 0, 0, 0, 1, 0, 0 },
                             { 6, 133, -36, 62, 0, 0, 17, 1, 51, 0, 0, 0, 1, 0, 0 } };

const DADD p_05_lv_08[2] = { { 6, 140, -40, 96, 0, 0, 17, 1, 52, 0, 0, 0, 1, 0, 0 },
                             { 6, 139, -36, 88, 0, 0, 17, 1, 53, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_02[2] = { { 6, 134, 64, 36, 0, 0, 17, 1, 54, 0, 0, 0, 1, 0, 0 },
                             { 6, 135, 56, 28, 0, 0, 17, 1, 55, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_03[3] = { { 1, 101, 144, 96, 0, 0, 0, 0, 14, 2, 0, 2, 1, 0, 0 },
                             { 6, 133, 144, 96, 0, 0, 17, 1, 56, 0, 0, 0, 1, 0, 0 },
                             { 6, 135, 144, 96, 0, 0, 17, 1, 57, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_04[3] = { { 8, 103, 64, 32, 2, 2, 17, 0, 15, 3, 1, 2, 1, 0, 0 },
                             { 6, 138, 48, 32, 0, 0, 17, 1, 58, 0, 0, 0, 1, 0, 0 },
                             { 6, 136, 64, 40, 0, 0, 17, 1, 59, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_05[3] = { { 8, 105, 64, 48, 2, 2, 17, 0, 16, 3, 1, 2, 1, 0, -16 },
                             { 6, 136, 48, 56, 0, 0, 17, 1, 60, 0, 0, 0, 1, 0, 0 },
                             { 6, 139, 56, 48, 0, 0, 17, 1, 61, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_06[2] = { { 6, 136, 160, 112, 0, 0, 17, 1, 62, 0, 0, 0, 1, 0, 0 },
                             { 6, 138, 168, 88, 0, 0, 17, 1, 63, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_07[3] = { { 0, 108, 182, 0, -1, -1, 0, 0, 0, 0, 0, 2, 1, 0, 0 },
                             { 6, 142, 192, 96, 0, 0, 17, 1, 64, 0, 0, 0, 1, 0, 0 },
                             { 6, 143, 176, 80, 0, 0, 17, 1, 65, 0, 0, 0, 1, 0, 0 } };

const DADD p_04_lv_08[3] = { { 2, 109, 193, 24, -1, -1, 0, 0, 17, 0, 0, 2, 1, 0, 1 },
                             { 6, 137, 200, 32, 0, 0, 17, 1, 66, 0, 0, 0, 1, 0, 0 },
                             { 6, 138, 192, 64, 0, 0, 17, 1, 67, 0, 0, 0, 1, 0, 0 } };

const DADD p_03_lv_07[2] = { { 6, 142, 16, 80, 0, 0, 17, 1, 79, 0, 0, 0, 1, 0, 0 },
                             { 6, 143, 48, 88, 0, 0, 17, 1, 80, 0, 0, 0, 1, 0, 0 } };

const DADD p_03_lv_08[2] = { { 6, 140, 32, 80, 0, 0, 17, 1, 81, 0, 0, 0, 1, 0, 0 },
                             { 6, 141, 64, 96, 0, 0, 17, 1, 82, 0, 0, 0, 1, 0, 0 } };

const DADD p_00_lv_F0[8] = {
    { 1, 110, 96, -21, 2, 2, 17, 0, 18, 5, 1, 2, 1, 0, 0 },  { 1, 111, 163, 4, -1, -1, 0, 0, 19, 5, 1, 2, 1, 0, 0 },
    { 1, 112, -35, 10, 1, 1, 0, 0, 20, 5, 1, 2, 1, 0, 0 },   { 6, 141, 64, 96, 0, 0, 17, 1, 68, 0, 0, 0, 1, 0, 0 },
    { 6, 142, 80, 128, 0, 0, 17, 1, 69, 0, 0, 0, 1, 0, 0 },  { 6, 140, 112, 112, 0, 0, 17, 1, 70, 0, 0, 0, 1, 0, 0 },
    { 6, 136, -24, 112, 0, 0, 17, 1, 71, 0, 0, 0, 1, 0, 0 }, { 6, 143, -48, 80, 0, 0, 17, 1, 72, 0, 0, 0, 1, 0, 0 }
};

const DADD p_00_lv_F1[6] = {
    { 0, 117, -64, -4, -1, -1, 0, 0, 21, 0, 0, 0, 2, 0, 0 },  { 0, 117, -40, -8, -1, -1, 0, 0, 22, 0, 0, 0, 2, 0, 0 },
    { 0, 117, -16, -10, -1, -1, 0, 0, 23, 0, 0, 0, 2, 0, 0 }, { 0, 117, 8, -9, -1, -1, 0, 0, 24, 0, 0, 0, 2, 0, 0 },
    { 0, 117, 32, -8, -1, -1, 0, 0, 25, 0, 0, 0, 2, 0, 0 },   { 0, 117, 56, -4, -1, -1, 0, 0, 26, 0, 0, 0, 2, 0, 0 }
};

const DADD p_00_lv_F2[4] = { { 0, 117, 112, 4, -1, -1, 0, 0, 27, 0, 0, 0, 2, 0, 0 },
                             { 0, 117, 132, 6, -1, -1, 0, 0, 28, 0, 0, 0, 2, 0, 0 },
                             { 0, 117, 146, 10, -1, -1, 0, 0, 29, 0, 0, 0, 2, 0, 0 },
                             { 0, 117, 160, 14, -1, -1, 0, 0, 30, 0, 0, 0, 2, 0, 0 } };

const DADD p_00_lv_F3[9] = {
    { 0, 117, -80, 8, -1, -1, 0, 0, 31, 0, 0, 0, 2, 0, 0 }, { 0, 117, -62, 4, -1, -1, 0, 0, 32, 0, 0, 0, 2, 0, 0 },
    { 0, 117, -42, 0, -1, -1, 0, 0, 33, 0, 0, 0, 2, 0, 0 }, { 0, 117, -8, -2, -1, -1, 0, 0, 34, 0, 0, 0, 2, 0, 0 },
    { 0, 117, 96, -2, -1, -1, 0, 0, 35, 0, 0, 0, 2, 0, 0 }, { 0, 117, 124, 0, -1, -1, 0, 0, 36, 0, 0, 0, 2, 0, 0 },
    { 0, 117, 148, 4, -1, -1, 0, 0, 37, 0, 0, 0, 2, 0, 0 }, { 0, 117, 168, 10, -1, -1, 0, 0, 38, 0, 0, 0, 2, 0, 0 },
    { 0, 117, 184, 18, -1, -1, 0, 0, 39, 0, 0, 0, 2, 0, 0 }
};

const DADD p_00_lv_F4[3] = { { 0, 130, 64, -20, 2, 1, 17, 0, 0, 0, 0, 0, 1, 0, 0 },
                             { 0, 130, 32, 24, 3, 1, 17, 0, 0, 0, 0, 0, 1, 0, 0 },
                             { 0, 130, 0, 28, -1, 1, 255, 0, 0, 0, 0, 0, 1, 0, 0 } };

const DADD p_00_lv_02[5] = { { 1, 118, 97, 49, 1, 1, 0, 0, 41, 1, 0, 2, 1, 0, 8 },
                             { 1, 131, 72, 48, 1, 1, 0, 0, 42, 1, 0, 2, 1, 0, 10 },
                             { 2, 119, 72, 88, 1, 4, 0, 0, 40, 0, 0, 2, 1, 0, 20 },
                             { 6, 141, 104, 96, 0, 0, 17, 1, 73, 0, 0, 0, 1, 0, 0 },
                             { 6, 142, 40, 88, 0, 0, 17, 1, 74, 0, 0, 0, 1, 0, 0 } };

const DADD p_00_lv_03[4] = { { 2, 120, 8, 80, 2, 2, 17, 0, 43, 1, 1, 2, 1, 0, -10 },
                             { 5, 125, 0, 0, -1, -1, 0, 0, 0, 0, 0, 2, 1, 0, 0 },
                             { 6, 143, 40, 96, 0, 0, 17, 1, 75, 0, 0, 0, 1, 0, 0 },
                             { 6, 141, -32, 88, 0, 0, 17, 1, 76, 0, 0, 0, 1, 0, 0 } };

const DADD p_00_lv_04[5] = { { 0, 121, 89, 75, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
                             { 0, 122, 39, 75, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
                             { 0, 124, 17, -2, -2, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0 },
                             { 6, 136, -64, 64, 0, 0, 17, 1, 77, 0, 0, 0, 1, 0, 0 },
                             { 6, 137, 16, 56, 0, 0, 17, 1, 78, 0, 0, 0, 1, 0, 0 } };

const DADD p_00_lv_05[2] = { { 0, 126, 0, 0, -2, -2, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
                             { 5, 127, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 1, 0, 0 } };

const DADD p_00_lv_06[2] = { { 4, 128, 64, -55, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
                             { 7, 144, 64, -7, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 } };

const HAHEN hahen_data[8][8] = { { { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 5, 0, p_00_lv_02 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 2, 1, p_04_lv_02 },
                                   { 3, 0, p_05_lv_02 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 4, 0, p_00_lv_03 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 3, 0, p_04_lv_03 },
                                   { 3, 0, p_05_lv_03 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 5, 0, p_00_lv_04 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 3, 0, p_04_lv_04 },
                                   { 4, 0, p_05_lv_04 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 2, 0, p_00_lv_05 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 3, 0, p_04_lv_05 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 2, 0, p_00_lv_06 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 2, 1, p_04_lv_06 },
                                   { 8, 1, p_05_lv_06 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 2, 1, p_03_lv_07 },
                                   { 3, 0, p_04_lv_07 },
                                   { 3, 0, p_05_lv_07 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } },
                                 { { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy },
                                   { 2, 0, p_03_lv_08 },
                                   { 3, 0, p_04_lv_08 },
                                   { 2, 1, p_05_lv_08 },
                                   { 0, 0, hahen_dummy },
                                   { 0, 0, hahen_dummy } } };

const HAHEN ill_hahen_data[5] = {
    { 8, 0, p_00_lv_F0 }, { 6, 0, p_00_lv_F1 }, { 4, 0, p_00_lv_F2 }, { 9, 0, p_00_lv_F3 }, { 3, 0, p_00_lv_F4 }
};

const s16 k2_kidou[83][4] = {
    { 0, 0, 0, 0 },           { 512, 0, 1280, -112 },  { 512, 0, 1536, -112 },   { 1536, 0, 0, -112 },
    { 512, 0, 1536, -112 },   { 256, 0, 1536, -112 },  { 512, 0, 768, -112 },    { 768, 0, 256, -112 },
    { 256, 0, 0, -112 },      { -256, 0, 1280, -112 }, { -512, 0, 1024, -112 },  { -768, 0, 512, -112 },
    { -1024, 0, 0, -112 },    { 768, 0, 1536, -112 },  { -1024, 0, 768, -112 },  { -1792, 0, 0, -112 },
    { -1024, 0, 1280, -112 }, { -64, 0, 512, -112 },   { -2560, 0, 1024, -112 }, { -1792, 0, 1536, -112 },
    { 2048, 0, 1280, -112 },  { 64, 0, 64, 0 },        { 80, 0, 96, 0 },         { 96, 0, 112, 0 },
    { 96, 0, 96, 0 },         { 80, 0, 64, 0 },        { 64, 0, 32, 0 },         { -80, 0, 64, 0 },
    { -80, 0, 80, 0 },        { -64, 0, 96, 0 },       { -64, 0, 96, 0 },        { 320, 0, 32, 0 },
    { 384, 0, 48, 0 },        { 448, 0, 16, 0 },       { 512, 0, 0, 0 },         { -576, 0, -32, 0 },
    { -544, 0, -16, 0 },      { -480, 0, 0, 0 },       { -384, 0, 16, 0 },       { -256, 0, 32, 0 },
    { 512, 0, 2048, -112 },   { -768, 0, 1536, -112 }, { 512, 0, 1280, -112 },   { -768, 0, 1792, -112 },
    { 256, 0, 1024, -96 },    { 128, 0, 1280, -96 },   { 1024, 0, 256, -96 },    { 128, 0, 1024, -96 },
    { 768, 0, 256, -96 },     { 128, 0, 2048, -96 },   { 256, 0, 1536, -96 },    { -128, 0, 1024, -96 },
    { 768, 0, 1792, -96 },    { 1280, 0, 768, -96 },   { -1792, 0, 512, -96 },   { -1024, 0, 0, -96 },
    { -128, 0, 2048, -96 },   { -1024, 0, 512, -96 },  { -2048, 0, 0, -96 },     { -256, 0, 1024, -96 },
    { -512, 0, 2048, -96 },   { -1024, 0, 1536, -96 }, { -512, 0, 1536, -96 },   { -768, 0, 1024, -96 },
    { -1024, 0, 768, -96 },   { -768, 0, 256, -96 },   { -1024, 0, 768, -96 },   { -768, 0, 256, -96 },
    { -512, 0, 2048, -96 },   { -2048, 0, 768, -96 },  { -1280, 0, 1280, -96 },  { 1536, 0, 1024, -96 },
    { 1024, 0, 1792, -96 },   { 768, 0, 1536, -96 },   { -512, 0, 768, -96 },    { 1280, 0, 768, -96 },
    { 256, 0, 2048, -96 },    { 256, 0, 512, -96 },    { -768, 0, 0, -96 },      { 1024, 0, 512, -96 },
    { 512, 0, 1280, -96 },    { 768, 0, 768, -96 },    { 128, 0, 1536, -96 }
};

void (*const effK2_main_process[9])();

void effect_K2_move(WORK_Other* ewk) {
    DADD* hahen = (DADD*)ewk->wu.target_adrs;
    WORK* mwk = (WORK*)ewk->my_master;

    if (ewk->wu.dir_old == 0 && (mwk->id != ewk->master_work_id || mwk->dir_old != 0)) {
        ewk->wu.dir_old = 1;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.routine_no[1] = hahen->rno;
        ewk->wu.disp_flag = hahen->init_dsp;
        ewk->wu.blink_timing = ewk->master_id;
        ewk->wu.xyz[0].disp.pos += hahen->hx;
        ewk->wu.xyz[1].disp.pos += hahen->hy;
        ewk->wu.position_z = 24;
        ewk->wu.kage_hy = 0;

        if ((ewk->wu.next_y = hahen->gr1st) == 0) {
            ewk->wu.next_y = (random_16() & 7) + 4;

            if (hahen->kage_char) {
                ewk->wu.next_y = -ewk->wu.next_y;
            }
            if ((ewk->wu.xyz[1].disp.pos) < 0) {
                ewk->wu.next_y += ewk->wu.xyz[1].disp.pos;
            }
        }

        setup_move_data_easy(&ewk->wu, k2_kidou[hahen->ispix], 1, 0);
        set_char_move_init(&ewk->wu, 0, (hahen->cix));

        if (hahen->cix == 0x78) {
            setup_demojump((PLW*)ewk->wu.hit_adrs, 1);
        }

        disp_effK2(&ewk->wu, mwk, hahen);
        break;

    case 1:
        if (ewk->wu.dead_f == 1 || Suicide[0] != 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            break;
        }

        if (EXE_flag == 0 && Game_pause == 0) {
            effK2_main_process[ewk->wu.routine_no[1]](ewk, hahen);
        }

        disp_effK2(&ewk->wu, mwk, hahen);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void disp_effK2(WORK* wk, WORK* mk, DADD* hk) {
    s16 flag;
    s16 hz;

    wk->position_x = wk->xyz[0].disp.pos;
    wk->position_y = wk->xyz[1].disp.pos;

    if (wk->dir_old == 0) {
        flag = hk->pri_use;
        hz = hk->hzd;

        if (wk->mvxy.a[1].real.h >= 0) {
            flag >>= 4;
            hz = hk->hzr;
        }

        flag &= 0xF;

        switch (flag) {
        case 1:
            wk->position_z = hz + 24;
            break;

        case 15:
            wk->position_z = hz + 68;
            break;

        default:
            wk->position_z = hz + mk->position_z;
            break;
        }
    }

    sort_push_request(wk);
}

void effK2_parts_move_type_0(WORK_Other* ewk, DADD*) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[1] = 1;
            ewk->wu.routine_no[2] = 10;
            reset_mvxy_data(&ewk->wu);
        } else if (screen_x_range_check(&ewk->wu)) {
            ewk->wu.routine_no[2] = 20;
            ewk->wu.routine_no[1] = 1;
        }

        break;
    }
}

void effK2_parts_move_type_1(WORK_Other* ewk, DADD* hahen) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        switch (ewk->wu.dm_attlv) {
        case 2:
        case 3:
            char_move(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);
            break;
        }

        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[2] = 10;
            break;
        }

        if (ewk->wu.mvxy.a[1].sp <= 0) {
            if (ewk->wu.xyz[1].disp.pos <= ewk->wu.next_y) {
                ewk->wu.xyz[1].disp.pos = ewk->wu.next_y;

                if (++ewk->wu.kage_hy > hahen->bau) {
                    ewk->wu.routine_no[2] = 10;
                } else {
                    ewk->wu.routine_no[2] = 1;
                }
            }

            if (screen_x_range_check(&ewk->wu)) {
                ewk->wu.routine_no[2] = 20;
            }
        }

        break;

    case 1:
        ewk->wu.routine_no[2] = 0;
        ewk->wu.mvxy.a[0].sp /= 2;
        ewk->wu.mvxy.d[0].sp /= 2;
        ewk->wu.mvxy.a[1].sp = -ewk->wu.mvxy.a[1].sp;
        ewk->wu.mvxy.a[1].sp /= 3;
        set_next_next_y(&ewk->wu, hahen->kage_char);
        break;

    case 10:
        switch (hahen->doa) {
        case 0:
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[2] = 20;
            break;

        case 1:
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[2] = 15;
            break;

        default:
            ewk->wu.disp_flag = 2;
            ewk->wu.kage_prio = 20;
            ewk->wu.routine_no[2] = 11;
            break;
        }

        break;

    case 11:
        if (--ewk->wu.kage_prio > 0) {
            break;
        }

        /* fallthrough */

    case 20:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 2;
        break;
    }
}

void effK2_parts_move_type_2(WORK_Other* ewk, DADD* /* unused */) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.xyz[1].disp.pos <= ewk->wu.next_y) {
            ewk->wu.xyz[1].disp.pos = ewk->wu.next_y;
            ewk->wu.routine_no[1] = 0;
            char_move_cmja(&ewk->wu);
            reset_mvxy_data(&ewk->wu);
        } else if (screen_x_range_check(&ewk->wu) != 0) {
            ewk->wu.routine_no[2] = 20;
            ewk->wu.routine_no[1] = 1;
        }

        break;
    }
}

void effK2_parts_move_type_3(WORK_Other* ewk, DADD* hahen) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.mvxy.a[1].sp > 0) {
            break;
        }

        if (ewk->wu.xyz[1].disp.pos <= ewk->wu.next_y) {
            ewk->wu.xyz[1].disp.pos = ewk->wu.next_y;

            if (++ewk->wu.kage_hy > hahen->bau) {
                ewk->wu.routine_no[2] = 10;
            } else {
                ewk->wu.routine_no[2] = 1;
            }

            ewk->wu.routine_no[1] = 1;
            char_move_cmja(&ewk->wu);
            reset_mvxy_data(&ewk->wu);
        } else if (screen_x_range_check(&ewk->wu)) {
            ewk->wu.routine_no[1] = 1;
            ewk->wu.routine_no[2] = 20;
        }

        break;
    }
}

void effK2_parts_move_type_4(WORK_Other* ewk, DADD* arg1) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        char_move(&ewk->wu);

        if (ewk->wu.dir_old) {
            ewk->wu.routine_no[2] = 1;
            ewk->wu.kage_prio = 60;
        }

        break;

    case 1:
        char_move(&ewk->wu);

        if (--ewk->wu.kage_prio <= 0) {
            char_move_cmja(&ewk->wu);
            ewk->wu.routine_no[1] = 0;
            ewk->wu.routine_no[2] = 0;
        }

        break;
    }
}

void effK2_parts_move_type_5(WORK_Other* ewk, DADD* arg1) {
    char_move(&ewk->wu);

    if (ewk->wu.dir_old) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[1] = 1;
        ewk->wu.routine_no[2] = 20;
    }
}

void effK2_parts_move_type_6(WORK_Other* ewk, DADD* arg1) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.kage_prio = (random_16() & 7) + 28;
        ewk->wu.kage_hy = ewk->wu.kage_prio / 2;
        ewk->wu.routine_no[2]++;
        /* fallthrough */

    case 1:
        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.kage_hy) {
            ewk->wu.kage_hy--;
        } else {
            ewk->wu.disp_flag = 2;
        }

        if (--ewk->wu.kage_prio < 0) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0] = 2;
        }

        break;
    }
}

void effK2_parts_move_type_7(WORK_Other* ewk, DADD* arg1) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        char_move(&ewk->wu);

        if (ewk->wu.dir_old) {
            ewk->wu.routine_no[1] = 1;
            ewk->wu.routine_no[2] = 10;
        }

        break;
    }
}

void effK2_parts_move_type_8(WORK_Other* ewk, DADD* hahen) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        switch (ewk->wu.dm_attlv) {
        case 2:
        case 3:
            char_move(&ewk->wu);
            /* fallthrough */

        case 1:
            char_move(&ewk->wu);
            break;
        }

        char_move(&ewk->wu);
        add_mvxy_speed(&ewk->wu);
        cal_mvxy_speed(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.routine_no[2] = 10;
            break;
        }

        if (ewk->wu.mvxy.a[1].sp > 0) {
            break;
        }

        if (ewk->wu.xyz[1].disp.pos <= ewk->wu.next_y) {
            ewk->wu.xyz[1].disp.pos = ewk->wu.next_y;

            if (++ewk->wu.kage_hy > hahen->bau) {
                ewk->wu.routine_no[2] = 10;
            } else {
                ewk->wu.routine_no[2] = 1;
            }

            sound_effect_request[0x3E4](ewk, 0x3E4);
        }

        if (screen_x_range_check(&ewk->wu)) {
            ewk->wu.routine_no[2] = 20;
        }

        break;

    case 1:
        ewk->wu.routine_no[2] = 0;
        ewk->wu.mvxy.a[0].sp /= 2;
        ewk->wu.mvxy.d[0].sp /= 2;
        ewk->wu.mvxy.a[1].sp = -ewk->wu.mvxy.a[1].sp;
        ewk->wu.mvxy.a[1].sp /= 3;
        set_next_next_y(&ewk->wu, hahen->kage_char);
        break;

    case 10:
        switch (hahen->doa) {
        case 0:
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[2] = 20;
            break;

        case 1:
            ewk->wu.disp_flag = 1;
            ewk->wu.routine_no[2] = 15;
            break;

        default:
            ewk->wu.disp_flag = 2;
            ewk->wu.kage_prio = 20;
            ewk->wu.routine_no[2] = 11;
            break;
        }

        break;

    case 11:
        if (--ewk->wu.kage_prio > 0) {
            break;
        }

        /* fallthrough */

    case 20:
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0] = 2;
        break;
    }
}

void set_next_next_y(WORK* wk, u8 flag) {
    if (flag) {
        wk->next_y -= (random_16() & 4) + 2;
    } else {
        wk->next_y += (random_16() & 4) + 2;
    }
}

s32 effect_K2_init(WORK_Other* wk, u32* dad) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(1)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 202;
    ewk->wu.work_id = 16;
    ewk->wu.type = wk->wu.type;
    ewk->wu.dm_rl = wk->wu.dm_rl;
    ewk->wu.dm_dir = wk->wu.dm_dir;
    ewk->wu.dm_attlv = wk->wu.dm_attlv;
    ewk->wu.my_family = wk->wu.my_family;
    ewk->wu.cgromtype = wk->wu.cgromtype;
    ewk->wu.my_col_mode = wk->wu.my_col_mode;
    ewk->wu.my_col_code = wk->wu.my_col_code;
    ewk->wu.my_priority = wk->wu.position_z;
    ewk->wu.my_mts = 5;
    ewk->master_id = wk->master_id;
    ewk->master_work_id = wk->wu.id;
    ewk->wu.dir_old = 0;
    ewk->my_master = (u32*)wk;
    ewk->wu.target_adrs = dad;
    ewk->wu.hit_adrs = wk->wu.target_adrs;
    ewk->wu.xyz[0].disp.pos = 448;
    ewk->wu.xyz[1].disp.pos = 0;
    *ewk->wu.char_table = _bonus_char_table;
    effect_K2_move(ewk);
    return 0;
}

void setup_effK2(WORK* wk) {
    const DADD* dhead;
    s16 i;
    s16 num;

    if (!(num = hahen_data[wk->vital_old][wk->type].kosuu)) {
        return;
    }

    dhead = hahen_data[wk->vital_old][wk->type].dadd;

    for (i = 0; i < num; i++) {
        effect_K2_init((WORK_Other*)wk, (u32*)&dhead[i]);
    }
}

void setup_effK2_sync_bomb(WORK* wk) {
    const DADD* dhead;
    s16 i;
    s16 j;
    s16 num;

    for (j = wk->vital_old + 1; j < 8; j++) {
        if (!(num = hahen_data[j][wk->type].kosuu) || hahen_data[j][wk->type].bomb != 0) {
            continue;
        }

        dhead = hahen_data[j][wk->type].dadd;

        for (i = 0; i < num; i++) {
            if (dhead[i].bomb == 0) {
                effect_K2_init((WORK_Other*)wk, (u32*)&dhead[i]);
            }
        }
    }
}

void illegal_setup_effK2(WORK* wk, s16 ix) {
    const DADD* dhead;
    s16 i;
    s16 num = ill_hahen_data[ix].kosuu;

    dhead = ill_hahen_data[ix].dadd;

    for (i = 0; i < num; i++) {
        effect_K2_init((WORK_Other*)wk, (u32*)&dhead[i]);
    }
}

void (*const effK2_main_process[9])() = { effK2_parts_move_type_0, effK2_parts_move_type_1, effK2_parts_move_type_2,
                                          effK2_parts_move_type_3, effK2_parts_move_type_4, effK2_parts_move_type_5,
                                          effK2_parts_move_type_6, effK2_parts_move_type_7, effK2_parts_move_type_8 };

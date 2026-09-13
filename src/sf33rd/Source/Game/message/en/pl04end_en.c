/**
 * @file pl04end_en.c
 * Dudley Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl04end_en.h"
#include "common.h"

static char* msgEndPL04_00[1] = { "" };
static char* msgEndPL04_01[2] = {
    "   The subtle beauty of St. Swithun...          ",
    "   The aroma of Abraham Darby...                ",
};
static char* msgEndPL04_02[2] = {
    "   The lightness of Dainty Bess...              ",
    "   The style of Uncle Walter...                 ",
};
static char* msgEndPL04_03[2] = {
    "   Dad... You left this majestic                ",
    "   rose garden for me...                        ",
};
static char* msgEndPL04_04[2] = {
    "   The roses' ever-changing                     ",
    "   beauty still heals my heart.                 ",
};
static char* msgEndPL04_05[1] = { "   Sir Dudley... It's about time...             " };
static char* msgEndPL04_06[2] = {
    "   Oh, that's right. Today is the day           ",
    "   of that important royal match...             ",
};
static char* msgEndPL04_07[1] = { "   When does the match start?                   " };
static char* msgEndPL04_08[1] = { "   It starts at 8:00 PM, sharp.                 " };
static char* msgEndPL04_09[2] = {
    "   What...?!                                    ",
    "   We only have five hours left?!               ",
};
static char* msgEndPL04_10[2] = {
    "   We've got to get back to the                 ",
    "   mansion immediately! Run!!                   ",
};

static char** msgEndAdrPL04[11] = {
    msgEndPL04_00, msgEndPL04_01, msgEndPL04_02, msgEndPL04_03, msgEndPL04_04, msgEndPL04_05,
    msgEndPL04_06, msgEndPL04_07, msgEndPL04_08, msgEndPL04_09, msgEndPL04_10,
};

static s8 msgEndCtrPL04[11] = { 1, 2, 2, 2, 2, 1, 2, 1, 1, 2, 2 };

MessageTable pl04end_usa_tbl = { msgEndAdrPL04, msgEndCtrPL04 };

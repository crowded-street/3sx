/**
 * @file pl00end_en.c
 * Gill Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl00end_en.h"
#include "common.h"

static char* msgEndPL00_00[1] = { "" };
static char* msgEndPL00_01[1] = { "   You were wrong...                            " };
static char* msgEndPL00_02[2] = {
    "   People can't control hatred or love          ",
    "   within their minds...                        ",
};
static char* msgEndPL00_03[2] = {
    "   Every soul feeds and survives upon           ",
    "   feelings...                                  ",
};
static char* msgEndPL00_04[2] = {
    "   If you try and ignore certain emotions,      ",
    "   people may be damaged or destroyed!          ",
};
static char* msgEndPL00_05[2] = {
    "   Open your mind and accept the truth that     ",
    "   one feeling needs the other...               ",
};
static char* msgEndPL00_06[1] = { "   Let me lead you...                           " };
static char* msgEndPL00_07[1] = { "   \"...Huh? What is going on...?\"               " };
static char* msgEndPL00_08[2] = {
    "   \"Something has been taken away... And        ",
    "   yet, something new has be added...\"          ",
};
static char* msgEndPL00_09[2] = {
    "   Paradise... Everyone dreams of it and        ",
    "   searches their entire for it...              ",
};
static char* msgEndPL00_10[1] = { "   But paradise doesn't exist.                  " };
static char* msgEndPL00_11[1] = { "   People are blind to the truth...             " };
static char* msgEndPL00_12[1] = { "   2000 years was too long.                     " };
static char* msgEndPL00_13[2] = {
    "   I'm going to complete the mission which      ",
    "   my predecessor tried to accomplish.          ",
};
static char* msgEndPL00_14[1] = { "   The dark future is now over...               " };
static char* msgEndPL00_15[1] = { "   Now... The road has been shown.              " };
static char* msgEndPL00_16[2] = {
    "   This road will lead the pitiful people       ",
    "   of this world to evolution...                ",
};
static char* msgEndPL00_17[1] = { "   Yes... This is the way to the paradise!      " };

static char** msgEndAdrPL00[18] = {
    msgEndPL00_00, msgEndPL00_01, msgEndPL00_02, msgEndPL00_03, msgEndPL00_04, msgEndPL00_05,
    msgEndPL00_06, msgEndPL00_07, msgEndPL00_08, msgEndPL00_09, msgEndPL00_10, msgEndPL00_11,
    msgEndPL00_12, msgEndPL00_13, msgEndPL00_14, msgEndPL00_15, msgEndPL00_16, msgEndPL00_17,
};

static s8 msgEndCtrPL00[18] = { 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1 };

MessageTable pl00end_usa_tbl = { msgEndAdrPL00, msgEndCtrPL00 };

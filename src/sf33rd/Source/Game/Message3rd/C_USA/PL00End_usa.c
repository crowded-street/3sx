#include "sf33rd/Source/Game/Message3rd/C_USA/PL00End_usa.h"
#include "common.h"

static s8* msgEndPL00_00[1] = { "" };
static s8* msgEndPL00_01[1] = { "   You were wrong...                            " };
static s8* msgEndPL00_02[2] = {
    "   People can't control hatred or love          ",
    "   within their minds...                        ",
};
static s8* msgEndPL00_03[2] = {
    "   Every soul feeds and survives upon           ",
    "   feelings...                                  ",
};
static s8* msgEndPL00_04[2] = {
    "   If you try and ignore certain emotions,      ",
    "   people may be damaged or destroyed!          ",
};
static s8* msgEndPL00_05[2] = {
    "   Open your mind and accept the truth that     ",
    "   one feeling needs the other...               ",
};
static s8* msgEndPL00_06[1] = { "   Let me lead you...                           " };
static s8* msgEndPL00_07[1] = { "   \"...Huh? What is going on...?\"               " };
static s8* msgEndPL00_08[2] = {
    "   \"Something has been taken away... And        ",
    "   yet, something new has be added...\"          ",
};
static s8* msgEndPL00_09[2] = {
    "   Paradise... Everyone dreams of it and        ",
    "   searches their entire for it...              ",
};
static s8* msgEndPL00_10[1] = { "   But paradise doesn't exist.                  " };
static s8* msgEndPL00_11[1] = { "   People are blind to the truth...             " };
static s8* msgEndPL00_12[1] = { "   2000 years was too long.                     " };
static s8* msgEndPL00_13[2] = {
    "   I'm going to complete the mission which      ",
    "   my predecessor tried to accomplish.          ",
};
static s8* msgEndPL00_14[1] = { "   The dark future is now over...               " };
static s8* msgEndPL00_15[1] = { "   Now... The road has been shown.              " };
static s8* msgEndPL00_16[2] = {
    "   This road will lead the pitiful people       ",
    "   of this world to evolution...                ",
};
static s8* msgEndPL00_17[1] = { "   Yes... This is the way to the paradise!      " };

static s8** msgEndAdrPL00[18] = {
    msgEndPL00_00, msgEndPL00_01, msgEndPL00_02, msgEndPL00_03, msgEndPL00_04, msgEndPL00_05,
    msgEndPL00_06, msgEndPL00_07, msgEndPL00_08, msgEndPL00_09, msgEndPL00_10, msgEndPL00_11,
    msgEndPL00_12, msgEndPL00_13, msgEndPL00_14, msgEndPL00_15, msgEndPL00_16, msgEndPL00_17,
};

static s8 msgEndCtrPL00[18] = { 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1 };

MessageTable pl00end_usa_tbl = { msgEndAdrPL00, msgEndCtrPL00 };

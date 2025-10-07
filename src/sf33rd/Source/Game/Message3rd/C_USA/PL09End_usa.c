#include "sf33rd/Source/Game/Message3rd/C_USA/PL09End_usa.h"
#include "common.h"

static s8* msgEndPL09_00[1] = { "" };
static s8* msgEndPL09_01[2] = {
    "   Hee hee hee... It's been 50 years            ",
    "   since I've trained this seriously!           ",
};
static s8* msgEndPL09_02[2] = {
    "   Whenever I see that kid,                     ",
    "   my blood begins to boil again...             ",
};
static s8* msgEndPL09_03[2] = {
    "   He's pretty unique... I guess I              ",
    "   won't be bored for the time being!           ",
};
static s8* msgEndPL09_04[1] = { "   Hmm??                                        " };
static s8* msgEndPL09_05[2] = {
    "   I thought I heard that                       ",
    "   strange old man's voice...?                  ",
};
static s8* msgEndPL09_06[2] = {
    "   Maybe it was just                            ",
    "   my imagination...                            ",
};
static s8* msgEndPL09_07[2] = {
    "   He must be training so                       ",
    "   hard, this very moment!                      ",
};
static s8* msgEndPL09_08[2] = {
    "   I hope that he becomes so strong,            ",
    "   that I'll have to use both my arms!          ",
};
static s8* msgEndPL09_09[2] = {
    "   But... It would take at least 15             ",
    "   years before that could happen...            ",
};
static s8* msgEndPL09_10[1] = { "   Maybe I should take a rest now...            " };
static s8* msgEndPL09_11[1] = { "   Whew!                                        " };

static s8** msgEndAdrPL09[12] = {
    msgEndPL09_00, msgEndPL09_01, msgEndPL09_02, msgEndPL09_03, msgEndPL09_04, msgEndPL09_05,
    msgEndPL09_06, msgEndPL09_07, msgEndPL09_08, msgEndPL09_09, msgEndPL09_10, msgEndPL09_11,
};

static s8 msgEndCtrPL09[12] = { 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 1 };

MessageTable pl09end_usa_tbl = { msgEndAdrPL09, msgEndCtrPL09 };

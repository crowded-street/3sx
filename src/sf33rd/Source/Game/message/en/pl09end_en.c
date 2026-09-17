/**
 * @file pl09end_en.c
 * Oro Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl09end_en.h"
#include "common.h"

static char* msgEndPL09_00[1] = { "" };
static char* msgEndPL09_01[2] = {
    "   Hee hee hee... It's been 50 years            ",
    "   since I've trained this seriously!           ",
};
static char* msgEndPL09_02[2] = {
    "   Whenever I see that kid,                     ",
    "   my blood begins to boil again...             ",
};
static char* msgEndPL09_03[2] = {
    "   He's pretty unique... I guess I              ",
    "   won't be bored for the time being!           ",
};
static char* msgEndPL09_04[1] = { "   Hmm??                                        " };
static char* msgEndPL09_05[2] = {
    "   I thought I heard that                       ",
    "   strange old man's voice...?                  ",
};
static char* msgEndPL09_06[2] = {
    "   Maybe it was just                            ",
    "   my imagination...                            ",
};
static char* msgEndPL09_07[2] = {
    "   He must be training so                       ",
    "   hard, this very moment!                      ",
};
static char* msgEndPL09_08[2] = {
    "   I hope that he becomes so strong,            ",
    "   that I'll have to use both my arms!          ",
};
static char* msgEndPL09_09[2] = {
    "   But... It would take at least 15             ",
    "   years before that could happen...            ",
};
static char* msgEndPL09_10[1] = { "   Maybe I should take a rest now...            " };
static char* msgEndPL09_11[1] = { "   Whew!                                        " };

static char** msgEndAdrPL09[12] = {
    msgEndPL09_00, msgEndPL09_01, msgEndPL09_02, msgEndPL09_03, msgEndPL09_04, msgEndPL09_05,
    msgEndPL09_06, msgEndPL09_07, msgEndPL09_08, msgEndPL09_09, msgEndPL09_10, msgEndPL09_11,
};

static s8 msgEndCtrPL09[12] = { 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 1 };

MessageTable pl09end_usa_tbl = { msgEndAdrPL09, msgEndCtrPL09 };

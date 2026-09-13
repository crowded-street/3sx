/**
 * @file pl01end_en.c
 * Alex Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl01end_en.h"
#include "common.h"

static char* msgEndPL01_00[1] = { "" };
static char* msgEndPL01_01[2] = {
    "   Ugh! I couldn't counter even                 ",
    "   one of his blows!                            ",
};
static char* msgEndPL01_02[2] = {
    "   (His reputation does not do him              ",
    "   justice! He is much stronger!)               ",
};
static char* msgEndPL01_03[2] = {
    "   \"I'm sorry that I can't go camping.          ",
    "   Please give my apologies to Pat.\"            ",
};
static char* msgEndPL01_04[2] = {
    "   \"...I know it sounds crazy, but              ",
    "   I just can't control myself.\"                ",
};
static char* msgEndPL01_05[2] = {
    "   \"To see him again and to fight!              ",
    "   That's the only thing on my mind!\"           ",
};
static char* msgEndPL01_06[2] = {
    "   \"I'll be on the road for a while...          ",
    "   Tom, I'll catch up with you later!\"          ",
};
static char* msgEndPL01_07[1] = { "   (Now, his time has come...)                  " };
static char* msgEndPL01_08[2] = {
    "   (I guess he thinks that I'm no               ",
    "   longer a match for him...)                   ",
};
static char* msgEndPL01_09[2] = {
    "   (Alex, when you return, I'll see for         ",
    "   myself how strong you've become!)            ",
};
static char* msgEndPL01_10[1] = { "   Dad! Hey, where's Alex, anyway?              " };
static char* msgEndPL01_11[2] = {
    "   We need him to set up our tent               ",
    "   by the lake, you know.                       ",
};
static char* msgEndPL01_12[1] = { "   Hey daddy, are you listening?!               " };
static char* msgEndPL01_13[2] = {
    "   What's the matter?!                          ",
    "   Is that all you've got?                      ",
};
static char* msgEndPL01_14[2] = {
    "   Ha ha... No way!                             ",
    "   The show has just begun!                     ",
};
static char* msgEndPL01_15[1] = { "   Get ready!                                   " };

static char** msgEndAdrPL01[16] = {
    msgEndPL01_00, msgEndPL01_01, msgEndPL01_02, msgEndPL01_03, msgEndPL01_04, msgEndPL01_05,
    msgEndPL01_06, msgEndPL01_07, msgEndPL01_08, msgEndPL01_09, msgEndPL01_10, msgEndPL01_11,
    msgEndPL01_12, msgEndPL01_13, msgEndPL01_14, msgEndPL01_15,
};

static s8 msgEndCtrPL01[16] = { 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1 };

MessageTable pl01end_usa_tbl = { msgEndAdrPL01, msgEndCtrPL01 };

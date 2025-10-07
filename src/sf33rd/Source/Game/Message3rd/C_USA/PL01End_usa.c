#include "sf33rd/Source/Game/Message3rd/C_USA/PL01End_usa.h"
#include "common.h"

static s8* msgEndPL01_00[1] = { "" };
static s8* msgEndPL01_01[2] = {
    "   Ugh! I couldn't counter even                 ",
    "   one of his blows!                            ",
};
static s8* msgEndPL01_02[2] = {
    "   (His reputation does not do him              ",
    "   justice! He is much stronger!)               ",
};
static s8* msgEndPL01_03[2] = {
    "   \"I'm sorry that I can't go camping.          ",
    "   Please give my apologies to Pat.\"            ",
};
static s8* msgEndPL01_04[2] = {
    "   \"...I know it sounds crazy, but              ",
    "   I just can't control myself.\"                ",
};
static s8* msgEndPL01_05[2] = {
    "   \"To see him again and to fight!              ",
    "   That's the only thing on my mind!\"           ",
};
static s8* msgEndPL01_06[2] = {
    "   \"I'll be on the road for a while...          ",
    "   Tom, I'll catch up with you later!\"          ",
};
static s8* msgEndPL01_07[1] = { "   (Now, his time has come...)                  " };
static s8* msgEndPL01_08[2] = {
    "   (I guess he thinks that I'm no               ",
    "   longer a match for him...)                   ",
};
static s8* msgEndPL01_09[2] = {
    "   (Alex, when you return, I'll see for         ",
    "   myself how strong you've become!)            ",
};
static s8* msgEndPL01_10[1] = { "   Dad! Hey, where's Alex, anyway?              " };
static s8* msgEndPL01_11[2] = {
    "   We need him to set up our tent               ",
    "   by the lake, you know.                       ",
};
static s8* msgEndPL01_12[1] = { "   Hey daddy, are you listening?!               " };
static s8* msgEndPL01_13[2] = {
    "   What's the matter?!                          ",
    "   Is that all you've got?                      ",
};
static s8* msgEndPL01_14[2] = {
    "   Ha ha... No way!                             ",
    "   The show has just begun!                     ",
};
static s8* msgEndPL01_15[1] = { "   Get ready!                                   " };

static s8** msgEndAdrPL01[16] = {
    msgEndPL01_00, msgEndPL01_01, msgEndPL01_02, msgEndPL01_03, msgEndPL01_04, msgEndPL01_05,
    msgEndPL01_06, msgEndPL01_07, msgEndPL01_08, msgEndPL01_09, msgEndPL01_10, msgEndPL01_11,
    msgEndPL01_12, msgEndPL01_13, msgEndPL01_14, msgEndPL01_15,
};

static s8 msgEndCtrPL01[16] = { 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1 };

MessageTable pl01end_usa_tbl = { msgEndAdrPL01, msgEndCtrPL01 };

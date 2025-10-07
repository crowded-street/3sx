#include "sf33rd/Source/Game/Message3rd/C_USA/PL13End_usa.h"
#include "common.h"

static s8* msgEndPL13_00[1] = { "" };
static s8* msgEndPL13_01[2] = {
    "   So those \"brains\" they've collected          ",
    "   for 2000 years are stored here...            ",
};
static s8* msgEndPL13_02[2] = {
    "   I don't know the value of these,             ",
    "   but they are ugly and stupid.                ",
};
static s8* msgEndPL13_03[2] = {
    "   ...Just a mass of junk with no               ",
    "   ability to take positive action!             ",
};
static s8* msgEndPL13_04[2] = {
    "   Brother! What are you looking at             ",
    "   now? Can you see the world?                  ",
};
static s8* msgEndPL13_05[2] = {
    "   Are you watching how your                    ",
    "   ideal world is being built?                  ",
};
static s8* msgEndPL13_06[2] = {
    "   Ha... ha ha ha ha ha!                        ",
    "   Ha ha ha ha ha ha ha ha!!                    ",
};
static s8* msgEndPL13_07[2] = {
    "   Checking ID... Verified as                   ",
    "   the legitimate successor...                  ",
};
static s8* msgEndPL13_08[2] = {
    "   Enter self-destruction code:                 ",
    "   A22ST8996HYXW...                             ",
};
static s8* msgEndPL13_09[2] = {
    "   There is no mission or legacy                ",
    "   left for me to succeed!                      ",
};
static s8* msgEndPL13_10[2] = {
    "   Now, my own story will                       ",
    "   unfold from this moment on!                  ",
};
static s8* msgEndPL13_11[2] = {
    "   I hereby define this year as the             ",
    "   first year of \"A.U.\"(After Urien)!!          ",
};

static s8** msgEndAdrPL13[12] = {
    msgEndPL13_00, msgEndPL13_01, msgEndPL13_02, msgEndPL13_03, msgEndPL13_04, msgEndPL13_05,
    msgEndPL13_06, msgEndPL13_07, msgEndPL13_08, msgEndPL13_09, msgEndPL13_10, msgEndPL13_11,
};

static s8 msgEndCtrPL13[12] = { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl13end_usa_tbl = { msgEndAdrPL13, msgEndCtrPL13 };

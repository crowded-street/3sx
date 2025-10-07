#include "sf33rd/Source/Game/Message3rd/C_USA/PL15End_usa.h"
#include "common.h"

static s8* msgEndPL15_00[1] = { "" };
static s8* msgEndPL15_01[1] = { "   \"Miss!\"                                      " };
static s8* msgEndPL15_02[1] = { "   \"You're going to be okay now...\"             " };
static s8* msgEndPL15_03[1] = { "   ...It's been 6 months since then...          " };
static s8* msgEndPL15_04[2] = {
    "   ...When I held in my arms,                   ",
    "   the girl whose life I had saved...           ",
};
static s8* msgEndPL15_05[2] = {
    "   I finally realized what my                   ",
    "   true \"fight\" should be...                    ",
};
static s8* msgEndPL15_06[2] = {
    "   For our mother country,                      ",
    "   that my father tried to protect...           ",
};
static s8* msgEndPL15_07[2] = {
    "   Ya!                                          ",
    "   Haah!                                        ",
};
static s8* msgEndPL15_08[2] = {
    "   For all of the warm-hearted people           ",
    "   I've met throughout the world...             ",
};
static s8* msgEndPL15_09[2] = {
    "   Ha!                                          ",
    "   Taaah!                                       ",
};
static s8* msgEndPL15_10[2] = {
    "   To teach the meaning of true                 ",
    "   strength and beauty to these kids...         ",
};
static s8* msgEndPL15_11[2] = {
    "   That is the purpose of my life,              ",
    "   and my future.                               ",
};
static s8* msgEndPL15_12[2] = {
    "   I'll use my fists to                         ",
    "   create our tomorrow!                         ",
};
static s8* msgEndPL15_13[1] = { "   Iyaaaaahh!!                                  " };

static s8** msgEndAdrPL15[14] = {
    msgEndPL15_00, msgEndPL15_01, msgEndPL15_02, msgEndPL15_03, msgEndPL15_04, msgEndPL15_05, msgEndPL15_06,
    msgEndPL15_07, msgEndPL15_08, msgEndPL15_09, msgEndPL15_10, msgEndPL15_11, msgEndPL15_12, msgEndPL15_13,
};

static s8 msgEndCtrPL15[14] = { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 };

MessageTable pl15end_usa_tbl = { msgEndAdrPL15, msgEndCtrPL15 };

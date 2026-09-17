/**
 * @file pl15end_en.c
 * Chun-Li Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl15end_en.h"
#include "common.h"

static char* msgEndPL15_00[1] = { "" };
static char* msgEndPL15_01[1] = { "   \"Miss!\"                                      " };
static char* msgEndPL15_02[1] = { "   \"You're going to be okay now...\"             " };
static char* msgEndPL15_03[1] = { "   ...It's been 6 months since then...          " };
static char* msgEndPL15_04[2] = {
    "   ...When I held in my arms,                   ",
    "   the girl whose life I had saved...           ",
};
static char* msgEndPL15_05[2] = {
    "   I finally realized what my                   ",
    "   true \"fight\" should be...                    ",
};
static char* msgEndPL15_06[2] = {
    "   For our mother country,                      ",
    "   that my father tried to protect...           ",
};
static char* msgEndPL15_07[2] = {
    "   Ya!                                          ",
    "   Haah!                                        ",
};
static char* msgEndPL15_08[2] = {
    "   For all of the warm-hearted people           ",
    "   I've met throughout the world...             ",
};
static char* msgEndPL15_09[2] = {
    "   Ha!                                          ",
    "   Taaah!                                       ",
};
static char* msgEndPL15_10[2] = {
    "   To teach the meaning of true                 ",
    "   strength and beauty to these kids...         ",
};
static char* msgEndPL15_11[2] = {
    "   That is the purpose of my life,              ",
    "   and my future.                               ",
};
static char* msgEndPL15_12[2] = {
    "   I'll use my fists to                         ",
    "   create our tomorrow!                         ",
};
static char* msgEndPL15_13[1] = { "   Iyaaaaahh!!                                  " };

static char** msgEndAdrPL15[14] = {
    msgEndPL15_00, msgEndPL15_01, msgEndPL15_02, msgEndPL15_03, msgEndPL15_04, msgEndPL15_05, msgEndPL15_06,
    msgEndPL15_07, msgEndPL15_08, msgEndPL15_09, msgEndPL15_10, msgEndPL15_11, msgEndPL15_12, msgEndPL15_13,
};

static s8 msgEndCtrPL15[14] = { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 };

MessageTable pl15end_usa_tbl = { msgEndAdrPL15, msgEndCtrPL15 };

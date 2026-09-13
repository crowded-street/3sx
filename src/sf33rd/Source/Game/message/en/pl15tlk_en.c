/**
 * @file pl15tlk_en.c
 * Chun-Li Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl15tlk_en.h"
#include "common.h"

static char* msgTlkPL15_00P[1] = { "" };
static char* msgTlkPL15_01P[2] = {
    "  I don't know what your                        ",
    "  scheme is...                                  ",
};
static char* msgTlkPL15_02P[2] = {
    "  But I can't let you                           ",
    "  harm small children!                          ",
};
static char* msgTlkPL15_03P[1] = { "  Where is that girl?                           " };
static char* msgTlkPL15_04P[2] = {
    "  How could you ask to see me for such          ",
    "  a trifle thing? You must be a fool!           ",
};
static char* msgTlkPL15_05P[2] = {
    "  I'll give you the girl, but you               ",
    "  must entertain me in return.                  ",
};
static char* msgTlkPL15_06P[2] = {
    "  Your legendary legs are said to               ",
    "  have destroyed \"that organization.\"           ",
};
static char* msgTlkPL15_07P[2] = {
    "  Let me find out for myself,                   ",
    "  if the rumors about you are true...           ",
};

static char** msgTalkAdrPL15[8] = {
    msgTlkPL15_00P, msgTlkPL15_01P, msgTlkPL15_02P, msgTlkPL15_03P,
    msgTlkPL15_04P, msgTlkPL15_05P, msgTlkPL15_06P, msgTlkPL15_07P,
};

static s8 msgTalkCtrPL15[8] = { 1, 2, 2, 1, 2, 2, 2, 2 };

MessageTable pl15tlk_usa_tbl = { msgTalkAdrPL15, msgTalkCtrPL15 };

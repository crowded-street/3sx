/**
 * @file pl01tlk_en.c
 * Alex Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl01tlk_en.h"
#include "common.h"

static char* msgTlkPL01_00P[1] = { "" };
static char* msgTlkPL01_01P[2] = {
    "  A karate uniform and headband...              ",
    "  You must be Ryu.                              ",
};
static char* msgTlkPL01_02P[2] = {
    "  I've heard a lot about you!                   ",
    "  Now, I'll confirm those rumors.               ",
};
static char* msgTlkPL01_03P[2] = {
    "  Hmm... I can see the                          ",
    "  power in your eyes...                         ",
};
static char* msgTlkPL01_04P[2] = {
    "  Don't hold back!                              ",
    "  Show me everything you've got!                ",
};

static char** msgTalkAdrPL01[5] = {
    msgTlkPL01_00P, msgTlkPL01_01P, msgTlkPL01_02P, msgTlkPL01_03P, msgTlkPL01_04P,
};

static s8 msgTalkCtrPL01[5] = { 1, 2, 2, 2, 2 };

MessageTable pl01tlk_usa_tbl = { msgTalkAdrPL01, msgTalkCtrPL01 };

/**
 * @file pl02tlk_en.c
 * Ryu Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl02tlk_en.h"
#include "common.h"

static char* msgTlkPL02_00P[1] = { "" };
static char* msgTlkPL02_01P[1] = { "  You never get bored of this, do you?          " };
static char* msgTlkPL02_02P[1] = { "  I could say the same to you...                " };
static char* msgTlkPL02_03P[2] = {
    "  Okay then, I'll make this quick and           ",
    "  easy for you. Prepare for defeat!             ",
};
static char* msgTlkPL02_04P[2] = {
    "  You may think so, but I've still got          ",
    "  the better win record! Stand strong!          ",
};

static char** msgTalkAdrPL02[5] = {
    msgTlkPL02_00P, msgTlkPL02_01P, msgTlkPL02_02P, msgTlkPL02_03P, msgTlkPL02_04P,
};

static s8 msgTalkCtrPL02[5] = { 1, 1, 1, 2, 2 };

MessageTable pl02tlk_usa_tbl = { msgTalkAdrPL02, msgTalkCtrPL02 };

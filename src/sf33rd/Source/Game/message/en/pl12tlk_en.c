/**
 * @file pl12tlk_en.c
 * Sean Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl12tlk_en.h"
#include "common.h"

static char* msgTlkPL12_00P[1] = { "" };
static char* msgTlkPL12_01P[2] = {
    "  Just as we agreed upon...                     ",
    "  Toady is the day.                             ",
};
static char* msgTlkPL12_02P[2] = {
    "  Your own special moves... your own            ",
    "  strategies... Anything is fine!               ",
};
static char* msgTlkPL12_03P[2] = {
    "  Just try to impress me! And if you            ",
    "  see a chance, try to win a round!             ",
};
static char* msgTlkPL12_04P[1] = { "  Well, that is... if you can!                  " };
static char* msgTlkPL12_05P[2] = {
    "  Master... I'm not the same                    ",
    "  person that I was a year ago!                 ",
};
static char* msgTlkPL12_06P[1] = { "  I'll show you what I mean!!                   " };

static char** msgTalkAdrPL12[7] = {
    msgTlkPL12_00P, msgTlkPL12_01P, msgTlkPL12_02P, msgTlkPL12_03P, msgTlkPL12_04P, msgTlkPL12_05P, msgTlkPL12_06P,
};

static s8 msgTalkCtrPL12[7] = { 1, 2, 2, 2, 1, 2, 1 };

MessageTable pl12tlk_usa_tbl = { msgTalkAdrPL12, msgTalkCtrPL12 };

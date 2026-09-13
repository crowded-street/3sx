/**
 * @file pl07tlk_en.c
 * Ibuki Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl07tlk_en.h"
#include "common.h"

static char* msgTlkPL07_00P[1] = { "" };
static char* msgTlkPL07_01P[1] = { "  Excuse me, are you a Sennin?                  " };
static char* msgTlkPL07_02P[2] = {
    "  That's right. I'm the great Sennin!           ",
    "  What do you want, girl?                       ",
};
static char* msgTlkPL07_03P[2] = {
    "  (He's as old as they get, but he is           ",
    "  also a master fighter, I guess...)            ",
};
static char* msgTlkPL07_04P[2] = {
    "  Well, I know it's unusual, but I'm            ",
    "  in the middle of graduation exam.             ",
};
static char* msgTlkPL07_05P[2] = {
    "  They won't allow me to pass unless            ",
    "  I defeat you in 5 minutes... So...            ",
};
static char* msgTlkPL07_06P[1] = { "  Fight with me!                                " };
static char* msgTlkPL07_07P[2] = {
    "  What?! I thought you wanted to go on          ",
    "  a date with me! I'm disappointed!             ",
};

static char** msgTalkAdrPL07[8] = {
    msgTlkPL07_00P, msgTlkPL07_01P, msgTlkPL07_02P, msgTlkPL07_03P,
    msgTlkPL07_04P, msgTlkPL07_05P, msgTlkPL07_06P, msgTlkPL07_07P,
};

static s8 msgTalkCtrPL07[8] = { 1, 1, 2, 2, 2, 2, 1, 2 };

MessageTable pl07tlk_usa_tbl = { msgTalkAdrPL07, msgTalkCtrPL07 };

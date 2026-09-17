/**
 * @file pl03tlk_en.c
 * Yun Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl03tlk_en.h"
#include "common.h"

static char* msgTlkPL03_00P[1] = { "" };
static char* msgTlkPL03_01P[1] = { "  So, you are Gill...                           " };
static char* msgTlkPL03_02P[2] = {
    "  What's up with your plan to conquer           ",
    "  the world?! Wake up, already!                 ",
};
static char* msgTlkPL03_03P[2] = {
    "  Youth tends to be reckless, but it            ",
    "  is not my intention to blame you.             ",
};
static char* msgTlkPL03_04P[2] = {
    "  Just think about it... What is                ",
    "  the ideal way for you to live?                ",
};
static char* msgTlkPL03_05P[2] = {
    "  It should be obvious to a boy like            ",
    "  you. Just leave everything to me!             ",
};
static char* msgTlkPL03_06P[2] = {
    "  Arrrgh! Stop preaching to me!                 ",
    "  Let our fists do the talking!                 ",
};

static char** msgTalkAdrPL03[7] = {
    msgTlkPL03_00P, msgTlkPL03_01P, msgTlkPL03_02P, msgTlkPL03_03P, msgTlkPL03_04P, msgTlkPL03_05P, msgTlkPL03_06P,
};

static s8 msgTalkCtrPL03[7] = { 1, 1, 2, 2, 2, 2, 2 };

MessageTable pl03tlk_usa_tbl = { msgTalkAdrPL03, msgTalkCtrPL03 };

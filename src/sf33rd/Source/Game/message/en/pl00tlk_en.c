/**
 * @file pl00tlk_en.c
 * Gill Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl00tlk_en.h"
#include "common.h"

static char* msgTlkPL00_00P[1] = { "" };
static char* msgTlkPL00_01P[1] = { "  I've been waiting for you, Alex.              " };
static char* msgTlkPL00_02P[2] = {
    "  Since the first time we met, I knew           ",
    "  that our destinies were intertwined...        ",
};
static char* msgTlkPL00_03P[2] = {
    "  I knew that you would seek me out to          ",
    "  learn the truth...                            ",
};
static char* msgTlkPL00_04P[2] = {
    "  What you do mean you knew I would             ",
    "  come here...?                                 ",
};
static char* msgTlkPL00_05P[2] = {
    "  Your visions don't impress me.                ",
    "  I just want to beat you down!!                ",
};
static char* msgTlkPL00_06P[2] = {
    "  I know that I'll find what I've been          ",
    "  looking for, once I destroy you.              ",
};
static char* msgTlkPL00_07P[2] = {
    "  I knew you would say that...                  ",
    "  Ha ha ha!                                     ",
};
static char* msgTlkPL00_08P[1] = { "  Let us embrace our destiny!!                  " };

static char** msgTalkAdrPL00[9] = {
    msgTlkPL00_00P, msgTlkPL00_01P, msgTlkPL00_02P, msgTlkPL00_03P, msgTlkPL00_04P,
    msgTlkPL00_05P, msgTlkPL00_06P, msgTlkPL00_07P, msgTlkPL00_08P,
};

static s8 msgTalkCtrPL00[9] = { 1, 1, 2, 2, 2, 2, 2, 2, 1 };

MessageTable pl00tlk_usa_tbl = { msgTalkAdrPL00, msgTalkCtrPL00 };

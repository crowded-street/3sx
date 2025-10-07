#include "sf33rd/Source/Game/Message3rd/C_USA/PL18Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL18_00P[1] = { "" };
static s8* msgTlkPL18_01P[2] = {
    "  Why?! Why can't you                           ",
    "  just leave me alone?!                         ",
};
static s8* msgTlkPL18_02P[2] = {
    "  I'm just a failure, right?!                   ",
    "  I have nothing you want!                      ",
};
static s8* msgTlkPL18_03P[2] = {
    "  Your body is full of dangerous,               ",
    "  confidential information.                     ",
};
static s8* msgTlkPL18_04P[2] = {
    "  Knowledge of your existence can               ",
    "  never be allowed into the world.              ",
};
static s8* msgTlkPL18_05P[2] = {
    "  It's up to each of us to decide               ",
    "  whether or not we should exist!               ",
};

static s8** msgTalkAdrPL18[6] = {
    msgTlkPL18_00P, msgTlkPL18_01P, msgTlkPL18_02P, msgTlkPL18_03P, msgTlkPL18_04P, msgTlkPL18_05P,
};

static s8 msgTalkCtrPL18[6] = { 1, 2, 2, 2, 2, 2 };

MessageTable pl18tlk_usa_tbl = { msgTalkAdrPL18, msgTalkCtrPL18 };

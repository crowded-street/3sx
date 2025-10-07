#include "sf33rd/Source/Game/Message3rd/C_USA/PL05Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL05_00P[1] = { "" };
static s8* msgTlkPL05_01P[2] = {
    "  Stop already! It's time for                   ",
    "  you to show your true self!                   ",
};
static s8* msgTlkPL05_02P[2] = {
    "  It's clear to me! Gill was the                ",
    "  one who sent you, right?!                     ",
};
static s8* msgTlkPL05_03P[1] = { "" };
static s8* msgTlkPL05_04P[2] = {
    "  I won't lose to a mass production             ",
    "  model, even if you are \"perfect!\"             ",
};
static s8* msgTlkPL05_05P[2] = {
    "  I'll put what's left of you into              ",
    "  a box that I will send to Gill!               ",
};

static s8** msgTalkAdrPL05[6] = {
    msgTlkPL05_00P, msgTlkPL05_01P, msgTlkPL05_02P, msgTlkPL05_03P, msgTlkPL05_04P, msgTlkPL05_05P,
};

static s8 msgTalkCtrPL05[6] = { 1, 2, 2, 1, 2, 2 };

MessageTable pl05tlk_usa_tbl = { msgTalkAdrPL05, msgTalkCtrPL05 };

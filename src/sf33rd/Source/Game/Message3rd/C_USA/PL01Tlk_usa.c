#include "sf33rd/Source/Game/Message3rd/C_USA/PL01Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL01_00P[1] = { "" };
static s8* msgTlkPL01_01P[2] = {
    "  A karate uniform and headband...              ",
    "  You must be Ryu.                              ",
};
static s8* msgTlkPL01_02P[2] = {
    "  I've heard a lot about you!                   ",
    "  Now, I'll confirm those rumors.               ",
};
static s8* msgTlkPL01_03P[2] = {
    "  Hmm... I can see the                          ",
    "  power in your eyes...                         ",
};
static s8* msgTlkPL01_04P[2] = {
    "  Don't hold back!                              ",
    "  Show me everything you've got!                ",
};

static s8** msgTalkAdrPL01[5] = {
    msgTlkPL01_00P, msgTlkPL01_01P, msgTlkPL01_02P, msgTlkPL01_03P, msgTlkPL01_04P,
};

static s8 msgTalkCtrPL01[5] = { 1, 2, 2, 2, 2 };

MessageTable pl01tlk_usa_tbl = { msgTalkAdrPL01, msgTalkCtrPL01 };

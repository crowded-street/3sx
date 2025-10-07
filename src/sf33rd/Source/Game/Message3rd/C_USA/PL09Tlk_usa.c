#include "sf33rd/Source/Game/Message3rd/C_USA/PL09Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL09_00P[1] = { "" };
static s8* msgTlkPL09_01P[2] = {
    "  Wow! You possess an                           ",
    "  incredible level of Chi!                      ",
};
static s8* msgTlkPL09_02P[2] = {
    "  That's what you call... Evil intent?          ",
    "  Impressive for a young man like you!          ",
};
static s8* msgTlkPL09_03P[1] = { "  I've heard about you, Sennin.                 " };
static s8* msgTlkPL09_04P[2] = {
    "  Watch your words, or they will                ",
    "  reveal your destruction.                      ",
};
static s8* msgTlkPL09_05P[2] = {
    "  You claim to be the master of                 ",
    "  fists... This disturbs me...                  ",
};
static s8* msgTlkPL09_06P[2] = {
    "  The road to true mastery is much              ",
    "  harder. Let us travel it now...               ",
};

static s8** msgTalkAdrPL09[7] = {
    msgTlkPL09_00P, msgTlkPL09_01P, msgTlkPL09_02P, msgTlkPL09_03P, msgTlkPL09_04P, msgTlkPL09_05P, msgTlkPL09_06P,
};

static s8 msgTalkCtrPL09[7] = { 1, 2, 2, 1, 2, 2, 2 };

MessageTable pl09tlk_usa_tbl = { msgTalkAdrPL09, msgTalkCtrPL09 };

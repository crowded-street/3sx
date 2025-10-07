#include "sf33rd/Source/Game/Message3rd/C_USA/PL11Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL11_00P[1] = { "" };
static s8* msgTlkPL11_01P[1] = { "  You're not quite what I expected...           " };
static s8* msgTlkPL11_02P[2] = {
    "  But, you sure do have                         ",
    "  the face of a fighter!                        ",
};
static s8* msgTlkPL11_03P[2] = {
    "  Behave yourself... I am destined to           ",
    "  become the king of the new century.           ",
};
static s8* msgTlkPL11_04P[2] = {
    "  You should thank me for using                 ",
    "  you as my entertainment!                      ",
};
static s8* msgTlkPL11_05P[2] = {
    "  Keep talking like that, and I'll do           ",
    "  much more than just entertain you!            ",
};

static s8** msgTalkAdrPL11[6] = {
    msgTlkPL11_00P, msgTlkPL11_01P, msgTlkPL11_02P, msgTlkPL11_03P, msgTlkPL11_04P, msgTlkPL11_05P,
};

static s8 msgTalkCtrPL11[6] = { 1, 1, 2, 2, 2, 2 };

MessageTable pl11tlk_usa_tbl = { msgTalkAdrPL11, msgTalkCtrPL11 };

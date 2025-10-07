#include "sf33rd/Source/Game/Message3rd/C_USA/PL08Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL08_00P[1] = { "" };
static s8* msgTlkPL08_01P[2] = {
    "  What's wrong with you?! Stop smiling          ",
    "  like that! It's creepy!                       ",
};
static s8* msgTlkPL08_02P[2] = {
    "  I'm here for a serious fight! I'll            ",
    "  punish you if you make fun of me!             ",
};
static s8* msgTlkPL08_03P[1] = { "  I just want to be your friend!                " };
static s8* msgTlkPL08_04P[2] = {
    "  That's the reason why we fight!               ",
    "  So let's enjoy the battle!                    ",
};
static s8* msgTlkPL08_05P[2] = {
    "  Friend?! I don't think I                      ",
    "  could ever understand you.                    ",
};

static s8** msgTalkAdrPL08[6] = {
    msgTlkPL08_00P, msgTlkPL08_01P, msgTlkPL08_02P, msgTlkPL08_03P, msgTlkPL08_04P, msgTlkPL08_05P,
};

static s8 msgTalkCtrPL08[6] = { 1, 2, 2, 1, 2, 2 };

MessageTable pl08tlk_usa_tbl = { msgTalkAdrPL08, msgTalkCtrPL08 };

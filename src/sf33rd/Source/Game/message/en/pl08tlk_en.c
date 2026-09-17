/**
 * @file pl08tlk_en.c
 * Elena Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl08tlk_en.h"
#include "common.h"

static char* msgTlkPL08_00P[1] = { "" };
static char* msgTlkPL08_01P[2] = {
    "  What's wrong with you?! Stop smiling          ",
    "  like that! It's creepy!                       ",
};
static char* msgTlkPL08_02P[2] = {
    "  I'm here for a serious fight! I'll            ",
    "  punish you if you make fun of me!             ",
};
static char* msgTlkPL08_03P[1] = { "  I just want to be your friend!                " };
static char* msgTlkPL08_04P[2] = {
    "  That's the reason why we fight!               ",
    "  So let's enjoy the battle!                    ",
};
static char* msgTlkPL08_05P[2] = {
    "  Friend?! I don't think I                      ",
    "  could ever understand you.                    ",
};

static char** msgTalkAdrPL08[6] = {
    msgTlkPL08_00P, msgTlkPL08_01P, msgTlkPL08_02P, msgTlkPL08_03P, msgTlkPL08_04P, msgTlkPL08_05P,
};

static s8 msgTalkCtrPL08[6] = { 1, 2, 2, 1, 2, 2 };

MessageTable pl08tlk_usa_tbl = { msgTalkAdrPL08, msgTalkCtrPL08 };

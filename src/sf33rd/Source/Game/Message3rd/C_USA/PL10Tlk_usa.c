#include "sf33rd/Source/Game/Message3rd/C_USA/PL10Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL10_00P[1] = { "" };
static s8* msgTlkPL10_01P[2] = {
    "  ...Why do you care about                      ",
    "  that so much?                                 ",
};
static s8* msgTlkPL10_02P[2] = {
    "  It's just a waste of time                     ",
    "  to determine who is stronger.                 ",
};
static s8* msgTlkPL10_03P[1] = { "  I know. I just...                             " };
static s8* msgTlkPL10_04P[2] = {
    "  I'd like you to see the progress              ",
    "  I've made in last couple of weeks.            ",
};
static s8* msgTlkPL10_05P[2] = {
    "  Okay then.                                    ",
    "  I have no choice but to accept...             ",
};
static s8* msgTlkPL10_06P[2] = {
    "  Let me show you what kind of walls            ",
    "  will always block your progress!              ",
};

static s8** msgTalkAdrPL10[7] = {
    msgTlkPL10_00P, msgTlkPL10_01P, msgTlkPL10_02P, msgTlkPL10_03P, msgTlkPL10_04P, msgTlkPL10_05P, msgTlkPL10_06P,
};

static s8 msgTalkCtrPL10[7] = { 1, 2, 2, 1, 2, 2, 2 };

MessageTable pl10tlk_usa_tbl = { msgTalkAdrPL10, msgTalkCtrPL10 };

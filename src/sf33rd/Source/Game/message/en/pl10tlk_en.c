/**
 * @file pl10tlk_en.c
 * Yang Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl10tlk_en.h"
#include "common.h"

static char* msgTlkPL10_00P[1] = { "" };
static char* msgTlkPL10_01P[2] = {
    "  ...Why do you care about                      ",
    "  that so much?                                 ",
};
static char* msgTlkPL10_02P[2] = {
    "  It's just a waste of time                     ",
    "  to determine who is stronger.                 ",
};
static char* msgTlkPL10_03P[1] = { "  I know. I just...                             " };
static char* msgTlkPL10_04P[2] = {
    "  I'd like you to see the progress              ",
    "  I've made in last couple of weeks.            ",
};
static char* msgTlkPL10_05P[2] = {
    "  Okay then.                                    ",
    "  I have no choice but to accept...             ",
};
static char* msgTlkPL10_06P[2] = {
    "  Let me show you what kind of walls            ",
    "  will always block your progress!              ",
};

static char** msgTalkAdrPL10[7] = {
    msgTlkPL10_00P, msgTlkPL10_01P, msgTlkPL10_02P, msgTlkPL10_03P, msgTlkPL10_04P, msgTlkPL10_05P, msgTlkPL10_06P,
};

static s8 msgTalkCtrPL10[7] = { 1, 2, 2, 1, 2, 2, 2 };

MessageTable pl10tlk_usa_tbl = { msgTalkAdrPL10, msgTalkCtrPL10 };

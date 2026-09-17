/**
 * @file pl19tlk_en.c
 * Remy Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl19tlk_en.h"
#include "common.h"

static char* msgTlkPL19_00P[1] = { "" };
static char* msgTlkPL19_01P[2] = {
    "  Answer me...                                  ",
    "  Why do you fight?                             ",
};
static char* msgTlkPL19_02P[2] = {
    "  I don't know... I started fighting            ",
    "  long before I knew the reasons why.           ",
};
static char* msgTlkPL19_03P[2] = {
    "  That selfishness victimizes people,           ",
    "  destroys love, and creates hatred!            ",
};
static char* msgTlkPL19_04P[2] = {
    "  I'll never forgive them! Those who            ",
    "  call themselves \"warriors\"...                 ",
};
static char* msgTlkPL19_05P[2] = {
    "  Just like my father, a man who                ",
    "  deserted and betrayed his family!             ",
};
static char* msgTlkPL19_06P[2] = {
    "  So what? If you need to see a                 ",
    "  shrink, go somewhere else!                    ",
};
static char* msgTlkPL19_07P[2] = {
    "  But, if it's pain that you want,              ",
    "  I have plenty to give to you!                 ",
};

static char** msgTalkAdrPL19[8] = {
    msgTlkPL19_00P, msgTlkPL19_01P, msgTlkPL19_02P, msgTlkPL19_03P,
    msgTlkPL19_04P, msgTlkPL19_05P, msgTlkPL19_06P, msgTlkPL19_07P,
};

static s8 msgTalkCtrPL19[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl19tlk_usa_tbl = { msgTalkAdrPL19, msgTalkCtrPL19 };

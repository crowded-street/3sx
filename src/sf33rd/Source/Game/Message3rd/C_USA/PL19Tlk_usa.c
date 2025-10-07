#include "sf33rd/Source/Game/Message3rd/C_USA/PL19Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL19_00P[1] = { "" };
static s8* msgTlkPL19_01P[2] = {
    "  Answer me...                                  ",
    "  Why do you fight?                             ",
};
static s8* msgTlkPL19_02P[2] = {
    "  I don't know... I started fighting            ",
    "  long before I knew the reasons why.           ",
};
static s8* msgTlkPL19_03P[2] = {
    "  That selfishness victimizes people,           ",
    "  destroys love, and creates hatred!            ",
};
static s8* msgTlkPL19_04P[2] = {
    "  I'll never forgive them! Those who            ",
    "  call themselves \"warriors\"...                 ",
};
static s8* msgTlkPL19_05P[2] = {
    "  Just like my father, a man who                ",
    "  deserted and betrayed his family!             ",
};
static s8* msgTlkPL19_06P[2] = {
    "  So what? If you need to see a                 ",
    "  shrink, go somewhere else!                    ",
};
static s8* msgTlkPL19_07P[2] = {
    "  But, if it's pain that you want,              ",
    "  I have plenty to give to you!                 ",
};

static s8** msgTalkAdrPL19[8] = {
    msgTlkPL19_00P, msgTlkPL19_01P, msgTlkPL19_02P, msgTlkPL19_03P,
    msgTlkPL19_04P, msgTlkPL19_05P, msgTlkPL19_06P, msgTlkPL19_07P,
};

static s8 msgTalkCtrPL19[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl19tlk_usa_tbl = { msgTalkAdrPL19, msgTalkCtrPL19 };

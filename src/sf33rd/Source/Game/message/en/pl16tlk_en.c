/**
 * @file pl16tlk_en.c
 * Makoto Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl16tlk_en.h"
#include "common.h"

static char* msgTlkPL16_00P[1] = { "" };
static char* msgTlkPL16_01P[2] = {
    "  I've heard about you...                       ",
    "  You're Ryu, aren't you?                       ",
};
static char* msgTlkPL16_02P[2] = {
    "  Your headband and that all-season             ",
    "  Karate uniform! I can't be mistaken!          ",
};
static char* msgTlkPL16_03P[1] = { "  Now, accept my challenge!                     " };
static char* msgTlkPL16_04P[2] = {
    "  Hmm... Your style is Karate...                ",
    "  And you seem to be skilled...                 ",
};
static char* msgTlkPL16_05P[2] = {
    "  Interesting!                                  ",
    "  Show me your best!!                           ",
};

static char** msgTalkAdrPL16[6] = {
    msgTlkPL16_00P, msgTlkPL16_01P, msgTlkPL16_02P, msgTlkPL16_03P, msgTlkPL16_04P, msgTlkPL16_05P,
};

static s8 msgTalkCtrPL16[6] = { 1, 2, 2, 1, 2, 2 };

MessageTable pl16tlk_usa_tbl = { msgTalkAdrPL16, msgTalkCtrPL16 };

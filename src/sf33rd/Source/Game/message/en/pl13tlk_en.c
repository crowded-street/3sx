/**
 * @file pl13tlk_en.c
 * Urien Cutscene Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl13tlk_en.h"
#include "common.h"

static char* msgTlkPL13_00P[1] = { "" };
static char* msgTlkPL13_01P[2] = {
    "  I understand your jealousy. There is          ",
    "  a huge gap between humans and a god.          ",
};
static char* msgTlkPL13_02P[2] = {
    "  You cannot forgive me                         ",
    "  because you cannot deny me.                   ",
};
static char* msgTlkPL13_03P[2] = {
    "  Let me grant you your death as a              ",
    "  show of mercy from your brother!              ",
};
static char* msgTlkPL13_04P[2] = {
    "  When I look into the past,                    ",
    "  I see dishonor and hatred.                    ",
};
static char* msgTlkPL13_05P[2] = {
    "  When I look into the future,                  ",
    "  I am free from the corruption.                ",
};
static char* msgTlkPL13_06P[2] = {
    "  How glorious it is...                         ",
    "  A world without you!                          ",
};

static char** msgTalkAdrPL13[7] = {
    msgTlkPL13_00P, msgTlkPL13_01P, msgTlkPL13_02P, msgTlkPL13_03P, msgTlkPL13_04P, msgTlkPL13_05P, msgTlkPL13_06P,
};

static s8 msgTalkCtrPL13[7] = { 1, 2, 2, 2, 2, 2, 2 };

MessageTable pl13tlk_usa_tbl = { msgTalkAdrPL13, msgTalkCtrPL13 };

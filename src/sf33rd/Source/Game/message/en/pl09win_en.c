/**
 * @file pl09win_en.c
 * Oro Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl09win_en.h"
#include "common.h"

static char* msgWinPL09_00P[1] = { "" };
static char* msgWinPL09_01P[1] = { "" };
static char* msgWinPL09_02P[1] = { "" };
static char* msgWinPL09_03P[1] = { "" };
static char* msgWinPL09_04P[1] = { "" };
static char* msgWinPL09_05P[1] = { "" };
static char* msgWinPL09_06P[1] = { "" };
static char* msgWinPL09_07P[1] = { "" };
static char* msgWinPL09_08P[1] = { "" };
static char* msgWinPL09_09P[1] = { "" };
static char* msgWinPL09_10P[1] = { "" };
static char* msgWinPL09_11P[1] = { "" };
static char* msgWinPL09_12P[1] = { "" };
static char* msgWinPL09_13P[1] = { "" };
static char* msgWinPL09_14P[1] = { "" };
static char* msgWinPL09_15P[1] = { "" };
static char* msgWinPL09_16P[1] = { "" };
static char* msgWinPL09_17P[1] = { "" };
static char* msgWinPL09_18P[1] = { "" };
static char* msgWinPL09_19P[1] = { "" };
static char* msgWinPL09_20P[2] = {
    "   Your moves are deadly... They could          ",
    "   kill me by boring me to death!               ",
};
static char* msgWinPL09_21P[2] = {
    "   Have you no sense of defense?                ",
    "   Or do you just like to feel pain?            ",
};
static char* msgWinPL09_22P[2] = {
    "   If we went home right now, would             ",
    "   anyone really care? Probably not.",
};
static char* msgWinPL09_23P[2] = {
    "   Keep it up and you'll suffer the             ",
    "   horror of pain!                              ",
};
static char* msgWinPL09_24P[2] = {
    "   Would you like to meet my pets?              ",
    "   Or are you too injured?                      ",
};
static char* msgWinPL09_25P[2] = {
    "   If you are angry, how can you                ",
    "   hope to strike accurately?                   ",
};
static char* msgWinPL09_26P[2] = {
    "   If I kept fighting against you,              ",
    "   I might actually get worse!                  ",
};
static char* msgWinPL09_27P[2] = {
    "   I'm so bored! I feel so limited!             ",
    "   Huh? You feel the same way?                  ",
};

static char** msgWinAdrPL09[28] = {
    msgWinPL09_00P, msgWinPL09_01P, msgWinPL09_02P, msgWinPL09_03P, msgWinPL09_04P, msgWinPL09_05P, msgWinPL09_06P,
    msgWinPL09_07P, msgWinPL09_08P, msgWinPL09_09P, msgWinPL09_10P, msgWinPL09_11P, msgWinPL09_12P, msgWinPL09_13P,
    msgWinPL09_14P, msgWinPL09_15P, msgWinPL09_16P, msgWinPL09_17P, msgWinPL09_18P, msgWinPL09_19P, msgWinPL09_20P,
    msgWinPL09_21P, msgWinPL09_22P, msgWinPL09_23P, msgWinPL09_24P, msgWinPL09_25P, msgWinPL09_26P, msgWinPL09_27P,
};

static s8 msgWinCtrPL09[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl09win_usa_tbl = { msgWinAdrPL09, msgWinCtrPL09 };

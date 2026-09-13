/**
 * @file pl13win_en.c
 * Urien Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl13win_en.h"
#include "common.h"

static char* msgWinPL13_00P[1] = { "" };
static char* msgWinPL13_01P[1] = { "" };
static char* msgWinPL13_02P[1] = { "" };
static char* msgWinPL13_03P[1] = { "" };
static char* msgWinPL13_04P[1] = { "" };
static char* msgWinPL13_05P[1] = { "" };
static char* msgWinPL13_06P[1] = { "" };
static char* msgWinPL13_07P[1] = { "" };
static char* msgWinPL13_08P[1] = { "" };
static char* msgWinPL13_09P[1] = { "" };
static char* msgWinPL13_10P[1] = { "" };
static char* msgWinPL13_11P[1] = { "" };
static char* msgWinPL13_12P[1] = { "" };
static char* msgWinPL13_13P[1] = { "" };
static char* msgWinPL13_14P[1] = { "" };
static char* msgWinPL13_15P[1] = { "" };
static char* msgWinPL13_16P[1] = { "" };
static char* msgWinPL13_17P[1] = { "" };
static char* msgWinPL13_18P[1] = { "" };
static char* msgWinPL13_19P[1] = { "" };
static char* msgWinPL13_20P[2] = {
    "   How dost thou suck? Let me                   ",
    "   count thy wounds! 1, 2, 3...                 ",
};
static char* msgWinPL13_21P[2] = {
    "   Look! Your cells have                        ",
    "   betrayed you to serve me!                    ",
};
static char* msgWinPL13_22P[2] = {
    "   That's right... Kneel before me!             ",
    "   How do you like the view now?!               ",
};
static char* msgWinPL13_23P[2] = {
    "   Lie there as long as you want.               ",
    "   I've had my fun with you.                    ",
};
static char* msgWinPL13_24P[2] = {
    "   You are not even worthy enough               ",
    "   to do my bidding! Be gone!                   ",
};
static char* msgWinPL13_25P[2] = {
    "   Pounding on you is satisfying,               ",
    "   but I need something more...                 ",
};
static char* msgWinPL13_26P[2] = {
    "   Your inferior vital fluids are               ",
    "   tainting the ground! Clean it up!            ",
};
static char* msgWinPL13_27P[2] = {
    "   How can you refuse to bow                    ",
    "   to a superior being?                         ",
};

static char** msgWinAdrPL13[28] = {
    msgWinPL13_00P, msgWinPL13_01P, msgWinPL13_02P, msgWinPL13_03P, msgWinPL13_04P, msgWinPL13_05P, msgWinPL13_06P,
    msgWinPL13_07P, msgWinPL13_08P, msgWinPL13_09P, msgWinPL13_10P, msgWinPL13_11P, msgWinPL13_12P, msgWinPL13_13P,
    msgWinPL13_14P, msgWinPL13_15P, msgWinPL13_16P, msgWinPL13_17P, msgWinPL13_18P, msgWinPL13_19P, msgWinPL13_20P,
    msgWinPL13_21P, msgWinPL13_22P, msgWinPL13_23P, msgWinPL13_24P, msgWinPL13_25P, msgWinPL13_26P, msgWinPL13_27P,
};

static s8 msgWinCtrPL13[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl13win_usa_tbl = { msgWinAdrPL13, msgWinCtrPL13 };

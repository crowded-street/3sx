/**
 * @file pl02win_en.c
 * Ryu Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl02win_en.h"
#include "common.h"

static char* msgWinPL02_00P[1] = { "" };
static char* msgWinPL02_01P[1] = { "" };
static char* msgWinPL02_02P[1] = { "" };
static char* msgWinPL02_03P[1] = { "" };
static char* msgWinPL02_04P[1] = { "" };
static char* msgWinPL02_05P[1] = { "" };
static char* msgWinPL02_06P[1] = { "" };
static char* msgWinPL02_07P[1] = { "" };
static char* msgWinPL02_08P[1] = { "" };
static char* msgWinPL02_09P[1] = { "" };
static char* msgWinPL02_10P[1] = { "" };
static char* msgWinPL02_11P[1] = { "" };
static char* msgWinPL02_12P[1] = { "" };
static char* msgWinPL02_13P[1] = { "" };
static char* msgWinPL02_14P[1] = { "" };
static char* msgWinPL02_15P[1] = { "" };
static char* msgWinPL02_16P[1] = { "" };
static char* msgWinPL02_17P[1] = { "" };
static char* msgWinPL02_18P[1] = { "" };
static char* msgWinPL02_19P[1] = { "" };
static char* msgWinPL02_20P[2] = {
    "   The eyes of a warrior never stray            ",
    "   from the spectacle of battle.                ",
};
static char* msgWinPL02_21P[2] = {
    "   Every moment gives us a chance to            ",
    "   become more than what we are.                ",
};
static char* msgWinPL02_22P[2] = {
    "   Range... Speed... Priority...                ",
    "   Know and master all of your attacks!         ",
};
static char* msgWinPL02_23P[2] = {
    "   A defeat learned from is more                ",
    "   important than an empty victory.             ",
};
static char* msgWinPL02_24P[2] = {
    "   Seek an opponent who is your equal.          ",
    "   Only then can you improve.                   ",
};
static char* msgWinPL02_25P[2] = {
    "   It's better to conquer a hardship            ",
    "   than to conquer nothing at all.              ",
};
static char* msgWinPL02_26P[2] = {
    "   Keep your mind calm and pure.                ",
    "   Focus on what is important!                  ",
};
static char* msgWinPL02_27P[2] = {
    "   To try something new, is to                  ",
    "   explore your true potential.                 ",
};

static char** msgWinAdrPL02[28] = {
    msgWinPL02_00P, msgWinPL02_01P, msgWinPL02_02P, msgWinPL02_03P, msgWinPL02_04P, msgWinPL02_05P, msgWinPL02_06P,
    msgWinPL02_07P, msgWinPL02_08P, msgWinPL02_09P, msgWinPL02_10P, msgWinPL02_11P, msgWinPL02_12P, msgWinPL02_13P,
    msgWinPL02_14P, msgWinPL02_15P, msgWinPL02_16P, msgWinPL02_17P, msgWinPL02_18P, msgWinPL02_19P, msgWinPL02_20P,
    msgWinPL02_21P, msgWinPL02_22P, msgWinPL02_23P, msgWinPL02_24P, msgWinPL02_25P, msgWinPL02_26P, msgWinPL02_27P,
};

static s8 msgWinCtrPL02[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl02win_usa_tbl = { msgWinAdrPL02, msgWinCtrPL02 };

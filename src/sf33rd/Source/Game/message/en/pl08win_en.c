/**
 * @file pl08win_en.c
 * Elena Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl08win_en.h"
#include "common.h"

static char* msgWinPL08_00P[1] = { "" };
static char* msgWinPL08_01P[1] = { "" };
static char* msgWinPL08_02P[1] = { "" };
static char* msgWinPL08_03P[1] = { "" };
static char* msgWinPL08_04P[1] = { "" };
static char* msgWinPL08_05P[1] = { "" };
static char* msgWinPL08_06P[1] = { "" };
static char* msgWinPL08_07P[1] = { "" };
static char* msgWinPL08_08P[1] = { "" };
static char* msgWinPL08_09P[1] = { "" };
static char* msgWinPL08_10P[1] = { "" };
static char* msgWinPL08_11P[1] = { "" };
static char* msgWinPL08_12P[1] = { "" };
static char* msgWinPL08_13P[1] = { "" };
static char* msgWinPL08_14P[1] = { "" };
static char* msgWinPL08_15P[1] = { "" };
static char* msgWinPL08_16P[1] = { "" };
static char* msgWinPL08_17P[1] = { "" };
static char* msgWinPL08_18P[1] = { "" };
static char* msgWinPL08_19P[1] = { "" };
static char* msgWinPL08_20P[2] = {
    "   I hope this moment of friendship             ",
    "   will bring you many happy memories!          ",
};
static char* msgWinPL08_21P[2] = {
    "   My accent? I must have picked                ",
    "   it up during my travels!                     ",
};
static char* msgWinPL08_22P[2] = {
    "   That was my best dance,                      ",
    "   performed especially for you!                ",
};
static char* msgWinPL08_23P[2] = {
    "   When you feel the wind, you can feel         ",
    "   many elements of nature at work.             ",
};
static char* msgWinPL08_24P[2] = {
    "   You need to be more flexible. Let            ",
    "   me help you to stretch your body!            ",
};
static char* msgWinPL08_25P[2] = {
    "   Forget about your troubles and               ",
    "   realize the harmony around us!               ",
};
static char* msgWinPL08_26P[2] = {
    "   Get addicted to the beat!                    ",
    "   Rhythm can be your friend too!               ",
};
static char* msgWinPL08_27P[2] = {
    "   You could teach me some new moves!           ",
    "   Let's dance together again!                  ",
};

static char** msgWinAdrPL08[28] = {
    msgWinPL08_00P, msgWinPL08_01P, msgWinPL08_02P, msgWinPL08_03P, msgWinPL08_04P, msgWinPL08_05P, msgWinPL08_06P,
    msgWinPL08_07P, msgWinPL08_08P, msgWinPL08_09P, msgWinPL08_10P, msgWinPL08_11P, msgWinPL08_12P, msgWinPL08_13P,
    msgWinPL08_14P, msgWinPL08_15P, msgWinPL08_16P, msgWinPL08_17P, msgWinPL08_18P, msgWinPL08_19P, msgWinPL08_20P,
    msgWinPL08_21P, msgWinPL08_22P, msgWinPL08_23P, msgWinPL08_24P, msgWinPL08_25P, msgWinPL08_26P, msgWinPL08_27P,
};

static s8 msgWinCtrPL08[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl08win_usa_tbl = { msgWinAdrPL08, msgWinCtrPL08 };

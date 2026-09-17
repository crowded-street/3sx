/**
 * @file pl16win_en.c
 * Makoto Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl16win_en.h"
#include "common.h"

static char* msgWinPL16_00P[1] = { "" };
static char* msgWinPL16_01P[1] = { "" };
static char* msgWinPL16_02P[1] = { "" };
static char* msgWinPL16_03P[1] = { "" };
static char* msgWinPL16_04P[1] = { "" };
static char* msgWinPL16_05P[1] = { "" };
static char* msgWinPL16_06P[1] = { "" };
static char* msgWinPL16_07P[1] = { "" };
static char* msgWinPL16_08P[1] = { "" };
static char* msgWinPL16_09P[1] = { "" };
static char* msgWinPL16_10P[1] = { "" };
static char* msgWinPL16_11P[1] = { "" };
static char* msgWinPL16_12P[1] = { "" };
static char* msgWinPL16_13P[1] = { "" };
static char* msgWinPL16_14P[1] = { "" };
static char* msgWinPL16_15P[1] = { "" };
static char* msgWinPL16_16P[1] = { "" };
static char* msgWinPL16_17P[1] = { "" };
static char* msgWinPL16_18P[1] = { "" };
static char* msgWinPL16_19P[1] = { "" };
static char* msgWinPL16_20P[2] = {
    "   You'd fare better if you                     ",
    "   would just cheer up!                         ",
};
static char* msgWinPL16_21P[2] = {
    "   That was a lot easier than                   ",
    "   I thought it would be!                       ",
};
static char* msgWinPL16_22P[2] = {
    "   That couldn't have been your                 ",
    "   best effort! Stop holding back!              ",
};
static char* msgWinPL16_23P[2] = {
    "   It'll take a lot more than that to           ",
    "   take me out! Let's do it again!              ",
};
static char* msgWinPL16_24P[2] = {
    "   It hurts twice as much when you              ",
    "   know that it was your fault!                 ",
};
static char* msgWinPL16_25P[2] = {
    "   What's going through your mind               ",
    "   when we fight? I want to know!               ",
};
static char* msgWinPL16_26P[2] = {
    "   Let's keep fighting until we are             ",
    "   both completely satisfied!                   ",
};
static char* msgWinPL16_27P[2] = {
    "   Daddy, you believed in me!                   ",
    "   That win was for our Dojo!                   ",
};

static char** msgWinAdrPL16[28] = {
    msgWinPL16_00P, msgWinPL16_01P, msgWinPL16_02P, msgWinPL16_03P, msgWinPL16_04P, msgWinPL16_05P, msgWinPL16_06P,
    msgWinPL16_07P, msgWinPL16_08P, msgWinPL16_09P, msgWinPL16_10P, msgWinPL16_11P, msgWinPL16_12P, msgWinPL16_13P,
    msgWinPL16_14P, msgWinPL16_15P, msgWinPL16_16P, msgWinPL16_17P, msgWinPL16_18P, msgWinPL16_19P, msgWinPL16_20P,
    msgWinPL16_21P, msgWinPL16_22P, msgWinPL16_23P, msgWinPL16_24P, msgWinPL16_25P, msgWinPL16_26P, msgWinPL16_27P,
};

static s8 msgWinCtrPL16[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl16win_usa_tbl = { msgWinAdrPL16, msgWinCtrPL16 };

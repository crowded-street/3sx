/**
 * @file pl01win_en.c
 * Alex Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl01win_en.h"
#include "common.h"

static char* msgWinPL01_00P[1] = { "" };
static char* msgWinPL01_01P[1] = { "" };
static char* msgWinPL01_02P[1] = { "" };
static char* msgWinPL01_03P[1] = { "" };
static char* msgWinPL01_04P[1] = { "" };
static char* msgWinPL01_05P[1] = { "" };
static char* msgWinPL01_06P[1] = { "" };
static char* msgWinPL01_07P[1] = { "" };
static char* msgWinPL01_08P[1] = { "" };
static char* msgWinPL01_09P[1] = { "" };
static char* msgWinPL01_10P[1] = { "" };
static char* msgWinPL01_11P[1] = { "" };
static char* msgWinPL01_12P[1] = { "" };
static char* msgWinPL01_13P[1] = { "" };
static char* msgWinPL01_14P[1] = { "" };
static char* msgWinPL01_15P[1] = { "" };
static char* msgWinPL01_16P[1] = { "" };
static char* msgWinPL01_17P[1] = { "" };
static char* msgWinPL01_18P[1] = { "" };
static char* msgWinPL01_19P[1] = { "" };
static char* msgWinPL01_20P[2] = {
    "   Defeat will help you                         ",
    "   to solve your ego problem.                   ",
};
static char* msgWinPL01_21P[2] = {
    "   Be a master of your mind before              ",
    "   attempting to master your body!              ",
};
static char* msgWinPL01_22P[2] = {
    "   Don't waste my time.                         ",
    "   I can learn nothing from you.                ",
};
static char* msgWinPL01_23P[2] = {
    "   Study your opponents and exploit             ",
    "   their weaknesses. That is the key...         ",
};
static char* msgWinPL01_24P[2] = {
    "   Use your head, or I will!                    ",
    "   I need a new bowling ball...                 ",
};
static char* msgWinPL01_25P[2] = {
    "   You don't have to be big,                    ",
    "   to look like a big loser.                    ",
};
static char* msgWinPL01_26P[2] = {
    "   You can't escape! Real fighters              ",
    "   know their true calling!                     ",
};
static char* msgWinPL01_27P[2] = {
    "   A lack of spirit will                        ",
    "   always lead to defeat!                       ",
};

static char** msgWinAdrPL01[28] = {
    msgWinPL01_00P, msgWinPL01_01P, msgWinPL01_02P, msgWinPL01_03P, msgWinPL01_04P, msgWinPL01_05P, msgWinPL01_06P,
    msgWinPL01_07P, msgWinPL01_08P, msgWinPL01_09P, msgWinPL01_10P, msgWinPL01_11P, msgWinPL01_12P, msgWinPL01_13P,
    msgWinPL01_14P, msgWinPL01_15P, msgWinPL01_16P, msgWinPL01_17P, msgWinPL01_18P, msgWinPL01_19P, msgWinPL01_20P,
    msgWinPL01_21P, msgWinPL01_22P, msgWinPL01_23P, msgWinPL01_24P, msgWinPL01_25P, msgWinPL01_26P, msgWinPL01_27P,
};

static s8 msgWinCtrPL01[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl01win_usa_tbl = { msgWinAdrPL01, msgWinCtrPL01 };

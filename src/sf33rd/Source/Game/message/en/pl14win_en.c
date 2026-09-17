/**
 * @file pl14win_en.c
 * Akuma/Gouki Win Quotes (English)
 */

#include "sf33rd/Source/Game/message/en/pl14win_en.h"
#include "common.h"

static char* msgWinPL14_00P[1] = { "" };
static char* msgWinPL14_01P[1] = { "" };
static char* msgWinPL14_02P[1] = { "" };
static char* msgWinPL14_03P[1] = { "" };
static char* msgWinPL14_04P[1] = { "" };
static char* msgWinPL14_05P[1] = { "" };
static char* msgWinPL14_06P[1] = { "" };
static char* msgWinPL14_07P[1] = { "" };
static char* msgWinPL14_08P[1] = { "" };
static char* msgWinPL14_09P[1] = { "" };
static char* msgWinPL14_10P[1] = { "" };
static char* msgWinPL14_11P[1] = { "" };
static char* msgWinPL14_12P[1] = { "" };
static char* msgWinPL14_13P[1] = { "" };
static char* msgWinPL14_14P[1] = { "" };
static char* msgWinPL14_15P[1] = { "" };
static char* msgWinPL14_16P[1] = { "" };
static char* msgWinPL14_17P[1] = { "" };
static char* msgWinPL14_18P[1] = { "" };
static char* msgWinPL14_19P[1] = { "" };
static char* msgWinPL14_20P[1] = { "   My fist knows no equal!                      " };
static char* msgWinPL14_21P[1] = { "   Perfection equals obliteration!              " };
static char* msgWinPL14_22P[2] = {
    "   Your body assumed its proper form            ",
    "   when my fists tore it apart!                 ",
};
static char* msgWinPL14_23P[2] = {
    "   Weakness is a disease...                     ",
    "   I am the cure!                               ",
};
static char* msgWinPL14_24P[2] = {
    "   The unworthy must be                         ",
    "   removed from existence.                      ",
};
static char* msgWinPL14_25P[2] = {
    "   You've fulfilled your purpose in             ",
    "   life by allowing me to end it!               ",
};
static char* msgWinPL14_26P[2] = {
    "   A weak shell for a weak soul. It was         ",
    "   an easy task to separate the two...          ",
};
static char* msgWinPL14_27P[2] = {
    "   To show mercy is to show                     ",
    "   proof of your imperfections!                 ",
};

static char** msgWinAdrPL14[28] = {
    msgWinPL14_00P, msgWinPL14_01P, msgWinPL14_02P, msgWinPL14_03P, msgWinPL14_04P, msgWinPL14_05P, msgWinPL14_06P,
    msgWinPL14_07P, msgWinPL14_08P, msgWinPL14_09P, msgWinPL14_10P, msgWinPL14_11P, msgWinPL14_12P, msgWinPL14_13P,
    msgWinPL14_14P, msgWinPL14_15P, msgWinPL14_16P, msgWinPL14_17P, msgWinPL14_18P, msgWinPL14_19P, msgWinPL14_20P,
    msgWinPL14_21P, msgWinPL14_22P, msgWinPL14_23P, msgWinPL14_24P, msgWinPL14_25P, msgWinPL14_26P, msgWinPL14_27P,
};

static s8 msgWinCtrPL14[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
};

MessageTable pl14win_usa_tbl = { msgWinAdrPL14, msgWinCtrPL14 };

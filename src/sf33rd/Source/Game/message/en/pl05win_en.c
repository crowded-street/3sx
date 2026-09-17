/**
 * @file pl05win_en.c
 * Necro Win Quotes
 */

#include "sf33rd/Source/Game/message/en/pl05win_en.h"
#include "common.h"

static char* msgWinPL05_00P[1] = { "" };
static char* msgWinPL05_01P[1] = { "" };
static char* msgWinPL05_02P[1] = { "" };
static char* msgWinPL05_03P[1] = { "" };
static char* msgWinPL05_04P[1] = { "" };
static char* msgWinPL05_05P[1] = { "" };
static char* msgWinPL05_06P[1] = { "" };
static char* msgWinPL05_07P[1] = { "" };
static char* msgWinPL05_08P[1] = { "" };
static char* msgWinPL05_09P[1] = { "" };
static char* msgWinPL05_10P[1] = { "" };
static char* msgWinPL05_11P[1] = { "" };
static char* msgWinPL05_12P[1] = { "" };
static char* msgWinPL05_13P[1] = { "" };
static char* msgWinPL05_14P[1] = { "" };
static char* msgWinPL05_15P[1] = { "" };
static char* msgWinPL05_16P[1] = { "" };
static char* msgWinPL05_17P[1] = { "" };
static char* msgWinPL05_18P[1] = { "" };
static char* msgWinPL05_19P[1] = { "" };
static char* msgWinPL05_20P[2] = {
    "   What's that smell? That's the aroma          ",
    "   of freshly cooked brain matter!              ",
};
static char* msgWinPL05_21P[2] = {
    "   The pain sensitivity of the flesh            ",
    "   varies. Where does it hurt the most?         ",
};
static char* msgWinPL05_22P[2] = {
    "   Freedom was once just a dream.               ",
    "   Now, I can fight to achieve it!              ",
};
static char* msgWinPL05_23P[2] = {
    "   When I realized that I was no longer         ",
    "   alone, I became determined to win!           ",
};
static char* msgWinPL05_24P[2] = {
    "   I'm long and I'm strong!                     ",
    "   To challenge me would be so wrong!           ",
};
static char* msgWinPL05_25P[2] = {
    "   You think I'm a freak? You should            ",
    "   see some of these other characters!          ",
};
static char* msgWinPL05_26P[2] = {
    "   A little battery won't kill ya...            ",
    "   Or will it?                                  ",
};
static char* msgWinPL05_27P[2] = {
    "   If you thought that was abnormal,            ",
    "   wait until you see THIS!!                    ",
};

static char** msgWinAdrPL05[28] = {
    msgWinPL05_00P, msgWinPL05_01P, msgWinPL05_02P, msgWinPL05_03P, msgWinPL05_04P, msgWinPL05_05P, msgWinPL05_06P,
    msgWinPL05_07P, msgWinPL05_08P, msgWinPL05_09P, msgWinPL05_10P, msgWinPL05_11P, msgWinPL05_12P, msgWinPL05_13P,
    msgWinPL05_14P, msgWinPL05_15P, msgWinPL05_16P, msgWinPL05_17P, msgWinPL05_18P, msgWinPL05_19P, msgWinPL05_20P,
    msgWinPL05_21P, msgWinPL05_22P, msgWinPL05_23P, msgWinPL05_24P, msgWinPL05_25P, msgWinPL05_26P, msgWinPL05_27P,
};

static s8 msgWinCtrPL05[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl05win_usa_tbl = { msgWinAdrPL05, msgWinCtrPL05 };

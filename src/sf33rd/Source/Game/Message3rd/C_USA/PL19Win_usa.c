#include "sf33rd/Source/Game/Message3rd/C_USA/PL19Win_usa.h"
#include "common.h"

static s8* msgWinPL19_00P[1] = { "" };
static s8* msgWinPL19_01P[1] = { "" };
static s8* msgWinPL19_02P[1] = { "" };
static s8* msgWinPL19_03P[1] = { "" };
static s8* msgWinPL19_04P[1] = { "" };
static s8* msgWinPL19_05P[1] = { "" };
static s8* msgWinPL19_06P[1] = { "" };
static s8* msgWinPL19_07P[1] = { "" };
static s8* msgWinPL19_08P[1] = { "" };
static s8* msgWinPL19_09P[1] = { "" };
static s8* msgWinPL19_10P[1] = { "" };
static s8* msgWinPL19_11P[1] = { "" };
static s8* msgWinPL19_12P[1] = { "" };
static s8* msgWinPL19_13P[1] = { "" };
static s8* msgWinPL19_14P[1] = { "" };
static s8* msgWinPL19_15P[1] = { "" };
static s8* msgWinPL19_16P[1] = { "" };
static s8* msgWinPL19_17P[1] = { "" };
static s8* msgWinPL19_18P[1] = { "" };
static s8* msgWinPL19_19P[1] = { "" };
static s8* msgWinPL19_20P[2] = {
    "   Victory, glory, and honor...                 ",
    "   Those ideas don't excite me a bit.           ",
};
static s8* msgWinPL19_21P[2] = {
    "   In death, you may find the                   ",
    "   answers that you seek.                       ",
};
static s8* msgWinPL19_22P[2] = {
    "   I have no intention of beating               ",
    "   you up again. Just leave...                  ",
};
static s8* msgWinPL19_23P[1] = { "   Why am I here, anyway?                       " };
static s8* msgWinPL19_24P[2] = {
    "   Pain is a cheap substitute                   ",
    "   for a vitality gauge!                        ",
};
static s8* msgWinPL19_25P[2] = {
    "   Don't you realize that the path you          ",
    "   are taking will lead you nowhere?!           ",
};
static s8* msgWinPL19_26P[2] = {
    "   Stay down. Or do you want                    ",
    "   me to hurt you again?                        ",
};
static s8* msgWinPL19_27P[2] = {
    "   I find your cause interesting,               ",
    "   but I could never relate to it.              ",
};

static s8** msgWinAdrPL19[28] = {
    msgWinPL19_00P, msgWinPL19_01P, msgWinPL19_02P, msgWinPL19_03P, msgWinPL19_04P, msgWinPL19_05P, msgWinPL19_06P,
    msgWinPL19_07P, msgWinPL19_08P, msgWinPL19_09P, msgWinPL19_10P, msgWinPL19_11P, msgWinPL19_12P, msgWinPL19_13P,
    msgWinPL19_14P, msgWinPL19_15P, msgWinPL19_16P, msgWinPL19_17P, msgWinPL19_18P, msgWinPL19_19P, msgWinPL19_20P,
    msgWinPL19_21P, msgWinPL19_22P, msgWinPL19_23P, msgWinPL19_24P, msgWinPL19_25P, msgWinPL19_26P, msgWinPL19_27P,
};

static s8 msgWinCtrPL19[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 2,
};

MessageTable pl19win_usa_tbl = { msgWinAdrPL19, msgWinCtrPL19 };

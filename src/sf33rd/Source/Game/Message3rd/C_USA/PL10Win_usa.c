#include "sf33rd/Source/Game/Message3rd/C_USA/PL10Win_usa.h"
#include "common.h"

static s8* msgWinPL10_00P[1] = { "" };
static s8* msgWinPL10_01P[1] = { "" };
static s8* msgWinPL10_02P[1] = { "" };
static s8* msgWinPL10_03P[1] = { "" };
static s8* msgWinPL10_04P[1] = { "" };
static s8* msgWinPL10_05P[1] = { "" };
static s8* msgWinPL10_06P[1] = { "" };
static s8* msgWinPL10_07P[1] = { "" };
static s8* msgWinPL10_08P[1] = { "" };
static s8* msgWinPL10_09P[1] = { "" };
static s8* msgWinPL10_10P[1] = { "" };
static s8* msgWinPL10_11P[1] = { "" };
static s8* msgWinPL10_12P[1] = { "" };
static s8* msgWinPL10_13P[1] = { "" };
static s8* msgWinPL10_14P[1] = { "" };
static s8* msgWinPL10_15P[1] = { "" };
static s8* msgWinPL10_16P[1] = { "" };
static s8* msgWinPL10_17P[1] = { "" };
static s8* msgWinPL10_18P[1] = { "" };
static s8* msgWinPL10_19P[1] = { "" };
static s8* msgWinPL10_20P[2] = {
    "   I was trying not to piss you off...          ",
    "   I guess I wasn't trying hard enough!         ",
};
static s8* msgWinPL10_21P[2] = {
    "   I hope you planned on taking a long          ",
    "   vacation. You'll need it to heal!            ",
};
static s8* msgWinPL10_22P[2] = {
    "   Don't assume that the same patterns          ",
    "   you use on others will work on me!           ",
};
static s8* msgWinPL10_23P[2] = {
    "   Every wound is a punishment                  ",
    "   for your careless training!                  ",
};
static s8* msgWinPL10_24P[2] = {
    "   I wonder if my brother would have            ",
    "   hurt you even more than I did...             ",
};
static s8* msgWinPL10_25P[2] = {
    "   Unless you want to disgrace history,         ",
    "   don't call what we did a fight!              ",
};
static s8* msgWinPL10_26P[2] = {
    "   An angry fighter is an ineffective           ",
    "   one... Not to mention ugly, too...           ",
};
static s8* msgWinPL10_27P[2] = {
    "   I move with the grace of the wind,           ",
    "   you just make it smell bad!                  ",
};

static s8** msgWinAdrPL10[28] = {
    msgWinPL10_00P, msgWinPL10_01P, msgWinPL10_02P, msgWinPL10_03P, msgWinPL10_04P, msgWinPL10_05P, msgWinPL10_06P,
    msgWinPL10_07P, msgWinPL10_08P, msgWinPL10_09P, msgWinPL10_10P, msgWinPL10_11P, msgWinPL10_12P, msgWinPL10_13P,
    msgWinPL10_14P, msgWinPL10_15P, msgWinPL10_16P, msgWinPL10_17P, msgWinPL10_18P, msgWinPL10_19P, msgWinPL10_20P,
    msgWinPL10_21P, msgWinPL10_22P, msgWinPL10_23P, msgWinPL10_24P, msgWinPL10_25P, msgWinPL10_26P, msgWinPL10_27P,
};

static s8 msgWinCtrPL10[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
};

MessageTable pl10win_usa_tbl = { msgWinAdrPL10, msgWinCtrPL10 };

#include "sf33rd/Source/Game/Message3rd/C_USA/PL06Win_usa.h"
#include "common.h"

static s8* msgWinPL06_00P[1] = { "" };
static s8* msgWinPL06_01P[1] = { "" };
static s8* msgWinPL06_02P[1] = { "" };
static s8* msgWinPL06_03P[1] = { "" };
static s8* msgWinPL06_04P[1] = { "" };
static s8* msgWinPL06_05P[1] = { "" };
static s8* msgWinPL06_06P[1] = { "" };
static s8* msgWinPL06_07P[1] = { "" };
static s8* msgWinPL06_08P[1] = { "" };
static s8* msgWinPL06_09P[1] = { "" };
static s8* msgWinPL06_10P[1] = { "" };
static s8* msgWinPL06_11P[1] = { "" };
static s8* msgWinPL06_12P[1] = { "" };
static s8* msgWinPL06_13P[1] = { "" };
static s8* msgWinPL06_14P[1] = { "" };
static s8* msgWinPL06_15P[1] = { "" };
static s8* msgWinPL06_16P[1] = { "" };
static s8* msgWinPL06_17P[1] = { "" };
static s8* msgWinPL06_18P[1] = { "" };
static s8* msgWinPL06_19P[1] = { "" };
static s8* msgWinPL06_20P[2] = {
    "   Would you be able to get up if I             ",
    "   sat down on you? Let's find out!             ",
};
static s8* msgWinPL06_21P[1] = { "   I am number one!                             " };
static s8* msgWinPL06_22P[2] = {
    "   The bigger I am, the                         ",
    "   harder they fall!                            ",
};
static s8* msgWinPL06_23P[2] = {
    "   The force of your body smashed the           ",
    "   pavement! Help me fix it!                    ",
};
static s8* msgWinPL06_24P[2] = {
    "   It's a natural instinct for you              ",
    "   to fear my size and strength!                ",
};
static s8* msgWinPL06_25P[2] = {
    "   Strange... Is suicide a popular              ",
    "   activity where you come from?                ",
};
static s8* msgWinPL06_26P[2] = {
    "   I held back. If I used any more              ",
    "   force, there would be a huge mess!           ",
};
static s8* msgWinPL06_27P[2] = {
    "   You enjoyed that, didn't you?                ",
    "   Let's do it again!                           ",
};

static s8** msgWinAdrPL06[28] = {
    msgWinPL06_00P, msgWinPL06_01P, msgWinPL06_02P, msgWinPL06_03P, msgWinPL06_04P, msgWinPL06_05P, msgWinPL06_06P,
    msgWinPL06_07P, msgWinPL06_08P, msgWinPL06_09P, msgWinPL06_10P, msgWinPL06_11P, msgWinPL06_12P, msgWinPL06_13P,
    msgWinPL06_14P, msgWinPL06_15P, msgWinPL06_16P, msgWinPL06_17P, msgWinPL06_18P, msgWinPL06_19P, msgWinPL06_20P,
    msgWinPL06_21P, msgWinPL06_22P, msgWinPL06_23P, msgWinPL06_24P, msgWinPL06_25P, msgWinPL06_26P, msgWinPL06_27P,
};

static s8 msgWinCtrPL06[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2,
};

MessageTable pl06win_usa_tbl = { msgWinAdrPL06, msgWinCtrPL06 };

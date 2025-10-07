#include "sf33rd/Source/Game/Message3rd/C_USA/PL03Win_usa.h"
#include "common.h"

static s8* msgWinPL03_00P[1] = { "" };
static s8* msgWinPL03_01P[1] = { "" };
static s8* msgWinPL03_02P[1] = { "" };
static s8* msgWinPL03_03P[1] = { "" };
static s8* msgWinPL03_04P[1] = { "" };
static s8* msgWinPL03_05P[1] = { "" };
static s8* msgWinPL03_06P[1] = { "" };
static s8* msgWinPL03_07P[1] = { "" };
static s8* msgWinPL03_08P[1] = { "" };
static s8* msgWinPL03_09P[1] = { "" };
static s8* msgWinPL03_10P[1] = { "" };
static s8* msgWinPL03_11P[1] = { "" };
static s8* msgWinPL03_12P[1] = { "" };
static s8* msgWinPL03_13P[1] = { "" };
static s8* msgWinPL03_14P[1] = { "" };
static s8* msgWinPL03_15P[1] = { "" };
static s8* msgWinPL03_16P[1] = { "" };
static s8* msgWinPL03_17P[1] = { "" };
static s8* msgWinPL03_18P[1] = { "" };
static s8* msgWinPL03_19P[1] = { "" };
static s8* msgWinPL03_20P[2] = {
    "   I see why you lost... You weren't            ",
    "   having any fun during the fight!             ",
};
static s8* msgWinPL03_21P[2] = {
    "   My style may change, but the outcome         ",
    "   is always the same. I win. You lose!         ",
};
static s8* msgWinPL03_22P[2] = {
    "   Now you're always afraid of things           ",
    "   falling on you from above? Sorry!            ",
};
static s8* msgWinPL03_23P[2] = {
    "   I'd rather rave than                         ",
    "   misbehave! Later!                            ",
};
static s8* msgWinPL03_24P[2] = {
    "   Perhaps you'd prefer to challenge            ",
    "   me at a skateboarding game?                  ",
};
static s8* msgWinPL03_25P[1] = { "   Cheap? I paid a lot for this hat!            " };
static s8* msgWinPL03_26P[2] = {
    "   My skateboard isn't a stretcher,             ",
    "   but it'll get you to the hospital!           ",
};
static s8* msgWinPL03_27P[2] = {
    "   Don't you know it's rude to ask your         ",
    "   opponent to let you win a round?!            ",
};

static s8** msgWinAdrPL03[28] = {
    msgWinPL03_00P, msgWinPL03_01P, msgWinPL03_02P, msgWinPL03_03P, msgWinPL03_04P, msgWinPL03_05P, msgWinPL03_06P,
    msgWinPL03_07P, msgWinPL03_08P, msgWinPL03_09P, msgWinPL03_10P, msgWinPL03_11P, msgWinPL03_12P, msgWinPL03_13P,
    msgWinPL03_14P, msgWinPL03_15P, msgWinPL03_16P, msgWinPL03_17P, msgWinPL03_18P, msgWinPL03_19P, msgWinPL03_20P,
    msgWinPL03_21P, msgWinPL03_22P, msgWinPL03_23P, msgWinPL03_24P, msgWinPL03_25P, msgWinPL03_26P, msgWinPL03_27P,
};

static s8 msgWinCtrPL03[28] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 2, 2,
};

MessageTable pl03win_usa_tbl = { msgWinAdrPL03, msgWinCtrPL03 };

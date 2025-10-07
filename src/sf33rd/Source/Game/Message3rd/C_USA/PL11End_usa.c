#include "sf33rd/Source/Game/Message3rd/C_USA/PL11End_usa.h"
#include "common.h"

static s8* msgEndPL11_00[1] = { "" };
static s8* msgEndPL11_01[2] = {
    "   Here we go!                                  ",
    "   Sho... Ryuuuu... Kennnn!                     ",
};
static s8* msgEndPL11_02[2] = {
    "   Oh no! He doesn't seem                       ",
    "   to be getting back up!                       ",
};
static s8* msgEndPL11_03[1] = { "   The battle is over!!                         " };
static s8* msgEndPL11_04[2] = {
    "   Showing the strength that we've              ",
    "   come to expect, the winner is Ken!           ",
};
static s8* msgEndPL11_05[2] = {
    "   Three U.S. Championships in a row!           ",
    "   History has been made!                       ",
};
static s8* msgEndPL11_06[2] = {
    "   This is an exciting moment for Ken,          ",
    "   his family, and his friends!                 ",
};
static s8* msgEndPL11_07[2] = {
    "   See? I knocked him out in                    ",
    "   30 seconds, as I promised!                   ",
};
static s8* msgEndPL11_08[2] = {
    "   Oh yeah! Here you go, Sean...                ",
    "   This is for you!                             ",
};
static s8* msgEndPL11_09[2] = {
    "   The U.S. Championship trophy...              ",
    "   You wanted it, right?                        ",
};
static s8* msgEndPL11_10[2] = {
    "   M... Master!                                 ",
    "   That's not what I meant!                     ",
};
static s8* msgEndPL11_11[2] = {
    "   It's meaningless if I did not                ",
    "   win it with my own effort!                   ",
};
static s8* msgEndPL11_12[2] = {
    "   Oh, is that so? You shouldn't                ",
    "   be too worried about that.                   ",
};
static s8* msgEndPL11_13[2] = {
    "   At the very least, you must pass             ",
    "   the qualifying rounds next time!             ",
};
static s8* msgEndPL11_14[2] = {
    "   But... You won't be the winner,              ",
    "   since you can't beat me! Ha ha ha!           ",
};

static s8** msgEndAdrPL11[15] = {
    msgEndPL11_00, msgEndPL11_01, msgEndPL11_02, msgEndPL11_03, msgEndPL11_04,
    msgEndPL11_05, msgEndPL11_06, msgEndPL11_07, msgEndPL11_08, msgEndPL11_09,
    msgEndPL11_10, msgEndPL11_11, msgEndPL11_12, msgEndPL11_13, msgEndPL11_14,
};

static s8 msgEndCtrPL11[15] = { 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl11end_usa_tbl = { msgEndAdrPL11, msgEndCtrPL11 };

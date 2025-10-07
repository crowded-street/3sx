#include "sf33rd/Source/Game/Message3rd/C_USA/PL06End_usa.h"
#include "common.h"

static s8* msgEndPL06_00[1] = { "" };
static s8* msgEndPL06_01[2] = {
    "   The arena is exploding with                  ",
    "   excitement tonight!                          ",
};
static s8* msgEndPL06_02[2] = {
    "   The winner has been decided for the          ",
    "   world's tag team championship!               ",
};
static s8* msgEndPL06_03[1] = { "" };
static s8* msgEndPL06_04[1] = { "   Er... Earthquake!!                           " };
static s8* msgEndPL06_05[2] = {
    "   Sorry to interrupt, but we have              ",
    "   an important announcement...                 ",
};
static s8* msgEndPL06_06[2] = {
    "   We are from the H.W.A.!                      ",
    "   ...The Huge Wrestling Army!                  ",
};
static s8* msgEndPL06_07[2] = {
    "   Effective immediately,                       ",
    "   you belong to us!                            ",
};
static s8* msgEndPL06_08[2] = {
    "   If you have any objections, you              ",
    "   must defeat our Hugo, first!                 ",
};
static s8* msgEndPL06_09[2] = {
    "   So far, over 50 have challenged and          ",
    "   they have all become our soldiers!           ",
};
static s8* msgEndPL06_10[2] = {
    "   Oh no... Why am I here?                      ",
    "   Hey... Ryu?!                                 ",
};
static s8* msgEndPL06_11[2] = {
    "   Hmm. This could be exciting. I               ",
    "   believe this will be a good test!            ",
};
static s8* msgEndPL06_12[2] = {
    "   (Ryu! I can't believe you're having          ",
    "   fun in a situation like this!)               ",
};
static s8* msgEndPL06_13[2] = {
    "   What's up? Are you scared                    ",
    "   of our members, or what?!                    ",
};
static s8* msgEndPL06_14[2] = {
    "   No objections? Okay then! This ring          ",
    "   will become our base of operations!          ",
};

static s8** msgEndAdrPL06[15] = {
    msgEndPL06_00, msgEndPL06_01, msgEndPL06_02, msgEndPL06_03, msgEndPL06_04,
    msgEndPL06_05, msgEndPL06_06, msgEndPL06_07, msgEndPL06_08, msgEndPL06_09,
    msgEndPL06_10, msgEndPL06_11, msgEndPL06_12, msgEndPL06_13, msgEndPL06_14,
};

static s8 msgEndCtrPL06[15] = { 1, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl06end_usa_tbl = { msgEndAdrPL06, msgEndCtrPL06 };

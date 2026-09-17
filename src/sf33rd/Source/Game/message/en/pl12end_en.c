/**
 * @file pl12end_en.c
 * Sean Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl12end_en.h"
#include "common.h"

static char* msgEndPL12_00[1] = { "" };
static char* msgEndPL12_01[1] = { "   Victory!!                                    " };
static char* msgEndPL12_02[2] = {
    "   Sean has won the first tournament            ",
    "   of his fighting career!                      ",
};
static char* msgEndPL12_03[1] = { "   I did it! Finally!                           " };
static char* msgEndPL12_04[1] = { "   Now... I'm the U.S. Champion!                " };
static char* msgEndPL12_05[2] = {
    "   Now I'm at the same level as my              ",
    "   master! It's a dream come true!              ",
};
static char* msgEndPL12_06[2] = {
    "   ...llo ...Hello? Sean!                       ",
    "   (...Wh ...What?)                             ",
};
static char* msgEndPL12_07[1] = { "   Are you okay?!                               " };
static char* msgEndPL12_08[2] = {
    "   Good! You've come to your senses...          ",
    "   I think...                                   ",
};
static char* msgEndPL12_09[2] = {
    "   It was close... You almost passed            ",
    "   the qualifying rounds!                       ",
};
static char* msgEndPL12_10[2] = {
    "   You did not train yourself                   ",
    "   properly. Try harder next time!              ",
};
static char* msgEndPL12_11[2] = {
    "   Okay then, leave it to me! I'll get          ",
    "   my revenge by winning the next one!          ",
};
static char* msgEndPL12_12[2] = {
    "   Wha? It was just a dream?!                   ",
    "   But someday... Someday I will win!           ",
};

static char** msgEndAdrPL12[13] = {
    msgEndPL12_00, msgEndPL12_01, msgEndPL12_02, msgEndPL12_03, msgEndPL12_04, msgEndPL12_05, msgEndPL12_06,
    msgEndPL12_07, msgEndPL12_08, msgEndPL12_09, msgEndPL12_10, msgEndPL12_11, msgEndPL12_12,
};

static s8 msgEndCtrPL12[13] = { 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2, 2, 2 };

MessageTable pl12end_usa_tbl = { msgEndAdrPL12, msgEndCtrPL12 };

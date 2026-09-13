/**
 * @file pl19end_en.c
 * Remy Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl19end_en.h"
#include "common.h"

static char* msgEndPL19_00[1] = { "" };
static char* msgEndPL19_01[2] = {
    "   Whenever I dive into this sea                ",
    "   of Bisque, I feel so cold.                   ",
};
static char* msgEndPL19_02[2] = {
    "   It is as cold as my heart,                   ",
    "   which cannot be healed.                      ",
};
static char* msgEndPL19_03[2] = {
    "   (The tides are increasing in their           ",
    "   speed and frequency...)                      ",
};
static char* msgEndPL19_04[2] = {
    "   (This will be the last time                  ",
    "   I visit this place...)                       ",
};
static char* msgEndPL19_05[2] = {
    "   I've been struggling in the                  ",
    "   darkness for a long time...                  ",
};
static char* msgEndPL19_06[2] = {
    "   I've been trying to paint the                ",
    "   darkness with another shadow...              ",
};
static char* msgEndPL19_07[2] = {
    "   After all... It was the darkness             ",
    "   of being a \"warrior\"...                      ",
};
static char* msgEndPL19_08[2] = {
    "   That caused our father to disappear,         ",
    "   leaving my sister and me alone...            ",
};
static char* msgEndPL19_09[1] = { "   ...I cannot forgive him yet.                 " };
static char* msgEndPL19_10[2] = {
    "   But... Nothing would change                  ",
    "   if I went on like this.                      ",
};
static char* msgEndPL19_11[1] = { "   So... I'll forget everything.                " };
static char* msgEndPL19_12[2] = {
    "   I believe that doing so                      ",
    "   could lead to our happiness.                 ",
};
static char* msgEndPL19_13[1] = { "   Goodbye... My sister!                        " };
static char* msgEndPL19_14[2] = {
    "   After saying goodbye, Remy                   ",
    "   sees a light on his way back.                ",
};
static char* msgEndPL19_15[2] = {
    "   He never believed that he would              ",
    "   be able to see such a light...               ",
};
static char* msgEndPL19_16[2] = {
    "   To discover the meaning of the               ",
    "   light, Remy begins to walk forth...          ",
};

static char** msgEndAdrPL19[17] = {
    msgEndPL19_00, msgEndPL19_01, msgEndPL19_02, msgEndPL19_03, msgEndPL19_04, msgEndPL19_05,
    msgEndPL19_06, msgEndPL19_07, msgEndPL19_08, msgEndPL19_09, msgEndPL19_10, msgEndPL19_11,
    msgEndPL19_12, msgEndPL19_13, msgEndPL19_14, msgEndPL19_15, msgEndPL19_16,
};

static s8 msgEndCtrPL19[17] = { 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 2, 2 };

MessageTable pl19end_usa_tbl = { msgEndAdrPL19, msgEndCtrPL19 };

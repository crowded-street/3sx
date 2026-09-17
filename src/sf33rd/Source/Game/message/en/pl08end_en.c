/**
 * @file pl08end_en.c
 * Elena Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl08end_en.h"
#include "common.h"

static char* msgEndPL08_00[1] = { "" };
static char* msgEndPL08_01[1] = { "   My dear Naru-chan,                           " };
static char* msgEndPL08_02[2] = {
    "   How are you?                                 ",
    "   I'm fine as always.                          ",
};
static char* msgEndPL08_03[2] = {
    "   I feel lonely, now that the Harvest          ",
    "   Festival here in France is over.             ",
};
static char* msgEndPL08_04[2] = {
    "   As I sip my tea, I'm reminded of             ",
    "   our happy days of summer in Japan.           ",
};
static char* msgEndPL08_05[2] = {
    "   ...When we went to the festival all          ",
    "   dressed up, and saw the fireworks...         ",
};
static char* msgEndPL08_06[2] = {
    "   Everything was so fun! Fights                ",
    "   were especially exciting, too!               ",
};
static char* msgEndPL08_07[2] = {
    "   I'd like to invite your family to            ",
    "   visit the great plain in my country.         ",
};
static char* msgEndPL08_08[2] = {
    "   I'm sure that my                             ",
    "   family will like you!                        ",
};
static char* msgEndPL08_09[2] = {
    "   Maybe next winter holiday... I know          ",
    "   you're working hard on your exams.           ",
};
static char* msgEndPL08_10[2] = {
    "   Take good care of your health!               ",
    "   See you again soon!                          ",
};
static char* msgEndPL08_11[2] = {
    "   There's a lot of hard work ahead             ",
    "   of me! I'll be studying, too!                ",
};

static char** msgEndAdrPL08[12] = {
    msgEndPL08_00, msgEndPL08_01, msgEndPL08_02, msgEndPL08_03, msgEndPL08_04, msgEndPL08_05,
    msgEndPL08_06, msgEndPL08_07, msgEndPL08_08, msgEndPL08_09, msgEndPL08_10, msgEndPL08_11,
};

static s8 msgEndCtrPL08[12] = { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl08end_usa_tbl = { msgEndAdrPL08, msgEndCtrPL08 };

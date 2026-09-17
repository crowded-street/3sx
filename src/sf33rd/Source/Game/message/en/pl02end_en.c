/**
 * @file pl02end_en.c
 * Ryu Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl02end_en.h"
#include "common.h"

static char* msgEndPL02_00[1] = { "" };
static char* msgEndPL02_01[1] = { "   \"What is a true warrior?\"                    " };
static char* msgEndPL02_02[2] = {
    "   \"Why should I be strong?                     ",
    "   What awaits beyond victory?\"                 ",
};
static char* msgEndPL02_03[2] = {
    "   Since the days of my youth, these            ",
    "   thoughts have never left my mind.            ",
};
static char* msgEndPL02_04[2] = {
    "   I had to decide... Either to stop            ",
    "   asking myself those questions...             ",
};
static char* msgEndPL02_05[2] = {
    "   ...or to struggle to                         ",
    "   find the answers...                          ",
};
static char* msgEndPL02_06[1] = { "" };
static char* msgEndPL02_07[2] = {
    "   Hmm. I'm not close. I can't even             ",
    "   tell where the leaves will fall...           ",
};
static char* msgEndPL02_08[2] = {
    "   As long as strong opponents and              ",
    "   fierce battles excite me...                  ",
};
static char* msgEndPL02_09[2] = {
    "   As long as I possess the strength            ",
    "   to move even one of my fingers...            ",
};
static char* msgEndPL02_10[1] = { "   ...I will keep on fighting!                  " };

static char** msgEndAdrPL02[11] = {
    msgEndPL02_00, msgEndPL02_01, msgEndPL02_02, msgEndPL02_03, msgEndPL02_04, msgEndPL02_05,
    msgEndPL02_06, msgEndPL02_07, msgEndPL02_08, msgEndPL02_09, msgEndPL02_10,
};

static s8 msgEndCtrPL02[11] = { 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1 };

MessageTable pl02end_usa_tbl = { msgEndAdrPL02, msgEndCtrPL02 };

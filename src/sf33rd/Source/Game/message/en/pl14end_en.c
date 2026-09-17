/**
 * @file pl14end_en.c
 * Akuma/Gouki Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl14end_en.h"
#include "common.h"

static char* msgEndPL14_00[1] = { "" };
static char* msgEndPL14_01[2] = {
    "   This is Deep Sea 3000.                       ",
    "   150m to the destination... Over.             ",
};
static char* msgEndPL14_02[2] = {
    "   We expect to reach the observation           ",
    "   point in approximately 40 seconds.           ",
};
static char* msgEndPL14_03[2] = {
    "   Confirmed visual contact with ship.          ",
    "   Switching to phase three... Over.            ",
};
static char* msgEndPL14_04[1] = { "   ...What?!                                    " };
static char* msgEndPL14_05[2] = {
    "   Is that... a man?!                           ",
    "   No... it can't be!                           ",
};
static char* msgEndPL14_06[1] = { "   This can't be happening...                   " };
static char* msgEndPL14_07[2] = {
    "   What? What did you see?!                     ",
    "   Respond!! Over!                              ",
};
static char* msgEndPL14_08[2] = {
    "   Something is wrong!                          ",
    "   The radio has been cut... Uwaahh!!           ",
};
static char* msgEndPL14_09[1] = { "   Muuuuuuunn!!                                 " };
static char* msgEndPL14_10[1] = { "   Tenshou Kaireki Jin!!                        " };

static char** msgEndAdrPL14[11] = {
    msgEndPL14_00, msgEndPL14_01, msgEndPL14_02, msgEndPL14_03, msgEndPL14_04, msgEndPL14_05,
    msgEndPL14_06, msgEndPL14_07, msgEndPL14_08, msgEndPL14_09, msgEndPL14_10,
};

static s8 msgEndCtrPL14[11] = { 1, 2, 2, 2, 1, 2, 1, 2, 2, 1, 1 };

MessageTable pl14end_usa_tbl = { msgEndAdrPL14, msgEndCtrPL14 };

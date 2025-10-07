#include "sf33rd/Source/Game/Message3rd/C_USA/PL14End_usa.h"
#include "common.h"

static s8* msgEndPL14_00[1] = { "" };
static s8* msgEndPL14_01[2] = {
    "   This is Deep Sea 3000.                       ",
    "   150m to the destination... Over.             ",
};
static s8* msgEndPL14_02[2] = {
    "   We expect to reach the observation           ",
    "   point in approximately 40 seconds.           ",
};
static s8* msgEndPL14_03[2] = {
    "   Confirmed visual contact with ship.          ",
    "   Switching to phase three... Over.            ",
};
static s8* msgEndPL14_04[1] = { "   ...What?!                                    " };
static s8* msgEndPL14_05[2] = {
    "   Is that... a man?!                           ",
    "   No... it can't be!                           ",
};
static s8* msgEndPL14_06[1] = { "   This can't be happening...                   " };
static s8* msgEndPL14_07[2] = {
    "   What? What did you see?!                     ",
    "   Respond!! Over!                              ",
};
static s8* msgEndPL14_08[2] = {
    "   Something is wrong!                          ",
    "   The radio has been cut... Uwaahh!!           ",
};
static s8* msgEndPL14_09[1] = { "   Muuuuuuunn!!                                 " };
static s8* msgEndPL14_10[1] = { "   Tenshou Kaireki Jin!!                        " };

static s8** msgEndAdrPL14[11] = {
    msgEndPL14_00, msgEndPL14_01, msgEndPL14_02, msgEndPL14_03, msgEndPL14_04, msgEndPL14_05,
    msgEndPL14_06, msgEndPL14_07, msgEndPL14_08, msgEndPL14_09, msgEndPL14_10,
};

static s8 msgEndCtrPL14[11] = { 1, 2, 2, 2, 1, 2, 1, 2, 2, 1, 1 };

MessageTable pl14end_usa_tbl = { msgEndAdrPL14, msgEndCtrPL14 };

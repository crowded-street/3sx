#include "sf33rd/Source/Game/Message3rd/C_USA/PL18End_usa.h"
#include "common.h"

static s8* msgEndPL18_00[1] = { "" };
static s8* msgEndPL18_01[2] = {
    "   Internal pressure of the cradle              ",
    "   is holding. Situation normal.                ",
};
static s8* msgEndPL18_02[1] = { "   Supervisor, the doctor is back...            " };
static s8* msgEndPL18_03[1] = { "   Welcome back, Doctor Kure.                   " };
static s8* msgEndPL18_04[2] = {
    "   We've already withdrawn #12 and #14.         ",
    "   The process is complete.                     ",
};
static s8* msgEndPL18_05[1] = { "   ...How is he doing?                          " };
static s8* msgEndPL18_06[1] = { "   He's absolutely perfect.                     " };
static s8* msgEndPL18_07[2] = {
    "   Every cell of his has been restored          ",
    "   to its state before the battle.              ",
};
static s8* msgEndPL18_08[2] = {
    "   Sir Urien's theory of human body             ",
    "   reconstruction is simply brilliant!          ",
};
static s8* msgEndPL18_09[2] = {
    "   With this technology... Eventually,          ",
    "   man would never grow old nor die.            ",
};
static s8* msgEndPL18_10[2] = {
    "   The only task left is to keep the            ",
    "   memory damage levels under 0.0001%.          ",
};
static s8* msgEndPL18_11[2] = {
    "   So... The \"never dying warrior\"              ",
    "   has been born...                             ",
};
static s8* msgEndPL18_12[2] = {
    "   All of the pain and pleasure will be         ",
    "   gone. You will be born once again...         ",
};
static s8* msgEndPL18_13[2] = {
    "   In your deep slumber... What kind            ",
    "   of dream are you dreaming?                   ",
};
static s8* msgEndPL18_14[2] = {
    "   No... He does not possess                    ",
    "   such an ability.                             ",
};
static s8* msgEndPL18_15[2] = {
    "   Because his memory is in a constant          ",
    "   state of activity, he cannot dream.          ",
};
static s8* msgEndPL18_16[2] = {
    "   Well, I guess it's not that great to         ",
    "   be able to live forever! Ha ha ha!!          ",
};

static s8** msgEndAdrPL18[17] = {
    msgEndPL18_00, msgEndPL18_01, msgEndPL18_02, msgEndPL18_03, msgEndPL18_04, msgEndPL18_05,
    msgEndPL18_06, msgEndPL18_07, msgEndPL18_08, msgEndPL18_09, msgEndPL18_10, msgEndPL18_11,
    msgEndPL18_12, msgEndPL18_13, msgEndPL18_14, msgEndPL18_15, msgEndPL18_16,
};

static s8 msgEndCtrPL18[17] = { 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

MessageTable pl18end_usa_tbl = { msgEndAdrPL18, msgEndCtrPL18 };

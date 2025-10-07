#include "sf33rd/Source/Game/Message3rd/C_USA/PL16End_usa.h"
#include "common.h"

static s8* msgEndPL16_00[1] = { "" };
static s8* msgEndPL16_01[2] = {
    "   I won...?!                                   ",
    "   I've finally won!!                           ",
};
static s8* msgEndPL16_02[2] = {
    "   Now, no one can stand                        ",
    "   against my moves!                            ",
};
static s8* msgEndPL16_03[2] = {
    "   The name of Rindoukan Dojo will              ",
    "   resound throughout the country!              ",
};
static s8* msgEndPL16_04[2] = {
    "   News of the dojo's youngest                  ",
    "   master quickly spreads...                    ",
};
static s8* msgEndPL16_05[2] = {
    "   As a result, many confident fighters         ",
    "   assemble at the Rindoukan Dojo.              ",
};
static s8* msgEndPL16_06[1] = { "   Chestoooo!!                                  " };
static s8* msgEndPL16_07[1] = { "   Doshaaaaaah!!                                " };
static s8* msgEndPL16_08[2] = {
    "   Attention, everyone! This is the end         ",
    "   of the line. Please wait patiently!          ",
};
static s8* msgEndPL16_09[2] = {
    "   Only 30 challengers will be accepted         ",
    "   today. Are you in the right line?!           ",
};
static s8* msgEndPL16_10[2] = {
    "   Masaru... Our dojo's popularity has          ",
    "   skyrocketed since Makoto's return!           ",
};
static s8* msgEndPL16_11[2] = {
    "   There are more people interested in          ",
    "   learning our style than ever before!         ",
};
static s8* msgEndPL16_12[2] = {
    "   Yes, it's all thanks to her.                 ",
    "   I'm very proud of her.                       ",
};
static s8* msgEndPL16_13[2] = {
    "   Although I am her senior, I have no          ",
    "   martial arts talent in comparison.           ",
};
static s8* msgEndPL16_14[2] = {
    "   I can leave the future of our dojo           ",
    "   in her hands without worrying.               ",
};
static s8* msgEndPL16_15[2] = {
    "   Grandpa, I'm going on a trip. Take           ",
    "   good care of her while I'm gone!             ",
};
static s8* msgEndPL16_16[1] = { "   Seyaaaaaah!!                                 " };
static s8* msgEndPL16_17[2] = {
    "   I haven't had enough yet!                    ",
    "   I'm just getting started!!                   ",
};

static s8** msgEndAdrPL16[18] = {
    msgEndPL16_00, msgEndPL16_01, msgEndPL16_02, msgEndPL16_03, msgEndPL16_04, msgEndPL16_05,
    msgEndPL16_06, msgEndPL16_07, msgEndPL16_08, msgEndPL16_09, msgEndPL16_10, msgEndPL16_11,
    msgEndPL16_12, msgEndPL16_13, msgEndPL16_14, msgEndPL16_15, msgEndPL16_16, msgEndPL16_17,
};

static s8 msgEndCtrPL16[18] = { 1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2 };

MessageTable pl16end_usa_tbl = { msgEndAdrPL16, msgEndCtrPL16 };

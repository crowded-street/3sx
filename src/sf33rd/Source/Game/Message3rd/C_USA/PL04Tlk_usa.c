#include "sf33rd/Source/Game/Message3rd/C_USA/PL04Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL04_00P[1] = { "" };
static s8* msgTlkPL04_01P[2] = {
    "  So, you're that famous                        ",
    "  Samurai fighter, aren't you?                  ",
};
static s8* msgTlkPL04_02P[2] = {
    "  What a coincidence! I've                      ",
    "  been looking for you, too!                    ",
};
static s8* msgTlkPL04_03P[2] = {
    "  I'd love a chance to fight with               ",
    "  England's #1 perfect boxer!                   ",
};
static s8* msgTlkPL04_04P[2] = {
    "  Good show lad! You behave like a              ",
    "  gentleman, yet have great strength!           ",
};
static s8* msgTlkPL04_05P[2] = {
    "  This shall be quite enjoyable!                ",
    "  Don't you agree?                              ",
};

static s8** msgTalkAdrPL04[6] = {
    msgTlkPL04_00P, msgTlkPL04_01P, msgTlkPL04_02P, msgTlkPL04_03P, msgTlkPL04_04P, msgTlkPL04_05P,
};

static s8 msgTalkCtrPL04[6] = { 1, 2, 2, 2, 2, 2 };

MessageTable pl04tlk_usa_tbl = { msgTalkAdrPL04, msgTalkCtrPL04 };

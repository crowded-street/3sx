#include "sf33rd/Source/Game/Message3rd/C_USA/PL06Tlk_usa.h"
#include "common.h"

static s8* msgTlkPL06_00P[1] = { "" };
static s8* msgTlkPL06_01P[2] = {
    "  So, you must be the one that                  ",
    "  Poison has been talking about.                ",
};
static s8* msgTlkPL06_02P[2] = {
    "  We're recruiting those who have the           ",
    "  potential to become great wrestlers.          ",
};
static s8* msgTlkPL06_03P[2] = {
    "  Now, let's make a deal. Promise me            ",
    "  that you will join us if you lose!            ",
};
static s8* msgTlkPL06_04P[2] = {
    "  That's fine... Maybe I can learn              ",
    "  something worthwhile from wrestling.          ",
};
static s8* msgTlkPL06_05P[2] = {
    "  However, I won't lose! I'll                   ",
    "  show you what true strength is!               ",
};

static s8** msgTalkAdrPL06[6] = {
    msgTlkPL06_00P, msgTlkPL06_01P, msgTlkPL06_02P, msgTlkPL06_03P, msgTlkPL06_04P, msgTlkPL06_05P,
};

static s8 msgTalkCtrPL06[6] = { 1, 2, 2, 2, 2, 2 };

MessageTable pl06tlk_usa_tbl = { msgTalkAdrPL06, msgTalkCtrPL06 };

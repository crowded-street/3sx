#include "sf33rd/Source/Game/Message3rd/C_USA/msgMenu_usa.h"
#include "common.h"

static s8* msgMenu_000[3] = {
    "OK to quit without saving the system data?",
    "",
    "               :OK     :CANCEL",
};

static s8** msgMenuAdr[1] = { msgMenu_000 };

static s8 msgMenuCtr[1] = { 3 };

MessageTable msgMenuTbl_usa = { msgMenuAdr, msgMenuCtr };

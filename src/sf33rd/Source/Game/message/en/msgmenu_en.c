/**
 * @file msgmenu_en.c
 * Exit Menu Message (English)
 */

#include "sf33rd/Source/Game/message/en/msgmenu_en.h"
#include "common.h"

static char* msgMenu_000[3] = {
    "Are you sure you want to exit the game?",
    "",
    "              :YES   :NO      ",
};

static char** msgMenuAdr[1] = { msgMenu_000 };

static s8 msgMenuCtr[1] = { 3 };

MessageTable msgMenuTbl_usa = { msgMenuAdr, msgMenuCtr };

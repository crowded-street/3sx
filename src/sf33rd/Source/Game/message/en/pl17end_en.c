/**
 * @file pl17end_en.c
 * Q Ending Text (English)
 */

#include "sf33rd/Source/Game/message/en/pl17end_en.h"
#include "common.h"

static char* msgEndPL17_00[1] = { "" };
static char* msgEndPL17_01[2] = {
    "   13 bizarre cases in the last 10 days         ",
    "   at sites located around the world...         ",
};
static char* msgEndPL17_02[2] = {
    "   After seeing those on-site pictures,         ",
    "   haven't you noticed something?               ",
};
static char* msgEndPL17_03[2] = {
    "   What's up, Juliana? You don't have           ",
    "   to beat around the bush with me!             ",
};
static char* msgEndPL17_04[2] = {
    "   Were you going to say that some              ",
    "   spooky guy appears in every scene?           ",
};
static char* msgEndPL17_05[2] = {
    "   David... You may have some                   ",
    "   telepathic talent, after all.                ",
};
static char* msgEndPL17_06[2] = {
    "   Look here... In every picture,               ",
    "   a man in a trench coat appears.              ",
};
static char* msgEndPL17_07[2] = {
    "   No way! These cases all happened             ",
    "   in different parts of the world!             ",
};
static char* msgEndPL17_08[1] = { "   It couldn't be the same person!              " };
static char* msgEndPL17_09[2] = {
    "   Well, they at least have similar             ",
    "   taste in clothing...                         ",
};
static char* msgEndPL17_10[2] = {
    "   Anyway, this \"mysterious guy\" is the         ",
    "   common thread in all of the cases.           ",
};
static char* msgEndPL17_11[2] = {
    "   So, have you                                 ",
    "   identified that man?                         ",
};
static char* msgEndPL17_12[1] = { "   Unfortunately, not yet...                    " };
static char* msgEndPL17_13[2] = {
    "   But, we have used photo analysis             ",
    "   to synthesize his face.                      ",
};
static char* msgEndPL17_14[2] = {
    "   Well, not exactly his face...                ",
    "   But... Anyway, have a look.                  ",
};
static char* msgEndPL17_15[2] = {
    "   What's this?!                                ",
    "   A mask... An iron mask?                      ",
};
static char* msgEndPL17_16[2] = {
    "   Okay! Send this picture out to each          ",
    "   of our branches, security level 4!           ",
};
static char* msgEndPL17_17[2] = {
    "   You are to start an investigation            ",
    "   on this man immediately. Dismissed!          ",
};

static char** msgEndAdrPL17[18] = {
    msgEndPL17_00, msgEndPL17_01, msgEndPL17_02, msgEndPL17_03, msgEndPL17_04, msgEndPL17_05,
    msgEndPL17_06, msgEndPL17_07, msgEndPL17_08, msgEndPL17_09, msgEndPL17_10, msgEndPL17_11,
    msgEndPL17_12, msgEndPL17_13, msgEndPL17_14, msgEndPL17_15, msgEndPL17_16, msgEndPL17_17,
};

static s8 msgEndCtrPL17[18] = { 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2 };

MessageTable pl17end_usa_tbl = { msgEndAdrPL17, msgEndCtrPL17 };

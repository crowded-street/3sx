#include "sf33rd/Source/Game/Message3rd/C_USA/PL10End_usa.h"
#include "common.h"

static s8* msgEndPL10_00[1] = { "" };
static s8* msgEndPL10_01[2] = {
    "   Stop! Will you two stop your                 ",
    "   quarreling already!!                         ",
};
static s8* msgEndPL10_02[2] = {
    "   You're always like this! You should          ",
    "   be more concerned about our town!            ",
};
static s8* msgEndPL10_03[2] = {
    "   You've been away for weeks                   ",
    "   and I've been so worried!                    ",
};
static s8* msgEndPL10_04[2] = {
    "   Maybe you would be better off if you         ",
    "   were dead in the middle of nowhere!          ",
};
static s8* msgEndPL10_05[2] = {
    "   Watch your mouth! You have no                ",
    "   idea what we've been through!                ",
};
static s8* msgEndPL10_06[2] = {
    "   This town could have been in                 ",
    "   big trouble if we hadn't fought!             ",
};
static s8* msgEndPL10_07[2] = {
    "   If it happened, no one would be left         ",
    "   to care about your lousy fried rice!         ",
};
static s8* msgEndPL10_08[2] = {
    "   What did you say is lousy?!                  ",
    "   Say that again!                              ",
};
static s8* msgEndPL10_09[2] = {
    "   Oh no... As I suspected, Hoimei              ",
    "   must be in love with my brother...           ",
};
static s8* msgEndPL10_10[1] = { "   I can never win...                           " };
static s8* msgEndPL10_11[1] = { "   My sister can be so clumsy...                " };
static s8* msgEndPL10_12[2] = {
    "   She's been worried. She just has             ",
    "   a hard time expressing her relief.           ",
};
static s8* msgEndPL10_13[2] = {
    "   You know, she didn't eat a thing             ",
    "   while you two were gone...                   ",
};
static s8* msgEndPL10_14[1] = { "   Shaomei...                                   " };
static s8* msgEndPL10_15[2] = {
    "   Although I care about the town, I            ",
    "   care a lot more about you two...             ",
};
static s8* msgEndPL10_16[2] = {
    "   So please...                                 ",
    "   Don't be so reckless, okay?                  ",
};
static s8* msgEndPL10_17[2] = {
    "   Yes... I understand. I'm sorry               ",
    "   if I caused you to worry.                    ",
};
static s8* msgEndPL10_18[2] = {
    "   (It's not just my sister... I must           ",
    "   be honest about my feelings, too)            ",
};
static s8* msgEndPL10_19[1] = { "   (I've got to be brave!)                      " };

static s8** msgEndAdrPL10[20] = {
    msgEndPL10_00, msgEndPL10_01, msgEndPL10_02, msgEndPL10_03, msgEndPL10_04, msgEndPL10_05, msgEndPL10_06,
    msgEndPL10_07, msgEndPL10_08, msgEndPL10_09, msgEndPL10_10, msgEndPL10_11, msgEndPL10_12, msgEndPL10_13,
    msgEndPL10_14, msgEndPL10_15, msgEndPL10_16, msgEndPL10_17, msgEndPL10_18, msgEndPL10_19,
};

static s8 msgEndCtrPL10[20] = { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 1, 2, 2, 2, 2, 1 };

MessageTable pl10end_usa_tbl = { msgEndAdrPL10, msgEndCtrPL10 };

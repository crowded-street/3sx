/**
 * @file msgsysdir_en.c
 * System Direction/Dipswitch Option Descriptors (English)
 */

#include "sf33rd/Source/Game/message/en/msgsysdir_en.h"
#include "common.h"

/**
 * Notes:
 * Each string is a different line
 * most lines are about 40 characters
 * If replacing an entry that initially had 2 lines, find the number at the same index in msgSysDirCtr and change it from 2 to 1.
 * In-game the text is centered in the box where it's rendered, so shorter strings won't line up properly.
 * Spaces after the last non-space character seem to be ignored
 */

static char* msgSYSDIR_000[1] = { "GROUND PARRY..................." };
static char* msgSYSDIR_001[2] = { "Enable/Disable the ability to perform", "a parry on the ground." };
static char* msgSYSDIR_002[1] = { "ANTI-AIR PARRY................." };
static char* msgSYSDIR_003[2] = { "Enable/Disable the ability to perform", "an anti-air parry." };
static char* msgSYSDIR_004[1] = { "AIR PARRY......................" };
static char* msgSYSDIR_005[2] = { "Enable/Disable the ability to perform", "a parry in the air." };
static char* msgSYSDIR_006[1] = { "PARRY COMMAND INPUT TIME......." };
static char* msgSYSDIR_007[2] = { "Adjust the parry window timing." };
static char* msgSYSDIR_008[1] = { "RED PARRY...................." };
static char* msgSYSDIR_009[2] = { "Enable/Disable the ability to perform", "a Red Parry." };
static char* msgSYSDIR_010[1] = { "RED PARRY C. INPUT TIME........" };
static char* msgSYSDIR_011[2] = { "Adjust the Red Parry window timing."};
static char* msgSYSDIR_012[1] = { "GUARD.........................." };
static char* msgSYSDIR_013[1] = { "Enable/Disable the ability to Guard." };
static char* msgSYSDIR_014[1] = { "ABSOLUTE GUARD................." };
static char* msgSYSDIR_015[2] = {
    "Choose \"ON\" to automatically keep guarding",
    "attacks after you start guarding.",
};
static char* msgSYSDIR_016[1] = { "GUARD DISTANCE................." };
static char* msgSYSDIR_017[1] = { "Adjust the proximity guard distance." };
static char* msgSYSDIR_018[1] = { "GUARD DAMAGE..................." };
static char* msgSYSDIR_019[2] = {
    "Choose \"ON\" to enable chip damage",
    "when blocking a Special Move or Super Art.",
};
static char* msgSYSDIR_020[1] = { "KO WHILE GUARDING.............." };
static char* msgSYSDIR_021[2] = {
    "Decide whether or not fighters can be knocked",
    "out by chip damage.",
};
static char* msgSYSDIR_022[1] = { "" };
static char* msgSYSDIR_023[1] = { "" };
static char* msgSYSDIR_024[1] = { "FORWARD DASH..................." };
static char* msgSYSDIR_025[1] = { "Enable/Disable the ability to dash forward." };
static char* msgSYSDIR_026[1] = { "BACK DASH......................" };
static char* msgSYSDIR_027[1] = { "Enable/Disable the ability to dash backward." };
static char* msgSYSDIR_028[1] = { "JUMP..........................." };
static char* msgSYSDIR_029[1] = { "Enable/Disable the ability to jump." };
static char* msgSYSDIR_030[1] = { "HIGH-JUMP......................" };
static char* msgSYSDIR_031[1] = { "Enable/Disable the ability to High-Jump." };
static char* msgSYSDIR_032[1] = { "QUICK STANDING................." };
static char* msgSYSDIR_033[2] = {
    "Enable/Disable the ability to stand up",
    "quickly when you are knocked down.",
};
static char* msgSYSDIR_034[1] = { "" };
static char* msgSYSDIR_035[1] = { "" };
static char* msgSYSDIR_036[1] = { "THROW.........................." };
static char* msgSYSDIR_037[1] = { "Enable/Disable the ability to throw." };
static char* msgSYSDIR_038[1] = { "THROW TECH....................." };
static char* msgSYSDIR_039[1] = { "Enable/Disable the ability to tech throws." };
static char* msgSYSDIR_040[1] = { "THROW TECH CONTROLS............" };
static char* msgSYSDIR_041[2] = {
    "Enable/Disable the ability to tech throws",
    "when holding up or down.",
};
static char* msgSYSDIR_042[1] = { "" };
static char* msgSYSDIR_043[1] = { "" };
static char* msgSYSDIR_044[1] = { "" };
static char* msgSYSDIR_045[1] = { "" };
static char* msgSYSDIR_046[1] = { "" };
static char* msgSYSDIR_047[1] = { "" };
static char* msgSYSDIR_048[1] = { "UNIVERSAL OVERHEAD............." };
static char* msgSYSDIR_049[2] = { "Enable/Disable the ability to perform a", "Universal Overhead attack." };
static char* msgSYSDIR_050[1] = { "UNIVERSAL OVERHEAD CONTROLS...." };
static char* msgSYSDIR_051[2] = {
    "Change the command of the Universal Overhead to",
    "\"^D^D+button\" which was used in \"2nd Impact.\"",
};
static char* msgSYSDIR_052[1] = { "TAUNT.........................." };
static char* msgSYSDIR_053[1] = { "Enable/Disable the ability to perform a Taunt." };
static char* msgSYSDIR_054[1] = { "TAUNT AFTER KO................." };
static char* msgSYSDIR_055[2] = {
    "Enable/Disable the ability to perform a",
    "Taunt after a knock out.",
};
static char* msgSYSDIR_056[1] = { "" };
static char* msgSYSDIR_057[1] = { "" };
static char* msgSYSDIR_058[1] = { "" };
static char* msgSYSDIR_059[1] = { "" };
static char* msgSYSDIR_060[1] = { "SUPER ART......................" };
static char* msgSYSDIR_061[2] = { "Enable/Disable the ability to perform", "Super Arts." };
static char* msgSYSDIR_062[1] = { "SPECIAL MOVE..................." };
static char* msgSYSDIR_063[2] = { "Enable/Disable the ability to perform", "Special Moves." };
static char* msgSYSDIR_064[1] = { "EX MOVE........................" };
static char* msgSYSDIR_065[2] = { "Enable/Disable the ability to perform", "EX Moves." };
static char* msgSYSDIR_066[1] = { "EX MOVE ENERGY REQUIREMENT....." };
static char* msgSYSDIR_067[2] = { "Adjust the amount of S.A. gauge", "required to perform an EX Move." };
static char* msgSYSDIR_068[1] = { "" };
static char* msgSYSDIR_069[1] = { "" };
static char* msgSYSDIR_070[1] = { "" };
static char* msgSYSDIR_071[1] = { "" };
static char* msgSYSDIR_072[1] = { "TARGET COMBO..................." };
static char* msgSYSDIR_073[2] = { "Enable/Disable the ability to perform a", "Target Combo." };
static char* msgSYSDIR_074[1] = { "SPECIAL MOVE/SUPER ART CANCEL.." };
static char* msgSYSDIR_075[2] = { "Enable/Disable the ability to cancel into", "Special Moves or Super Arts." };
static char* msgSYSDIR_076[1] = { "SUPER ART CANCEL..............." };
static char* msgSYSDIR_077[2] = { "Enable/Disable the ability to cancel into a", "Super Art." };
static char* msgSYSDIR_078[1] = { "HIGH-JUMP CANCEL..............." };
static char* msgSYSDIR_079[2] = { "Enable/Disable the ability to cancel into a", "High-Jump." }; //assumed meaning. I'm not good enough to do HJCs.
static char* msgSYSDIR_080[1] = { "HIGH-JUMP CANCEL TYPE.........." };
static char* msgSYSDIR_081[2] = {
    "Change the High-Jump type to \"2nd Impact\"   ",
    "style which can be canceled by a Special Move.",
};
static char* msgSYSDIR_082[1] = { "" };
static char* msgSYSDIR_083[1] = { "" };
static char* msgSYSDIR_084[1] = { "AIR GUARD......................" };
static char* msgSYSDIR_085[2] = { "Enable/Disable the ability to Guard", "in the air." };
static char* msgSYSDIR_086[1] = { "AUTO AIR RECOVERY.............." };
static char* msgSYSDIR_087[2] = {
    "When blown away by an attack, the character", //Revise this to reflect what it actually does in game.
    "will automatically recover.",
};
static char* msgSYSDIR_088[1] = { "KNOCK DOWNS FROM AIR..........." };
static char* msgSYSDIR_089[2] = {
    "When on, characters hit in mid-jump will",
    "automatically be knocked down.",
};
static char* msgSYSDIR_090[1] = { "EXTREME GUARD DAMAGE..........." };
static char* msgSYSDIR_091[2] = {
    "Choose \"ON\" to allow every attack to inflict",
    "Chip Damage when it is blocked.",
};
static char* msgSYSDIR_092[1] = { "S.A. GAUGE MAX START..........." };
static char* msgSYSDIR_093[2] = {
    "If choose \"ON,\" the game will start with",
    "the Super Arts Gauge at MAX.",
};
static char* msgSYSDIR_094[1] = { "S.A. GAUGE ROUND RESET........." };
static char* msgSYSDIR_095[2] = {
    "If choose \"ON,\" the Super Arts Gauge will be",
    "reset at the beginning of each round.",
};
static char* msgSYSDIR_096[1] = { "GROUND CHAIN COMBO............." };
static char* msgSYSDIR_097[2] = { "Enable/Disable the ability to perform Chain", "Combos on the ground." }; //Meaning: chain normals into others of higher strength
static char* msgSYSDIR_098[1] = { "AIR CHAIN COMBO................" };
static char* msgSYSDIR_099[2] = { "Enable/Disable to perform Chain Combos in", "the air." };
static char* msgSYSDIR_100[1] = { "CANCEL NORMAL MOVES............" };
static char* msgSYSDIR_101[2] = { "All of your normal moves will be able", "to be canceled into Special Moves!" };
static char* msgSYSDIR_102[1] = { "HIGH JUMP FROM ALL MOVES......." };
static char* msgSYSDIR_103[2] = {
    "Enable the ability to High-Jump cancel",
    "any move.",
};
static char* msgSYSDIR_104[1] = { "DASH FROM ALL MOVES............" };
static char* msgSYSDIR_105[1] = { "Enable the ability to dash-cancel any move." };
static char* msgSYSDIR_106[1] = { "SPECIAL TO SPECIAL CANCEL......" };
static char* msgSYSDIR_107[2] = {
    "Choose \"ON\" to be able to cancel Specials",
    "with other Specials or Super Arts.",
};
static char* msgSYSDIR_108[1] = { "ALL SUPER ARTS................." };
static char* msgSYSDIR_109[2] = {
    "If you choose \"ON,\" all of the Super Arts",
    "will be available during a match.",
};
static char* msgSYSDIR_110[1] = { "SUPER A. TO SUPER A. CANCEL...." };
static char* msgSYSDIR_111[2] = {
    "If you choose \"ON,\" you'll be able to",
    "cancel a Super Art with another Super Art.",
};
static char* msgSYSDIR_112[1] = { "AIR COMBO SKILL................" };
static char* msgSYSDIR_113[1] = { "Adjust how easily you can perform Air Combos." }; //Presumably this has something to do with juggle counter values, so being vague here is fine IMO.
static char* msgSYSDIR_114[1] = { "WHIFFED MOVE S.A. GAUGE BUILD.." };
static char* msgSYSDIR_115[2] = { "Decide whether the S.A. Gauge builds", "when moves are whiffed." };
static char* msgSYSDIR_200[1] = { "Return to the previous page." };
static char* msgSYSDIR_201[1] = { "Exit this mode and return to the Menu Screen." };
static char* msgSYSDIR_202[1] = { "Proceed to the next page." };

static char** msgSysDirAdr[119] = {
    msgSYSDIR_000, msgSYSDIR_001, msgSYSDIR_002, msgSYSDIR_003, msgSYSDIR_004, msgSYSDIR_005, msgSYSDIR_006,
    msgSYSDIR_007, msgSYSDIR_008, msgSYSDIR_009, msgSYSDIR_010, msgSYSDIR_011, msgSYSDIR_012, msgSYSDIR_013,
    msgSYSDIR_014, msgSYSDIR_015, msgSYSDIR_016, msgSYSDIR_017, msgSYSDIR_018, msgSYSDIR_019, msgSYSDIR_020,
    msgSYSDIR_021, msgSYSDIR_022, msgSYSDIR_023, msgSYSDIR_024, msgSYSDIR_025, msgSYSDIR_026, msgSYSDIR_027,
    msgSYSDIR_028, msgSYSDIR_029, msgSYSDIR_030, msgSYSDIR_031, msgSYSDIR_032, msgSYSDIR_033, msgSYSDIR_034,
    msgSYSDIR_035, msgSYSDIR_036, msgSYSDIR_037, msgSYSDIR_038, msgSYSDIR_039, msgSYSDIR_040, msgSYSDIR_041,
    msgSYSDIR_042, msgSYSDIR_043, msgSYSDIR_044, msgSYSDIR_045, msgSYSDIR_046, msgSYSDIR_047, msgSYSDIR_048,
    msgSYSDIR_049, msgSYSDIR_050, msgSYSDIR_051, msgSYSDIR_052, msgSYSDIR_053, msgSYSDIR_054, msgSYSDIR_055,
    msgSYSDIR_056, msgSYSDIR_057, msgSYSDIR_058, msgSYSDIR_059, msgSYSDIR_060, msgSYSDIR_061, msgSYSDIR_062,
    msgSYSDIR_063, msgSYSDIR_064, msgSYSDIR_065, msgSYSDIR_066, msgSYSDIR_067, msgSYSDIR_068, msgSYSDIR_069,
    msgSYSDIR_070, msgSYSDIR_071, msgSYSDIR_072, msgSYSDIR_073, msgSYSDIR_074, msgSYSDIR_075, msgSYSDIR_076,
    msgSYSDIR_077, msgSYSDIR_078, msgSYSDIR_079, msgSYSDIR_080, msgSYSDIR_081, msgSYSDIR_082, msgSYSDIR_083,
    msgSYSDIR_084, msgSYSDIR_085, msgSYSDIR_086, msgSYSDIR_087, msgSYSDIR_088, msgSYSDIR_089, msgSYSDIR_090,
    msgSYSDIR_091, msgSYSDIR_092, msgSYSDIR_093, msgSYSDIR_094, msgSYSDIR_095, msgSYSDIR_096, msgSYSDIR_097,
    msgSYSDIR_098, msgSYSDIR_099, msgSYSDIR_100, msgSYSDIR_101, msgSYSDIR_102, msgSYSDIR_103, msgSYSDIR_104,
    msgSYSDIR_105, msgSYSDIR_106, msgSYSDIR_107, msgSYSDIR_108, msgSYSDIR_109, msgSYSDIR_110, msgSYSDIR_111,
    msgSYSDIR_112, msgSYSDIR_113, msgSYSDIR_114, msgSYSDIR_115, msgSYSDIR_200, msgSYSDIR_201, msgSYSDIR_202
};

static s8 msgSysDirCtr[119] = { 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1,
                                1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,
                                1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1,
                                1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
                                1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1 };

MessageTable msgSysDirTbl_usa = { msgSysDirAdr, msgSysDirCtr };

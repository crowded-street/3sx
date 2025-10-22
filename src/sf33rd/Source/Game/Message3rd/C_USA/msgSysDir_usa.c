#include "sf33rd/Source/Game/Message3rd/C_USA/msgSysDir_usa.h"
#include "common.h"

/**
 * Notes:
 * Each string is a different line
 * most lines are about 40 characters
 * If replacing an entry that initially had 2 lines, find the number at the same index in msgSysDirCtr and change it from 2 to 1.
 * In-game the text is centered in the box where it's rendered, so shorter strings won't line up properly.
 * Spaces after the last non-space character seem to be ignored
 */

static s8* msgSYSDIR_000[1] = { "GROUND PARRY..................." };
static s8* msgSYSDIR_001[2] = { "Enable/Disable the ability to perform", "a parry on the ground." };
static s8* msgSYSDIR_002[1] = { "ANTI-AIR PARRY................." };
static s8* msgSYSDIR_003[2] = { "Enable/Disable the ability to perform", "an anti-air parry." };
static s8* msgSYSDIR_004[1] = { "AIR PARRY......................" };
static s8* msgSYSDIR_005[2] = { "Enable/Disable the ability to perform", "a parry in the air." };
static s8* msgSYSDIR_006[1] = { "PARRY COMMAND INPUT TIME......." };
static s8* msgSYSDIR_007[2] = { "Adjust the parry window timing." };
static s8* msgSYSDIR_008[1] = { "RED PARRY...................." };
static s8* msgSYSDIR_009[2] = { "Enable/Disable the ability to perform", "a Red Parry." };
static s8* msgSYSDIR_010[1] = { "RED PARRY C. INPUT TIME........" };
static s8* msgSYSDIR_011[2] = { "Adjust the Red Parry window timing."};
static s8* msgSYSDIR_012[1] = { "GUARD.........................." };
static s8* msgSYSDIR_013[1] = { "Enable/Disable the ability to Guard." };
static s8* msgSYSDIR_014[1] = { "ABSOLUTE GUARD................." };
static s8* msgSYSDIR_015[2] = {
    "Choose \"ON\" to automatically keep guarding",
    "attacks after you start guarding.",
};
static s8* msgSYSDIR_016[1] = { "GUARD DISTANCE................." };
static s8* msgSYSDIR_017[1] = { "Adjust the proximity guard distance." };
static s8* msgSYSDIR_018[1] = { "GUARD DAMAGE..................." };
static s8* msgSYSDIR_019[2] = {
    "Choose \"ON\" to enable chip damage",
    "when blocking a Special Move or Super Art.",
};
static s8* msgSYSDIR_020[1] = { "KO WHILE GUARDING.............." };
static s8* msgSYSDIR_021[2] = {
    "Decide whether or not fighters can be knocked",
    "out by chip damage.",
};
static s8* msgSYSDIR_022[1] = { "" };
static s8* msgSYSDIR_023[1] = { "" };
static s8* msgSYSDIR_024[1] = { "FORWARD DASH..................." };
static s8* msgSYSDIR_025[1] = { "Enable/Disable the ability to dash forward." };
static s8* msgSYSDIR_026[1] = { "BACK DASH......................" };
static s8* msgSYSDIR_027[1] = { "Enable/Disable the ability to dash backward." };
static s8* msgSYSDIR_028[1] = { "JUMP..........................." };
static s8* msgSYSDIR_029[1] = { "Enable/Disable the ability to jump." };
static s8* msgSYSDIR_030[1] = { "HIGH-JUMP......................" };
static s8* msgSYSDIR_031[1] = { "Enable/Disable the ability to High-Jump." };
static s8* msgSYSDIR_032[1] = { "QUICK STANDING................." };
static s8* msgSYSDIR_033[2] = {
    "Enable/Disable the ability to stand up",
    "quickly when you are knocked down.",
};
static s8* msgSYSDIR_034[1] = { "" };
static s8* msgSYSDIR_035[1] = { "" };
static s8* msgSYSDIR_036[1] = { "THROW.........................." };
static s8* msgSYSDIR_037[1] = { "Enable/Disable the ability to throw." };
static s8* msgSYSDIR_038[1] = { "THROW TECH....................." };
static s8* msgSYSDIR_039[1] = { "Enable/Disable the ability to tech throws." };
static s8* msgSYSDIR_040[1] = { "THROW TECH CONTROLS............" };
static s8* msgSYSDIR_041[2] = {
    "Enable/Disable the ability to tech throws",
    "when holding up or down.",
};
static s8* msgSYSDIR_042[1] = { "" };
static s8* msgSYSDIR_043[1] = { "" };
static s8* msgSYSDIR_044[1] = { "" };
static s8* msgSYSDIR_045[1] = { "" };
static s8* msgSYSDIR_046[1] = { "" };
static s8* msgSYSDIR_047[1] = { "" };
static s8* msgSYSDIR_048[1] = { "UNIVERSAL OVERHEAD............." };
static s8* msgSYSDIR_049[2] = { "Enable/Disable the ability to perform a", "Universal Overhead attack." };
static s8* msgSYSDIR_050[1] = { "UNIVERSAL OVERHEAD CONTROLS...." };
static s8* msgSYSDIR_051[2] = {
    "Change the command of the Universal Overhead to",
    "\"^D^D+button\" which was used in \"2nd Impact.\"",
};
static s8* msgSYSDIR_052[1] = { "TAUNT.........................." };
static s8* msgSYSDIR_053[1] = { "Enable/Disable the ability to perform a Taunt." };
static s8* msgSYSDIR_054[1] = { "TAUNT AFTER KO................." };
static s8* msgSYSDIR_055[2] = {
    "Enable/Disable the ability to perform a",
    "Taunt after a knock out.",
};
static s8* msgSYSDIR_056[1] = { "" };
static s8* msgSYSDIR_057[1] = { "" };
static s8* msgSYSDIR_058[1] = { "" };
static s8* msgSYSDIR_059[1] = { "" };
static s8* msgSYSDIR_060[1] = { "SUPER ART......................" };
static s8* msgSYSDIR_061[2] = { "Enable/Disable the ability to perform", "Super Arts." };
static s8* msgSYSDIR_062[1] = { "SPECIAL MOVE..................." };
static s8* msgSYSDIR_063[2] = { "Enable/Disable the ability to perform", "Special Moves." };
static s8* msgSYSDIR_064[1] = { "EX MOVE........................" };
static s8* msgSYSDIR_065[2] = { "Enable/Disable the ability to perform", "EX Moves." };
static s8* msgSYSDIR_066[1] = { "EX MOVE ENERGY REQUIREMENT....." };
static s8* msgSYSDIR_067[2] = { "Adjust the amount of S.A. gauge", "required to perform an EX Move." };
static s8* msgSYSDIR_068[1] = { "" };
static s8* msgSYSDIR_069[1] = { "" };
static s8* msgSYSDIR_070[1] = { "" };
static s8* msgSYSDIR_071[1] = { "" };
static s8* msgSYSDIR_072[1] = { "TARGET COMBO..................." };
static s8* msgSYSDIR_073[2] = { "Enable/Disable the ability to perform a", "Target Combo." };
static s8* msgSYSDIR_074[1] = { "SPECIAL MOVE/SUPER ART CANCEL.." };
static s8* msgSYSDIR_075[2] = { "Enable/Disable the ability to cancel into", "Special Moves or Super Arts." };
static s8* msgSYSDIR_076[1] = { "SUPER ART CANCEL..............." };
static s8* msgSYSDIR_077[2] = { "Enable/Disable the ability to cancel into a", "Super Art." };
static s8* msgSYSDIR_078[1] = { "HIGH-JUMP CANCEL..............." };
static s8* msgSYSDIR_079[2] = { "Enable/Disable the ability to cancel into a", "High-Jump." }; //assumed meaning. I'm not good enough to do HJCs.
static s8* msgSYSDIR_080[1] = { "HIGH-JUMP CANCEL TYPE.........." };
static s8* msgSYSDIR_081[2] = {
    "Change the High-Jump type to \"2nd Impact\"   ",
    "style which can be canceled by a Special Move.",
};
static s8* msgSYSDIR_082[1] = { "" };
static s8* msgSYSDIR_083[1] = { "" };
static s8* msgSYSDIR_084[1] = { "AIR GUARD......................" };
static s8* msgSYSDIR_085[2] = { "Enable/Disable the ability to Guard", "in the air." };
static s8* msgSYSDIR_086[1] = { "AUTO AIR RECOVERY.............." };
static s8* msgSYSDIR_087[2] = {
    "When blown away by an attack, the character", //Revise this to reflect what it actually does in game.
    "will automatically recover.",
};
static s8* msgSYSDIR_088[1] = { "KNOCK DOWNS FROM AIR..........." };
static s8* msgSYSDIR_089[2] = {
    "When on, characters hit in mid-jump will",
    "automatically be knocked down.",
};
static s8* msgSYSDIR_090[1] = { "EXTREME GUARD DAMAGE..........." };
static s8* msgSYSDIR_091[2] = {
    "Choose \"ON\" to allow every attack to inflict",
    "Chip Damage when it is blocked.",
};
static s8* msgSYSDIR_092[1] = { "S.A. GAUGE MAX START..........." };
static s8* msgSYSDIR_093[2] = {
    "If choose \"ON,\" the game will start with",
    "the Super Arts Gauge at MAX.",
};
static s8* msgSYSDIR_094[1] = { "S.A. GAUGE ROUND RESET........." };
static s8* msgSYSDIR_095[2] = {
    "If choose \"ON,\" the Super Arts Gauge will be",
    "reset at the beginning of each round.",
};
static s8* msgSYSDIR_096[1] = { "GROUND CHAIN COMBO............." };
static s8* msgSYSDIR_097[2] = { "Enable/Disable the ability to perform Chain", "Combos on the ground." }; //Meaning: chain normals into others of higher strength
static s8* msgSYSDIR_098[1] = { "AIR CHAIN COMBO................" };
static s8* msgSYSDIR_099[2] = { "Enable/Disable to perform Chain Combos in", "the air." };
static s8* msgSYSDIR_100[1] = { "CANCEL NORMAL MOVES............" };
static s8* msgSYSDIR_101[2] = { "All of your normal moves will be able", "to be canceled into Special Moves!" };
static s8* msgSYSDIR_102[1] = { "HIGH JUMP FROM ALL MOVES......." };
static s8* msgSYSDIR_103[2] = {
    "Enable the ability to High-Jump cancel",
    "any move.",
};
static s8* msgSYSDIR_104[1] = { "DASH FROM ALL MOVES............" };
static s8* msgSYSDIR_105[1] = { "Enable the ability to dash-cancel any move." };
static s8* msgSYSDIR_106[1] = { "SPECIAL TO SPECIAL CANCEL......" };
static s8* msgSYSDIR_107[2] = {
    "Choose \"ON\" to be able to cancel Specials",
    "with other Specials or Super Arts.",
};
static s8* msgSYSDIR_108[1] = { "ALL SUPER ARTS................." };
static s8* msgSYSDIR_109[2] = {
    "If you choose \"ON,\" all of the Super Arts",
    "will be available during a match.",
};
static s8* msgSYSDIR_110[1] = { "SUPER A. TO SUPER A. CANCEL...." };
static s8* msgSYSDIR_111[2] = {
    "If you choose \"ON,\" you'll be able to",
    "cancel a Super Art with another Super Art.",
};
static s8* msgSYSDIR_112[1] = { "AIR COMBO SKILL................" };
static s8* msgSYSDIR_113[1] = { "Adjust how easily you can perform Air Combos." }; //Presumably this has something to do with juggle counter values, so being vague here is fine IMO.
static s8* msgSYSDIR_114[1] = { "WHIFFED MOVE S.A. GAUGE BUILD.." };
static s8* msgSYSDIR_115[2] = { "Decide whether the S.A. Gauge builds", "when moves are whiffed." };
static s8* msgSYSDIR_200[1] = { "Return to the previous page." };
static s8* msgSYSDIR_201[1] = { "Exit this mode and return to the Menu Screen." };
static s8* msgSYSDIR_202[1] = { "Proceed to the next page." };

static s8** msgSysDirAdr[119] = {
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

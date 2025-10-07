#include "common.h"

static s8* mcard_msg_00[] = { NULL };

static s8* mcard_msg_01[] = {
    "\x7f",       "MEMORY CARD slot 1", "MEMORY CARD slot 2", "YES",         "NO",     "LOAD",
    "SAVE",       "AUTOLOAD",           "AUTOSAVE",           "load",        "saving", "autoload",
    "autosaving", "system data",        "direction data",     "replay data", NULL
};

static s8* mcard_msg_02[] = { "Which MEMORY CARD slot will you use?", NULL };

static s8* mcard_msg_03[] = { "Now loading the %f from",
                              "the memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s.",
                              "Do not remove the memory card (8MB)",
                              "(for PlayStationｮ2), reset, or switch",
                              "off the Console or remove controllers.",
                              NULL };

static s8* mcard_msg_04[] = {
    "ERROR: Loading the %f from", "the memory card (8MB) (for PlayStationｮ2)", "in MEMORY CARD slot %s failed.%r", NULL
};

static s8* mcard_msg_05[] = { "ERROR: The memory card (8MB) (for PlayStationｮ2)",
                              "cannot be found in MEMORY CARD slot %s.%r",
                              NULL };

static s8* mcard_msg_06[] = { "ERROR: The memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s is unformatted.%r",
                              NULL };

static s8* mcard_msg_07[] = {
    "ERROR: No %f can be found on", "the memory card (8MB) (for PlayStationｮ2)", "in MEMORY CARD slot %s.%r", NULL
};

static s8* mcard_msg_08[] = { "Accessing the memory card (8MB)",        "(for PlayStationｮ2) in MEMORY CARD slot %s.",
                              "Do not remove the memory card (8MB)",    "(for PlayStationｮ2), reset, or switch",
                              "off the Console or remove controllers.", NULL };

static s8* mcard_msg_09[] = { "ERROR: Accessing the memory card (8MB)",
                              "(for PlayStationｮ2) in MEMORY CARD slot %s failed.%r",
                              NULL };

static s8* mcard_msg_10[] = { "(NO USE)", NULL };
static s8* mcard_msg_11[] = { "(NO USE)", NULL };
static s8* mcard_msg_12[] = { "(NO USE)", NULL };
static s8* mcard_msg_13[] = { "(NO USE)", NULL };

static s8* mcard_msg_14[] = { "WARNING: The memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s is unformatted.",
                              "Will you format it?",
                              NULL };

static s8* mcard_msg_15[] = { "Now formatting the memory card (8MB)",   "(for PlayStationｮ2) in MEMORY CARD slot %s.",
                              "Do not remove the memory card (8MB)",    "(for PlayStationｮ2), reset, or switch",
                              "off the Console or remove controllers.", NULL };

static s8* mcard_msg_16[] = {
    "ERROR: Formatting the memory card", "(8MB) (for PlayStationｮ2)", "in MEMORY CARD slot %s failed.%r", NULL
};

static s8* mcard_msg_17[] = { "Please specify a %f to load.", NULL };
static s8* mcard_msg_18[] = { "Please specify a %f to save.", NULL };

static s8* mcard_msg_19[] = { "WARNING: A %f already exists on",
                              "the memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s.",
                              "Will you overwrite?",
                              NULL };

static s8* mcard_msg_20[] = { "Now %e the %f to",
                              "the memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s.",
                              "Do not remove the memory card (8MB)",
                              "(for PlayStationｮ2), reset, or switch",
                              "off the Console or remove controllers.",
                              NULL };

static s8* mcard_msg_21[] = {
    "%E the %f to", "the memory card (8MB) (for PlayStationｮ2)", "in the MEMORY CARD slot %s is complete.%r", NULL
};

static s8* mcard_msg_22[] = {
    "ERROR: %E the %f to", "the memory card (8MB) (for PlayStationｮ2)", "in the MEMORY CARD slot %s failed.%r", NULL
};

static s8* mcard_msg_23[] = { "ERROR: Insufficient space available on",
                              "the memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s.",
                              "At least %a KB of free space is required",
                              "to save data.%r",
                              NULL };

static s8* mcard_msg_24[] = { "(NO USE)", NULL };
static s8* mcard_msg_25[] = { "(NO USE)", NULL };

static s8* mcard_msg_26[] = {
    "WARNING: A %f has been %eed", "from memory card (8MB) (for PlayStationｮ2)", "in MEMORY CARD slot %s.%r", NULL
};

static s8* mcard_msg_27[] = { "ERROR: %Eing the %f from",
                              "the memory card (8MB) (for PlayStationｮ2)",
                              "in MEMORY CARD slot %s failed.",
                              "Would you like to start without saving?",
                              NULL };

static s8* mcard_msg_28[] = {
    "ERROR: The memory card (8MB) (for PlayStationｮ2)", "cannot be found in MEMORY CARD slot.",
    "At least %a KB of free space is required",         "to save data.",
    "Would you like to start without saving?",          NULL
};

static s8* mcard_msg_29[] = {
    "ERROR: Insufficient space on memory card (8MB)", "(for PlayStationｮ2) in MEMORY CARD slot %s.",
    "At least %a KB of free space is required",       "to save data.",
    "Would you like to start without saving?",        NULL
};

static s8* mcard_msg_30[] = { "ERROR: Insufficient space on memory card",
                              "(8MB) (for PlayStationｮ2) in either",
                              "MEMORY CARD slot <C2>1<C7> or MEMORY CARD slot <C2>2<C7>.",
                              "At least %a KB of free space is required",
                              "to save data.",
                              "Would you like to start without saving?",
                              NULL };

static s8* mcard_msg_31[] = {
    "The %f will be auto-saved to the", "memory card (8MB) (for PlayStationｮ2)", "in MEMORY CARD slot %s.%r", NULL
};

static s8* mcard_msg_32[] = { "ERROR: The memory card (8MB) (for PlayStationｮ2)",
                              "cannot be found in MEMORY CARD slot %s.",
                              "Auto-save has been turned off.%r",
                              NULL };

static s8* mcard_msg_33[] = { "ERROR: The memory card (8MB) (for PlayStationｮ2)",
                              "in the MEMORY CARD slot %s is changed.",
                              "Auto-save has been turned off.%r",
                              NULL };

static s8* mcard_msg_34[] = { "OK to quit without saving the system data?", NULL };

static s8** mcard_msg_tbl[] = { mcard_msg_00, mcard_msg_01, mcard_msg_02, mcard_msg_03, mcard_msg_04, mcard_msg_05,
                                mcard_msg_06, mcard_msg_07, mcard_msg_08, mcard_msg_09, mcard_msg_10, mcard_msg_11,
                                mcard_msg_12, mcard_msg_13, mcard_msg_14, mcard_msg_15, mcard_msg_16, mcard_msg_17,
                                mcard_msg_18, mcard_msg_19, mcard_msg_20, mcard_msg_21, mcard_msg_22, mcard_msg_23,
                                mcard_msg_24, mcard_msg_25, mcard_msg_26, mcard_msg_27, mcard_msg_28, mcard_msg_29,
                                mcard_msg_30, mcard_msg_31, mcard_msg_32, mcard_msg_33, mcard_msg_34 };

void* MessageTableENG[] = { mcard_msg_tbl };

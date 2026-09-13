/**
 * @file msgextra_en.c
 * Extra Option Menu Option Descriptors (English)
 */

#include "sf33rd/Source/Game/message/en/msgextra_en.h"
#include "common.h"

static char* msgEXTRA_000[1] = { "Change the Life Gauge settings." };
static char* msgEXTRA_001[1] = { "Adjust the vitality of Player 1." };
static char* msgEXTRA_002[1] = { "Adjust the vitality of Player 2." };
static char* msgEXTRA_003[1] = { "Change the guard type." };
static char* msgEXTRA_004[1] = { "Turn rapid fire on and off." };
static char* msgEXTRA_005[1] = { "Turn bonus stages on and off." };
static char* msgEXTRA_006[1] = { "Restore the initial settings." };
static char* msgEXTRA_008[1] = { "Select the Super Arts Gauge type for Player 1." };
static char* msgEXTRA_009[1] = { "Select the Super Arts Gauge type for Player 2." };
static char* msgEXTRA_010[2] = { "Change the number of the Super Arts Gauge", "stocks of Player 1." };
static char* msgEXTRA_011[2] = { "Change the number of the Super Arts Gauge", "stocks of Player 2." };
static char* msgEXTRA_012[2] = { "Adjust the Super Arts Gauge length of", "Player 1." };
static char* msgEXTRA_013[2] = { "Adjust the Super Arts Gauge length of", "Player 2." };
static char* msgEXTRA_014[1] = { "Adjust the rate that the Super Arts Gauge." };
static char* msgEXTRA_016[1] = { "Adjust the Stun Gauge length of Player 1." };
static char* msgEXTRA_017[1] = { "Adjust the Stun Gauge length of Player 2." };
static char* msgEXTRA_018[2] = { "Adjust the rate that the Stun Gauge", "increases." };
static char* msgEXTRA_019[2] = { "Adjust the rate that the Stun Gauge", "recovers." };
static char* msgEXTRA_024[2] = {
    "Show/Erase the screen text display. This",
    "includes the Timer, S.A. Gauge, etc.",
};
static char* msgEXTRA_025[1] = { "Show/Erase the Life Gauge." };
static char* msgEXTRA_026[1] = { "Show/Erase the Timer." };
static char* msgEXTRA_027[1] = { "Show/Erase the Stun Gauge." };
static char* msgEXTRA_028[1] = { "Show/Erase the Super Arts Gauge." };
static char* msgEXTRA_029[2] = { "Turn the \"PRESS START\" displayed", "onscreen on and off." };
static char* msgEXTRA_100[1] = { "Return to the previous page." };
static char* msgEXTRA_101[1] = { "Return to the Option Menu." };
static char* msgEXTRA_102[1] = { "Proceed to the next page." };
static char* msgEXTRA_xxx[1] = { "..." };

static char** msgExtraAdr[35] = { msgEXTRA_000, msgEXTRA_001, msgEXTRA_002, msgEXTRA_003, msgEXTRA_004, msgEXTRA_005,
                                  msgEXTRA_006, msgEXTRA_xxx, msgEXTRA_008, msgEXTRA_009, msgEXTRA_010, msgEXTRA_011,
                                  msgEXTRA_012, msgEXTRA_013, msgEXTRA_014, msgEXTRA_xxx, msgEXTRA_016, msgEXTRA_017,
                                  msgEXTRA_018, msgEXTRA_019, msgEXTRA_xxx, msgEXTRA_xxx, msgEXTRA_xxx, msgEXTRA_xxx,
                                  msgEXTRA_024, msgEXTRA_025, msgEXTRA_026, msgEXTRA_027, msgEXTRA_028, msgEXTRA_029,
                                  msgEXTRA_xxx, msgEXTRA_xxx, msgEXTRA_100, msgEXTRA_101, msgEXTRA_102 };

static s8 msgExtraCtr[35] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1,
                              2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1 };

MessageTable msgExtraTbl_usa = { msgExtraAdr, msgExtraCtr };

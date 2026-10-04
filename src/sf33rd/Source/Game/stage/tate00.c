/**
 * @file tate00.c
 * Main Background and Stage Animation Controller
 */

#include "sf33rd/Source/Game/stage/tate00.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg000.h"
#include "sf33rd/Source/Game/stage/bg010.h"
#include "sf33rd/Source/Game/stage/bg020.h"
#include "sf33rd/Source/Game/stage/bg030.h"
#include "sf33rd/Source/Game/stage/bg040.h"
#include "sf33rd/Source/Game/stage/bg050.h"
#include "sf33rd/Source/Game/stage/bg060.h"
#include "sf33rd/Source/Game/stage/bg070.h"
#include "sf33rd/Source/Game/stage/bg080.h"
#include "sf33rd/Source/Game/stage/bg090.h"
#include "sf33rd/Source/Game/stage/bg100.h"
#include "sf33rd/Source/Game/stage/bg120.h"
#include "sf33rd/Source/Game/stage/bg130.h"
#include "sf33rd/Source/Game/stage/bg140.h"
#include "sf33rd/Source/Game/stage/bg160.h"
#include "sf33rd/Source/Game/stage/bg170.h"
#include "sf33rd/Source/Game/stage/bg190.h"
#include "sf33rd/Source/Game/stage/bg200.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/bns_bg2.h"
#include "sf33rd/Source/Game/stage/bonus_bg.h"
#include "sf33rd/Source/Game/stage/ng/bg230.h"
#include "sf33rd/Source/Game/stage/ng/bg240.h"

void (*ta_move_tbl[AREA_COUNT])() = {
    [AREA_3S_GILL] = BG000,
    [AREA_3S_ALEX] = BG010,
    [AREA_3S_RYU] = BG020,
    [AREA_3S_YUN] = BG030,
    [AREA_3S_DUDLEY] = BG040,
    [AREA_3S_NECRO] = BG050,
    [AREA_3S_HUGO] = BG060,
    [AREA_3S_IBUKI] = BG070,
    [AREA_3S_ELENA] = BG080,
    [AREA_3S_ORO] = BG090,
    [AREA_3S_YANG] = BG100,
    [AREA_3S_KEN] = BG010,
    [AREA_3S_SEAN] = BG120,
    [AREA_3S_URIEN] = BG130,
    [AREA_3S_AKUMA] = BG140,
    [AREA_3S_SHIN_AKUMA] = BG140,
    [AREA_3S_CHUNLI] = BG160,
    [AREA_3S_MAKOTO] = BG170,
    [AREA_3S_Q] = BG190,
    [AREA_3S_TWELVE] = BG190,
    [AREA_3S_REMY] = BG200,
    [AREA_3S_BONUS_CAR] = Bonus_bg,
    [AREA_3S_BONUS_BALLS] = Bonus_bg2,
    [AREA_NG_GILL] = BG230,
    [AREA_NG_ALEX] = BG240,
};

void ta0_init00();
void ta0_init01();
void ta0_init02();
void ta0_move();

void TATE00() {
    void (*jump_tbl[4])() = { ta0_init00, ta0_init01, ta0_init02, ta0_move };

    if (Game_pause & 0x80) {
        return;
    }

    jump_tbl[bg_w.bg_routine]();
    Scrn_Renew();
    Irl_Family();
    Irl_Scrn();
}

void ta0_init00() {
    bg_w.bg_routine++;

    // Calling this function is necessary for Random_ix16 to be in sync with the arcade version
    random_16();

    bg_initialize();
}

void ta0_init01() {
    bg_w.bg_routine++;
    akebono_initialize();
    ta_move_tbl[bg_w.bg_index]();
}

void ta0_init02() {
    bg_w.bg_routine++;
    ta_move_tbl[bg_w.bg_index]();
}

void ta0_move() {
    ta_move_tbl[bg_w.bg_index]();

    if (bg_w.quake_x_index > 0) {
        bg_w.quake_x_index--;
    }

    if (bg_w.quake_y_index > 0) {
        bg_w.quake_y_index--;
    }
}

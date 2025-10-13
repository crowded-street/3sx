/**
 * @file tate00.c
 * Main Background and Stage Animation Controller
 */

#include "sf33rd/Source/Game/stage/tate00.h"
#include "common.h"
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
#include "sf33rd/Source/Game/stage/bg150.h"
#include "sf33rd/Source/Game/stage/bg160.h"
#include "sf33rd/Source/Game/stage/bg180.h"
#include "sf33rd/Source/Game/stage/bg190.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/bns_bg2.h"
#include "sf33rd/Source/Game/stage/bonus_bg.h"
#include "sf33rd/Source/Game/workuser.h"

void (*ta_move_tbl[22])() = { BG000, BG010, BG020, BG030, BG040, BG050, BG060, BG070, BG080, BG090,    BG100,
                              BG010, BG120, BG130, BG140, BG150, BG160, BG180, BG180, BG190, Bonus_bg, Bonus_bg2 };

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

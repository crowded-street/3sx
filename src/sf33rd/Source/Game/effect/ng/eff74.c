/**
 * @file eff74.c
 * Bats hanging from the ceiling of Oro's cave from New Generation
 */

#include "sf33rd/Source/Game/effect/ng/eff74.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/stage/ta_sub.h"

#define COLOR_CODE 0x2144 // NG 0x2058
#define BAT_COUNT 4
#define CHAR_HANGING 34
#define CHAR_STILL 35
#define NOTICE_RANGE 224
#define NOTICE_DELAY 48
#define RIGHT_LIMIT 400
#define LEFT_LIMIT -416

typedef struct BatPerch {
    s16 x;
    s16 y;
    s16 priority;
} BatPerch;

static const BatPerch perches[BAT_COUNT] = {
    { 567, 288, 80 },
    { 396, 320, 79 },
    { 502, 337, 79 },
    { 535, 313, 79 },
};

typedef enum FlightKind {
    FLIGHT_FLAP,
    FLIGHT_RIGHT,
    FLIGHT_LEFT,
} FlightKind;

typedef struct Flight {
    FlightKind kind;
    s16 char_index;
    s32 speed_x;
    s32 speed_y;
    s32 accel_x;
    s32 accel_y;
} Flight;

static const Flight flights[8] = {
    { FLIGHT_FLAP, 33, 0, 0, 0, 0 },
    { FLIGHT_RIGHT, 32, 0x48000, 0x200, -0x8000, -0x100 },
    { FLIGHT_LEFT, 36, -0x48000, -0x100, 0, -0x100 },
    { FLIGHT_LEFT, 36, -0x68000, -0x200, 0, -0x100 },
    { FLIGHT_RIGHT, 32, 0x78000, 0, 0x100, 0 },
    { FLIGHT_FLAP, 37, 0, 0, 0, 0 },
    { FLIGHT_RIGHT, 32, 0x78000, -0x28000, 0, 0x2000 },
    { FLIGHT_LEFT, 36, -0x78000, -0x48000, -0x400, 0x3000 },
};

static void start_flight(WORK_Other* ewk, s16 index) {
    const Flight* flight = &flights[index];

    ewk->wu.routine_no[1] = flight->kind;
    ewk->wu.char_index = flight->char_index;
    ewk->wu.mvxy.a[0].sp = flight->speed_x;
    ewk->wu.mvxy.a[1].sp = flight->speed_y;
    ewk->wu.mvxy.d[0].sp = flight->accel_x;
    ewk->wu.mvxy.d[1].sp = flight->accel_y;
    set_char_move_init(&ewk->wu, 0, ewk->wu.char_index);
}

static bool is_fighter_near(WORK_Other* ewk) {
    return is_near(plw[0].wu.position_y, ewk->wu.position_y, NOTICE_RANGE) ||
           is_near(plw[1].wu.position_y, ewk->wu.position_y, NOTICE_RANGE);
}

static void eff74_hang(WORK_Other* ewk) {
    ewk->wu.routine_no[0]++;
    ewk->wu.disp_flag = 1;
    set_char_move_init(&ewk->wu, 0, CHAR_HANGING);
    ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[0];
    ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[1];
}

static void eff74_watch_1(WORK_Other* ewk) {
    if (!is_fighter_near(ewk)) {
        char_move(&ewk->wu);
        return;
    }

    ewk->wu.routine_no[0]++;
    start_flight(ewk, random_16() & 7);
}

static void eff74_watch(WORK_Other* ewk, bool (*flees)(s16 roll)) {
    s16 roll;

    switch (ewk->wu.routine_no[1]) {
    case 0:
        if (!is_fighter_near(ewk)) {
            char_move(&ewk->wu);
            break;
        }

        roll = random_16();

        if (flees(roll)) {
            ewk->wu.routine_no[0]++;
            start_flight(ewk, roll & 7);
            break;
        }

        ewk->wu.routine_no[1]++;
        ewk->wu.old_rno[2] = NOTICE_DELAY;
        break;

    case 1:
        ewk->wu.old_rno[2]--;

        if (ewk->wu.old_rno[2] < 0) {
            ewk->wu.routine_no[1] = 0;
        }

        break;
    }
}

static bool flees_on_odd_roll(s16 roll) {
    return roll & 1;
}

static bool flees_on_high_roll(s16 roll) {
    return roll > 10;
}

static void eff74_watch_2(WORK_Other* ewk) {
    eff74_watch(ewk, flees_on_odd_roll);
}

static void eff74_watch_3(WORK_Other* ewk) {
    eff74_watch(ewk, flees_on_high_roll);
}

static void eff74_fly(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[1]) {
    case FLIGHT_FLAP:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type != 0) {
            ewk->wu.routine_no[0] = 0;
        }

        break;

    case FLIGHT_RIGHT:
        char_move(&ewk->wu);
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos > RIGHT_LIMIT) {
            ewk->wu.routine_no[0] = 0;
        }

        break;

    case FLIGHT_LEFT:
        char_move(&ewk->wu);
        add_x_sub_ng(&ewk->wu);
        add_y_sub_ng(&ewk->wu);

        if (ewk->wu.xyz[0].disp.pos < LEFT_LIMIT) {
            ewk->wu.routine_no[0] = 0;
        }

        break;
    }
}

static void eff74_still(WORK_Other* ewk) {
    if (ewk->wu.routine_no[0] == 0) {
        ewk->wu.routine_no[0]++;
        ewk->wu.disp_flag = 1;
        set_char_move_init(&ewk->wu, 0, CHAR_STILL);
    }
}

void effect_ng74_move(WORK_Other* ewk) {
    void (*const watchers[4])(WORK_Other*) = { NULL, eff74_watch_1, eff74_watch_2, eff74_watch_3 };

    if (compel_dead_check(ewk)) {
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        return;
    }

    if (!EXE_flag && !Game_pause) {
        if (ewk->wu.type == 0) {
            eff74_still(ewk);
        } else {
            switch (ewk->wu.routine_no[0]) {
            case 0:
                eff74_hang(ewk);
                break;

            case 1:
                watchers[ewk->wu.type](ewk);
                break;

            case 2:
                eff74_fly(ewk);
                break;
            }
        }
    }

    disp_pos_trans_entry_rs_ng(ewk);
}

s32 effect_ng74_init() {
    for (s16 i = 0; i < BAT_COUNT; i++) {
        const BatPerch* perch = &perches[i];
        WORK_Other* ewk;
        s16 ix;

        if ((ix = pull_effect_work(4)) == -1) {
            return -1;
        }

        ewk = (WORK_Other*)frw[ix];
        ewk->wu.be_flag = 1;
        ewk->wu.id = EFFECT_NG74_ID;
        ewk->wu.work_id = 16;
        ewk->wu.cgromtype = 1;
        ewk->wu.rl_flag = 0;
        ewk->wu.my_col_mode = 0x4200;
        ewk->wu.type = i;
        ewk->wu.dead_f = 1;
        ewk->wu.sync_suzi = 0;
        ewk->wu.my_family = 2;
        ewk->wu.my_col_code = COLOR_CODE;
        ewk->wu.char_table[0] = _ng_brz_char_table;
        ewk->wu.xyz[0].disp.pos = ewk->wu.old_rno[0] = perch->x;
        ewk->wu.xyz[1].disp.pos = ewk->wu.old_rno[1] = perch->y;
        ewk->wu.my_priority = ewk->wu.position_z = perch->priority;
        ewk->wu.my_mts = 7;
        ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
        suzi_offset_set(ewk);
    }

    return 0;
}

/**
 * @file effd3.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effd3.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff20.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effg8.h"
#include "sf33rd/Source/Game/effect/effl9.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

// forward declarations
const s16 ake_timer_tbl[13];
const u32 ake_color[13];

void effect_D3_move(WORK_Other* ewk) {
    if (ewk->wu.type == 0) {
        akebono_finish(ewk);
    } else {
        syungoku_finish(ewk);
    }
}

static void set_stage_backgrounds_D3(void (*set_background)(u16)) {
    u16 bg = ake_bg_off[bg_w.stage];
    u16 mask = 1 & 0xFFFF;
    u16 assign;
    s16 i;

    for (i = 0; i < 4; i++, assign = mask *= 2) {
        if (bg & mask) {
            set_background(1 << i);
        }
    }
}

static void initialize_akebono_finish(WORK_Other* ewk) {
    ewk->wu.routine_no[0] += 1;
    ewk->wu.old_rno[1] = 0;
    ewk->wu.dir_timer = ake_timer_tbl[0];
    ewk->wu.disp_flag = 0;
    set_stage_backgrounds_D3(Bg_Off_R);
    Bg_On_R(8);
    akebono_flag = 1;
    Sound_SE(117);
    Scrn_Move_Set(3, 192 - bg_w.pos_offset, 16);
    bg_w.bgw[3].position_x = 192 - bg_w.pos_offset;
    bg_w.bgw[3].position_y = 16;
    overwrite_panel(ake_color[ewk->wu.old_rno[1]], 69);
}

static void advance_akebono_flash(WORK_Other* ewk) {
    ewk->wu.dir_timer -= 1;

    if ((ewk->wu.dir_timer) < 0) {
        akebono_flag = 1;
        ewk->wu.old_rno[1] += 1;

        if (ewk->wu.old_rno[1] < 4) {
            ewk->wu.dir_timer = ake_timer_tbl[ewk->wu.old_rno[1]];
        } else {
            ewk->wu.routine_no[0] += 1;
            effect_G8_init();
        }
    }

    overwrite_panel(ake_color[ewk->wu.old_rno[1]], 69);
    ake_Family_Set2();
}

static void fade_akebono_finish(WORK_Other* ewk) {
    ewk->wu.dir_timer -= 1;

    if ((ewk->wu.dir_timer) < 0) {
        ewk->wu.old_rno[1] += 1;
        ewk->wu.dir_timer = ake_timer_tbl[ewk->wu.old_rno[1]];

        if (ewk->wu.old_rno[1] == 12) {
            ewk->wu.routine_no[0] += 1;
            Bg_Off_R(8);
        }
    }

    overwrite_panel(ake_color[ewk->wu.old_rno[1]], 69);
    ake_Family_Set2();
}

static void restore_akebono_background(WORK_Other* ewk) {
    ewk->wu.dir_timer -= 1;

    if (ewk->wu.dir_timer == 0) {
        set_stage_backgrounds_D3(Bg_On_R);
    }

    if (ewk->wu.dir_timer < 0) {
        ewk->wu.routine_no[0] += 1;
        akebono_flag = 0;
        return;
    }

    overwrite_panel(ake_color[0], 69);
}

void akebono_finish(WORK_Other* ewk) {
    s16 i;

    for (i = 0; i < 3; i++) {
        scr_calc(i);
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        initialize_akebono_finish(ewk);
        break;

    case 1:
        advance_akebono_flash(ewk);
        break;

    case 2:
        fade_akebono_finish(ewk);
        break;

    case 3:
        restore_akebono_background(ewk);
        break;

    case 4:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

static void show_syungoku_panel(s16 position_x) {
    overwrite_panel(0xFF000000, 70);
    bg_w.bgw[3].position_x = position_x - bg_w.pos_offset;
    bg_w.bgw[3].position_y = 0;
    Bg_Family_Set_appoint(3);
}

static void initialize_syungoku_finish(WORK_Other* ewk) {
    ewk->wu.routine_no[0] += 1;
    akebono_flag = 1;
    Extra_Break = 1;
    Sound_SE(126);
    plw[0].wu.disp_flag = 0;
    plw[1].wu.disp_flag = 0;
    Pause_Hit_Marks = 1;
    set_stage_backgrounds_D3(Bg_Off_R);
    show_syungoku_panel(256);
    ewk->wu.dir_timer = 2;
}

static void start_syungoku_effects(WORK_Other* ewk) {
    ewk->wu.dir_timer -= 1;

    if (ewk->wu.dir_timer <= 0) {
        ewk->wu.routine_no[0] += 1;
        ewk->wu.old_rno[0] = 0;
        effect_20_init(ewk);
        effect_L9_init(ewk, 0);
        effect_L9_init(ewk, 1);
    }

    show_syungoku_panel(768);
}

static void wait_for_syungoku_effects(WORK_Other* ewk) {
    if (ewk->wu.old_rno[0]) {
        ewk->wu.routine_no[0] += 1;
        akebono_flag = 0;
        plw[0].wu.disp_flag = 1;
        plw[1].wu.disp_flag = 1;
        Pause_Hit_Marks = 0;
        set_stage_backgrounds_D3(Bg_On_R);
    }

    show_syungoku_panel(768);
}

static void conclude_syungoku_finish(WORK_Other* ewk) {
    ewk->wu.routine_no[0] += 1;
    Conclusion_Flag = 1;
    Conclusion_Type = 0;
    request_center_message(0);
    bg_w.bgw[3].position_x = 256 - bg_w.pos_offset;
    bg_w.bgw[3].position_y = 0;
    Bg_Family_Set_appoint(3);
}

void syungoku_finish(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        initialize_syungoku_finish(ewk);
        break;

    case 1:
        start_syungoku_effects(ewk);
        break;

    case 2:
        wait_for_syungoku_effects(ewk);
        break;

    case 3:
        conclude_syungoku_finish(ewk);
        break;

    default:
        akebono_flag = 0;
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_D3_init(u8 ake_type) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.id = 133;
    ewk->wu.be_flag = 1;
    ewk->wu.work_id = 16;
    ewk->wu.cgromtype = 1;
    ewk->wu.old_rno[1] = 0;
    ewk->wu.type = ake_type;
    return 0;
}

const s16 ake_timer_tbl[13] = { 4, 4, 4, 4, 4, 10, 10, 10, 10, 10, 10, 10, 20 };

const u32 ake_color[13] = { 0xFFFFFFFF, 0x10FFFFFF, 0xFFFFFFFF, 0x10FFFFFF, 0xFFFFFFFF, 0x10FFFFFF, 0x28FFFFFF,
                            0x40FFFFFF, 0x60FFFFFF, 0x88FFFFFF, 0xB0FFFFFF, 0xD8FFFFFF, 0xFFFFFFFF };

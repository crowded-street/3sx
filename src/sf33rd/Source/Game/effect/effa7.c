/**
 * @file effa7.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/effa7.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sound/se_data.h"
#include "sf33rd/Source/Game/stage/bg.h"

static s32 game_is_active(void) {
    return EXE_flag == 0 && Game_pause == 0;
}

static void update_active_hit_mark_A7(WORK_Other* ewk) {
    if (ewk->wu.dead_f == 1 || Suicide[6] != 0) {
        ewk->wu.disp_flag = 0;
        ewk->wu.routine_no[0]++;
        return;
    }

    if (Pause_Hit_Marks) {
        return;
    }

    if (game_is_active()) {
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 0xFF) {
            ewk->wu.disp_flag = 0;
            ewk->wu.routine_no[0]++;
            return;
        }

        if (ewk->wu.scr_mv_x && --ewk->wu.scr_mv_x == 0) {
            bg_w.quake_y_index = ewk->wu.scr_mv_y;
            pp_screen_quake(bg_w.quake_y_index);
        }
    }

    sort_push_request8(&ewk->wu);
}

static void configure_hit_mark_A7(WORK_Other* ewk, const HMDT* tad) {
    if (tad->status & 8) {
        ewk->wu.disp_flag = 2;
    } else {
        ewk->wu.disp_flag = 1;
    }

    if (tad->status & 0x40) {
        if (((WORK*)ewk->wu.target_adrs)->work_id == 1) {
            ewk->wu.dir_timer = ((PLW*)ewk->wu.target_adrs)->player_number;
        } else {
            ewk->wu.dir_timer = ((WORK_Other*)ewk->wu.target_adrs)->master_player;
        }
    }

    if (tad->col) {
        ewk->wu.my_col_code = hcct[tad->col];
    } else if (tad->status & 0x80) {
        ewk->wu.my_col_code = ((WORK*)ewk->wu.target_adrs)->my_col_code + 1;
    }
}

static void play_hit_mark_sound_A7(WORK_Other* ewk, const HMDT* tad) {
    if (tad->se) {
        sound_effect_request[tad->se](ewk, tad->se);
        Last_Called_SE = tad->se;
    } else {
        Last_Called_SE = 0;
    }
}

static const EXPLEM* apply_extended_hit_mark_offset_A7(WORK_Other* ewk, const HMDT* tad) {
    const EXPLEM* edt;

    if (tad->status & 0x20) {
        edt = &explem2[tad->emhix][ewk->wu.dir_timer];
    } else {
        edt = &explem[tad->myhix];
    }

    if (ewk->wu.rl_flag) {
        ewk->wu.xyz[0].disp.pos -= *(s16*)&edt->hx;
    } else {
        ewk->wu.xyz[0].disp.pos += *(s16*)&edt->hx;
    }

    ewk->wu.xyz[1].disp.pos += *(s16*)&edt->hy;
    return edt;
}

static void snapshot_hit_mark_position_A7(WORK_Other* ewk, const HMDT* tad) {
    ewk->wu.scr_mv_x = gqdt[tad->quake][0];
    ewk->wu.scr_mv_y = gqdt[tad->quake][1];
    ewk->wu.position_x = ewk->wu.xyz[0].disp.pos;
    ewk->wu.position_y = ewk->wu.xyz[1].disp.pos;
    ewk->wu.position_z = ewk->wu.xyz[2].disp.pos;
}

static const EXPLEM* position_hit_mark_A7(WORK_Other* ewk, const HMDT* tad) {
    const EXPLEM* edt = NULL;

    if (tad->status & 0x10) {
        edt = apply_extended_hit_mark_offset_A7(ewk, tad);
    }

    if (tad->status & 2) {
        ewk->wu.xyz[0].disp.pos += random_16() - 7;
        ewk->wu.xyz[1].disp.pos += (random_16() & 7) - 3;
    }

    snapshot_hit_mark_position_A7(ewk, tad);
    return edt;
}

static void select_directional_hit_mark_A7(WORK_Other* ewk, const HMDT* tad) {
    ewk->wu.dir_old = 0;

    if (tad->dir) {
        ewk->wu.dir_old = hit_mark_dir_table[ewk->wu.direction];

        if (ewk->wu.dir_old < 0) {
            ewk->wu.rl_flag = 1;
            ewk->wu.dir_old = -ewk->wu.dir_old;
        }
    }

    set_char_move_init(&ewk->wu, 0, tad->hits + ewk->wu.dir_old);
}

static void initialize_hit_mark_animation_A7(WORK_Other* ewk, const HMDT* tad, const EXPLEM* edt) {
    if (tad->status & 0x10) {
        set_char_move_init(&ewk->wu, 0, edt->chix);
        return;
    }

    select_directional_hit_mark_A7(ewk, tad);
}

static void configure_special_hit_mark_A7(WORK_Other* ewk) {
    if (ewk->wu.char_index != 75) {
        return;
    }

    ewk->wu.my_mr_flag = 1;
    ewk->wu.my_mr.size.x = 127;
    ewk->wu.my_mr.size.y = 63;
    ewk->wu.my_col_code |= (ewk->master_id == 1) * 16;
}

void effect_A7_move(WORK_Other* ewk) {
    const HMDT* tad;
    const EXPLEM* edt;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        *ewk->wu.char_table = _ef01_char_table;
        tad = &hmdt[ewk->wu.kohm];

        if (tad->hits == 0) {
            play_hit_mark_sound_A7(ewk, tad);

            if (tad->quake != 0) {
                bg_w.quake_y_index = gqdt[tad->quake][1];
                pp_screen_quake(bg_w.quake_y_index);
            }

            push_effect_work(&ewk->wu);
            break;
        }

        configure_hit_mark_A7(ewk, tad);
        play_hit_mark_sound_A7(ewk, tad);

        edt = position_hit_mark_A7(ewk, tad);
        initialize_hit_mark_animation_A7(ewk, tad, edt);
        configure_special_hit_mark_A7(ewk);

        if (!Pause_Hit_Marks) {
            sort_push_request8(&ewk->wu);
        }

        break;

    case 1:
        update_active_hit_mark_A7(ewk);
        break;

    case 2:
        ewk->wu.routine_no[0] = 3;
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_A7_init(PLW* wk) {
    WORK_Other* ewk;
    PLW* twk;
    s16 ix;

    if (wk->wu.work_id != 1) {
        return -1;
    }

    if ((ix = pull_effect_work(2)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    twk = (PLW*)wk->wu.target_adrs;
    ewk->wu.be_flag = 1;
    ewk->wu.id = 107;
    ewk->wu.work_id = 64;
    ewk->wu.rl_flag = wk->wu.rl_flag;
    ewk->wu.kohm = wk->wu.hm_dm_side;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.my_family = 2;
    ewk->wu.my_mr_flag = 0;
    ewk->wu.xyz[0].disp.pos = ewk->wu.position_x = wk->wu.xyz[0].disp.pos;
    ewk->wu.xyz[1].disp.pos = ewk->wu.position_y = wk->wu.xyz[1].disp.pos;
    ewk->wu.xyz[2].disp.pos = 26;
    ewk->my_master = wk->wu.target_adrs;
    ewk->master_id = twk->wu.id;
    ewk->master_work_id = twk->wu.work_id;
    ewk->wu.target_adrs = wk;
    ewk->wu.my_col_code = twk->wu.my_col_code + 1;
    ewk->master_player = ewk->wu.dir_timer = twk->player_number;
    ewk->wu.blink_timing = ewk->master_id;
    return 0;
}

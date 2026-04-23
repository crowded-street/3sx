#if STATCHECK

#include "test/test_runner_compare.h"
#include "arcade/arcade_constants.h"
#include "constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "test/test_runner_utils.h"
#include "types.h"

#include <SDL3/SDL_endian.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>

typedef struct Position {
    s16 x;
    s16 y;
} Position;

// Data reading

static Sint64 calc_plw_offset(int player) {
    return PLW_OFFSET + player * PLW_SIZE;
}

static Position read_position(SDL_IOStream* io, int player) {
    const Sint64 xyz_offset = calc_plw_offset(player) + WORK_XYZ_OFFSET;
    const Sint64 x_offset = xyz_offset;
    const Sint64 y_offset = x_offset + sizeof(XY);
    return (Position) { .x = read_s16(io, x_offset), .y = read_s16(io, y_offset) };
}

static Position get_position(int player) {
    const XY* xyz = plw[player].wu.xyz;
    return (Position) { .x = xyz[0].disp.pos, .y = xyz[1].disp.pos };
}

static u8 read_allow_a_battle_f(SDL_IOStream* io) {
    return read_u8(io, ALLOW_A_BATTLE_F_OFFSET);
}

static u16 read_game_timer(SDL_IOStream* io) {
    return read_u16(io, GAME_TIMER_OFFSET);
}

static void read_wcp(SDL_IOStream* io, WORK_CP dst[2]) {
    SDL_SeekIO(io, WCP_OFFSET, SDL_IO_SEEK_SET);
    SDL_ReadIO(io, dst, sizeof(wcp));

    for (int i = 0; i < 2; i++) {
        WORK_CP* w = &dst[i];

        w->sw_lvbt = SDL_Swap16BE(w->sw_lvbt);
        w->sw_new = SDL_Swap16BE(w->sw_new);
        w->sw_old = SDL_Swap16BE(w->sw_old);
        w->sw_now = SDL_Swap16BE(w->sw_now);
        w->sw_off = SDL_Swap16BE(w->sw_off);
        w->sw_chg = SDL_Swap16BE(w->sw_chg);
        w->old_now = SDL_Swap16BE(w->old_now);
        w->lgp = SDL_Swap16BE(w->lgp);

        for (int j = 0; j < 56; j++) {
            w->waza_flag[j] = SDL_Swap16BE(w->waza_flag[j]);
            w->reset[j] = SDL_Swap16BE(w->reset[j]);
            w->btix[j] = SDL_Swap16BE(w->btix[j]);

            for (int k = 0; k < 4; k++) {
                w->exdt[j][k] = SDL_Swap16BE(w->exdt[j][k]);
            }
        }
    }
}

static void read_waza_work(SDL_IOStream* io, WAZA_WORK dst[2][56]) {
    SDL_SeekIO(io, WAZA_WORK_OFFSET, SDL_IO_SEEK_SET);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 56; j++) {
            WAZA_WORK* wk = &dst[i][j];

            SDL_ReadS16BE(io, &wk->w_type);
            SDL_ReadS16BE(io, &wk->w_int);
            SDL_ReadS16BE(io, &wk->free1);
            SDL_ReadS16BE(io, &wk->w_lvr);

            u32 w_ptr;
            SDL_ReadU32BE(io, &w_ptr);
            wk->w_ptr = (s16*)(uintptr_t)w_ptr;

            SDL_ReadS16BE(io, &wk->free2);
            SDL_ReadS16BE(io, &wk->w_dead);
            SDL_ReadS16BE(io, &wk->w_dead2);
            SDL_ReadS16BE(io, &wk->uni0.tame.flag);
            SDL_ReadS16BE(io, &wk->uni0.tame.shot_flag);
            SDL_ReadS16BE(io, &wk->uni0.tame.shot_flag2);
            SDL_ReadS16BE(io, &wk->free3);
            SDL_ReadS16BE(io, &wk->shot_ok);
        }
    }
}

static void read_t_pl_lvr(SDL_IOStream* io, T_PL_LVR dst[2]) {
    SDL_SeekIO(io, T_PL_LVR_OFFSET, SDL_IO_SEEK_SET);

    u16* ptr = dst;

    // T_PL_LVR consists of 16-bit ints. We need to read sizeof(T_PL_LVR) / 2 * 2 such ints
    for (int i = 0; i < sizeof(T_PL_LVR); i++) {
        SDL_ReadU16BE(io, ptr);
        ptr++;
    }
}

// Comparison

static void compare_main_values(SDL_IOStream* io) {
    const u8 allow_a_battle_f_cps3 = read_allow_a_battle_f(io);
    stop_if(Allow_a_battle_f != allow_a_battle_f_cps3);

    const u8 round_timer_cps3 = read_u8(io, ROUND_TIMER_OFFSET);
    stop_if(round_timer != round_timer_cps3);

    for (int i = 0; i < 2; i++) {
        const Sint64 plw_offset = calc_plw_offset(i);

        const Position pos_3sx = get_position(i);
        const Position pos_cps3 = read_position(io, i);
        stop_if(pos_3sx.x != pos_cps3.x);
        stop_if(pos_3sx.y != pos_cps3.y);

        // if (i == 0) {
        //     printf("🔴 %llu pos x: %d vs %d\n", frame, pos_cps3.x, pos_3sx.x);
        // }

        const s16 vital_new_3sx = plw[i].wu.vital_new;
        const s16 vital_new_cps3 = read_s16(io, plw_offset + WORK_VITAL_NEW_OFFSET);
        stop_if(vital_new_3sx != vital_new_cps3);

        const s16 stun_3sx = piyori_type[i].now.quantity.h;
        const s16 stun_cps3 = read_s16(io, PIYORI_TYPE_OFFSET + i * sizeof(PiyoriType) + offsetof(PiyoriType, now));
        stop_if(stun_3sx != stun_cps3);

        const s16 sa_gauge_3sx = super_arts[i].gauge.s.h;
        const s16 sa_gauge_cps3 = read_s16(io, SUPER_ARTS_WORK_OFFSET + i * sizeof(SA_WORK) + offsetof(SA_WORK, gauge));
        stop_if(sa_gauge_3sx != sa_gauge_cps3);

        const s16 sa_store_3sx = super_arts[i].store;
        const s16 sa_store_cps3 = read_s16(io, SUPER_ARTS_WORK_OFFSET + i * sizeof(SA_WORK) + offsetof(SA_WORK, store));
        stop_if(sa_store_3sx != sa_store_cps3);
    }
}

static void compare_service_values(SDL_IOStream* io, bool compare_characters, Uint64 frame) {
    const u16 game_timer_cps3 = read_game_timer(io);
    stop_if(Game_timer != game_timer_cps3);

    const s16 counter_hi_cps3 = read_s16(io, COUNTER_HI_OFFSET);
    stop_if(Counter_hi != counter_hi_cps3);

    const s16 counter_low_cps3 = read_s16(io, COUNTER_LOW_OFFSET);
    stop_if(Counter_low != counter_low_cps3);

    const s16 random_ix16_cps3 = read_s16(io, RANDOM_IX_16_OFFSET);
    // This is dirty, but syncing Random_ix16 every frame helps avoid animation-related desyncs
    Random_ix16 = random_ix16_cps3;

    const s16 random_ix32_cps3 = read_s16(io, RANDOM_IX_32_OFFSET);
    stop_if(Random_ix32 != random_ix32_cps3);

    const u8 cmb_stock_0_cps3 = read_u8(io, CMB_STOCK_OFFSET);
    const u8 cmb_stock_1_cps3 = read_u8(io, CMB_STOCK_OFFSET + 1);
    stop_if(cmb_stock[0] != cmb_stock_0_cps3);
    stop_if(cmb_stock[1] != cmb_stock_1_cps3);

    const u8 cmb_all_stock_cps3 = read_u8(io, CMB_ALL_STOCK_OFFSET);
    stop_if(cmb_all_stock[0] != cmb_all_stock_cps3);

    for (int i = 0; i < 4; i++) {
        const u16 c_no_cps3 = read_u16(io, C_NO_OFFSET + i * sizeof(u16));
        stop_if(C_No[i] != c_no_cps3);

        const u16 g_no_cps3 = read_u16(io, G_NO_OFFSET + i * sizeof(u16));

        if (i != 0) {
            stop_if(G_No[i] != g_no_cps3);
        }
    }

    if (!compare_characters) {
        return;
    }

    for (int i = 0; i < 2; i++) {
        const Sint64 plw_offset = calc_plw_offset(i);

        // const u32 curr_rca_cps3 = read_u32(io, plw_offset + WORK_CURR_RCA_OFFSET);
        // printf("%llu curr_rca: 0x%x\n", frame, curr_rca_cps3);

        const u8 caution_flag_3sx = plw[i].caution_flag;
        const u8 caution_flag_cps3 = read_u8(io, plw_offset + PLW_CAUTION_FLAG_OFFSET);
        stop_if(caution_flag_3sx != caution_flag_cps3);

        const u8 do_not_move_3sx = plw[i].do_not_move;
        const u8 do_not_move_cps3 = read_u8(io, plw_offset + PLW_DO_NOT_MOVE_OFFSET);
        stop_if(do_not_move_3sx != do_not_move_cps3);

        for (int j = 0; j < 8; j++) {
            const s16 routine_no_3sx = plw[i].wu.routine_no[j];
            const s16 routine_no_cps3 = read_s16(io, plw_offset + WORK_ROUTINE_NO_OFFSET + j * 2);
            stop_if(routine_no_3sx != routine_no_cps3);
        }

        const s16 dm_stop_3sx = plw[i].wu.dm_stop;
        const s16 dm_stop_cps3 = read_s16(io, plw_offset + WORK_DM_STOP_OFFSET);
        stop_if(dm_stop_3sx != dm_stop_cps3);

        const s16 hit_stop_3sx = plw[i].wu.hit_stop;
        const s16 hit_stop_cps3 = read_s16(io, plw_offset + WORK_HIT_STOP_OFFSET);
        stop_if(hit_stop_3sx != hit_stop_cps3);

        const u8 sa_stop_flag_3sx = plw[i].sa_stop_flag;
        const u8 sa_stop_flag_cps3 = read_u8(io, plw_offset + PLW_SA_STOP_FLAG_OFFSET);
        stop_if(sa_stop_flag_3sx != sa_stop_flag_cps3);

        // const u16 cg_ix_cps3 = read_u16(io, plw_offset + WORK_CG_IX_OFFSET);
        // const u16 cg_ix_3sx = plw[i].wu.cg_ix;
        // stop_if(cg_ix_cps3 != cg_ix_3sx);

        const u16 cg_add_xy_cps3 = read_u16(io, plw_offset + WORK_CG_ADD_XY_OFFSET);
        const u16 cg_add_xy_3sx = plw[i].wu.cg_add_xy;
        stop_if(cg_add_xy_3sx != cg_add_xy_cps3);
    }
}

static void compare_lvr(SDL_IOStream* io) {
    T_PL_LVR t_pl_lvr_cps3[2];
    read_t_pl_lvr(io, t_pl_lvr_cps3);

    for (int i = 0; i < 2; i++) {
        const T_PL_LVR* lvr_cps3 = &t_pl_lvr_cps3[i];
        const T_PL_LVR* lvr_3sx = &t_pl_lvr[i];

        stop_if(lvr_3sx->sw_new != lvr_cps3->sw_new);
        stop_if(lvr_3sx->sw_old != lvr_cps3->sw_old);
        stop_if(lvr_3sx->sw_chg != lvr_cps3->sw_chg);
        stop_if(lvr_3sx->sw_now != lvr_cps3->sw_now);
        stop_if(lvr_3sx->old_now != lvr_cps3->old_now);
        stop_if(lvr_3sx->now_lvbt != lvr_cps3->now_lvbt);
        stop_if(lvr_3sx->old_lvbt != lvr_cps3->old_lvbt);
        stop_if(lvr_3sx->new_lvbt != lvr_cps3->new_lvbt);
        stop_if(lvr_3sx->sw_lever != lvr_cps3->sw_lever);
        stop_if(lvr_3sx->shot_up != lvr_cps3->shot_up);
        stop_if(lvr_3sx->shot_down != lvr_cps3->shot_down);
        stop_if(lvr_3sx->shot_ud != lvr_cps3->shot_ud);
        stop_if(lvr_3sx->lvr_status != lvr_cps3->lvr_status);
        stop_if(lvr_3sx->jaku_cnt != lvr_cps3->jaku_cnt);
        stop_if(lvr_3sx->chuu_cnt != lvr_cps3->chuu_cnt);
        stop_if(lvr_3sx->kyou_cnt != lvr_cps3->kyou_cnt);
        stop_if(lvr_3sx->up_cnt != lvr_cps3->up_cnt);
        stop_if(lvr_3sx->down_cnt != lvr_cps3->down_cnt);
        stop_if(lvr_3sx->left_cnt != lvr_cps3->left_cnt);
        stop_if(lvr_3sx->right_cnt != lvr_cps3->right_cnt);
        stop_if(lvr_3sx->s1_cnt != lvr_cps3->s1_cnt);
        stop_if(lvr_3sx->s2_cnt != lvr_cps3->s2_cnt);
        stop_if(lvr_3sx->s3_cnt != lvr_cps3->s3_cnt);
        stop_if(lvr_3sx->s4_cnt != lvr_cps3->s4_cnt);
        stop_if(lvr_3sx->s5_cnt != lvr_cps3->s5_cnt);
        stop_if(lvr_3sx->s6_cnt != lvr_cps3->s6_cnt);
        stop_if(lvr_3sx->lu_cnt != lvr_cps3->lu_cnt);
        stop_if(lvr_3sx->ld_cnt != lvr_cps3->ld_cnt);
        stop_if(lvr_3sx->ru_cnt != lvr_cps3->ru_cnt);
        stop_if(lvr_3sx->rd_cnt != lvr_cps3->rd_cnt);
        stop_if(lvr_3sx->waza_num != lvr_cps3->waza_num);
        // stop_if(lvr_3sx->waza_no != lvr_cps3->waza_no);
        stop_if(lvr_3sx->wait_cnt != lvr_cps3->wait_cnt);
        stop_if(lvr_3sx->cmd_r_no != lvr_cps3->cmd_r_no);
    }
}

static void compare_waza_work(SDL_IOStream* io) {
    WAZA_WORK waza_work_cps3[2][56];
    read_waza_work(io, waza_work_cps3);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 56; j++) {
            const WAZA_WORK* w_3sx = &waza_work[i][j];
            const WAZA_WORK* w_cps3 = &waza_work_cps3[i][j];

            stop_if(w_3sx->w_type != w_cps3->w_type);
            stop_if(w_3sx->w_int != w_cps3->w_int);
            stop_if(w_3sx->free1 != w_cps3->free1);
            stop_if(w_3sx->w_lvr != w_cps3->w_lvr);
            stop_if(w_3sx->free2 != w_cps3->free2);
            stop_if(w_3sx->w_dead != w_cps3->w_dead);
            stop_if(w_3sx->w_dead2 != w_cps3->w_dead2);
            stop_if(w_3sx->uni0.tame.flag != w_cps3->uni0.tame.flag);
            stop_if(w_3sx->uni0.tame.shot_flag != w_cps3->uni0.tame.shot_flag);
            stop_if(w_3sx->uni0.tame.shot_flag2 != w_cps3->uni0.tame.shot_flag2);
            stop_if(w_3sx->free3 != w_cps3->free3);
            stop_if(w_3sx->shot_ok != w_cps3->shot_ok);
        }
    }
}

static void compare_wcp(SDL_IOStream* io) {
    WORK_CP wcp_cps3[2];
    read_wcp(io, wcp_cps3);

    for (int i = 0; i < 2; i++) {
        const s16 waza_type_cps3 = read_s16(io, WAZA_TYPE_OFFSET + i * sizeof(s16));
        stop_if(waza_type[i] != waza_type_cps3);

        const WORK_CP* w_3sx = &wcp[i];
        const WORK_CP* w_cps3 = &wcp_cps3[i];

        stop_if(w_3sx->sw_lvbt != w_cps3->sw_lvbt);
        stop_if(w_3sx->sw_new != w_cps3->sw_new);
        stop_if(w_3sx->sw_old != w_cps3->sw_old);
        stop_if(w_3sx->sw_now != w_cps3->sw_now);
        stop_if(w_3sx->sw_off != w_cps3->sw_off);
        stop_if(w_3sx->sw_chg != w_cps3->sw_chg);
        stop_if(w_3sx->old_now != w_cps3->old_now);
        stop_if(w_3sx->lgp != w_cps3->lgp);
        stop_if(w_3sx->ca14 != w_cps3->ca14);
        stop_if(w_3sx->ca25 != w_cps3->ca25);
        stop_if(w_3sx->ca36 != w_cps3->ca36);
        stop_if(w_3sx->calf != w_cps3->calf);
        stop_if(w_3sx->calr != w_cps3->calr);
        stop_if(w_3sx->lever_dir != w_cps3->lever_dir);

        for (int j = 0; j < 56; j++) {
            stop_if(w_3sx->waza_flag[j] != w_cps3->waza_flag[j]);

            if (w_3sx->waza_flag[j] == -1) {
                continue;
            }

            stop_if(w_3sx->reset[j] != w_cps3->reset[j]);
            stop_if(w_3sx->btix[j] != w_cps3->btix[j]);

            for (int k = 0; k < 4; k++) {
                stop_if(w_3sx->waza_r[j][k] != w_cps3->waza_r[j][k]);
                stop_if(w_3sx->exdt[j][k] != w_cps3->exdt[j][k]);
            }
        }
    }
}

static Uint64 start_frame = 0;

void compare_values(SDL_IOStream* io, Uint64 frame) {
    if (start_frame == 0) {
        start_frame = frame;
    }

    // Wait a bit so that the game has time to clear garbage values
    if (frame - start_frame > 5) {
        compare_lvr(io);
        compare_waza_work(io);
        compare_wcp(io);
    }

    const bool compare_characters = G_No[1] == 2 && G_No[2] == 1;
    compare_service_values(io, compare_characters, frame);

    if (compare_characters) {
        compare_main_values(io);
    }
}

// Syncing

static void sync_lvr(T_PL_LVR* dst, const T_PL_LVR* src) {
    dst->sw_new = src->sw_new;
    dst->sw_old = src->sw_old;
    dst->sw_chg = src->sw_chg;
    dst->sw_now = src->sw_now;
    dst->old_now = src->old_now;
    dst->now_lvbt = src->now_lvbt;
    dst->old_lvbt = src->old_lvbt;
    dst->new_lvbt = src->new_lvbt;
    dst->sw_lever = src->sw_lever;
    dst->shot_up = src->shot_up;
    dst->shot_down = src->shot_down;
    dst->shot_ud = src->shot_ud;
}

static void sync_wcp(WORK_CP* dst, const WORK_CP* src) {
    dst->sw_lvbt = src->sw_lvbt;
    dst->sw_new = src->sw_new;
    dst->sw_old = src->sw_old;
    dst->sw_now = src->sw_now;
    dst->sw_off = src->sw_off;
    dst->sw_chg = src->sw_chg;
    dst->old_now = src->old_now;
    dst->lgp = src->lgp;
    dst->ca14 = src->ca14;
    dst->ca25 = src->ca25;
    dst->ca36 = src->ca36;
    dst->lever_dir = src->lever_dir;

    for (int i = 0; i < 56; i++) {
        if (dst->waza_flag[i] != -1) {
            dst->waza_flag[i] = src->waza_flag[i];
        }
    }
}

static void sync_waza_work(WAZA_WORK* dst, const WAZA_WORK* src, Character character) {
    if (dst->w_type == 15 && character == CHAR_URIEN) {
        dst->w_int = src->w_int;
        dst->uni0.tame.flag = src->uni0.tame.flag;
    }
}

void sync_values(SDL_IOStream* io) {
    Random_ix16 = read_s16(io, RANDOM_IX_16_OFFSET);
    Random_ix32 = read_s16(io, RANDOM_IX_32_OFFSET);

    // WORK_CP wcp_cps3[2];
    // read_wcp(io, wcp_cps3);

    // WAZA_WORK waza_work_cps3[2][56];
    // read_waza_work(io, waza_work_cps3);

    // T_PL_LVR t_pl_lvr_cps3[2];
    // read_t_pl_lvr(io, t_pl_lvr_cps3);

    for (int i = 0; i < 2; i++) {
        const Character character = plw[i].player_number;

        // sync_lvr(&t_pl_lvr[i], &t_pl_lvr_cps3[i]);
        // sync_wcp(&wcp[i], &wcp_cps3[i]);

        // for (int j = 0; j < 56; j++) {
        //     sync_waza_work(&waza_work[i][j], &waza_work_cps3[i][j], character);
        // }
    }
}

#endif

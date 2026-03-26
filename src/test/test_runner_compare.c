#include "test/test_runner_compare.h"
#include "arcade/arcade_constants.h"
#include "port/utils.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
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

static void compare_main_values(SDL_IOStream* io) {
    const u8 allow_a_battle_f_cps3 = read_u8(io, ALLOW_A_BATTLE_F_OFFSET);
    stop_if(Allow_a_battle_f != allow_a_battle_f_cps3);

    const u8 round_timer_cps3 = read_u8(io, ROUND_TIMER_OFFSET);
    stop_if(round_timer != round_timer_cps3);

    const u16 game_timer_cps3 = read_u16(io, GAME_TIMER_OFFSET);
    // printf("⏱️ %d game_timer: %d\n", comparison_index, game_timer_cps3);

    // Some interactions are decided by the evenness of Game_timer. After the first round Game_timer inevitably
    // goes out of sync, which is why we have to sync it manually.
    // This is not the best place to do this, but we need to sync Game_timer somewhere, so ...
    if (Game_timer != game_timer_cps3) {
        Game_timer = game_timer_cps3;
    }

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

static void compare_service_values(SDL_IOStream* io) {
    const s16 counter_hi_cps3 = read_s16(io, COUNTER_HI_OFFSET);
    stop_if(Counter_hi != counter_hi_cps3);

    const s16 counter_low_cps3 = read_s16(io, COUNTER_LOW_OFFSET);
    stop_if(Counter_low != counter_low_cps3);

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

static void compare_wcp(SDL_IOStream* io) {
    SDL_SeekIO(io, WCP_OFFSET, SDL_IO_SEEK_SET);

    WORK_CP wcp_cps3[2];
    SDL_ReadIO(io, wcp_cps3, sizeof(wcp_cps3));

    for (int i = 0; i < 2; i++) {
        WORK_CP* w = &wcp_cps3[i];

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
            stop_if(w_3sx->reset[j] != w_cps3->reset[j]);
            stop_if(w_3sx->btix[j] != w_cps3->btix[j]);

            for (int k = 0; k < 4; k++) {
                stop_if(w_3sx->waza_r[j][k] != w_cps3->waza_r[j][k]);
                stop_if(w_3sx->exdt[j][k] != w_cps3->exdt[j][k]);
            }
        }
    }
}

void compare_values(SDL_IOStream* io, size_t frames_elapsed) {
    if (frames_elapsed > 200) {
        compare_wcp(io);
    }

    compare_service_values(io);
    compare_main_values(io);
}

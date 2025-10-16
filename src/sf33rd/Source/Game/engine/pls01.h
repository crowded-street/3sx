#ifndef PLS01_H
#define PLS01_H

#include "structs.h"
#include "types.h"

s32 sa_stop_check();
void check_my_tk_power_off(PLW* wk, PLW* /* unused */);
void check_em_tk_power_off(PLW* wk, PLW* tk);
s16 check_ukemi_flag(PLW* wk);
s32 check_rl_flag(WORK* wk);
void set_rl_waza(PLW* wk);
s16 check_rl_on_car(PLW* wk);
s32 saishin_bs2_area_car(PLW* wk);
s8 saishin_bs2_on_car(PLW* wk);
s32 check_air_jump(PLW* wk);
s32 check_sankaku_tobi(PLW* wk);
void check_extra_jump_timer(PLW* wk);
void remake_sankaku_tobi_mvxy(WORK* wk, u8 kabe);
s16 check_F_R_dash(PLW* wk);
s32 check_jump_ready(PLW* wk);
s32 check_hijump_only(PLW* wk);
s32 check_bend_myself(PLW* wk);
s16 check_F_R_walk(PLW* wk);
s32 check_turn_to_back(PLW* wk);
s32 check_hurimuki(WORK* wk);
s16 check_walking_lv_dir(PLW* wk);
s32 check_stand_up(PLW* wk);
s32 check_defense_lever(PLW* wk);
s32 check_em_catt(PLW* wk);
s16 check_attbox_dir(PLW* wk);
u16 check_defense_kind(PLW* wk);
void jumping_union_process(WORK* wk, s16 num);
s32 check_floor(PLW* wk);
s32 check_ashimoto(PLW* wk);
s32 check_floor_2(PLW* wk);
s32 check_ashimoto_ex(PLW* wk);

#endif

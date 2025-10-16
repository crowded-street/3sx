#ifndef PLMAIN_H
#define PLMAIN_H

#include "structs.h"
#include "types.h"

extern void (*const plmain_lv_00[5])(PLW* wk);
extern void (*const plmain_lv_02[5])(PLW* wk);

void Player_move(PLW* wk, u16 lv_data);
u16 check_illegal_lever_data(u16 data);
void get_saikinnno_idouryou(PLW* wk);
void demo_set_sa_full(SA_WORK* sa);
void clear_attack_num(WORK* wk);
void clear_tk_flags(PLW* wk);
void about_gauge_process(PLW* wk);
s16 check_hit_stop(PLW* wk);
void look_after_timers(PLW* wk);

#endif

#ifndef EFFC2_H
#define EFFC2_H

#include "structs.h"
#include "types.h"

typedef struct {
    s16 dir_timer;
    s16 chix;
    s16 vital;
    s16 color;
    s16 prio_hi;
    s16 prio_low;
} BS2;

void effect_C2_move(WORK_Other* ewk);
void setup_demojump(PLW* twk, s16 ix);
s32 check_effc2_p2_rno(WORK* wk);
void player_hosei_data(WORK_Other* ewk, s16 flag, s16 f2);
void get_bs2_parts_data(WORK* wk);
void set_parts_priority(WORK* wk);
void c3_new_damage(WORK* wk);
s16 c3_hit_disp_check(u8 ix);
s16 bs2_sync_bomb(WORK* wk);
void bs2_get_parts_break(WORK* wk);
s32 check_parts_break_level(WORK* wk);
void get_shizumi_guai(WORK* wk);
s32 effect_C2_init(WORK* wk, u8 data);

#endif

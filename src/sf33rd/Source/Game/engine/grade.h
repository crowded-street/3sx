#ifndef GRADE_H
#define GRADE_H

#include "structs.h"
#include "types.h"

extern JudgeGals judge_gals[2];
extern JudgeCom judge_com[2];

extern GradeFinalData judge_final[2][2];
extern GradeData judge_item[2][2];

void grade_check_work_1st_init(s16 ix, s16 ix2);
void grade_check_work_stage_init(s16 ix);
void grade_check_work_round_init(s16 ix);
void grade_makeup_final_parameter(s16 ix, s16 pt);
void renew_judge_final_work(s16 ix, s16 pt);
void makeup_final_grade(s16 ix, s16 pt);
void grade_final_grade_bonus();
void makeup_spp_frdat(s16 ix, s16 pt);
void grade_makeup_round_parameter(s16 ix);
void backup_RO_PT();
void grade_makeup_round_para_dko();
void grade_makeup_judgement_gals();
void grade_makeup_stage_parameter(s16 ix);
s32 rannyuu_Q_check(s16 ix);
void grade_makeup_stage_para_com(s16 ix);
void grade_makeup_bonus_parameter(s16 ix);
s16 get_offence_total(s16 ix);
s16 get_defence_total(s16 ix, s16 wf);
s16 get_tech_pts_total(s16 ix);
s16 get_ex_point_total(s16 ix, s16 wf);
void grade_add_clean_hits(WORK_Other* wk);
void grade_add_att_renew(WORK_Other* wk);
void grade_add_guard_success(s16 ix);
void grade_add_em_stun(s16 ix);
void grade_max_combo_check(s16 ix, s16 num);
void grade_add_leap_attack(s16 ix);
void grade_add_grap_def(s16 ix);
void grade_add_quick_stand(s16 ix);
void grade_add_nml_nage(WORK* wk);
void grade_add_reversal(s16 ix);
void grade_add_target_combo(s16 ix);
void grade_add_command_waza(s16 ix);
void grade_add_super_arts(s16 ix, s16 num);
void grade_store_vitality(s16 ix);
void grade_add_blocking(PLW* wk);
void grade_get_first_attack(s16 ix);
void grade_set_round_result(s16 ix);
void grade_add_personal_action(s16 ix);
void grade_check_tairyokusa();
void grade_add_onaji_waza(s16 ix);
s16 grade_get_my_grade(s16 ix);
s16 grade_get_my_point_percentage(s16 ix, s16 flag);
s16 grade_get_cm_point_percentage(s16 ix, s16 flag);
s16 get_grade_ix(s16 pts);
void check_guard_miss(WORK* as, PLW* ds, s8 gddir);

#endif

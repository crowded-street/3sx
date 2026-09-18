/*
 * Internal declarations for the pass06 pattern scripts.
 *
 * pass06.c holds the dispatcher and Passive06_Tbl, which names every pattern in
 * the file, so the patterns that moved out need declaring here. They had
 * external linkage in the decompilation already - nothing is widened by the
 * split.
 *
 * The pattern_* skeletons are this campaign's own, extracted from the bodies
 * they are still the only callers of; the ones named here are reached from
 * more than one half of the split.
 */

#ifndef PASS06_INTERNAL_H
#define PASS06_INTERNAL_H

#include "sf33rd/Source/Game/com/com_sub.h"
#include "structs.h"
#include "types.h"

void passive06_pattern_em_term_normal_attack(PLW* wk, const EM_Term_Params* p, u16 lever_data);
void passive06_pattern_command_attack(PLW* wk, const Command_Attack_Args* p);
void passive06_pattern_normal_attack(PLW* wk, s16 reaction, u16 lever_data);
void passive06_pattern_approach_walk_em_term_command_attack(
    PLW* wk, s16 target_pos, const EM_Term_Params* p, const Command_Attack_Args* p_b
);
void passive06_pattern_jump_attack_term(PLW* wk, const Jump_Term_Args* a);
void passive06_pattern_wait_get_up_normal_attack(PLW* wk, u16 lever_data, s16 option, u16 lever_data_b);
void passive06_pattern_approach_walk_wait_get_up_wait(PLW* wk, u16 lever_data);
void passive06_pattern_approach_walk_check_ex_pierce_on(PLW* wk, s16 target_pos, s16 next_menu);
void passive06_pattern_hi_jump_attack_term_normal_attack(PLW* wk, u16 lever_data);
void passive06_pattern_em_term_command_attack(PLW* wk, const EM_Term_Params* p, const Command_Attack_Args* p_b);
void passive06_pattern_wait_get_up_com_random_select(PLW* wk, u16 lever_data, const Branch_Menu_Args* p);
void passive06_pattern_approach_walk(PLW* wk, s16 target_pos);
void passive06_pattern_jump(PLW* wk, s16 time);
void Passive06_0067(PLW* wk);
void Passive06_0068(PLW* wk);
void Passive06_0069(PLW* wk);
void Passive06_0070(PLW* wk);
void Passive06_0071(PLW* wk);
void Passive06_0072(PLW* wk);
void Passive06_0073(PLW* wk);
void Passive06_0074(PLW* wk);
void Passive06_0075(PLW* wk);
void Passive06_0076(PLW* wk);
void Passive06_0077(PLW* wk);
void Passive06_0078(PLW* wk);
void Passive06_0079(PLW* wk);
void Passive06_0080(PLW* wk);
void Passive06_0081(PLW* wk);
void Passive06_0082(PLW* wk);
void Passive06_0083(PLW* wk);
void Passive06_0084(PLW* wk);
void Passive06_0085(PLW* wk);
void Passive06_0086(PLW* wk);
void Passive06_0087(PLW* wk);
void Passive06_0088(PLW* wk);
void Passive06_0089(PLW* wk);
void Passive06_0090(PLW* wk);
void Passive06_0091(PLW* wk);
void Passive06_0092(PLW* wk);
void Passive06_0093(PLW* wk);
void Passive06_0094(PLW* wk);
void Passive06_0095(PLW* wk);
void Passive06_0096(PLW* wk);
void Passive06_0097(PLW* wk);
void Passive06_0098(PLW* wk);
void Passive06_0099(PLW* wk);
void Passive06_0100(PLW* wk);
void Passive06_0101(PLW* wk);
void Passive06_0102(PLW* wk);
void Passive06_0103(PLW* wk);
void Passive06_0104(PLW* wk);
void Passive06_0105(PLW* wk);
void Passive06_0106(PLW* wk);
void Passive06_0107(PLW* wk);
void Passive06_0108(PLW* wk);
void Passive06_0109(PLW* wk);
void Passive06_0110(PLW* wk);
void Passive06_0111(PLW* wk);
void Passive06_0112(PLW* wk);
void Passive06_0113(PLW* wk);
void Passive06_0114(PLW* wk);
void Passive06_0115(PLW* wk);
void Passive06_0116(PLW* wk);
void Passive06_0117(PLW* wk);
void Passive06_0118(PLW* wk);
void Passive06_0119(PLW* wk);
void Passive06_0120(PLW* wk);
void Passive06_0121(PLW* wk);
void Passive06_0122(PLW* wk);
void Passive06_0123(PLW* wk);
void Passive06_0124(PLW* wk);
void Passive06_0125(PLW* wk);
void Passive06_0126(PLW* wk);
void Passive06_0127(PLW* wk);
void Passive06_0128(PLW* wk);
void Passive06_0129(PLW* wk);
void Passive06_0130(PLW* wk);
void Passive06_0131(PLW* wk);
void Passive06_0132(PLW* wk);
void Passive06_0133(PLW* wk);
void Passive06_0134(PLW* wk);
void Passive06_0135(PLW* wk);
void Passive06_0136(PLW* wk);
void Passive06_0137(PLW* wk);
void Passive06_0138(PLW* wk);
void Passive06_0139(PLW* wk);
void Passive06_0140(PLW* wk);
void Passive06_0141(PLW* wk);
void Passive06_0142(PLW* wk);
void Passive06_0143(PLW* wk);
void Passive06_0144(PLW* wk);
void Passive06_0145(PLW* wk);
void Passive06_0146(PLW* wk);

#endif

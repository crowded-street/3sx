/**
 * @file ck_pass_checks.c
 * Passive AI logic
 * The checks a passive script runs against the opponent.
 */

#include "sf33rd/Source/Game/com/ck_pass.h"
#include "common.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/ck_pass_internal.h"

/* The opponent is mid-dash: the run routine has started and is past its first
 * step. Named so Check_Dash reads as the question it asks. */
/* Option 8 means answer only as a counter, and a counter is not available
 * while this side is already attacking. */
static s32 counter_barred_while_attacking(PLW* wk, const SP_Tech_Args* p) {
    return p->Option == 8 && Attack_Flag[wk->wu.id] != 0;
}

/* Technique 23 is answered whichever way the opponent is facing; everything
 * else is ruled out when the facing check says so. */
static s32 facing_rules_this_out(PLW* wk, WORK* em, const SP_Tech_Args* p) {
    return p->VS_Technique != 23 && Check_Attack_Direction(wk, em);
}

/* The opponent's move is the kind and the id being watched for. */
static s32 is_the_watched_technique(WORK* em, const SP_Tech_Args* p, u8 xx) {
    return xx == p->Kind_of_Tech && (em->sp_tech_id == p->SP_Tech_ID);
}

/* Option2 carries no strength filter, so any strength matches. */
static s32 option2_has_no_strength_filter(const SP_Tech_Args* p) {
    return (p->Option2 == -1 || !(p->Option2 & 8));
}

/* The opponent is not in the pattern status and waza kind this limited attack
 * is waiting for. */
static s32 not_the_watched_attack(WORK* em, const Limited_Attack_Args* p) {
    return (em->pat_status != p->PL_Status) || em->kind_of_waza != p->Status_00;
}

/* The same question for the jump form, which still takes its two values
 * separately. */
static s32 not_the_watched_jump_attack(WORK* em, u8 PL_Status, s8 Status_00) {
    return (em->pat_status != PL_Status) || (em->kind_of_waza != Status_00);
}

/* The opponent is in neither of the two squat statuses being watched. */
static s32 not_in_either_squat_status(WORK* em, const VS_Squat_Args* p) {
    return em->pat_status != p->Status_00 && em->pat_status != p->Status_01;
}

/* The opponent is low and still rising, so there is nothing to answer yet. */
static s32 rising_from_low(PLW* em) {
    return (em->wu.xyz[1].disp.pos < 32) && (em->wu.mvxy.a[1].real.h > 0);
}

/* The opponent is in the dizzy routine. */
static s32 enemy_is_fainting(PLW* enemy) {
    return (enemy->wu.routine_no[1] == 1) && (enemy->wu.routine_no[2] == 25);
}

static s32 enemy_is_dashing(const WORK* em) {
    return (em->routine_no[1] == 0) && (em->routine_no[2] == 5) && (em->routine_no[3] != 0);
}

/* None of the waza-kind bits that mark an attack worth answering are set. The
 * masks are the original ones, in the original order. */
static s32 waza_kind_is_unmarked(const WORK* em) {
    return !(em->kind_of_waza & 32) && !(em->kind_of_waza & 48) && !(em->kind_of_waza & 40) &&
           !(em->kind_of_waza & 56) && !(em->kind_of_waza & 8);
}

/* The opponent is in none of the four stances a forward cross chop answers. */
static s32 not_a_cross_chop_stance(const WORK* em) {
    return (em->pat_status != 22) && (em->pat_status != 20) && (em->pat_status != 26) && (em->pat_status != 28);
}

/* The opponent's move is the one being watched for. What is left is whether the
 * strength filter in Option2 rules it out, and what to set if it does not. The
 * block is Check_Special_Technique's own, unchanged. */
static s32 answer_matched_technique(PLW* wk, WORK* em, const SP_Tech_Args* p) {
    if (option2_has_no_strength_filter(p)) {
        if (p->Option2 == (em->kind_of_waza & 6)) {
            Last_Attack_Counter[(wk->wu.id)] = Attack_Counter[(wk->wu.id)];
            return 0;
        }
    } else if (!((p->Option2 & 6) & (em->kind_of_waza & 6))) {
        return 0;
    }

    if (p->Option == 8) {
        Counter_Attack[(wk->wu.id)] = 1;
    }

    if (p->Option == 1) {
        Counter_Attack[(wk->wu.id)] = 1;
    }

    VS_Tech[wk->wu.id] = p->VS_Technique;

    return PASSIVE_X = 1;
}

s32 Check_Special_Technique(PLW* wk, WORK* em, const SP_Tech_Args* p) {
    u8 xx;

    if (counter_barred_while_attacking(wk, p)) {
        return 0;
    }

    if (facing_rules_this_out(wk, em, p)) {
        return 0;
    }

    if (Last_Attack_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return 0;
    }

    xx = em->kind_of_waza & 0xF8;

    if (is_the_watched_technique(em, p, xx)) {
        return answer_matched_technique(wk, em, p);
    }

    return 0;
}

s32 Check_Attack_Direction(PLW* wk, WORK* em) {
    if (wk->wu.xyz[0].disp.pos < em->xyz[0].disp.pos) {
        if (em->xyz[0].disp.pos > em->old_pos[0]) {
            return 1;
        }
    } else if (em->xyz[0].disp.pos < em->old_pos[0]) {
        return 1;
    }

    return 0;
}

/* The four reasons not to answer a jump at all: the opponent is not airborne,
 * the move is the one this never answers, the per-area cooldown is still
 * running - which is also where it ticks down - or they are already falling
 * below the height being watched. */
static s32 jump_answer_is_blocked(PLW* wk, PLW* em, s16 Height) {
    if (em->wu.routine_no[1] == 1) {
        return 1;
    }

    if (em->wu.sp_tech_id == 33) {
        return 1;
    }

    if (Jump_Pass_Timer[wk->wu.id][Area_Number[wk->wu.id]]) {
        Jump_Pass_Timer[wk->wu.id][Area_Number[wk->wu.id]]--;
        return 1;
    }

    if ((em->wu.mvxy.a[1].real.h) < 0 && (em->wu.xyz[1].disp.pos <= Height)) {
        return 1;
    }

    return 0;
}

s32 Check_VS_Jump(PLW* wk, PLW* em, s16 Height) {
    if (jump_answer_is_blocked(wk, em, Height)) {
        return 0;
    }

    if (Check_Specific_Term(wk, &em->wu, &(Specific_Term_Args){4099, 14, 20, 26})) {
        return Counter_Attack[wk->wu.id] = 1;
    }

    if (em->wu.xyz[1].disp.pos == 0) {
        return 0;
    }

    if (rising_from_low(em)) {
        return 0;
    }

    if (em->micchaku_flag) {
        VS_Tech[wk->wu.id] = 18;
        return PASSIVE_X = 1;
    }

    if (Check_Specific_Term(wk, &em->wu, &(Specific_Term_Args){18, 22, 28, 16})) {
        return 1;
    }

    VS_Tech[wk->wu.id] = 0;
    return 0;
}

s32 Check_Rolling(PLW* wk, WORK* em) {
    if (em->pat_status != 34) {
        return 0;
    }

    if (Check_Attack_Direction(wk, em)) {
        VS_Tech[wk->wu.id] = 6;
    } else {
        VS_Tech[wk->wu.id] = 5;
    }

    return PASSIVE_X = 1;
}

s32 Check_Personal_Action(PLW* wk, WORK* em) {
    if (em->routine_no[1] != 4) {
        return 0;
    }
    if (em->routine_no[2] != 30) {
        return 0;
    }

    VS_Tech[wk->wu.id] = 4105;

    return PASSIVE_X = 1;
}

s32 Check_Specific_Term(PLW* wk, WORK* em, const Specific_Term_Args* p) {
    VS_Tech[wk->wu.id] = p->VS_Technique;

    if (em->pat_status == p->Status_00) {
        return PASSIVE_X = 1;
    }

    if (em->pat_status == p->Status_01) {
        return PASSIVE_X = 1;
    }

    if (em->pat_status == p->Status_02) {
        return PASSIVE_X = 1;
    }

    return 0;
}

s32 Check_Dash(PLW* wk, WORK* em, s16 VS_Technique) {
    if (enemy_is_dashing(em)) {
        VS_Tech[wk->wu.id] = VS_Technique;

        return PASSIVE_X = 1;
    }

    return 0;
}

/* Three opponents are allowed a longer limit on technique 7. The block is the
 * one Check_Limited_Attack ran inline; it takes the limit and gives it back. */
static s16 limit_for_this_opponent(WORK* em, const Limited_Attack_Args* p, s16 Limit_Number) {
    if ((((PLW*)em)->player_number == 17) && (p->VS_Technique == 7)) {
        Limit_Number += 1;
    }

    if ((((PLW*)em)->player_number == 10) && (p->VS_Technique == 7)) {
        Limit_Number += 1;
    }

    if ((((PLW*)em)->player_number == 3) && (p->VS_Technique == 7)) {
        Limit_Number += 2;
    }

    return Limit_Number;
}

s32 Check_Limited_Attack(PLW* wk, WORK* em, const Limited_Attack_Args* p) {
    s16 xx;
    /* The original took this by value and adjusted it per opponent; the copy
     * keeps that local, which is what a by-value parameter was. */
    s16 Limit_Number = p->Limit_Number;

    if (Attack_Flag[wk->wu.id] == 0) {
        return 0;
    }

    if (Last_Attack_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return 0;
    }

    if (not_the_watched_attack(em, p)) {
        return 0;
    }

    xx = (em->cg_ix / em->cgd_type);

    Limit_Number = limit_for_this_opponent(em, p, Limit_Number);

    if (xx > Limit_Number) {
        return 0;
    }

    VS_Tech[wk->wu.id] = p->VS_Technique;
    Limited_Flag[wk->wu.id] = 1;
    Counter_Attack[wk->wu.id] = 1;

    return PASSIVE_X = 1;
}

s32 Check_Limited_Jump_Attack(PLW* wk, WORK* em, u8 PL_Status, s8 Status_00) {
    if (not_the_watched_jump_attack(em, PL_Status, Status_00)) {
        return 0;
    }

    return 1;
}

s32 Check_Stand(PLW* wk, WORK* em, s16 VS_Technique) {
    if (Attack_Flag[wk->wu.id]) {
        return 0;
    }

    if (em->routine_no[1] != 0) {
        return 0;
    }

    if ((Standing_Timer[wk->wu.id] += 1) < Standing_Master_Timer[wk->wu.id]) {
        return 0;
    }

    Standing_Master_Timer[wk->wu.id] = Setup_Next_Stand_Timer(wk);
    VS_Tech[wk->wu.id] = VS_Technique;

    return PASSIVE_X = 1;
}

s32 Setup_Next_Stand_Timer(PLW* wk) {
    if (EM_Rank != 0) {
        return Standing_Time_Data[17][Area_Number[wk->wu.id]][(random_16_com() & 7)];
    }

    return Standing_Time_Data[wk->player_number][Area_Number[wk->wu.id]][(random_16_com() & 7)];
}

s32 Check_VS_Squat(PLW* wk, WORK* em, const VS_Squat_Args* p) {
    if (Attack_Flag[wk->wu.id]) {
        return Squat_Timer[wk->wu.id] = 0;
    }

    if (em->routine_no[1] != 0) {
        return Squat_Timer[wk->wu.id] = 0;
    }

    if (em->xyz[1].disp.pos) {
        return Squat_Timer[wk->wu.id] = 0;
    }

    if (not_in_either_squat_status(em, p)) {
        return Squat_Timer[wk->wu.id] = 0;
    }

    if ((Squat_Timer[wk->wu.id] += 1) < Squat_Master_Timer[wk->wu.id]) {
        return 0;
    }

    Squat_Master_Timer[wk->wu.id] = Setup_Next_Squat_Timer(wk);
    VS_Tech[wk->wu.id] = p->VS_Technique;

    return PASSIVE_X = 1;
}

s32 Setup_Next_Squat_Timer(PLW* wk) {
    return Squat_Time_Data[Setup_Lv08(0)][(random_16_com() & 7)];
}

s32 Check_Thrown(PLW* wk, WORK* em) {
    s16 Rnd;
    s16 x;

    if (em->xyz[1].disp.pos) {
        return 0;
    }

    x = Setup_VS_Catch_Data(wk);
    Rnd = random_32_com();

    if (x < Rnd) {
        return 0;
    }

    switch (Area_Number[wk->wu.id]) {
    case 0:
        if (Check_Catch(wk, em, 25)) {
            return 1;
        }

        break;

    case 1:
        if (Check_Catch(wk, em, 25)) {
            return 1;
        }

        break;

    default:
        break;
    }

    return 0;
}

s32 Check_Catch(PLW* wk, WORK* em, s16 VS_Technique) {
    u16 xx;

    if (Demo_Flag == 0) {
        return 0;
    }

    if (em->routine_no[1] != 0) {
        return 0;
    }

    if (em->xyz[1].disp.pos) {
        return 0;
    }

    if (wk->wu.id == 0) {
        xx = p2sw_0;
    } else {
        xx = p1sw_0;
    }

    if (wk->wu.rl_waza) {
        if (!(xx & 4)) {
            return 0;
        }
    } else if (!(xx & 8)) {
        return 0;
    }

    Counter_Attack[wk->wu.id] = 1;
    VS_Tech[wk->wu.id] = VS_Technique;

    return PASSIVE_X = 1;
}

s32 Check_Lie(PLW* wk) {
    WORK* em;
    PLW* enemy;

    em = (WORK*)wk->wu.target_adrs;
    enemy = (PLW*)wk->wu.target_adrs;

    if (Check_Faint(wk, enemy, 2)) {
        return Select_Passive(wk);
    }

    if (Check_Specific_Term(wk, em, &(Specific_Term_Args){0, 38, 38, 38})) {
        return Select_Passive(wk);
    }

    return 0;
}

s32 Check_Faint(PLW* wk, PLW* enemy, s16 VS_Technique) {
    Counter_Attack[wk->wu.id] = 1;
    VS_Tech[wk->wu.id] = VS_Technique;

    if (enemy_is_fainting(enemy)) {
        return 1;
    }

    return Counter_Attack[wk->wu.id] = 0;
}

s32 Check_Blow_Off(PLW* wk, WORK* em, s16 VS_Technique) {
    if (em->routine_no[1] != 1) {
        return 0;
    }

    if (PL_Blow_Off_Data[em->routine_no[2]] == 0) {
        return 0;
    }

    if (em->xyz[1].disp.pos == 0) {
        return 0;
    }

    VS_Tech[(wk->wu.id)] = VS_Technique;

    return PASSIVE_X = 1;
}

s32 Check_After_Attack(PLW* wk, WORK* em, s16 VS_Technique) {
    u8 xx;

    if (CP_No[wk->wu.id][0] == 7) {
        return 0;
    }

    if (Last_Attack_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return 0;
    }

    if (em->xyz[1].disp.pos) {
        return 0;
    }

    if (em->routine_no[1] != 4) {
        return 0;
    }

    Last_Attack_Counter[wk->wu.id] = Attack_Counter[wk->wu.id];

    if (waza_kind_is_unmarked(em)) {
        xx = em->kind_of_waza & 6;

        if (xx == 0) {
            return 0;
        }

        if (xx == 2) {
            return 0;
        }
    }

    VS_Tech[wk->wu.id] = VS_Technique;

    return PASSIVE_X = 1;
}

s32 Check_F_Cross_Chop(PLW* wk, WORK* em, s16 VS_Technique) {
    if (Last_Attack_Counter[wk->wu.id] == Attack_Counter[wk->wu.id]) {
        return 0;
    }

    if ((em->kind_of_waza) != 4) {
        return 0;
    }

    if (not_a_cross_chop_stance(em)) {
        return 0;
    }

    VS_Tech[wk->wu.id] = VS_Technique;
    Counter_Attack[wk->wu.id] = 1;

    return PASSIVE_X = 1;
}

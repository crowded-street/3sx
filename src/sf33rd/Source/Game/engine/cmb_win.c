/**
 * @file cmb_win.c
 * Manages in-game combo detection, scoring, and HUD display
 */

#include "sf33rd/Source/Game/engine/cmb_win.h"
#include "arcade/arcade_balance.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/ui/sc_data.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#include <SDL3/SDL.h>

#include <string.h>

// The port renderer keeps five records per player; CPS3 uses four.
CMST_BUFF cmst_buff[2][5];
s16 old_cmb_flag[2];
s8 cmb_stock[2];
s8 first_attack;
s8 rever_attack[2];
s8 paring_attack[2];
s8 bonus_pts[2];
s16 hit_num;
u8 sa_kind;
u8 end_flag[2];
s16 calc_hit[2][10];
s16 score_calc[2][12];
s8 cmb_all_stock[1];
s8 sarts_finish_flag[2];
s8 last_hit_time;
s8 cmb_calc_now[2];
u8 cst_read[2];
u8 cst_write[2];

const u8 cmb_pos_tbl[2][21] = { { 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27 },
                                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } };

const u8 cmb_window_move_tbl[7] = { 12, 16, 20, 16, 12, 10, 12 };

const u8 cmb_window_time_tbl[7] = { 28, 40, 60, 60, 50, 50, 24 };

const u16 combo_score_tbl[12][2] = { { 300, 200 },   { 500, 400 },   { 1000, 600 },  { 1000, 600 },
                                     { 2000, 800 },  { 3000, 600 },  { 4000, 800 },  { 5000, 1000 },
                                     { 6000, 2000 }, { 8000, 2000 }, { 9000, 3000 }, { 10000, 4000 } };

static s32 round_lost_by(s32 PL) {
    return Conclusion_Flag && Conclusion_Type == 0 && Loser_id == PL;
}

static s32 versus_rules_apply(s32 PLS) {
    return (!ArcadeBalance_IsEnabled() && Mode_Type == MODE_VERSUS) || plw[PLS].wu.operator;
}

void combo_cont_init() { // 🟡
    u8 i;

    for (i = 0; i < 2; i++) {
        old_cmb_flag[i] = 0;
        cmb_stock[i] = 0;
        rever_attack[i] = 0;
        paring_attack[i] = 0;
        bonus_pts[i] = 0;
        sarts_finish_flag[i] = 0;
        cmb_calc_now[i] = 0;
        cst_read[i] = 0;
        cst_write[i] = 0;
        SDL_zero(combo_type[i]);
        SDL_zero(remake_power[i]);
        SDL_zeroa(calc_hit[i]);
        SDL_zeroa(score_calc[i]);
    }

    first_attack = 0;
    hit_num = 0;
    sa_kind = 0;
    cmb_all_stock[0] = 0;
    last_hit_time = 0;
    // Arcade balance limits queue access to the first four port records.
    SDL_zero(cmst_buff);
}

/* One frame of both players' combo windows. The order the two players are
 * stepped in alternates with Game_timer, which matters when both are in a combo
 * at once; while paused only the window animation runs. Both kept as they were. */
/* One player's combo window: stepped, then drawn. */
static void step_one_combo_window(s8 i) {
    combo_control(i);
    combo_window_trans(i);
}

/* The two players in order, doing the same thing to each. The two ascending
 * passes below differ only in what that is. */
static void for_each_player(void (*step)(s8)) {
    s8 i;

    for (i = 0; i < 2; i++) {
        step(i);
    }
}

static void step_both_combo_windows(void) {
    s8 i;

    if (Game_pause != 0) {
        for_each_player(combo_window_trans);
        return;
    }

    if (Game_timer & 1) {
        for_each_player(step_one_combo_window);
        return;
    }

    for (i = 1; i > -1; i--) {
        step_one_combo_window(i);
    }
}

void combo_cont_main() { // 🟡
    if (Stop_Combo) {
        // CPS3 resets active combo state here; the port normally defers it.
        if (Demo_Flag && !ArcadeBalance_IsEnabled()) {
            return;
        }
        combo_cont_init();

        if (Demo_Flag) {
            Stop_Combo = 0;
        }

        return;
    }

    if (Demo_Flag != 0) {
        step_both_combo_windows();

        cmb_all_stock[0] = cmb_stock[0] + cmb_stock[1];
    }
}

/* One player's combo advancing by a hit: opening it, closing it, or counting
 * the hit. Each of the original's three `return`s ended combo_control with
 * nothing after the block, so returning from the helper reaches the same place. */
static void advance_combo_for(s8 PL, s16 cmb_flag) {
    if (first_attack == 0) {
        first_attack = plw[PL].wu.id + 1;
        combo_window_push(PL, 4);
        return;
    }

    if (pcon_dp_flag && last_hit_time == 0) {
        super_arts_last_check(PL);
    }

    if (cmb_flag != 0) {
        return;
    } else if (plw[PL].cb->total == 1) {
        // Port-only training telemetry does not affect combo rules.
        training_disp_data_set(PL, 1);
        super_arts_finish_check(PL);
        combo_hensuu_clear(PL);
        first_attack = 3;
        return;
    }

    check_and_set_combo(PL);
}

void combo_control(s8 PL) { // 🟡
    s16 cmb_flag;

    cmb_flag = check_combo_end(PL + 0);

    if (cmb_flag) {
        cmb_calc_now[PL] = 1;
    } else {
        cmb_calc_now[PL] = 0;
    }

    if (reversal_check(PL) == 0) {
        if (rever_attack[PL]) {
            reversal_continue_check(PL);
        }

        if (!paring_check(PL) && plw[PL].cb->total) {
            advance_combo_for(PL, cmb_flag);
        }
    }
}

// CPS3 inlines the gameplay portion of this port helper in combo_control.
void check_and_set_combo(s8 PL) { // 🔴
    s8 PLS;

    if (PL == 0) {
        PLS = 1;
    } else {
        PLS = 0;
    }

    hit_num = plw[PL].cb->total;

    if (hit_num > 99) {
        hit_num = 99;
    }

    training_disp_data_set(PL, hit_num);

    if (first_attack == 1 || first_attack == 2) {
        first_attack_pts_check(PL);
    }

    if (rever_attack[PLS] == 1) {
        reversal_attack_pts_check(PL);
    }

    hit_combo_check(PL);
    combo_hensuu_clear(PL);
}

void combo_hensuu_clear(s8 PL) { // 🟡
    SDL_zerop(plw[PL].cb);
    combo_rp_clear_check(PL);
    SDL_zeroa(calc_hit[PL]);
    SDL_zeroa(score_calc[PL]);
    bonus_pts[PL] = 0;
    plw[PL].cb->total = 0;
    hit_num = 0;
    // Port-only training telemetry; CPS3 does not clear this data here.
    tr_data[PL].total_damage = 0;
}

/* The reversal-pattern record only means anything while the player is in one
 * particular damage state; every other state makes it stale. */
static s32 rp_state_is_stale(s8 PL) {
    return plw[PL].wu.routine_no[1] != 1 || plw[PL].wu.routine_no[2] != 17 || plw[PL].wu.routine_no[3] == 0 ||
           plw[PL].wu.routine_no[3] == 3;
}

void combo_rp_clear_check(s8 PL) { // 🟢
    if (rp_state_is_stale(PL)) {
        SDL_zerop(plw[PL].rp);
    }
}

void super_arts_finish_check(s8 PL) { // 🟢
    if (arts_finish_check2(PL) != 0) {
        if ((plw[PL].cb->new_dm & 0x3F) < 48) {
            sa_kind = 2;
        } else {
            sa_kind = 3;
        }

        combo_window_push(PL, 3);
    }
}

void super_arts_last_check(s8 PL) { // 🟢
    if ((plw[PL].cb->new_dm & 0x3F) >= 0x20) {
        sarts_finish_flag[PL] = 1;
    } else {
        sarts_finish_flag[PL] = 0;
    }

    last_hit_time = 1;
}

void first_attack_pts_check(s8 PL) { // 🟢
    if (first_attack - 1 == plw[PL].wu.id) {
        first_attack = 3;
        bonus_pts[PL] += 2;
    }
}

/* The frames in which an attack counts as a reversal: coming out of a damage
 * state into an attack, past the first few patterns, and not during the
 * dramatic pause. */
static s32 reversal_window_is_open(s8 PL) {
    return plw[PL].wu.routine_no[1] == 4 && plw[PL].wu.old_rno[1] == 1 && !pcon_dp_flag &&
           plw[PL].wu.routine_no[2] >= 0x10;
}

s32 reversal_check(s8 PL) { // 🟢
    s8 PLS;

    if (rever_attack[PL]) {
        return 0;
    }

    if (reversal_window_is_open(PL)) {
        rever_attack[PL] = 1;

        if (PL == 0) {
            PLS = 1;
        } else {
            PLS = 0;
        }

        combo_window_push(PLS, 5);
        grade_add_reversal(PL);
        return 1;
    }

    return 0;
}

void reversal_continue_check(s8 PL) { // 🟢
    if (plw[PL].wu.routine_no[1] != 4) {
        rever_attack[PL] = 0;
    }
}

void reversal_attack_pts_check(s8 PL) { // 🟢
    bonus_pts[PL]++;
}

s32 paring_check(s8 PL) { // 🟢
    s8 PLS;

    if (paring_bonus_r[PL]) {
        paring_bonus_r[PL] = 0;
        paring_attack[PL] = 1;

        if (PL == 0) {
            PLS = 1;
        } else {
            PLS = 0;
        }

        combo_window_push(PLS, 6);
        return 1;
    }

    return 0;
}

/* The first art slot that was used decides the record kind: a finished super art
 * scores a bonus that depends on which half of the table the slot came from,
 * and anything else is an ordinary special combo. Both of the original's
 * `return`s left hit_combo_check with nothing after them. */
static void push_combo_for_used_art(s8 PL, s8 lpx) {
    if (arts_finish_check(PL)) {
        if (lpx < 8) {
            bonus_pts[PL] += 2;
            sa_kind = 2;
        } else {
            bonus_pts[PL] += 3;
            sa_kind = 3;
        }

        combo_window_push(PL, 2);
        return;
    }

    combo_window_push(PL, 1);
}

void hit_combo_check(s8 PL) { // 🟢
    s32* sa_ptr = (s32*)plw[PL].cb->kind_of[4][0];
    s8 lpx;

    for (lpx = 0; lpx < 20; lpx++) {
        if (!(*sa_ptr++ == 0)) {
            push_combo_for_used_art(PL, lpx);
            return;
        }
    }

    combo_window_push(PL, 0);
}

s32 arts_finish_check(s8 PL) { // 🟢
    if (round_lost_by(PL) && sarts_finish_flag[PL]) {
        return 1;
    }

    return 0;
}

s32 arts_finish_check2(u8 PL) { // 🟢
    if (round_lost_by(PL) && (plw[PL].cb->new_dm & 0x3F) >= 32) {
        return 1;
    }

    return 0;
}

u32 SCORE_CALCULATION(s8 PL) { // 🟢
    s16* c_ptr;
    s16* s_ptr;
    s16* k_ptr;
    s8 lpx;
    s8 lpy;
    s16 hit;
    s16 h;
    u32 score;
    s8 last = 4;

    k_ptr = plw[PL].cb->kind_of[0][0];
    c_ptr = &calc_hit[PL][1];
    s_ptr = score_calc[PL];

    for (lpx = 0; lpx < 4; lpx++) {
        *s_ptr++ = k_ptr[0] + k_ptr[1];
        k_ptr += 2;
    }

    s_ptr = &score_calc[PL][4];

    for (lpy = 0; lpy < 8; lpy++) {
        *s_ptr++ = *c_ptr++;
    }

    hit = 0;
    score = 0;

    for (lpy = 0; lpy < 12; lpy++) {
        if (score_calc[PL][lpy]) {
            last = lpy;
            h = score_calc[PL][lpy];
            hit += h;
            score += *combo_score_tbl[lpy];

            if (h - 1) {
                score += (hit - 1) * combo_score_tbl[lpy][1];
            }
        }
    }

    if (bonus_pts[PL]) {
        score += bonus_pts[PL] * combo_score_tbl[last][1];
    }

    return score;
}

void SCORE_PLUS(s8 PL, u32 PTS) { // 🟡
    if (ArcadeBalance_IsEnabled()) {
        // CPS3 caps only the active single-player score column.
        Score[PL][2] += PTS;

        if (Play_Type == 0) {
            Score[PL][0] += PTS;

            if (Score[PL][0] >= 99999900) {
                Score[PL][0] = 99999900;
            }
        } else {
            Score[PL][1] += PTS;
        }

        return;
    }

    // The port caps both accumulated score columns.
    Score[PL][2] += PTS;

    if (Score[PL][2] >= 99999900) {
        Score[PL][2] = 99999900;
    }

    Score[PL][Play_Type] += PTS;

    if (Score[PL][Play_Type] >= 99999900) {
        Score[PL][Play_Type] = 99999900;
    }
}

/* Outside arcade balance, the combo's score is shown at once in versus and
 * training, and whenever the other side is a human player. */
static s32 score_is_shown_immediately(s8 PLS) {
    return Mode_Type == MODE_VERSUS || Mode_Type == 5 || plw[PLS].wu.operator;
}

/* Split the queued combo's score into the digits the renderer draws, and note
 * which is the first non-zero one so the display knows where to start. */
static void write_score_digits(s8 PL, u32 score) {
    u32 Pts_Buff;
    s32 xx;
    s16 i;

    // The port precomputes renderer digits here; CPS3 prepares tiles while transitioning the record.
    Pts_Buff = cmst_buff[PL][cst_write[PL]].pts = score;
    i = 3;
    xx = 100000;

    cmst_buff[PL][cst_write[PL]].first_digit = -1;

    while (i >= 0) {
        cmst_buff[PL][cst_write[PL]].pts_digit[i] = Pts_Buff / xx & 0xFF;
        Pts_Buff -= (cmst_buff[PL][cst_write[PL]].pts_digit[i] * xx);

        if (cmst_buff[PL][cst_write[PL]].first_digit < 0 && cmst_buff[PL][cst_write[PL]].pts_digit[i]) {
            cmst_buff[PL][cst_write[PL]].first_digit = i;
        }

        i--;
        xx /= 10;
    }

    cmst_buff[PL][cst_write[PL]].move[1] = cmst_buff[PL][cst_write[PL]].first_digit + 6;
}

/* Step the write cursor round the queue. */
static void advance_combo_write_index(s8 PL, s8 stock_capacity) {
    if (cst_write[PL] == stock_capacity - 1) {
        cst_write[PL] = 0;
    } else {
        cst_write[PL]++;
    }
}

/* The display queue is full, so this combo's score is awarded at once rather
 * than queued for the animation to pay out. */
/* The super-art combo bonus, which depends on which art it was. Both score
 * functions used these two values twice each - once added to the running score
 * and once replacing it - so only the value is shared here; whether it is added
 * or assigned stays at the call site. */
static u32 super_art_bonus(void) {
    if (sa_kind == 2) {
        return 20000;
    }

    return 30000;
}

/* The score a combo is worth when the queue is full, by record kind. */
static u32 full_queue_score(s8 PLS, s8 KIND, u32 score) {
    switch (KIND) {
    case 2:
        score += super_art_bonus();
        break;

    case 3:
        score = super_art_bonus();
        break;

    case 4:
        score = 1500;
        grade_get_first_attack(PLS);
        break;

    case 6:
        score = paring_counter[PLS] * 100;
        break;
    }

    return score;
}

static void award_score_without_queueing(s8 PL, s8 PLS, s8 KIND, u32 score) {
    score = full_queue_score(PLS, KIND, score);

    if (score >= 1000000) {
        score = 999900;
    }

    SCORE_PLUS(PLS, score);

    if (versus_rules_apply(PLS)) {
        Score_Sub();
    }

    return;
}

/* Queue the record for the window animation to draw and pay out. */
/* The score a queued combo record is worth, by record kind. Not shared with
 * full_queue_score: this one has two extra arms, and its kind 5 also clears the
 * record's pts_flag, so it touches state the other does not. */
static u32 queued_record_score(s8 PL, s8 PLS, s8 KIND, u32 score) {
    switch (KIND) {
    case 0:
    case 1:
        break;

    case 2:
        score += super_art_bonus();
        break;

    case 3:
        score = super_art_bonus();
        break;

    case 4:
        score = 1500;
        grade_get_first_attack(PLS);
        break;

    case 5:
        score = 0;
        cmst_buff[PL][cst_write[PL]].pts_flag = 0;
        break;

    case 6:
        score = paring_counter[PLS] * 100;
        break;
    }

    return score;
}

/* The five values queue_combo_record is driven by. The field order is the
 * parameter order it used to take, and the types are the same. */
typedef struct {
    s8 PL;
    s8 PLS;
    s8 KIND;
    u32 score;
    s8 stock_capacity;
} ComboRecordArgs;

static void queue_combo_record(const ComboRecordArgs* args) {
    s8 PL = args->PL;
    s8 PLS = args->PLS;
    s8 KIND = args->KIND;
    u32 score = args->score;
    s8 stock_capacity = args->stock_capacity;

    cmb_stock[PL]++;
    cmst_buff[PL][cst_write[PL]].routine_num = 0;
    cmst_buff[PL][cst_write[PL]].hit_hi = (u8)hit_num / 10;
    cmst_buff[PL][cst_write[PL]].hit_low = (u8)hit_num - (cmst_buff[PL][cst_write[PL]].hit_hi * 10);
    cmst_buff[PL][cst_write[PL]].kind = KIND;

    if (ArcadeBalance_IsEnabled()) {
        // CPS3 ties delayed score display only to the opposing operator.
        cmst_buff[PL][cst_write[PL]].pts_flag = plw[PLS].wu.operator != 0;
    } else if (score_is_shown_immediately(PLS)) {
        cmst_buff[PL][cst_write[PL]].pts_flag = 1;
    } else {
        cmst_buff[PL][cst_write[PL]].pts_flag = 0;
    }

    score = queued_record_score(PL, PLS, KIND, score);

    if (score >= 1000000) {
        score = 999900;
    }

    write_score_digits(PL, score);
    advance_combo_write_index(PL, stock_capacity);
}

void combo_window_push(s8 PL, s8 KIND) { // 🟡
    u32 score;
    s8 PLS;

    if (KIND < 3) {
        score = SCORE_CALCULATION(PL);
        grade_max_combo_check(PL ^ 1, hit_num);
    }

    if (PL == 0) {
        PLS = 1;
    } else {
        PLS = 0;
    }

    // CPS3 has four queue records; the port renderer has five.
    const s8 stock_capacity = ArcadeBalance_IsEnabled() ? 4 : 5;

    if (cmb_stock[PL] == stock_capacity) {
        award_score_without_queueing(PL, PLS, KIND, score);
        return;
    }

    queue_combo_record(&(ComboRecordArgs){ PL, PLS, KIND, score, stock_capacity });
}

/* Animating one queued combo record that shows a score: the message slides in,
 * then the points, then the record is held and retired. */
/* Draw the combo message at its current slide position. Written out four times
 * across the two window animations, identically. */
static void draw_combo_message_now(s8 PL) {
    combo_message_set(&(ComboMessage){ PL, cmst_buff[PL][cst_read[PL]].kind, cmb_pos_tbl[PL][cmst_buff[PL][cst_read[PL]].x_posnum[0] - 1], cmst_buff[PL][cst_read[PL]].x_posnum[0], cmst_buff[PL][cst_read[PL]].hit_hi, cmst_buff[PL][cst_read[PL]].hit_low });
}

/* Redraw the message at its current slide position. */
static void draw_combo_message(s8 PL) {
    if (cmst_buff[PL][cst_read[PL]].x_posnum[0] != 0) {
        draw_combo_message_now(PL);
    }
}

/* Move the message one step along, until it reaches its resting place and sets
 * its end flag. */
static void slide_combo_message(s8 PL) {
    if (!(Game_pause & 0x80) && !(end_flag[PL] & 1)) {
        if (cmst_buff[PL][cst_read[PL]].x_posnum[0] < cmst_buff[PL][cst_read[PL]].move[0]) {
            cmst_buff[PL][cst_read[PL]].x_posnum[0]++;
        } else {
            end_flag[PL] |= 1;
        }
    }
}

/* Move the points display one digit-step along. Once it is fully out the `else`
 * arm keeps it drawn where it stopped. */
/* Draw the points row at a given slide position. Written out four times in the
 * points slide, identical apart from that position - twice at the row's resting
 * place and twice mid-slide. */
static void draw_combo_points_at(s8 PL, s16 posnum) {
    combo_pts_set(&(ComboPoints){ PL, cmb_pos_tbl[PL][cmst_buff[PL][cst_read[PL]].x_posnum[1] - 1], posnum, &cmst_buff[PL][cst_read[PL]].pts_digit[0] }, cmst_buff[PL][cst_read[PL]].first_digit);
}

/* One step of the points row sliding out, or the step that finishes it and sets
 * the end flag. The two draws differ in where they put the row: mid-slide it
 * follows x_posnum, and at the end it sits at its resting place. */
static void step_points_slide(s8 PL) {
    if (cmst_buff[PL][cst_read[PL]].x_posnum[1] < (cmst_buff[PL][cst_read[PL]].move[1] + 2)) {
        if (cmst_buff[PL][cst_read[PL]].x_posnum[1] < cmst_buff[PL][cst_read[PL]].move[1]) {
            if (cmst_buff[PL][cst_read[PL]].x_posnum[1] != 0) {
                draw_combo_points_at(PL, cmst_buff[PL][cst_read[PL]].x_posnum[1]);
            }
        } else if (cmst_buff[PL][cst_read[PL]].x_posnum[1] != 0) {
            draw_combo_points_at(PL, (cmst_buff[PL][cst_read[PL]].move[1] - 1));
        }

        if (!(Game_pause & 0x80)) {
            cmst_buff[PL][cst_read[PL]].x_posnum[1]++;
        }
    } else {
        end_flag[PL] |= 2;

        draw_combo_points_at(PL, (cmst_buff[PL][cst_read[PL]].move[1] - 1));
    }
}

static void slide_combo_points(s8 PL) {
    if (!(end_flag[PL] & 2)) {
        if (!(Game_pause & 0x80)) {
            cmst_buff[PL][cst_read[PL]].timer[0]--;
        }

        if (cmst_buff[PL][cst_read[PL]].timer[0] < 0) {
            step_points_slide(PL);
        }
    } else {
        draw_combo_points_at(PL, (cmst_buff[PL][cst_read[PL]].move[1] - 1));
    }
}

/* Once both the message and the points have finished, award the score to the
 * other side and start the hold timer. PLS was a local of the whole function
 * and is used only here, so it moves in with the block. */
static void commit_combo_points(s8 PL) {
    s8 PLS;

    if (!(Game_pause & 0x80) && ((end_flag[PL] & 3) == 3)) {
        cmst_buff[PL][cst_read[PL]].routine_num++;
        cmst_buff[PL][cst_read[PL]].timer[1] = cmb_window_time_tbl[cmst_buff[PL][cst_read[PL]].kind];

        if (PL == 0) {
            PLS = 1;
        } else {
            PLS = 0;
        }

        SCORE_PLUS(PLS, cmst_buff[PL][cst_read[PL]].pts);

        if (versus_rules_apply(PLS)) {
            Score_Sub();
        }
    }
}


static void run_scored_combo_window(s8 PL) {
    switch (cmst_buff[PL][cst_read[PL]].routine_num) {
    case 0:
        end_flag[PL] = 0;
        cmst_buff[PL][cst_read[PL]].move[0] = cmb_window_move_tbl[(cmst_buff[PL][cst_read[PL]].kind)];
        cmst_buff[PL][cst_read[PL]].x_posnum[0] = 0;
        cmst_buff[PL][cst_read[PL]].timer[0] = 8;
        cmst_buff[PL][cst_read[PL]].x_posnum[1] = 0;
        cmst_buff[PL][cst_read[PL]].routine_num++;
        break;

    case 1:
        draw_combo_message(PL);
        slide_combo_message(PL);
        slide_combo_points(PL);
        commit_combo_points(PL);
        break;

    case 2:
        if (!(Game_pause & 0x80)) {
            cmst_buff[PL][cst_read[PL]].timer[1]--;

            if (cmst_buff[PL][cst_read[PL]].timer[1]) {
                draw_combo_message_now(PL);

                draw_combo_points_at(PL, (cmst_buff[PL][cst_read[PL]].move[1] - 1));

                return;
            }
        } else {
            draw_combo_message_now(PL);
            draw_combo_points_at(PL, (cmst_buff[PL][cst_read[PL]].move[1] - 1));
            return;
        }

        // CPS3 wraps its four-record queue after index 3.
        if (cst_read[PL] == (ArcadeBalance_IsEnabled() ? 3 : 4)) {
            cst_read[PL] = 0;
        } else {
            cst_read[PL]++;
        }

        cmb_stock[PL]--;
        break;
    }
}

/* The same for a record with no score to show - the message only. */
/* Hold the finished message on screen, then either hand over to the scored
 * animation or retire the record and free its queue slot. Every `return` in the
 * original left the switch with nothing after it. */
static void hold_then_retire_plain_record(s8 PL) {
    if (!(Game_pause & 0x80)) {
        cmst_buff[PL][cst_read[PL]].timer[1]--;

        if (cmst_buff[PL][cst_read[PL]].timer[1]) {
            draw_combo_message_now(PL);

            return;
        }
    } else {
        draw_combo_message_now(PL);

        return;
    }

    if (cmst_buff[PL][cst_read[PL]].pts_flag) {
        cmst_buff[PL][cst_read[PL]].routine_num++;
        return;
    }

    if (cst_read[PL] == (ArcadeBalance_IsEnabled() ? 3 : 4)) {
        cst_read[PL] = 0;
    } else {
        cst_read[PL]++;
    }

    cmb_stock[PL]--;
}

static void run_plain_combo_window(s8 PL) {
    switch ((cmst_buff[PL][cst_read[PL]].routine_num)) {
    case 0:
        cmst_buff[PL][cst_read[PL]].move[0] = cmb_window_move_tbl[(cmst_buff[PL][cst_read[PL]].kind)];
        // CPS3 starts at zero and advances before drawing; the port normally starts at one.
        cmst_buff[PL][cst_read[PL]].x_posnum[0] = ArcadeBalance_IsEnabled() ? 0 : 1;
        cmst_buff[PL][cst_read[PL]].routine_num++;
        break;

    case 1:
        if (!(Game_pause & 0x80)) {
            if ((cmst_buff[PL][cst_read[PL]].x_posnum[0]) < (cmst_buff[PL][cst_read[PL]].move[0])) {
                cmst_buff[PL][cst_read[PL]].x_posnum[0]++;
            } else {
                cmst_buff[PL][cst_read[PL]].timer[1] = 36;
                cmst_buff[PL][cst_read[PL]].routine_num++;
            }
        }

        draw_combo_message_now(PL);

        break;

    case 2:
        hold_then_retire_plain_record(PL);
        break;
    }
}

void combo_window_trans(s8 PL) { // 🔴
    if (cmb_stock[PL] == 0) {
        return;
    }

    // Port-only training display suppression is retained.
    if (Mode_Type == MODE_NORMAL_TRAINING && Training_ID == PL) {
        cmb_stock[PL]--;
        return;
    }

    if (cmst_buff[PL][cst_read[PL]].pts_flag) {
        run_scored_combo_window(PL);
    } else {
        run_plain_combo_window(PL);
    }
}

void training_disp_data_set(s8 PL, s16 hnum) { // 🔴
    if (tr_data[PL].max_hitcombo < (u8)hnum) {
        if (Disp_Attack_Data != 0) {
            if (Champion != PL) {
                SsRequest(0x9C);
            }

            tr_data[PL].new_max_flag = 0x14;
            tr_data[PL].frash_switch = 2;
        }

        tr_data[PL].frash_flag = 0;
        tr_data[PL].max_hitcombo = hnum;

        if (tr_data[PL].max_hitcombo > 99) {
            tr_data[PL].max_hitcombo = 99;
        }
    }
}

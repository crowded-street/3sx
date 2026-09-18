/**
 * @file lose_pl.c
 * Losing Character Animations
 */

#include "sf33rd/Source/Game/animation/lose_pl.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effc1.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/work_sys.h"

const s16 loser_type_tbl[20] = { 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0 };

const s16 meta_lose_tbl[20] = { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 28, 24, 24, 24, 24, 24, 24 };

static s32 judge_screen_showing() {
    return (pcon_rno[0] == 2) && (pcon_rno[1] == 3);
}

static s32 lose_anime_suspended() {
    return (pcon_rno[1] == 0) || (pcon_rno[1] == 4);
}

static s32 final_defeat_effect_due() {
    return !Extra_Break && ((Round_num >= (save_w[Present_Mode].Battle_Number[Play_Type]) * 2) ||
                            (PL_Wins[Winner_id] >= (save_w[Present_Mode].Battle_Number[Play_Type]) + 1));
}

static void update_field_hosei_flags(PLW* wk) {
    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

void lose_player(PLW* wk) {
    void (*lose_jp_tbl[4])(PLW*) = { Lose_00000, Lose_10000, Lose_20000, Lose_30000 };

    if (My_char[wk->wu.id] != wk->player_number) {
        meta_lose_pause(wk);
        return;
    }

    lose_jp_tbl[loser_type_tbl[wk->player_number]](wk);
}

void Lose_00000(PLW* wk) {
    if (judge_screen_showing()) {
        Judge_normal_loser(wk);
        return;
    }

    Normal_normal_Loser(wk);
}

static void step_loss_state(PLW* wk, void (*start_anime)(PLW*)) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        start_anime(wk);
        break;

    case 1:
    case 9:
        char_move(&wk->wu);
        break;
    }
}

static void start_random_judge_loss(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.char_index = random_16();
    wk->wu.char_index &= 3;
    set_char_move_init(&wk->wu, 9, wk->wu.char_index + 0x38);
}

static void play_random_judge_loss(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        start_random_judge_loss(wk);
        break;

    default:
    case 1:
    case 9:
        char_move(&wk->wu);
        break;
    }
}

static void start_random_round_loss(PLW* wk) {
    wk->wu.routine_no[3]++;
    wk->wu.char_index = random_16();
    wk->wu.char_index &= 7;
    set_char_move_init(&wk->wu, 9, wk->wu.char_index + 0x18);
}

static void play_random_round_loss(PLW* wk) {
    step_loss_state(wk, start_random_round_loss);
}

static void step_loss_anime(PLW* wk, void (*judge_step)(PLW*), void (*round_step)(PLW*)) {
    if (judge_screen_showing()) {
        judge_step(wk);
    } else if (lose_anime_suspended()) {
        return;
    } else {
        round_step(wk);
    }

    update_field_hosei_flags(wk);
}

void Lose_10000(PLW* wk) {
    step_loss_anime(wk, play_random_judge_loss, play_random_round_loss);
}

void Lose_20000(PLW* wk) {
    s16 work;

    if (judge_screen_showing()) {
        Judge_normal_loser(wk);
        return;
    }

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;

        if (final_defeat_effect_due()) {
            effect_C1_init(&wk->wu);
        }

        if ((pcon_rno[1] != 0) && (pcon_rno[1] != 4)) {
            work = random_16();
            work &= 7;
            set_char_move_init(&wk->wu, 9, work + 0x18);
            break;
        }

        break;

    default:
        Normal_normal_Loser(wk);
        break;
    }

    update_field_hosei_flags(wk);
}

static void start_fixed_judge_loss(PLW* wk) {
    wk->wu.routine_no[3]++;
    set_char_move_init(&wk->wu, 9, 56);
}

static void play_fixed_judge_loss(PLW* wk) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        start_fixed_judge_loss(wk);
        break;

    default:
    case 1:
    case 9:
        char_move(&wk->wu);
        break;
    }
}

static void start_fixed_round_loss(PLW* wk) {
    wk->wu.routine_no[3]++;
    set_char_move_init(&wk->wu, 9, 24);
}

static void play_fixed_round_loss(PLW* wk) {
    step_loss_state(wk, start_fixed_round_loss);
}

void Lose_30000(PLW* wk) {
    step_loss_anime(wk, play_fixed_judge_loss, play_fixed_round_loss);
}

static void start_normal_round_loss(PLW* wk) {
    s16 work;

    wk->wu.routine_no[3]++;
    work = random_16();
    work &= 7;
    set_char_move_init(&wk->wu, 9, work + 0x18);
}

void Normal_normal_Loser(PLW* wk) {
    if (lose_anime_suspended()) {
        return;
    }

    step_loss_state(wk, start_normal_round_loss);

    update_field_hosei_flags(wk);
}

void Judge_normal_loser(PLW* wk) {
    s16 work;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        work = random_16();
        work &= 3;
        set_char_move_init(&wk->wu, 9, work + 0x38);
        break;

    case 1:
    case 9:
    default:
        char_move(&wk->wu);
        break;
    }

    update_field_hosei_flags(wk);
}

static void start_meta_loss(PLW* wk) {
    wk->wu.routine_no[3] += 1;
    set_char_move_init(&wk->wu, 9, meta_lose_tbl[wk->player_number]);
}

void meta_lose_pause(PLW* wk) {
    bg_app_stop = 1;

    if (lose_anime_suspended()) {
        return;
    }

    step_loss_state(wk, start_meta_loss);

    update_field_hosei_flags(wk);
}

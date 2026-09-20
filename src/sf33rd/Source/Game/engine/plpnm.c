/**
 * @file plpnm.c
 * Player Normal Move and State Controller
 */

#include "arcade/arcade_balance.h"
#include "common.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/animation/lose_pl.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/effect/effg6.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls00.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"

void Player_normal(PLW* wk);
void setup_normal_process_flags(PLW* wk);
void Normal_00000(PLW* wk);
void Normal_01000(PLW* wk);
void Normal_02000(PLW* wk);
void Normal_03000(PLW* wk);
void Normal_04000(PLW* wk);
void Normal_05000(PLW* wk);
void nm_05_0000(PLW* wk);
void nm_05_0100(PLW* wk);
void Normal_06000(PLW* wk);
void nm_06_0000(PLW* wk);
void nm_06_0100(PLW* wk);
void nm_06_0200(PLW* wk);
void Normal_07000(PLW* wk);
void Normal_08000(PLW* wk);
void Normal_09000(PLW* wk);
void Normal_10000(PLW* wk);
void Normal_11000(PLW* wk);
void Normal_12000(PLW* wk);
void Normal_13000(PLW* wk);
void Normal_16000(PLW* wk);
void Normal_17000(PLW* wk);
void Normal_18000(PLW* wk);
void Normal_18000_init_unit(PLW* wk, u8 ps);
void Normal_27000(PLW* wk);
void Normal_31000(PLW* wk);
void Normal_35000(PLW* wk);
void Normal_36000(PLW* wk);
void Normal_37000(PLW* wk);
void Normal_38000(PLW* wk);
void Normal_39000(PLW* wk);
void Normal_40000(PLW* wk);
/* Still alive, and not already sitting in the very first pattern of the first
 * animation set. */
static s32 alive_and_not_in_first_pattern(const PLW* wk) {
    return wk->wu.vital_new >= 0 && (wk->wu.now_koc != 0 || wk->wu.char_index != 0);
}

void Normal_41000(PLW* wk);
void Normal_42000(PLW* wk);
void Normal_47000(PLW* wk);
void Normal_48000(PLW* wk);
void Normal_50000(PLW* wk);
void Normal_51000(PLW* wk);
void Normal_52000(PLW* wk);
void Normal_53000(PLW* wk);
void Normal_54000(PLW* wk);
void Normal_55000(PLW* wk);
void make_nm55_init_sp(PLW* wk);
void Normal_56000(PLW* wk);
void nm56_char_select(PLW* wk);
void Normal_57000(PLW* wk);
void Normal_58000(PLW* wk);
void nm57_dir_select(PLW* wk);

const s16 nmPB_data[5][3];
const s16 nmCE_data[4][3];
const u16 jpdat_tbl[9][2];

void (*const plpnm_lv_00[59])(PLW* wk);
void (*const plpnm_lv_00_cps3[59])(PLW* wk);
void (*const normal_05[])(PLW* wk);
void (*const normal_06[])(PLW* wk);

void Player_normal(PLW* wk) { // 🟡
    setup_normal_process_flags(wk);

    // Chain-EX state is a port-only gameplay reset; CPS3 omits it.
    if (!ArcadeBalance_IsEnabled() && wk->wu.routine_no[3] == 0) {
        clear_chainex_check(wk->wu.id);
    }

    check_my_tk_power_off(wk, (PLW*)wk->wu.target_adrs);
    check_em_tk_power_off(wk, (PLW*)wk->wu.target_adrs);

    if (ArcadeBalance_IsEnabled()) {
        plpnm_lv_00_cps3[wk->wu.routine_no[2]](wk);
    } else {
        plpnm_lv_00[wk->wu.routine_no[2]](wk);
    }

    if (wk->wu.cg_prio) {
        wk->wu.next_z = ((WORK*)wk->wu.target_adrs)->my_priority;

        if (wk->wu.cg_prio == 1) {
            wk->wu.next_z += 1;
        } else {
            wk->wu.next_z -= 3;
        }
    }
}

void setup_normal_process_flags(PLW* wk) { // 🟡
    wk->wu.next_z = wk->wu.my_priority;
    wk->running_f = 0;
    wk->py->flag = 0;
    wk->guard_flag = 0;
    wk->guard_chuu = 0;
    wk->tsukami_f = false;
    wk->tsukamare_f = false;
    wk->scr_pos_set_flag = 1;
    wk->dm_hos_flag = 0;
    wk->ukemi_success = 0;
    wk->zuru_timer = 0;
    wk->zuru_ix_counter = 0;
    wk->sa_stop_flag = 0;
    wk->atemi_flag = 0;
    wk->caution_flag = 0;
    wk->sa->saeff_ok = 0;
    wk->sa->saeff_mp = 0;
    wk->ukemi_success = 0;
    wk->ukemi_ok_timer = 0;
    wk->uot_cd_ok_flag = 0;
    wk->cancel_timer = 0;
    wk->hazusenai_flag = 0;
    wk->cat_break_reserve = 0;
    wk->cmd_request = 0;
    wk->hsjp_ok = 0;

    if (wk->wu.routine_no[2] != 17) {
        wk->high_jump_flag = 0;
    }

    wk->wu.swallow_no_effect = 0; // Port-only visual suppression; CPS3 does not reset this effect flag here.
}

void Normal_00000(PLW* wk) { // 🟢
    appear_player(wk);
}

/* The plainest normal state there is: start the pattern on the first frame,
 * then run it. Five states differ in one value, the pattern index. */
static void run_simple_normal_state(PLW* wk, s16 index) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, index);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

/* The same, for the states that first turn the character to face the way the
 * move wants. Three states differ in one value, the pattern index. */
static void run_facing_normal_state(PLW* wk, s16 index) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 0, index);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

/* In a mirror match the two players are drawn one in front of the other. The
 * states that put this player in front, and the ones that put them behind,
 * each wrote the same three lines out - twenty and nine times. */
static void raise_z_when_mirrored(PLW* wk) {
    if (wk->the_same_players) {
        wk->wu.next_z = wk->wu.my_priority + 1;
    }
}

static void lower_z_when_mirrored(PLW* wk) {
    if (wk->the_same_players) {
        wk->wu.next_z = wk->wu.my_priority - 1;
    }
}

void Normal_01000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    run_simple_normal_state(wk, 0);
}

void Normal_02000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    run_facing_normal_state(wk, 1);
}

/* The moving normal states: start the pattern, load a movement row and apply
 * it, then cal/add/move every frame after. Three states differ in two values,
 * the pattern index and the movement row, both written out at the call site. */
static void run_moving_normal_state(PLW* wk, s16 index, s16 mvxy_row) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, index);
        setup_mvxy_data(&wk->wu, mvxy_row);
        add_mvxy_speed(&wk->wu);
        break;

    case 1:
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        char_move(&wk->wu);
        break;
    }
}

void Normal_03000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, 2);
        setup_mvxy_data(&wk->wu, 0);
        wk->wu.mvxy.a[0].sp >>= 1;
        add_mvxy_speed(&wk->wu);
        wk->wu.mvxy.a[0].sp *= 2;
        break;

    case 1:
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        char_move(&wk->wu);
        break;
    }
}

void Normal_04000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    run_moving_normal_state(wk, 3, 1);
}

void Normal_05000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    wk->running_f = 1;
    wk->guard_flag = 3;
    normal_05[wk->player_number](wk);
    jumping_guard_type_check(wk);
}

/* The state nm_05_0000 and nm_06_0100 share: one skeleton, byte for byte apart
 * from the pattern index and the movement row. */
static void run_nm_dash_state(PLW* wk, s16 index, s16 mvxy_row) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 0, index);
        setup_mvxy_data(&wk->wu, mvxy_row);
        /* fallthrough */

    case 1:
        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3]++;
            break;
        }

        char_move(&wk->wu);
        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void nm_05_0000(PLW* wk) { // 🟢
    run_nm_dash_state(wk, 4, 2);
}

/* The state nm_05_0100 and nm_06_0200 share: one skeleton, byte for byte apart
 * from the pattern index and the movement row. */
static void run_nm_dash_cancel_state(PLW* wk, s16 index, s16 mvxy_row) {
    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 0, index);
        setup_mvxy_data(&wk->wu, mvxy_row);

        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
        }

        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            add_mvxy_speed(&wk->wu);
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
        }

        break;

    case 2:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            wk->wu.routine_no[3]++;
            wk->wu.cg_type = 0;
            break;
        }

        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void nm_05_0100(PLW* wk) { // 🟢
    run_nm_dash_cancel_state(wk, 4, 2);
}

void Normal_06000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    wk->running_f = 2;
    wk->guard_flag = 3;
    normal_06[wk->player_number](wk);
    jumping_guard_type_check(wk);
}

void nm_06_0000(PLW* wk) { // 🟢
    run_facing_normal_state(wk, 5);
}

void nm_06_0100(PLW* wk) { // 🟢
    run_nm_dash_state(wk, 5, 3);
}

void nm_06_0200(PLW* wk) { // 🟢
    run_nm_dash_cancel_state(wk, 5, 3);
}

void Normal_07000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    run_simple_normal_state(wk, 11);
}

void Normal_08000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    run_simple_normal_state(wk, 6);
}

void Normal_09000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    run_simple_normal_state(wk, 7);
}

void Normal_10000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    run_facing_normal_state(wk, 8);
}

void Normal_11000(PLW* wk) { // 🔵
    lower_z_when_mirrored(wk);

    run_moving_normal_state(wk, 9, 4);
}

void Normal_12000(PLW* wk) { // 🔵
    raise_z_when_mirrored(wk);

    run_moving_normal_state(wk, 10, 5);
}

void Normal_13000(PLW* wk) { // 🔵
    raise_z_when_mirrored(wk);

    run_simple_normal_state(wk, 50);
}

/* The two air-guard states: put the player in front, take the guard flag, clear
 * the extra jump and run the pattern. They differ in one value, the pattern
 * index. */
static void run_guard_extra_jump_state(PLW* wk, s16 index) {
    raise_z_when_mirrored(wk);

    wk->guard_flag = 3;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->extra_jump = 0;
        set_char_move_init(&wk->wu, 0, index);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Normal_16000(PLW* wk) { // 🟢
    run_guard_extra_jump_state(wk, 12);
}

void Normal_17000(PLW* wk) { // 🟢
    run_guard_extra_jump_state(wk, 13);
}

void Normal_18000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, jpdat_tbl[wk->wu.routine_no[2] - 18][0]);
        setup_mvxy_data(&wk->wu, jpdat_tbl[wk->wu.routine_no[2] - 18][1]);
        add_mvxy_speed(&wk->wu);
        wk->air_jump_ok_time = 4;
        wk->bs2_on_car = 0;
        break;

    case 1:
        jumping_union_process(&wk->wu, 2);
        break;

    case 2:
        char_move(&wk->wu);
        break;
    }

    jumping_guard_type_check(wk);
}

void Normal_18000_init_unit(PLW* wk, u8 ps) { // 🟢
    ps = (ps - 14) / 2;

    if (ps > 8) {
        ps = 4;
    }

    set_char_move_init(&wk->wu, 0, jpdat_tbl[ps][0]);
    setup_mvxy_data(&wk->wu, jpdat_tbl[ps][1]);
    add_mvxy_speed(&wk->wu);
}

void Normal_27000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, wk->wu.routine_no[2] + 2);
        break;

    case 1:
        char_move(&wk->wu);
        break;
    }
}

void Normal_31000(PLW* wk) { // 🟡
    if (((WORK*)wk->wu.target_adrs)->cg_prio != 2) {
        wk->wu.next_z = 32;
    }

    wk->guard_chuu = guard_kind[wk->wu.routine_no[2] - 27];
    wk->scr_pos_set_flag = 0;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;
        set_char_move_init(&wk->wu, 0, wk->wu.routine_no[2] - 7);

        // CPS3 leaves damage-stop and vitality untouched in this guard state.
        if (!ArcadeBalance_IsEnabled() && wk->wu.dm_stop > 0) {
            wk->wu.dm_stop = -wk->wu.dm_stop;
        }

        set_hit_stop_hit_quake(&wk->wu);
        add_sp_arts_gauge_paring(wk);

        if (!ArcadeBalance_IsEnabled()) {
            subtract_dm_vital(wk);
        }

        pp_pulpara_blocking(&wk->wu);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca(&wk->wu);
        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

/* Entering the parry state: absorb the hit, turn to face the attacker if the
 * parry came from behind, award the gauge and play the sound. */
static void begin_parry(PLW* wk) {
    wk->wu.routine_no[3]++;

    // CPS3 leaves damage-stop and vitality untouched in this guard state.
    if (!ArcadeBalance_IsEnabled() && wk->wu.dm_stop > 0) {
        wk->wu.dm_stop = -wk->wu.dm_stop;
    }

    set_hit_stop_hit_quake(&wk->wu);

    if (wk->wu.rl_flag != ((wk->wu.dm_rl + 1) & 1)) {
        wk->wu.rl_flag = ((wk->wu.dm_rl + 1) & 1);
        wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
        wk->wu.mvxy.d[0].sp = -wk->wu.mvxy.d[0].sp;
    }

    remake_mvxy_PoSB(&wk->wu);
    set_char_move_init(&wk->wu, 0, 27);
    add_sp_arts_gauge_paring(wk);

    if (!ArcadeBalance_IsEnabled()) {
        subtract_dm_vital(wk);
    }

    pp_pulpara_blocking(&wk->wu);
}

void Normal_35000(PLW* wk) { // 🟡
    lower_z_when_mirrored(wk);

    wk->guard_chuu = guard_kind[wk->wu.routine_no[2] - 27];

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin_parry(wk);
        break;

    case 1:
        wk->wu.routine_no[3]++;
        char_move_wca_init(&wk->wu);
        /* fallthrough */

    case 2:
        if (((WORK*)wk->wu.target_adrs)->cg_prio != 2) {
            wk->wu.next_z = 32;
        }

        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void Normal_36000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    char_move(&wk->wu);
}

void Normal_37000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    char_move(&wk->wu);
}

void Normal_38000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
    case 1:
        jumping_union_process(&wk->wu, 2);
        break;

    case 2:
        char_move(&wk->wu);
        break;
    }

    jumping_guard_type_check(wk);
}

void Normal_39000(PLW* wk) { // 🟢
    lower_z_when_mirrored(wk);

    if (wk->wu.routine_no[3]) {
        char_move(&wk->wu);
        return;
    }

    wk->wu.routine_no[3]++;
    set_char_move_init(&wk->wu, 0, 23);
}

/* 40000's own version of the test 41000 spells alive_and_not_in_first_pattern. */
static s32 not_in_first_pattern(const PLW* wk) {
    return wk->wu.now_koc != 0 || (wk->wu.char_index != 0);
}

/* The round-end state Normal_40000 and Normal_41000 share. Outside training
 * they hand straight to the win or the loss sequence; inside it they take the
 * port's bypass, which resets the pattern when the fighter is not already at the
 * start of one and parks the routine at 9.
 *
 * Port training modes intentionally bypass CPS3's win and loss sequences. */
static void run_nm_round_end_state(PLW* wk, s16 next_z, s32 (*needs_reset)(const PLW* wk), void (*round_end)(PLW* wk)) {
    wk->wu.next_z = next_z;

    if ((Mode_Type == MODE_NORMAL_TRAINING) || (Mode_Type == MODE_PARRY_TRAINING)) {
        switch (wk->wu.routine_no[3]) {
        case 0:
            if (needs_reset(wk)) {
                set_char_move_init(&wk->wu, 0, 0);
            }

            wk->wu.routine_no[3] = 9;
            break;
        }

        char_move(&wk->wu);
        return;
    }

    round_end(wk);
}

void Normal_40000(PLW* wk) { // 🟡
    run_nm_round_end_state(wk, 38, not_in_first_pattern, win_player);
}

void Normal_41000(PLW* wk) { // 🟡
    run_nm_round_end_state(wk, 34, alive_and_not_in_first_pattern, lose_player);
}

/* Both parry states put the player in front unless the opponent's pattern
 * already claims that depth, and both mark the hosei flag for the same set of
 * work kinds. */
static void set_parry_depth_and_hosei(PLW* wk) {
    if (((WORK*)wk->wu.target_adrs)->cg_prio != 2) {
        wk->wu.next_z = 32;
    }

    if (wk->wu.dm_work_id & 11) {
        wk->dm_hos_flag = 1;
    }
}

/* The parry's first frame: the facing, the pattern and movement data its row
 * names, the flash, the gauge it earns, and a hit stop that always counts
 * down from negative. */
static void begin_parry_state(PLW* wk, const s16* dadr) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = (wk->wu.dm_rl + 1) & 1;

    if (dadr[2]) {
        wk->wu.xyz[1].disp.pos = 0;
    }

    set_char_move_init(&wk->wu, 0, dadr[0]);
    setup_mvxy_data(&wk->wu, dadr[1]);
    Flash_MT[wk->wu.id] = 2;
    add_sp_arts_gauge_paring(wk);
    set_hit_stop_hit_quake(&wk->wu);

    if (wk->wu.hit_stop > 0) {
        wk->wu.hit_stop = -wk->wu.hit_stop;
    }
}

/* Marker 1 launches the parry: step the state, apply the movement, and fire the
 * landing effect when the row asks for it. */
static void parry_launch_on_marker_1(PLW* wk, const s16* dadr) {
    if (wk->wu.cg_type == 1) {
        wk->wu.routine_no[3]++;
        add_mvxy_speed(&wk->wu);

        if (dadr[2]) {
            effect_G6_init(&wk->wu, wk->wu.weight_level);
        }
    }
}

/* The throw escape's version. It clears cg_type as well, which is why it is not
 * the same block; its inner `break` left the switch with the arm's own break
 * immediately after, so both paths reached the same place. */
static void escape_launch_on_marker_1(PLW* wk, const s16* datix) {
    if (wk->wu.cg_type == 1) {
        wk->wu.cg_type = 0;
        wk->wu.routine_no[3]++;
        add_mvxy_speed(&wk->wu);

        if (datix[2]) {
            effect_G6_init(&wk->wu, wk->wu.weight_level);
        }
    }
}

/* The throw escape's first frame: the facing, the pattern and movement data
 * its row names, a fixed hit stop, and the gauge and grade it earns. */
static void begin_throw_escape_state(PLW* wk, const s16* datix) {
    wk->wu.routine_no[3]++;
    wk->wu.rl_flag = wk->wu.rl_waza;

    if (datix[2]) {
        wk->wu.xyz[1].disp.pos = 0;
    }

    set_char_move_init(&wk->wu, 0, datix[0]);
    setup_mvxy_data(&wk->wu, datix[1]);
    wk->wu.hit_stop = -18;
    wk->wu.hit_quake = 0;
    wk->wu.dm_stop = wk->wu.dm_quake = 0;
    add_sp_arts_gauge_nagenuke(wk);
    grade_add_grap_def(wk->wu.id);
}

/* Normal_42000 and Normal_47000 are the same state machine: the same parry
 * depth and hosei, the same fallthrough into case 2, the same jump and move
 * arms. Only the data table and the two actions the arms call differ, so they
 * come in as function pointers. */
static void run_parry_like_state(
    PLW* wk, const s16* data, void (*begin)(PLW*, const s16*), void (*launch_on_marker_1)(PLW*, const s16*)
) {
    set_parry_depth_and_hosei(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        begin(wk, data);
        break;

    case 1:
        if (1) {
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
        } else {
            /* fallthrough */

        case 2:
            char_move(&wk->wu);
        }

        launch_on_marker_1(wk, data);
        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Normal_42000(PLW* wk) { // 🟢
    run_parry_like_state(wk, nmPB_data[wk->wu.routine_no[2] - 42], begin_parry_state, parry_launch_on_marker_1);
}

void Normal_47000(PLW* wk) { // 🟢
    run_parry_like_state(wk, nmCE_data[wk->wu.routine_no[2] - 47], begin_throw_escape_state, escape_launch_on_marker_1);
}

void Normal_48000(PLW* wk) { // 🟢
    wk->guard_flag = 3;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        wk->wu.xyz[1].disp.pos = 0;
        set_char_move_init(&wk->wu, 0, 44);
        setup_mvxy_data(&wk->wu, 27);
        wk->wu.hit_stop = -17;
        wk->wu.hit_quake = 8;
        wk->wu.dm_stop = wk->wu.dm_quake = 0;
        break;

    case 1:
        if (1) {
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
        } else {
            /* fallthrough */

        case 2:
            char_move(&wk->wu);
        }

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
            add_mvxy_speed(&wk->wu);
            effect_G6_init(&wk->wu, wk->wu.weight_level);
        }

        break;

    case 3:
        char_move(&wk->wu);
        cal_mvxy_speed(&wk->wu);
        add_mvxy_speed(&wk->wu);
        break;
    }
}

void Normal_50000(PLW* wk) { // 🟢
    wk->guard_flag = 3;

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->wu.rl_flag = wk->wu.rl_waza;
        set_char_move_init(&wk->wu, 0, 46);
        setup_mvxy_data(&wk->wu, 29);
        wk->wu.hit_stop = -17;
        wk->wu.hit_quake = 8;
        wk->wu.dm_stop = wk->wu.dm_quake = 0;
        return;

    case 1:
        if (1) {
            wk->wu.routine_no[3]++;
            char_move_wca(&wk->wu);
        } else {
            /* fallthrough */

        case 2:
            char_move(&wk->wu);
        }

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
            add_mvxy_speed(&wk->wu);
        }

        break;

    case 3:
        jumping_union_process(&wk->wu, 4);
        break;

    case 4:
        char_move(&wk->wu);
        break;
    }
}

void Normal_51000(PLW* wk) { // 🟢
    if (wk->wu.routine_no[3] == 0) {
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, 12);
    }
}

void Normal_52000(PLW* wk) { // 🟢
    wk->guard_flag = 3;

    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->extra_jump = 1;
        remake_sankaku_tobi_mvxy(&wk->wu, wk->micchaku_flag);
        set_char_move_init(&wk->wu, 0, 48);
        effect_I3_init(&wk->wu, 0);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.routine_no[2] = 21;
            wk->wu.routine_no[3] = 1;
            set_char_move_init(&wk->wu, 0, 14);
            char_move_z(&wk->wu);
            add_mvxy_speed(&wk->wu);
        }

        break;
    }
}

void Normal_53000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        wk->extra_jump = 1;
        set_char_move_init(&wk->wu, 0, 49);
        break;

    case 1:
        char_move(&wk->wu);
        set_new_jpdir(wk);
        if (wk->wu.cg_type == 0xFF) {
            check_jump_rl_dir(wk);

            switch (wk->jpdir) {
            case 1:
                wk->wu.routine_no[2] = 21;
                break;
            case 2:
                wk->wu.routine_no[2] = 23;
                break;
            default:
                wk->wu.routine_no[2] = 22;
                break;
            }

            wk->wu.routine_no[3] = 1;
            set_char_move_init(&wk->wu, 0, jpdat_tbl[wk->wu.routine_no[2] - 18][0]);
            char_move_z(&wk->wu);
            setup_mvxy_data(&wk->wu, jpdat_tbl[wk->wu.routine_no[2] - 18][1]);
            wk->wu.mvxy.a[0].real.h = ((wk->wu.mvxy.a[0].real.h * 3) << 1) / 10;
            wk->wu.mvxy.a[1].real.h = (wk->wu.mvxy.a[1].real.h << 3) / 10;
            add_mvxy_speed(&wk->wu);
        }
        break;
    }
}

void Normal_54000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        set_char_move_init(&wk->wu, 0, 12);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 0xFF) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[2] = 18;
            wk->wu.routine_no[3] = 0;

            if (wk->wu.rl_flag != check_work_position(&wk->wu, (WORK*)wk->wu.target_adrs)) {
                wk->wu.routine_no[2] = 20;
            }
        }

        break;
    }
}

/* What each extra-jump state does before its switch, and what it does on the
 * first frame. 58000 clears nothing before, which is what the empty function
 * says; a flag would have put a branch in the helper that none of the three
 * has. */
static void clear_bs2_on_car(PLW* wk) {
    wk->bs2_on_car = 0;
}

static void keep_bs2_on_car(PLW* wk) {}

static void begin_nm_55000(PLW* wk) {
    wk->extra_jump = 1;
    set_char_move_init(&wk->wu, 0, 18);
    setup_mvxy_data(&wk->wu, 7);
    make_nm55_init_sp(wk);
    add_mvxy_speed(&wk->wu);
}

static void begin_nm_56000(PLW* wk) {
    nm56_char_select(wk);
    add_mvxy_speed(&wk->wu);
    wk->bs2_on_car = 0;
}

static void begin_nm_58000(PLW* wk) {
    set_char_move_init(&wk->wu, 0, 18);
    setup_mvxy_data(&wk->wu, 7);
}

/* The extra-jump state Normal_55000, Normal_56000 and Normal_58000 share: raise
 * the z while mirrored, step the routine through its own opening, then the jump
 * union and the move. */
static void run_nm_extra_jump_state(PLW* wk, void (*prepare)(PLW* wk), void (*begin)(PLW* wk)) {
    raise_z_when_mirrored(wk);

    prepare(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        begin(wk);
        break;

    case 1:
        jumping_union_process(&wk->wu, 2);
        break;

    case 2:
        char_move(&wk->wu);
        break;
    }
}

void Normal_55000(PLW* wk) { // 🟢
    run_nm_extra_jump_state(wk, clear_bs2_on_car, begin_nm_55000);
}

void make_nm55_init_sp(PLW* wk) { // 🟢
    WORK* efw;
    s16* dad;
    s16 ix;
    s16 isp;

    wk->wu.mvxy.a[1].sp /= 3;
    isp = (wk->move_power << 2) / 5;

    if (isp < 3) {
        isp = 3;
    }

    wk->wu.mvxy.a[0].real.h = isp;
    efw = (WORK*)((WORK*)wk->wu.target_adrs)->my_effadrs;
    ix = get_sel_hosei_tbl_ix(wk->player_number) + 1;
    dad = efw->hosei_adrs[ix].hos_box;

    if (!check_work_position_bonus(&wk->wu, dad[0] + (dad[1] / 2) + efw->xyz[0].disp.pos)) {
        if (wk->wu.rl_flag) {
            wk->wu.mvxy.a[0].real.h = -wk->wu.mvxy.a[0].real.h;
        }
        return;
    }

    if (wk->wu.rl_flag == 0) {
        wk->wu.mvxy.a[0].real.h = -wk->wu.mvxy.a[0].real.h;
    }
}

void Normal_56000(PLW* wk) { // 🟢
    run_nm_extra_jump_state(wk, clear_bs2_on_car, begin_nm_56000);
}

void nm56_char_select(PLW* wk) { // 🟢
    WORK* efw;
    s16* dad;
    s16 ix;

    efw = (WORK*)((WORK*)wk->wu.target_adrs)->my_effadrs;
    ix = get_sel_hosei_tbl_ix(wk->player_number) + 1;
    dad = efw->hosei_adrs[ix].hos_box;
    setup_mvxy_data(&wk->wu, 17);
    ix = 16;

    if (check_work_position_bonus(&wk->wu, dad[0] + (dad[1] / 2) + efw->xyz[0].disp.pos)) {
        if (wk->wu.rl_flag) {
            ix = 14;
        }
    } else if (wk->wu.rl_flag == 0) {
        ix = 14;
    }

    if (ix == 14) {
        wk->wu.mvxy.a[0].sp = -wk->wu.mvxy.a[0].sp;
    }

    set_char_move_init(&wk->wu, 0, ix);
}

void Normal_57000(PLW* wk) { // 🟢
    raise_z_when_mirrored(wk);

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        nm57_dir_select(wk);
        wk->wu.xyz[1].disp.pos = 0;
        set_char_move_init(&wk->wu, 0, 50);
        setup_mvxy_data(&wk->wu, 18);
        break;

    case 1:
        char_move(&wk->wu);

        if (wk->wu.cg_type == 1) {
            wk->wu.cg_type = 0;
            wk->wu.routine_no[3]++;
            add_mvxy_speed(&wk->wu);
            effect_G6_init(&wk->wu, wk->wu.weight_level);
            break;
        }

        break;

    case 2:
        jumping_union_process(&wk->wu, 3);
        break;

    case 3:
        char_move(&wk->wu);
        break;
    }
}

void nm57_dir_select(PLW* wk) { // 🟢
    WORK* efw;
    s16* dad;
    s16 ix;

    efw = (WORK*)((WORK*)wk->wu.target_adrs)->my_effadrs;
    ix = get_sel_hosei_tbl_ix(wk->player_number) + 1;
    dad = (s16*)efw->hosei_adrs[ix].hos_box;
    wk->wu.rl_flag = 1;

    if (check_work_position_bonus(&wk->wu, dad[0] + (dad[1] / 2) + efw->xyz[0].disp.pos)) {
        wk->wu.rl_flag = 0;
    }
}

void Normal_58000(PLW* wk) { // 🟢
    run_nm_extra_jump_state(wk, keep_bs2_on_car, begin_nm_58000);
}

const s16 nmPB_data[5][3] = { { 38, 23, 1 }, { 39, 23, 1 }, { 40, 24, 1 }, { 41, 25, 0 }, { 42, 25, 0 } };
const s16 nmCE_data[4][3] = { { 43, 26, 1 }, { 44, 27, 1 }, { 45, 28, 0 }, { 46, 29, 0 } };

void (*const plpnm_lv_00[59])(PLW* wk) = {
    Normal_00000, Normal_01000, Normal_02000, Normal_03000, Normal_04000, Normal_05000, Normal_06000, Normal_07000,
    Normal_08000, Normal_09000, Normal_10000, Normal_03000, Normal_04000, Normal_03000, Normal_03000, Normal_03000,
    Normal_16000, Normal_17000, Normal_18000, Normal_18000, Normal_18000, Normal_18000, Normal_18000, Normal_18000,
    Normal_18000, Normal_18000, Normal_18000, Normal_27000, Normal_27000, Normal_27000, Normal_27000, Normal_31000,
    Normal_31000, Normal_31000, Normal_35000, Normal_35000, Normal_36000, Normal_37000, Normal_38000, Normal_39000,
    Normal_40000, Normal_41000, Normal_42000, Normal_42000, Normal_42000, Normal_42000, Normal_42000, Normal_47000,
    Normal_48000, Normal_47000, Normal_50000, Normal_51000, Normal_52000, Normal_53000, Normal_54000, Normal_55000,
    Normal_56000, Normal_57000, Normal_58000
};

void (*const plpnm_lv_00_cps3[59])(PLW* wk) = {
    Normal_00000, Normal_01000, Normal_02000, Normal_03000, Normal_04000, Normal_05000, Normal_06000, Normal_07000,
    Normal_08000, Normal_09000, Normal_10000, Normal_11000, Normal_12000, Normal_13000, Normal_13000, Normal_13000,
    Normal_16000, Normal_17000, Normal_18000, Normal_18000, Normal_18000, Normal_18000, Normal_18000, Normal_18000,
    Normal_18000, Normal_18000, Normal_18000, Normal_27000, Normal_27000, Normal_27000, Normal_27000, Normal_31000,
    Normal_31000, Normal_31000, Normal_35000, Normal_35000, Normal_36000, Normal_37000, Normal_38000, Normal_39000,
    Normal_40000, Normal_41000, Normal_42000, Normal_42000, Normal_42000, Normal_42000, Normal_42000, Normal_47000,
    Normal_48000, Normal_47000, Normal_50000, Normal_51000, Normal_52000, Normal_53000, Normal_54000, Normal_55000,
    Normal_56000, Normal_57000, Normal_58000
};

void (*const normal_05[])(PLW* wk) = { nm_05_0000, nm_05_0000, nm_05_0100, nm_05_0000, nm_05_0000,
                                       nm_05_0000, nm_05_0000, nm_05_0000, nm_05_0100, nm_05_0000,
                                       nm_05_0000, nm_05_0100, nm_05_0100, nm_05_0000, nm_05_0100,
                                       nm_05_0000, nm_05_0100, nm_05_0000, nm_05_0000, nm_05_0000 };

void (*const normal_06[])(PLW* wk) = { nm_06_0100, nm_06_0100, nm_06_0200, nm_06_0000, nm_06_0100,
                                       nm_06_0100, nm_06_0100, nm_06_0100, nm_06_0000, nm_06_0100,
                                       nm_06_0000, nm_06_0200, nm_06_0200, nm_06_0100, nm_06_0200,
                                       nm_06_0000, nm_06_0200, nm_06_0100, nm_06_0100, nm_06_0100 };

const u16 jpdat_tbl[9][2] = { { 17, 6 },  { 18, 7 },  { 19, 8 },  { 14, 9 }, { 15, 10 },
                              { 16, 11 }, { 20, 12 }, { 21, 13 }, { 22, 14 } };

/**
 * @file cmd_main.c
 * Command Input Parser
 */

#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "arcade/arcade_balance.h"
#include "arcade/arcade_cmd_data.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main_internal.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"

#include <SDL3/SDL.h>

void (*chk_move_jp[28])() = { check_init, check_0,  check_1,  check_2,  check_3,  check_4,  check_5,
                              check_6,    check_7,  check_7,  check_9,  check_10, check_11, check_12,
                              check_13,   check_14, check_15, check_16, check_16, check_18, check_19,
                              check_20,   check_21, check_22, check_23, check_24, check_25, check_26 };

static s32 no_dead_key_is_held(void) {
    return (!waza_ptr->w_dead || waza_ptr->w_dead != chk_pl->sw_new) &&
           (!waza_ptr->w_dead2 || waza_ptr->w_dead2 != chk_pl->sw_new);
}

void waza_check(PLW* pl) { // 🟢
    cmd_pl = pl;
    cmd_id = cmd_pl->wu.id;
    chk_pl = &t_pl_lvr[cmd_id];
    sw_pick_up();
    cmd_move();
}

void key_thru(PLW* pl) { // 🟢
    cmd_pl = pl;
    cmd_id = cmd_pl->wu.id;
    chk_pl = &t_pl_lvr[cmd_id];
    sw_pick_up();
}

void cmd_data_set(PLW* /* unused */, s16 i) { // 🟡
    u8* ptr3;
    u16* ptr4;

    wcp[cmd_id].reset[i] = *cmd_tbl_ptr++;
    waza_work[cmd_id][i].w_dead = *cmd_tbl_ptr++;
    waza_work[cmd_id][i].w_dead2 = *cmd_tbl_ptr++;

    ptr3 = wcp[cmd_id].waza_r[i];
    *ptr3++ = (s8)*cmd_tbl_ptr++;
    *ptr3++ = (s8)*cmd_tbl_ptr++;
    *ptr3++ = (s8)*cmd_tbl_ptr++;
    *ptr3++ = (s8)*cmd_tbl_ptr++;

    wcp[cmd_id].btix[i] = *cmd_tbl_ptr++;

    ptr4 = wcp[cmd_id].exdt[i];
    *ptr4++ = *cmd_tbl_ptr++;
    *ptr4++ = *cmd_tbl_ptr++;
    *ptr4++ = *cmd_tbl_ptr++;
    *ptr4++ = *cmd_tbl_ptr++;

    // The code below is PS2-specific, but default system-direction options produce CPS3's ground red-parry thresholds.
    switch (i) {
    case 3:
    case 4:
    case 5:
        wcp[cmd_id].reset[i] += blok_b_omake[omop_b_block_ix[cmd_id]];
        make_red_blocking_time(cmd_id, i, wcp[cmd_id].reset[i]);
        break;

    case 6:
    case 12:
        wcp[cmd_id].reset[i] += blok_b_omake[omop_b_block_ix[cmd_id]];
        break;
    }
}

void cmd_init(PLW* pl) { // 🟡
    s16 i;
    s16 j;

    cmd_id = pl->wu.id;
    pl->cp = &wcp[cmd_id];

    if (ArcadeBalance_IsEnabled()) {
        // CPS3 clears 0x540 bytes of each 0x620-byte command-state block, leaving entries 48-55 intact.
        SDL_memset(waza_work[cmd_id], 0, sizeof(WAZA_WORK) * 48);
    } else {
        SDL_zeroa(waza_work[cmd_id]);
    }

    for (i = 0; i < 56; i++) {
        wcp[cmd_id].waza_flag[i] = 0;

        for (j = 0; j < 4; j++) {
            wcp[cmd_id].waza_r[i][j] = 0;
        }
    }

    waza_compel_all_init(pl);
}

static const void* get_commands(s16 char_num) { // 🔴
    if (ArcadeBalance_IsEnabled()) {
        return ArcadeCommandData_Get(char_num);
    } else if (cmd_sel[cmd_id]) {
        return pl_CMD[char_num];
    } else {
        return pl_cmd[char_num];
    }
}

/* One command's own matcher, if it is enabled at all. */
static void step_one_command(s16 j, intptr_t* adrs) {
    if (wcp[cmd_id].waza_flag[j] == -1) {
        return;
    }

    waza_type[cmd_id] = j;
    cmd_tbl_ptr = (s16*)adrs[j];
    waza_ptr = &waza_work[cmd_id][j];
    chk_move_jp[waza_ptr->w_type]();
}

/* And the move behind a command that matched this frame. */
static void run_one_matched_command(s16 j) {
    if ((wcp[cmd_id].waza_flag[j] == -1) || (wcp[cmd_id].waza_flag[j] == 0)) {
        return;
    }

    waza_ptr = &waza_work[cmd_id][j];
    command_ok_move(j);
}

void cmd_move() { // 🟢
    s16 j;
    intptr_t* adrs;

    cmd_id = cmd_pl->wu.id;
    adrs = get_commands(cmd_pl->player_number);

    for (j = 0; j < 56; j++) {
        step_one_command(j, adrs);
    }

    for (j = 0; j < 56; j++) {
        run_one_matched_command(j);
    }
}


void command_ok() { // 🟢
    wcp[cmd_id].waza_flag[waza_type[cmd_id]] = wcp[cmd_id].reset[waza_type[cmd_id]];

    if (waza_ptr->w_type != 14) {
        waza_ptr->w_type = 0;
        chk_pl->waza_no = waza_type[cmd_id];
    }
}

void command_ok_move(s16 waza_num) { // 🟢
    if (dead_lvr_check()) {
        wcp[cmd_id].waza_flag[waza_num] = 0;
    } else {
        wcp[cmd_id].waza_flag[waza_num] -= 1;
    }
}

s32 dead_lvr_check() { // 🟢
    if (no_dead_key_is_held()) {
        return 0;
    }

    waza_ptr->w_type = 0;
    return 1;
}

static u16 latch_sw_lvbt_bit_0x80(u16 work2, u16 hana2, u16 sw_0) {
    switch (work2) {
    case 0x70:
    case 0x30:
    case 0x50:
    case 0x60:
        wcp[cmd_id].sw_lvbt |= 0x80;
        sw_0 |= 0x80;
        break;

    default:
        switch (hana2) {
        case 0x70:
        case 0x30:
        case 0x50:
        case 0x60:
            wcp[cmd_id].sw_lvbt |= 0x80;
            sw_0 |= 0x80;
            break;

        default:
            wcp[cmd_id].sw_lvbt &= 0xFF7F;
            sw_0 &= 0xFF7F;
            break;
        }
    }

    return sw_0;
}

static u16 latch_sw_lvbt_bit_0x800(u16 work2, u16 hana2, u16 sw_0) {
    switch (work2) {
    case 0x700:
    case 0x300:
    case 0x500:
    case 0x600:
        wcp[cmd_id].sw_lvbt |= 0x800;
        sw_0 |= 0x800;
        break;

    default:
        switch (hana2) {
        case 0x700:
        case 0x300:
        case 0x500:
        case 0x600:
            wcp[cmd_id].sw_lvbt |= 0x800;
            sw_0 |= 0x800;
            break;

        default:
            wcp[cmd_id].sw_lvbt &= 0xF7FF;
            sw_0 &= 0xF7FF;
            break;
        }
    }

    return sw_0;
}

/* A punch-and-kick pair is held when both of its bits are set. The three
 * pairs asked this identically; each keeps its own mask at the call site. */
static s16 both_buttons_held(u16 sw_work, u16 mask) {
    if ((sw_work & mask) == mask) {
        return 1;
    }

    return 0;
}

/* Swap the two horizontal lever bits, so the lever reads the way the player
 * faces. Both cases that need it - riding the bonus car, and a move that
 * reverses the lever - did this identically. */
static u16 mirror_lever_left_right(u16 sw_0) {
    u16 sw_work = sw_0 & 0xC;

    if (sw_work) {
        sw_0 &= 0xFF3;
        sw_work ^= 0xC;
        sw_0 |= sw_work;
    }

    return sw_0;
}

void pl_lvr_set() { // 🟢
    u16 sw_work;
    u16 work2;
    u16 sw_0;
    u16 sw_hana;
    u16 hana2;

    sw_0 = wcp[cmd_id].sw_lvbt;

    if (check_rl_on_car(cmd_pl)) {
        if (cmd_pl->wu.rl_flag) {
            sw_0 = mirror_lever_left_right(sw_0);
        }
    } else if (cmd_pl->wu.rl_waza) {
        sw_0 = mirror_lever_left_right(sw_0);
    }

    wcp[cmd_id].old_now = chk_pl->sw_now;
    chk_pl->old_now = chk_pl->sw_now;
    chk_pl->old_lvbt = chk_pl->new_lvbt;

    sw_work = ~(chk_pl->old_lvbt) & (wcp[cmd_id].sw_lvbt);
    sw_hana = chk_pl->sw_new & ~(sw_0);

    work2 = sw_work & 0xF0;
    hana2 = sw_hana & 0xF0;

    sw_0 = latch_sw_lvbt_bit_0x80(work2, hana2, sw_0);

    work2 = sw_work & 0xF00;
    hana2 = sw_hana & 0xF00;

    sw_0 = latch_sw_lvbt_bit_0x800(work2, hana2, sw_0);

    chk_pl->new_lvbt = wcp[cmd_id].sw_lvbt;
    chk_pl->sw_old = chk_pl->sw_new;
    chk_pl->sw_new = sw_0;
    chk_pl->sw_now = sw_0 & ~(chk_pl->sw_old);
    chk_pl->now_lvbt = ~(chk_pl->old_lvbt) & (wcp[cmd_id].sw_lvbt);
    chk_pl->sw_chg = (chk_pl->sw_now) | (chk_pl->sw_old & ~(sw_0));
    chk_pl->sw_lever = sw_0 & 0xF;
    chk_pl->shot_up = chk_pl->sw_now & 0x770;
    chk_pl->shot_down = chk_pl->sw_old & ~(sw_0) & 0x770;
    chk_pl->shot_ud = ((chk_pl->shot_up) | (chk_pl->shot_down));
    sw_work = ((chk_pl->sw_now) | (wcp[cmd_id].old_now));

    wcp[cmd_id].ca14 = both_buttons_held(sw_work, 0x110);
    wcp[cmd_id].ca25 = both_buttons_held(sw_work, 0x220);
    wcp[cmd_id].ca36 = both_buttons_held(sw_work, 0x440);

    wcp[cmd_id].lgp = lever_gacha_tbl[cmd_pl->cp->sw_now & 0xF] * 4;
    wcp[cmd_id].lgp += lever_gacha_tbl[cmd_pl->cp->sw_off & 0xF] * 2;
    wcp[cmd_id].lgp += lever_gacha_tbl[(cmd_pl->cp->sw_now / 16) & 7] * 2;
    wcp[cmd_id].lgp += lever_gacha_tbl[(cmd_pl->cp->sw_now / 256) & 7] * 1;
}

/* Advances one hold counter and returns the next. The two loops differed
 * only in which bit they tested. */
static s16* bump_or_reset_count(s16* counter, s32 pressed) {
    if (pressed) {
        *counter += 1;
    } else {
        *counter = 0;
    }

    return counter + 1;
}

/* Forward or back, as the player faces. The lever bits are read the same way
 * either way round; which of the two means "forward" is what the facing
 * decides. */
static void set_lever_dir() {
    s16 i;

    if ((i = wcp[cmd_id].sw_lvbt & 0xC)) {
        if (cmd_pl->wu.rl_flag) {
            if (i & 8) {
                wcp[cmd_id].lever_dir = 1;
            } else {
                wcp[cmd_id].lever_dir = 2;
            }
        } else if (i & 4) {
            wcp[cmd_id].lever_dir = 1;
        } else {
            wcp[cmd_id].lever_dir = 2;
        }
    } else {
        wcp[cmd_id].lever_dir = 0;
    }
}

void sw_pick_up() { // 🟢
    s16 i;
    s16* cnt_address1;

    pl_lvr_set();
    sw_work = 1;
    cnt_address1 = &chk_pl->up_cnt;

    for (i = 0; i < 10; i++) {
        cnt_address1 = bump_or_reset_count(cnt_address1, chk_pl->sw_new & sw_work);
        sw_work *= 2;
    }

    for (i = 0; i < 4; i++) {
        cnt_address1 = bump_or_reset_count(cnt_address1, chk_pl->sw_new & lvr_chk_tbl[0][i]);
    }

    wcp[cmd_id].sw_new = chk_pl->sw_new;
    wcp[cmd_id].sw_old = chk_pl->sw_old;
    wcp[cmd_id].sw_chg = chk_pl->sw_chg;
    wcp[cmd_id].sw_now = chk_pl->sw_now;
    wcp[cmd_id].sw_off = chk_pl->shot_down;

    set_lever_dir();

    if ((chk_pl->left_cnt != 0) && (chk_pl->left_cnt < 12)) {
        wcp[cmd_id].calf = 1;
    } else {
        wcp[cmd_id].calf = 0;
    }

    if ((chk_pl->right_cnt != 0) && (chk_pl->right_cnt < 12)) {
        wcp[cmd_id].calr = 1;
    } else {
        wcp[cmd_id].calr = 0;
    }
}

void dash_flag_clear(s16 pl_id) { // 🟢
    intptr_t* adrs = get_commands(plw[pl_id].player_number);

    waza_compel_init(pl_id, 0, adrs);
    waza_compel_init(pl_id, 1, adrs);
}

void hi_jump_flag_clear(s16 pl_id) { // 🟢
    intptr_t* adrs = get_commands(plw[pl_id].player_number);

    waza_compel_init(pl_id, 2, adrs);
}

void waza_flag_clear_only_1(s16 pl_id, s16 wznum) { // 🟢
    intptr_t* adrs = get_commands(plw[pl_id].player_number);

    waza_compel_init(pl_id, wznum, adrs);
}

void waza_compel_init(s16 pl_id, s16 num, intptr_t* adrs) { // 🟢
    WAZA_WORK* w_ptr;
    s16* ptr;

    ptr = (s16*)adrs[num];
    ptr += 12;
    w_ptr = &waza_work[pl_id][num];
    w_ptr->w_type = *ptr++;
    w_ptr->w_int = *ptr++;
    w_ptr->free1 = *ptr;
    w_ptr->free2 = *ptr++;
    w_ptr->w_lvr = *ptr++;
    w_ptr->w_ptr = ptr;
    w_ptr->uni0.tame.flag = 0;
    w_ptr->uni0.tame.shot_flag = 0;
    w_ptr->uni0.tame.shot_flag2 = 0;
    w_ptr->shot_ok = 0;
    w_ptr->free3 = 0;
    wcp[pl_id].waza_flag[num] = 0;
}

static void populate_cmd_range(PLW* pl, intptr_t* adrs, s16 start, s16 end) {
    s16 i;

    for (i = start; i < end; i++) {
        cmd_tbl_ptr = (s16*)adrs[i];
        cmd_data_set(pl, i);
    }
}

static void clear_waza_flag_range(s16 start, s16 end) {
    s16 i;

    for (i = start; i < end; i++) {
        wcp[cmd_id].waza_flag[i] = -1;
    }
}

void waza_compel_all_init(PLW* pl) { // 🟢
    intptr_t* adrs = get_commands(pl->player_number);

    populate_cmd_range(pl, adrs, 0, pl_cmd_num[pl->player_number][0]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][0], 20);

    populate_cmd_range(pl, adrs, 20, pl_cmd_num[pl->player_number][1]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][1], 24);

    populate_cmd_range(pl, adrs, 24, pl_cmd_num[pl->player_number][2]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][2], 28);

    populate_cmd_range(pl, adrs, 28, pl_cmd_num[pl->player_number][3]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][3], 38);

    populate_cmd_range(pl, adrs, 38, pl_cmd_num[pl->player_number][4]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][4], 42);

    populate_cmd_range(pl, adrs, 42, pl_cmd_num[pl->player_number][5]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][5], 46);

    populate_cmd_range(pl, adrs, 46, pl_cmd_num[pl->player_number][6]);
    clear_waza_flag_range(pl_cmd_num[pl->player_number][6], 56);
}

void waza_compel_all_init2(PLW* pl) { // 🟢
    s16 j;

    for (j = 0; j < 56; j++) {
        if (wcp[pl->wu.id].waza_flag[j] != -1) {
            waza_work[pl->wu.id][j].w_type = 0;
        }
    }
}

u16 processed_lvbt(u16 lv_data) { // 🟡
    return lv_data & 0xFFF;
}

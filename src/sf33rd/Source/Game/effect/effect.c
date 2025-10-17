/**
 * @file effect.c
 * Main Effect Functions
 */

#include "sf33rd/Source/Game/effect/effect.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effxx.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/stage/bg.h"

#include <SDL3/SDL.h>

s16 frwctr;
s16 frwctr_min;
s16 head_ix[8];
s16 tail_ix[8];
s16 exec_tm[8];
uintptr_t frw[EFFECT_MAX][448];
s16 frwque[EFFECT_MAX];

void move_effect_work(s16 index) {
    WORK* c_addr;
    s16 curr_ix;
    s16 next_ix;

    if (Debug_w[0x28]) {
        return;
    }

    exec_tm[index] += 1;

    for (curr_ix = head_ix[index]; curr_ix != -1; curr_ix = next_ix) {
        c_addr = (WORK*)frw[curr_ix];
        next_ix = c_addr->behind;

        if (c_addr->timing != exec_tm[index]) {
            c_addr->timing = exec_tm[index];
            effmovejptbl[c_addr->id](c_addr);
        }
    }
}

void disp_effect_work() {
    WORK* c_addr;
    s16 index;
    s16 curr_ix;
    s16 next_ix;
    s32 px;
    s32 py;

    if (Debug_w[0x29] == 0) {
        return;
    }

    px = 7;
    py = 15;

    for (index = 0; index <= 7; index += 1) {
        curr_ix = head_ix[index];
        px += 5;
        py = 14;

        for (; curr_ix != -1; curr_ix = next_ix) {
            if (py > 49) {
                py = 14;
                px += 3;
            }

            c_addr = (WORK*)frw[curr_ix];
            next_ix = c_addr->behind;
            flPrintL(px, py, "%c%d", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[(c_addr->id / 10)], c_addr -> id % 10);
            py++;
        }
    }
}

void effect_work_init() {
    WORK* c_addr;
    s16 i;

    SDL_zeroa(frw);

    for (i = 0; i < EFFECT_MAX; i++) {
        frwctr = (EFFECT_MAX - 1) - i;
        c_addr = (WORK*)frw[frwctr];
        frwque[i] = c_addr->myself = frwctr;
        c_addr->before = c_addr->behind = -1;
    }

    frwctr = EFFECT_MAX;
    frwctr_min = frwctr;

    for (i = 0; i < 8; i++) {
        head_ix[i] = tail_ix[i] = -1;
        exec_tm[i] = 0;
    }
}

void effect_work_quick_init() {
    s16 i;

    for (i = 0; i < 8; i += 1) {
        effect_work_list_init(i, -1);
    }
}

void effect_work_list_init(s16 lix, s16 iid) {
    WORK* c_addr;
    s16 curr_ix;
    s16 next_ix;

    curr_ix = head_ix[lix];

    if (iid == -1) {
        while (curr_ix != -1) {
            c_addr = (WORK*)frw[curr_ix];
            next_ix = c_addr->behind;
            push_effect_work(c_addr);
            curr_ix = next_ix;
        }

        exec_tm[lix] = 0;
    } else {
        while (curr_ix != -1) {
            c_addr = (WORK*)frw[curr_ix];
            next_ix = c_addr->behind;

            if (c_addr->id == iid) {
                push_effect_work(c_addr);
            }

            curr_ix = next_ix;
        }
    }
}

s16 pull_effect_work(s16 index) {
    s16 qix;
    WORK* tadr;
    WORK* wrk;

    if (frwctr < 1) {
        return -1;
    }

    qix = frwque[(frwctr -= 1)];
    tadr = (WORK*)frw[qix];

    if (head_ix[index] == -1) {
        tail_ix[index] = qix;
        head_ix[index] = qix;
    } else {
        wrk = (WORK*)frw[tail_ix[index]];
        wrk->behind = qix;
        tadr->before = tail_ix[index];
        tail_ix[index] = qix;
    }

    tadr->timing = exec_tm[index];
    tadr->listix = index;

    if (frwctr_min > frwctr) {
        frwctr_min = frwctr;
    }

    return qix;
}

/// @brief Searches for an effect.
/// @param index Index of the list to perform search in.
/// @param flag Set to `true` to search from the tail, `false` to search from the head.
/// @param tid ID to search for.
/// @return Index of the effect, or `-1` if it couldn't be found.
s16 search_effect_index(s16 index, s16 flag, s16 tid) {
    WORK* c_addr;
    s16 aix;

    if (flag) {
        aix = tail_ix[index];

        while (aix != -1) {
            c_addr = (WORK*)frw[aix];

            if (c_addr->id != tid) {
                aix = c_addr->before;
            } else {
                break;
            }
        }
    } else {
        aix = head_ix[index];

        while (aix != -1) {
            c_addr = (WORK*)frw[aix];

            if (c_addr->id != tid) {
                aix = c_addr->behind;
            } else {
                break;
            }
        }
    }

    return aix;
}

void push_effect_work(WORK* wkhd) {
    WORK* c_addr;
    WORK* c_addr2;
    s16 qix;
    s16 lix;

    lix = wkhd->listix;
    qix = wkhd->myself;
    c_addr = (WORK*)frw[qix];

#if !defined(TARGET_PS2)
    if (qix < 0 || qix >= 128) {
        fatal_error("qix is out of range");
    }
#endif

    switch ((qix == head_ix[lix]) + (qix == tail_ix[lix]) * 2) {
    case 0:
        c_addr2 = (WORK*)frw[c_addr->before];
        c_addr2->behind = c_addr->behind;
        c_addr2 = (WORK*)frw[c_addr->behind];
        c_addr2->before = c_addr->before;
        break;

    case 1:
        head_ix[lix] = c_addr->behind;
        c_addr2 = (WORK*)frw[c_addr->behind];
        c_addr2->before = -1;
        break;

    case 2:
        c_addr2 = (WORK*)frw[c_addr->before];
        c_addr2->behind = -1;
        tail_ix[lix] = c_addr->before;
        break;

    default:
        head_ix[lix] = tail_ix[lix] = -1;
        break;
    }

    SDL_zeroa(frw[qix]);

    c_addr->before = c_addr->behind = -1;
    frwque[frwctr++] = qix;
    c_addr->myself = qix;
}

void effect_work_kill(s16 index, s16 kill_id) {
    WORK* c_addr;
    s16 aix = head_ix[index];

    if (kill_id == -1) {
        while (aix != -1) {
            c_addr = (WORK*)frw[aix];
            c_addr->dead_f = 1;
            aix = c_addr->behind;
        }

        return;
    }

    while (aix != -1) {
        c_addr = (WORK*)frw[aix];

        if (c_addr->id == kill_id) {
            c_addr->dead_f = 1;
        }

        aix = c_addr->behind;
    }
}

void write_my_shell_ix(WORK* wk, s16 ix) {
    s32 i;

    for (i = 7; i > 0; i -= 1) {
        wk->shell_ix[i] = wk->shell_ix[i - 1];
    }

    wk->shell_ix[0] = ix;
}

s32 erase_my_shell_ix(WORK* wk, s16 ix) {
    s32 i;
    s32 j;

    for (i = 0; i < 8; i++) {
        if (wk->shell_ix[i] == ix) {
            goto ok;
        }
    }

    return 0;

ok:
    for (j = i; j < 7; j++) {
        wk->shell_ix[j] = wk->shell_ix[j + 1];
    }

    wk->shell_ix[7] = -1;
    return 1;
}

s32 get_my_shell_ix(WORK* wk, s16 ix, WORK** tmw) {
    if (wk->shell_ix[ix] == -1) {
        return 0;
    }

    *tmw = (WORK*)frw[wk->shell_ix[ix]];

    if ((*tmw)->be_flag) {
        return 1;
    }

    return 0;
}

s32 get_vs_shell_adrs(WORK* wk, s16 id, s16 ix, WORK_Other** tmw) {
    if (wk->shell_ix[ix] == -1) {
        return 0;
    }

    *tmw = (WORK_Other*)frw[wk->shell_ix[ix]];

    if ((*tmw)->master_id == id) {
        return 1;
    }

    return 0;
}

void clear_my_shell_ix(WORK* wk) {
    s32 i;

    for (i = 0; i < 8; i += 1) {
        wk->shell_ix[i] = -1;
    }
}

void setup_shell_hit_stop(WORK* wk, s16 tm, s16 fl) {
    WORK* tmw;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (get_my_shell_ix(wk, i, &tmw) != 0) {
            if (fl == 0 || tmw->id != 0x29) {
                tmw->hit_stop = tm;
            }
        }
    }
}

s32 shell_live_check(PLW* wk, s16 wix) {
    WORK_Other* tmw;
    s16 i;

    if (wk->player_number != 0xE) {
        for (i = 0; i < 8; i++) {
            if (wk->wu.shell_ix[i] == -1) {
                break;
            }

            tmw = (WORK_Other*)frw[wk->wu.shell_ix[i]];

            if ((!tmw->refrected) && (tmw->wu.original_vitality == wix)) {
                return 1;
            }
        }

        return 0;
    }

    for (i = 0; i < 8; i++) {
        if (wk->wu.shell_ix[i] == -1) {
            break;
        }

        tmw = (WORK_Other*)frw[wk->wu.shell_ix[i]];

        if (tmw->refrected) {
            continue;
        }

        if ((tmw->wu.original_vitality == 31) || (tmw->wu.original_vitality == 33) ||
            (tmw->wu.original_vitality == 46)) {
            return 1;
        }
    }

    return 0;
}

s32 clear_caution_flag(PLW* wk, u8 /* unused */) {
    wk->caution_flag = 0;
    return 0;
}

s32 set_caution_flag(PLW* wk, u8 /* unused */) {
    wk->caution_flag = 1;
    return 0;
}

s32 setup_status_flag(WORK* wk, u8 data) {
    wk->pat_status = data;
    return 0;
}

s32 reset_extra_bg_flag(WORK* wk, u8 /* unused */) {
    another_bg[wk->id] = 0;
    return 0;
}

s32 flip_my_rl_flag(WORK* wk, u8 /* unused */) {
    wk->rl_flag = wk->rl_flag + 1U & 1;
    return 0;
}

s32 setup_meoshi_hit_flag(WORK* wk, u8 data) {
    wk->meoshi_hit_flag = data;
    return 0;
}

s32 exec_char_asxy(WORK* wk, u8 data) {
    s16* from_rom2;
    s32 st;
    s16 ix = data;

    ix *= 2;
    from_rom2 = &wk->step_xy_table[ix];
    st = *from_rom2++;
    st *= 256;

    if (wk->rl_flag) {
        wk->xyz[0].cal += st;
    } else {
        wk->xyz[0].cal -= st;
    }

    st = *from_rom2;
    st *= 256;
    wk->xyz[1].cal += st;
    return 0;
}

s32 setup_my_clear_level(WORK* wk, u8 data) {
    wk->my_clear_level = data;
    return 0;
}

s32 setup_my_bright_level(WORK* wk, u8 data) {
    wk->my_bright_level = data;
    return 0;
}

s32 setup_free_program(s32 /* unused */, s32 /* unused */) {
    return 0;
}

s32 setup_bg_quake_x(s32 /* unused */, u8 data) {
    bg_w.quake_x_index = data;
    return 0;
}

s32 setup_bg_quake_y(s32 /* unused */, u8 data) {
    bg_w.quake_y_index = data;
    pp_screen_quake(bg_w.quake_y_index);
    return 0;
}

s32 setup_exdm_ix(PLW* wk, u8 data) {
    wk->exdm_ix = data;
    return 0;
}

s32 setup_dmv_use_flag(PLW* wk, u8 data) {
    wk->dm_vital_use = data;
    return 0;
}

s32 setup_disp_flag(WORK* wk, u8 data) {
    wk->disp_flag = data;
    return 0;
}

s32 setup_command_number(PLW* wk, u8 data) {
    wk->cmd_request = data;
    return 0;
}

u8 old_my_char_check(u8 num, u8 flag) {
    switch (flag) {
    case 0:
        if ((Country == 1) || (Country == 8)) {
            if (num > 14) {
                num += 1;
            }
        } else if (num > 13) {
            num += 1;
        }

        break;

    case 1:
        if (num > 14) {
            num += 1;
        }

        break;
    }

    return num;
}

void effect_work_kill_mod_plcol() {
    effect_work_kill(6, -1);
}

void setup_shadow_of_the_Effy(WORK* wk) {
    wk->kage_flag = 1;
    wk->kage_hx = 0;
    wk->kage_hy = -0xA;
    wk->kage_prio = 0x47;
    wk->kage_char = 0xC;
}

void set_init_A4_flag() {
    gs.plw[0].init_E3_flag = 1;
    gs.plw[1].init_E3_flag = 1;
    gs.plw[0].init_E4_flag = 1;
    gs.plw[1].init_E4_flag = 1;
}

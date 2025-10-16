/**
 * @file n_input.c
 * High Score Name Entry Screen
 */

#include "sf33rd/Source/Game/screen/n_input.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sc_sub.h"

void Name_Scs_Input_init();
void Name_Scs_Input_comm();
void Name_Input_wait();
void Name_Scs_Input_end();
void Name_Scs_Finish();
s32 Name_Input_sub();
s32 auto_n_check(u16 chk_lvr, s16 index, u16 sw_data, u16 sw_up_w);
s32 name_slang_check();
void define_name_input();
void ranking_state_check();
void ranking_name_entry();
void name_work_init(s16 pl_id);
void rank_display_set(s16 pl_id);
void Scs_char_move();
void Scs_move_sub();
void current_sc_move2();
void start_cut_check(s16 pl_id);
void all_name_display(s16 pl_id);
void all_flash_display(s16 pl_id);

void (*Name_Jmp_scs[8])() = { Name_Scs_Input_init, Name_Scs_Input_comm, Name_Input_wait,    Name_Scs_Input_comm,
                              Name_Input_wait,     Name_Scs_Input_comm, Name_Scs_Input_end, Name_Scs_Finish };

// sbss
NAME_WK name_wk[2];
NAME_WK* name_ptr;
s16 Name_Input_f;
s16 naming_cnt[2];
s16 n_disp_flag;
s16 Name_00[2];
s16 name_limit_timer[2];
u8 ne_flash_flag;
const u8* ne_pointer;
u8 ne_col;
u8 ne_timer;
SC_NAME_WK sc_name_wk[2][4];
SC_NAME_WK* nsc_ptr;
RANK_NAME_W rank_name_w[2];

const s16 slang_tbl[17][3] = { { 0, 7, 14 },  { 18, 4, 23 },  { 7, 8, 21 },   { 5, 20, 2 },  { 5, 20, 10 },
                               { 0, 18, 18 }, { 44, 44, 44 }, { 39, 39, 39 }, { 0, 20, 12 }, { 3, 8, 4 },
                               { 4, 19, 0 },  { 8, 17, 0 },   { 10, 10, 10 }, { 14, 18, 8 }, { 15, 4, 4 },
                               { 15, 8, 18 }, { 15, 11, 14 } };

const s8 name_code_tbl[47] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                               26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 72, 37, 38, 39, 40,
                               41, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  42, 46, 43, 44 };

const u8 ne_color_tbl[18] = { 21, 2, 22, 2, 21, 2, 20, 2, 21, 2, 22, 2, 21, 2, 20, 2, 20, 255 };

const s16 rank_stage_tbl[4] = { 0, 5, 10, 15 };

s16 Name_Input(s16 pl_id) {
    end_no_cut = 1;
    Name_Input_f = 0;
    name_ptr = &name_wk[pl_id];
    name_ptr->id = pl_id;
    start_cut_check(pl_id);

    switch (Name_00[pl_id]) {
    case 0:
        Name_00[pl_id]++;
        name_ptr->type = 0;
        name_work_init(pl_id);
        name_limit_timer[name_ptr->id] = 1800;
        ranking_state_check();
        break;

    default:
        Name_Jmp_scs[name_ptr->r_no_0]();
        break;
    }

    rank_display_set(name_ptr->id);

    if (ne_flash_flag == 0) {
        all_name_display(name_ptr->id);
    } else {
        all_flash_display(name_ptr->id);
    }

    return Name_Input_f;
}

void Name_Scs_Input_init() {
    s8 i;

    name_ptr->r_no_0++;
    name_ptr->timer = 1200;
    ne_flash_flag = 0;
    ne_pointer = ne_color_tbl;
    ne_col = *ne_pointer++;
    ne_timer = *ne_pointer++;
    name_ptr->index = 0;

    for (i = 0; i < 4; i++) {
        sc_name_wk[0][i].tenmetsu_flag = 0;
        sc_name_wk[1][i].tenmetsu_flag = 0;
    }

    Scs_char_move();
}

void Name_Scs_Input_comm() {
    s16 work;
    s16 i;

    name_ptr->timer--;
    name_limit_timer[name_ptr->id]--;

    if (name_limit_timer[name_ptr->id] < 0 || name_ptr->timer < 0) {
        name_ptr->r_no_0 = 6;

        for (i = name_ptr->index; i < 4; i++) {
            name_ptr->code[i] = 44;
        }
    } else {
        work = Name_Input_sub();

        switch (work) {
        case 1:
            name_ptr->r_no_0 += 1;
            name_ptr->timer += 420;
            name_ptr->end_flag[name_ptr->index] = 1;
            name_ptr->index += 1;
            name_ptr->index &= 3;
            name_ptr->wait_cnt = 10;

            if ((name_ptr->index) != 3) {
                name_ptr->code[name_ptr->index] = name_ptr->code[(name_ptr->index) - 1];
            } else {
                name_ptr->code[name_ptr->index] = 46;
            }

            sc_name_wk[name_ptr->id][name_ptr->index].n_disp_flag = 0;
            sc_name_wk[name_ptr->id][name_ptr->index].f_cnt = 0;
            sc_name_wk[name_ptr->id][name_ptr->index].tenmetsu_flag = 0;
            break;

        case 2:
            name_ptr->timer += 420;
            sc_name_wk[name_ptr->id][name_ptr->index].n_disp_flag = 0;
            sc_name_wk[name_ptr->id][name_ptr->index].f_cnt = 0;
            sc_name_wk[name_ptr->id][name_ptr->index].tenmetsu_flag = 0;
            name_ptr->index--;
            name_ptr->r_no_0 -= 2;

            if (name_ptr->index < 0) {
                name_ptr->index = 0;
                name_ptr->r_no_0 = 0;
            } else {
                name_ptr->code[name_ptr->index + 1] = 47;
            }

            name_ptr->end_flag[name_ptr->index] = 0;
            break;

        case 3:
            name_ptr->r_no_0 = 6;

            for (i = name_ptr->index; i < 4; i++) {
                name_ptr->code[i] = 44;
            }
            break;
        }
    }

    Scs_char_move();
}

void Name_Input_wait() {
    name_ptr->wait_cnt--;

    if (name_ptr->wait_cnt < 0) {
        name_ptr->r_no_0++;
    }
}

void Name_Scs_Input_end() {
    s16 i;

    name_ptr->r_no_0++;
    naming_cnt[name_ptr->id] = 120;
    n_disp_flag = 0;

    if (name_ptr->index != 0) {
        if (name_slang_check() != 0) {
            define_name_input();
        }
    } else {
        define_name_input();
    }

    for (i = 0; i < 4; i++) {
        name_ptr->end_flag[i] = 1;
    }

    ranking_name_entry();
    name_ptr->index = 3;
    ne_flash_flag = 1;
}

void Name_Scs_Finish() {
    naming_cnt[name_ptr->id]--;

    if (naming_cnt[name_ptr->id] < 0) {
        Name_Input_f = 1;
        end_no_cut = 0;
    }

    Scs_char_move();
}

s32 Name_Input_sub() {
    u16 sw_up_w;
    u16 sw_data;

    name_ptr->old_code[name_ptr->index] = name_ptr->code[name_ptr->index];

    if (name_ptr->id) {
        sw_data = p2sw_0;
        sw_up_w = ~p2sw_1 & p2sw_0;
    } else {
        sw_data = p1sw_0;
        sw_up_w = ~p1sw_1 & p1sw_0;
    }

    if (sw_up_w & 0x770) {
        switch (name_ptr->code[name_ptr->index]) {
        case 45:
            return 2;

        case 46:
            return 3;

        default:
            return 1;
        }
    } else {
        if (sw_data & 0xC) {
            if (auto_n_check(4, 0, sw_data, sw_up_w)) {
                name_ptr->code[name_ptr->index]--;

                if (name_ptr->code[name_ptr->index] < 0) {
                    name_ptr->code[name_ptr->index] = 46;
                }
            }
            if (auto_n_check(8, 1, sw_data, sw_up_w)) {
                name_ptr->code[name_ptr->index]++;

                if (name_ptr->code[name_ptr->index] > 46) {
                    name_ptr->code[name_ptr->index] = 0;
                }
            }
        }

        return 0;
    }
}

s32 auto_n_check(u16 chk_lvr, s16 index, u16 sw_data, u16 sw_up_w) {
    if (sw_up_w & chk_lvr) {
        name_ptr->count1[index] = 0;
        name_ptr->count2[index] = 0;
        return 1;
    }

    if (sw_data & chk_lvr) {
        if (name_ptr->count1[index] > 12) {
            name_ptr->count2[index]++;

            if (name_ptr->count2[index] > 4) {
                name_ptr->count2[index] = 0;
                return 1;
            }
        } else {
            name_ptr->count1[index]++;
            name_ptr->count2[index] = 0;
        }
    } else {
        name_ptr->count1[index] = 0;
        name_ptr->count2[index] = 0;
    }

    return 0;
}

s32 name_slang_check() {
    const s16* slang_ptr = &slang_tbl[0][0];
    s16 i;
    s16 j;
    s16 slang_cnt;

    for (i = 0; i < 17; i++) {
        slang_cnt = 0;

        for (j = 0; j < 3; j++) {
            if (*slang_ptr == name_ptr->code[j]) {
                slang_cnt++;
            }

            slang_ptr++;
        }

        if (slang_cnt == 3) {
            return 1;
        }
    }

    return 0;
}

void define_name_input() {
    s16 work = 1;

    if (work & 1) {
        name_ptr->code[0] = 2;
        name_ptr->code[1] = 0;
        name_ptr->code[2] = 15;
        return;
    }

    name_ptr->code[0] = 2;
    name_ptr->code[1] = 14;
    name_ptr->code[2] = 12;
}

void ranking_state_check() {
    s16 joui;
    s16 j;

    name_ptr->rank = -1;

    for (joui = 0; joui < 4; joui++) {
        if (Rank_In[name_ptr->id][joui] >= 0) {
            break;
        }
    }

    for (j = joui + 1; j < 4; j++) {
        if (Rank_In[name_ptr->id][j] >= 0 && Rank_In[name_ptr->id][joui] > Rank_In[name_ptr->id][j]) {
            joui = j;
        }
    }

    name_ptr->rank_in = name_ptr->rank = Rank_In[name_ptr->id][joui];
    name_ptr->rank_status = name_ptr->status = rank_stage_tbl[joui];
}

void ranking_name_entry() {
    RANK_NAME_W* ptr = &rank_name_w[name_ptr->id];

    ptr->code[0] = name_code_tbl[name_ptr->code[0]];
    ptr->code[1] = name_code_tbl[name_ptr->code[1]];
    ptr->code[2] = name_code_tbl[name_ptr->code[2]];
    ptr->code[3] = name_code_tbl[name_ptr->code[3]];
}

void name_work_init(s16 pl_id) {
    s16 j;

    name_wk[pl_id].r_no_0 = 0;
    name_wk[pl_id].r_no_1 = 0;
    name_wk[pl_id].end_flag[0] = 0;
    name_wk[pl_id].code[0] = 46;
    name_wk[pl_id].old_code[0] = 46;
    name_wk[pl_id].end_flag[3] = 0;
    name_wk[pl_id].old_code[3] = name_wk[pl_id].code[3] = 44;
    sc_name_wk[pl_id]->c_cnt = 0;
    sc_name_wk[pl_id]->type = 0;
    sc_name_wk[pl_id]->r_no_0 = 0;
    sc_name_wk[pl_id]->r_no_1 = 0;

    for (j = 1; j < 3; j++) {
        name_wk[pl_id].end_flag[j] = 0;
        name_wk[pl_id].code[j] = 0x2F;
        name_wk[pl_id].old_code[j] = 0x2F;
        sc_name_wk[pl_id][j].c_cnt = (s16)j;
        sc_name_wk[pl_id][j].type = j;
        sc_name_wk[pl_id][j].r_no_0 = 0;
        sc_name_wk[pl_id][j].r_no_1 = 0;
    }
}

void rank_display_set(s16 pl_id) {
    u16 pos_x;
    u8 rank;

    if (pl_id) {
        pos_x = 36;
    } else {
        pos_x = 9;
    }

    SSPutDec(pos_x, 0, 8, name_ptr->rank_in + 1, 1);
    rank = name_ptr->rank_in;

    if (rank == 4) {
        rank = 3;
    }

    scfont_sqput((pos_x + 1) & 0xFFFF, 0, 8, 0, (rank << 1) & 0xFF, 0x19, 2, 1, 2);
}

void Scs_char_move() {
    s16 i;

    for (i = 0; i < 4; i++) {
        nsc_ptr = &sc_name_wk[name_ptr->id][i];
        Scs_move_sub();
    }
}

void Scs_move_sub() {
    switch (nsc_ptr->r_no_0) {
    case 0:
    case_0:
        if (name_ptr->end_flag[nsc_ptr->type]) {
            nsc_ptr->r_no_0++;
        } else {
            current_sc_move2();
        }

        break;

    case 1:
        if (name_ptr->r_no_0 == 7) {
            nsc_ptr->r_no_0 = 2;
        }

        if (name_ptr->end_flag[nsc_ptr->type] == 0) {
            nsc_ptr->r_no_0 = 1;
            goto case_0;
        }

        break;

    case 2:
        break;
    }

    if (name_ptr->old_code[nsc_ptr->type] != name_ptr->code[nsc_ptr->type]) {
        nsc_ptr->n_disp_flag = 0;
        nsc_ptr->f_cnt = 0;
        nsc_ptr->tenmetsu_flag = 0;
    }
}

void current_sc_move2() {
    if (name_ptr->index != nsc_ptr->c_cnt) {
        return;
    }

    switch (nsc_ptr->r_no_1) {
    case 0:
        nsc_ptr->r_no_1++;
        nsc_ptr->f_cnt = 0;
        nsc_ptr->n_disp_flag = 0;
        break;

    case 1:
        if (name_ptr->r_no_0 > 5) {
            nsc_ptr->r_no_0++;
            break;
        }

        nsc_ptr->f_cnt++;

        if (nsc_ptr->f_cnt > 16) {
            nsc_ptr->f_cnt = 0;
            nsc_ptr->n_disp_flag++;

            if (nsc_ptr->n_disp_flag > 2) {
                nsc_ptr->n_disp_flag = 0;
            }

            if (nsc_ptr->n_disp_flag != 2) {
                nsc_ptr->tenmetsu_flag = 0;
                break;
            }

            nsc_ptr->tenmetsu_flag = 1;
        }

        break;
    }
}

void start_cut_check(s16 pl_id) {
    s16 i;

    if (Naming_Cut[pl_id]) {
        if (name_ptr->r_no_0 < 6) {
            name_ptr->r_no_0 = 6;
        }

        for (i = 0; i < 3; i++) {
            if (name_ptr->end_flag[i] == 0) {
                name_ptr->code[i] = 44;
            }
        }
    }
}

void all_name_display(s16 pl_id) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (sc_name_wk[pl_id][i].tenmetsu_flag == 1 && name_ptr->index == i) {
            naming_set(pl_id, i, 8, 0x2F);
        } else {
            naming_set(pl_id, i, 8, name_ptr->code[i]);
        }
    }
}

void all_flash_display(s16 pl_id) {
    s16 i;

    for (i = 0; i < 3; i++) {
        naming_set(pl_id, i, ne_col, name_ptr->code[i]);
    }

    if (ne_timer > 1) {
        ne_timer--;
        return;
    }

    ne_col = *ne_pointer++;
    ne_timer = *ne_pointer++;

    if (ne_timer == 0xFF) {
        ne_flash_flag = 0;
    }
}

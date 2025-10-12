#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/SE.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/ta_sub.h"
#include "sf33rd/Source/Game/texcash.h"

#include "sf33rd/Source/Game/opening/opening.h"

// forward decls
void eff36_move00(WORK_Other* ewk);
void eff36_move01(WORK_Other* ewk);
void eff36_move02(WORK_Other* ewk);
void eff36_move03(WORK_Other* ewk);
void eff36_move04(WORK_Other* ewk);
void eff36_move05(WORK_Other* ewk);
void eff36_move06(WORK_Other* ewk);

void effect_36_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (ewk->wu.old_rno[1] <= op_w.index) {
            ewk->wu.routine_no[0] += 1;
        } else if (ewk->wu.old_rno[2] <= op_w.index) {
            switch (ewk->wu.routine_no[1]) {
            case 0:
                eff36_move00(ewk);
                break;

            case 1:
                eff36_move01(ewk);
                break;

            case 2:
                eff36_move02(ewk);
                break;

            case 3:
                eff36_move03(ewk);
                break;

            case 4:
                eff36_move04(ewk);
                break;

            case 5:
                eff36_move05(ewk);
                break;

            case 6:
                eff36_move06(ewk);
                break;

            default:
                break;
            }
        }

        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

void eff36_move00(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 63;
        ewk->wu.my_mr.size.y = 63;
        push_color_trans_req(ewk->wu.my_col_code & 0x1FF, 0);
        ewk->wu.my_col_code = 0;
        break;

    case 1:
        ewk->wu.my_mr.size.x += 1;
        ewk->wu.my_mr.size.y += 1;

        if (ewk->wu.my_mr.size.x >= 88) {
            ewk->wu.routine_no[2] += 1;
            ewk->wu.my_mr.size.x = 88;
            ewk->wu.my_mr.size.y = 88;
        }

        disp_pos_trans_entry5(ewk);
        break;
    }
}

void eff36_move01(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);

        if (ewk->wu.type == 0) {
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 3, 1);
        }

        ewk->wu.my_col_code = 0;
        break;

    case 1:
        disp_pos_trans_entry(ewk);
        break;
    }
}

void eff36_move02(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);

        if (ewk->wu.type == 15) {
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 8, 0);
        } else if (ewk->wu.type < 19) {
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 8, 1);
        }

        ewk->wu.my_col_code = 0;
        break;

    case 1:
        char_move(&ewk->wu);
        disp_pos_trans_entry(ewk);
        break;
    }
}

void eff36_move03(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
        push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 8, 1);
        ewk->wu.my_col_code = 0;
        break;

    case 1:
        char_move(&ewk->wu);
        ewk->wu.xyz[0].cal -= 0x8000 + 0x8000;

        if (ewk->wu.xyz[0].disp.pos < 288) {
            ewk->wu.routine_no[0] = 99;
        }

        disp_pos_trans_entry(ewk);
        break;
    }
}

const s16 eff36_04_tbl[4] = { 0, 5, 9, 13 };

void eff36_move04(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        ewk->wu.old_rno[6] = 4;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
        push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 2, 1);
        ewk->wu.my_col_code = 0;
        break;

    case 1:
        if (ewk->wu.old_rno[6] <= 0) {
            ewk->wu.disp_flag = 0;
        } else {
            ewk->wu.old_rno[6] -= 1;
        }

        if ((gSeqStatus[0] >= eff36_04_tbl[ewk->wu.routine_no[2]]) && (gSeqStatus[0] != 0x74)) {
            ewk->wu.routine_no[2] += 1;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[6] = 4;
            set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], 0x17, 0);
        }

        disp_pos_trans_entry(ewk);
        break;

    case 2:
        if (ewk->wu.old_rno[6] <= 0) {
            ewk->wu.disp_flag = 0;
        } else {
            ewk->wu.old_rno[6] -= 1;
        }

        if (gSeqStatus[0] >= eff36_04_tbl[ewk->wu.routine_no[2]]) {
            ewk->wu.routine_no[2] += 1;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[6] = 4;
            set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], 0x18, 0);
        }

        disp_pos_trans_entry(ewk);
        break;

    case 3:
        if (ewk->wu.old_rno[6] <= 0) {
            ewk->wu.disp_flag = 0;
        } else {
            ewk->wu.old_rno[6] -= 1;
        }

        if (gSeqStatus[0] >= eff36_04_tbl[ewk->wu.routine_no[2]]) {
            ewk->wu.routine_no[2] += 1;
            ewk->wu.disp_flag = 1;
            ewk->wu.old_rno[6] = 4;
            set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], 0x19, 0);
        }

        disp_pos_trans_entry(ewk);
        break;

    case 4:
        disp_pos_trans_entry(ewk);
        break;
    }
}

void eff36_move05(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);

        if (ewk->wu.type < 25) {
            push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 8, 1);
        }

        ewk->wu.my_col_code = 0;
        break;

    case 1:
        char_move(&ewk->wu);

        if (ewk->wu.cg_type == 255) {
            ewk->wu.routine_no[0] += 1;
            break;
        }

        disp_pos_trans_entry(ewk);
        break;
    }
}

void eff36_move06(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void push_color_trans_req(s32 from_col, s32 to_col);
    void set_char_move_init2(WORK * wk, s16 koc, s32 index, s32 ip, s16 scf);
#endif

    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2] += 1;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, ewk->wu.old_rno[0], ewk->wu.char_index, 0);
        push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 0x15, 8);
        push_color_trans_req((ewk->wu.my_col_code & 0x1FF) + 0x16, 9);
        ewk->wu.my_col_code = 8;
        ewk->wu.my_mr_flag = 1;
        ewk->wu.my_mr.size.x = 66;
        ewk->wu.my_mr.size.y = 66;
        disp_pos_trans_entry5(ewk);
        ewk->wu.old_rno[6] = 2;
        break;

    case 1:
        ewk->wu.old_rno[6] -= 1;

        if (ewk->wu.old_rno[6] <= 0) {
            ewk->wu.old_rno[6] = 2;
            ewk->wu.my_mr.size.x += 1;
            ewk->wu.my_mr.size.y += 1;

            if (ewk->wu.my_mr.size.x >= 127) {
                ewk->wu.routine_no[2] += 1;
                ewk->wu.my_mr.size.x = 127;
                ewk->wu.my_mr.size.y = 127;
            }
        }

        /* fallthrough */

    case 2:
        disp_pos_trans_entry5(ewk);
        break;
    }
}

const s16 eff36_data_tbl[29][9] = { { 0, 1, 512, 128, 73, 5, 1, 41, 42 },   { 0, 1, 392, 128, 70, 5, 2, 41, 42 },
                                    { 25, 0, 496, 192, 80, 3, 12, 42, 45 }, { 0, 1, 512, 128, 73, 5, 1, 43, 44 },
                                    { 0, 1, 440, 128, 70, 5, 3, 43, 44 },   { 0, 1, 512, 128, 73, 5, 1, 45, 46 },
                                    { 0, 1, 488, 128, 70, 5, 4, 45, 46 },   { 27, 0, 480, 176, 80, 3, 13, 46, 49 },
                                    { 0, 1, 512, 128, 73, 5, 1, 47, 48 },   { 0, 1, 528, 128, 70, 5, 5, 47, 48 },
                                    { 0, 1, 512, 128, 73, 5, 1, 49, 50 },   { 0, 1, 576, 128, 70, 5, 6, 49, 50 },
                                    { 39, 0, 480, 160, 80, 3, 14, 50, 53 }, { 0, 1, 512, 128, 73, 5, 1, 51, 52 },
                                    { 0, 1, 624, 128, 70, 5, 7, 51, 52 },   { 0, 2, 512, 16, 70, 4, 1, 53, 54 },
                                    { 0, 3, 600, 128, 65, 10, 1, 89, 91 },  { 0, 4, 512, 128, 65, 3, 22, 91, 92 },
                                    { 0, 2, 320, 48, 70, 8, 1, 25, 26 },    { 0, 2, 320, 128, 70, 9, 1, 25, 26 },
                                    { 0, 2, 320, 32, 70, 8, 1, 26, 27 },    { 0, 2, 320, 160, 70, 9, 1, 26, 27 },
                                    { 0, 5, 512, 144, 70, 7, 1, 8, 9 },     { 0, 5, 512, 64, 70, 7, 1, 14, 15 },
                                    { 0, 5, 544, 136, 70, 6, 1, 54, 55 },   { 0, 5, 448, 136, 70, 6, 1, 57, 58 },
                                    { 0, 5, 544, 136, 70, 6, 1, 79, 80 },   { 0, 5, 464, 136, 70, 6, 1, 81, 82 },
                                    { 0, 6, 488, 120, 66, 11, 1, 91, 92 } };

s32 effect_36_init(u8 typenum) {
#if defined(TARGET_PS2)
    s16 get_my_trans_mode(s32 curr);
#endif

    WORK_Other* ewk;
    s16 ix;
    const s16* data_ptr;

    if (Debug_w[48] & 0x10) {
        return 0;
    }

    if ((ix = pull_effect_work(3)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    data_ptr = eff36_data_tbl[typenum];
    ewk->wu.id = 0x24;
    ewk->wu.be_flag = 1;
    ewk->wu.work_id = 0x10;
    ewk->wu.type = typenum;
    ewk->wu.cgromtype = 1;
    ewk->wu.my_col_mode = 0x4200;
    ewk->wu.char_table[0] = _op_char_table;
    ewk->wu.my_family = 2;
    ewk->wu.my_mts = 9;
    ewk->wu.my_trans_mode = get_my_trans_mode(ewk->wu.my_mts);
    ewk->wu.my_col_code = 0x12C;
    ewk->wu.my_col_code += *data_ptr++;
    ewk->wu.routine_no[1] = *data_ptr++;
    ewk->wu.xyz[0].disp.pos = *data_ptr++;
    ewk->wu.xyz[1].disp.pos = *data_ptr++;
    ewk->wu.my_priority = ewk->wu.position_z = *data_ptr++;
    ewk->wu.old_rno[0] = *data_ptr++;
    ewk->wu.char_index = *data_ptr++;
    ewk->wu.old_rno[2] = *data_ptr++;
    ewk->wu.old_rno[1] = *data_ptr++;
    effect_36_move(ewk);
    return 0;
}

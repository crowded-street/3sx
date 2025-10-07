#include "sf33rd/Source/Game/EFFB5.h"
#include "bin2obj/char_table.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/n_input.h"
#include "sf33rd/Source/Game/ta_sub.h"

void current_name_move(WORK_Other* ewk, NAME_WK* np);

void effect_B5_move(WORK_Other* ewk) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    NAME_WK* np = (NAME_WK*)ewk->my_master;

    switch (ewk->wu.routine_no[0]) {
    case 0:
        ewk->wu.routine_no[0]++;
        ewk->wu.hit_stop = 0;
        ewk->wu.old_rno[3] = 0;
        ewk->wu.old_rno[1] = 47;
        ewk->wu.disp_flag = 1;
        set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.old_rno[2]] + 1, 0);
        /* fallthrough */

    case 1:
    case_1:
        if (np->end_flag[ewk->wu.type]) {
            ewk->wu.disp_flag = 1;
            set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.old_rno[2]] + 1, 0);
            ewk->wu.routine_no[0]++;
        } else {
            current_name_move(ewk, (NAME_WK*)np);
        }

        break;

    case 2:
        if (np->r_no_0 == 7) {
            ewk->wu.routine_no[0] = 3;
            ewk->wu.routine_no[2] = 1;
            ewk->wu.routine_no[3] = 3;
            ewk->wu.routine_no[4] = 0;
            ewk->wu.vitality = 0;
            ewk->wu.vital_new = 66;
            ewk->wu.vital_old = 67;
        }

        if (np->end_flag[ewk->wu.type] == 0) {
            ewk->wu.old_rno[0] = 0;
            ewk->wu.old_rno[4] = 0;
            ewk->wu.routine_no[0] = 1;
            set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.old_rno[2]] + 1, 0);
            ewk->wu.old_rno[3] = np->code[np->index];
            goto case_1;
        }

        if (ewk->wu.old_rno[3] != np->code[ewk->wu.type]) {
            ewk->wu.old_rno[0] = 0;
            ewk->wu.old_rno[4] = 1;
            set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.old_rno[2]] + 1, 0);
            ewk->wu.old_rno[3] = np->code[ewk->wu.type];
        }

        break;

    case 3:
        if (Flash_Violent(ewk, 9)) {
            ewk->wu.routine_no[0]++;
        }

        if (ewk->wu.vitality) {
            set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.old_rno[2]] + 1, 0);
        }

        break;

    case 4:
        break;

    default:
        all_cgps_put_back(&ewk->wu);
        push_effect_work(&ewk->wu);
        break;
    }

    if (ewk->wu.old_rno[3] != np->code[ewk->wu.type]) {
        ewk->wu.old_rno[4] = 0;
        ewk->wu.old_rno[0] = 0;
        set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.type] + 1, 0);
        ewk->wu.old_rno[3] = np->code[ewk->wu.type];
    }

    disp_pos_trans_entry(ewk);
}

void current_name_move(WORK_Other* ewk, NAME_WK* np) {
#if defined(TARGET_PS2)
    void set_char_move_init2(WORK * wk, s32 koc, s32 index, s32 ip, s32 scf);
#endif

    if (np->index != ewk->wu.old_rno[2]) {
        return;
    }

    switch (ewk->wu.hit_stop) {
    case 0:
        ewk->wu.hit_stop++;
        ewk->wu.old_rno[4] = 0;
        ewk->wu.old_rno[0] = 0;
        set_char_move_init2(&ewk->wu, 0, 6, np->code[ewk->wu.old_rno[2]] + 1, 0);
        ewk->wu.old_rno[3] = np->code[np->index];
        break;

    case 1:
        if (np->r_no_0 > 5) {
            ewk->wu.routine_no[0]++;
            break;
        }

        ewk->wu.old_rno[0]++;

        if (ewk->wu.old_rno[0] <= 16) {
            break;
        }

        ewk->wu.old_rno[0] = 0;
        ewk->wu.old_rno[4]++;

        if (ewk->wu.old_rno[4] > 2) {
            ewk->wu.old_rno[4] = 0;
        }

        if (ewk->wu.old_rno[4] != 2) {
            set_char_move_init2(&ewk->wu, 0, 6, np->code[np->index] + 1, 0);
        } else {
            set_char_move_init2(&ewk->wu, 0, 6, 48, 0);
        }

        break;
    }
}

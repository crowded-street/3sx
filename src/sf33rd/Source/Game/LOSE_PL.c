#include "sf33rd/Source/Game/LOSE_PL.h"
#include "common.h"
#include "sf33rd/Source/Game/CHARSET.h"
#include "sf33rd/Source/Game/EFFC1.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/PLS02.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/workuser.h"

s16 lose_rno[3];
s16 lose_free[2];

const s16 loser_type_tbl[20] = { 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0 };

const s16 meta_lose_tbl[20] = { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 28, 24, 24, 24, 24, 24, 24 };

void lose_player(PLW* wk) {
    void (*lose_jp_tbl[4])(PLW*) = { Lose_00000, Lose_10000, Lose_20000, Lose_30000 };

    if (My_char[wk->wu.id] != wk->player_number) {
        meta_lose_pause(wk);
        return;
    }

    lose_jp_tbl[loser_type_tbl[wk->player_number]](wk);
}

void Lose_00000(PLW* wk) {
    if ((pcon_rno[0] == 2) && (pcon_rno[1] == 3)) {
        Judge_normal_loser(wk);
        return;
    }

    Normal_normal_Loser(wk);
}

void Lose_10000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    s32 set_field_hosei_flag(PLW * pl, s32 pos, s16 ix);
#endif

    if ((pcon_rno[0] == 2) && (pcon_rno[1] == 3)) {
        switch (wk->wu.routine_no[3]) {
        case 0:
            wk->wu.routine_no[3]++;
            lose_rno[0] = lose_rno[1] = lose_rno[2] = 0;
            wk->wu.char_index = random_16();
            wk->wu.char_index &= 3;
            set_char_move_init(&wk->wu, 9, wk->wu.char_index + 0x38);
            break;

        default:
        case 1:
        case 9:
            char_move(&wk->wu);
            break;
        }
    } else if ((pcon_rno[1] == 0) || (pcon_rno[1] == 4)) {
        return;
    } else {
        switch (wk->wu.routine_no[3]) {
        case 0:
            wk->wu.routine_no[3]++;
            lose_rno[0] = lose_rno[1] = lose_rno[2] = 0;
            wk->wu.char_index = random_16();
            wk->wu.char_index &= 7;
            set_char_move_init(&wk->wu, 9, wk->wu.char_index + 0x18);
            break;

        case 1:
        case 9:
            char_move(&wk->wu);
            break;
        }
    }

    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

void Lose_20000(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    s32 set_field_hosei_flag(PLW * pl, s32 pos, s16 ix);
    s32 effect_C1_init(WORK * wk);
#endif

    s16 work;

    if ((pcon_rno[0] == 2) && (pcon_rno[1] == 3)) {
        Judge_normal_loser(wk);
        return;
    }

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;

        if (!Extra_Break && ((Round_num >= (save_w[Present_Mode].Battle_Number[Play_Type]) * 2) ||
                             (PL_Wins[Winner_id] >= (save_w[Present_Mode].Battle_Number[Play_Type]) + 1))) {
            effect_C1_init(&wk->wu);
        }

        if ((pcon_rno[1] != 0) && (pcon_rno[1] != 4)) {
            lose_rno[0] = lose_rno[1] = lose_rno[2] = 0;
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

    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

void Lose_30000(PLW* wk) {
#if defined(TARGET_PS2)
    s32 set_field_hosei_flag(PLW * pl, s32 pos, s16 ix);
#endif

    if ((pcon_rno[0] == 2) && (pcon_rno[1] == 3)) {
        switch (wk->wu.routine_no[3]) {
        case 0:
            wk->wu.routine_no[3]++;
            lose_rno[0] = lose_rno[1] = lose_rno[2] = 0;
            if (Country != 1) {
                set_char_move_init(&wk->wu, 9, 0x3A);
            } else {
                set_char_move_init(&wk->wu, 9, 0x38);
            }
            break;

        default:
        case 1:
        case 9:
            char_move(&wk->wu);
            break;
        }

    } else if ((pcon_rno[1] == 0) || (pcon_rno[1] == 4)) {
        return;
    } else {
        switch (wk->wu.routine_no[3]) {
        case 0:
            wk->wu.routine_no[3]++;
            lose_rno[0] = lose_rno[1] = lose_rno[2] = 0;
            if (Country != 1) {
                set_char_move_init(&wk->wu, 9, 0x1C);
            } else {
                set_char_move_init(&wk->wu, 9, 0x18);
            }
            break;

        case 1:
        case 9:
            char_move(&wk->wu);
            break;
        }
    }

    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

void Normal_normal_Loser(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    s32 set_field_hosei_flag(PLW * pl, s32 pos, s16 ix);
#endif
    s16 work;

    if ((pcon_rno[1] == 0) || (pcon_rno[1] == 4)) {
        return;
    }

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3]++;
        lose_rno[0] = lose_rno[1] = lose_rno[2] = 0;
        work = random_16();
        work &= 7;
        set_char_move_init(&wk->wu, 9, work + 0x18);
        break;

    case 1:
    case 9:
        char_move(&wk->wu);
        break;
    }

    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

void Judge_normal_loser(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    s32 set_field_hosei_flag(PLW * pl, s32 pos, s16 ix);
#endif

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

    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

void meta_lose_pause(PLW* wk) {
#if defined(TARGET_PS2)
    void set_char_move_init(WORK * wk, s16 koc, s32 index);
    s32 set_field_hosei_flag(PLW * pl, s32 pos, s16 ix);
#endif

    bg_app_stop = 1;

    if ((pcon_rno[1] == 0) || (pcon_rno[1] == 4)) {
        return;
    }

    switch (wk->wu.routine_no[3]) {
    case 0:
        wk->wu.routine_no[3] += 1;
        set_char_move_init(&wk->wu, 9, meta_lose_tbl[wk->player_number]);
        break;

    case 1:
    case 9:
        char_move(&wk->wu);
        break;
    }

    if (set_field_hosei_flag(&plw[wk->wu.id], scrr, 1) != 0) {
        set_field_hosei_flag(&plw[wk->wu.id], scrl, 0);
    }
}

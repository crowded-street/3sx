#include "common.h"
#include "sf33rd/Source/Game/MMTMCNT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect_init.h"
#include "sf33rd/Source/Game/texgroup.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"

void Win_1st();
void Win_2nd();
void Win_3rd();
void Win_4th();
void Win_5th();
void Win_6th();
void Lose_2nd();
void Lose_3rd();
void Setup_Wins_OBJ();

u8 WIN_X;

s32 Winner_Scene() {
    void (*Win_Jmp_Tbl[6])() = {
        Win_1st, Win_2nd, Win_3rd, Win_4th, Win_5th, Win_6th,
    };

    if (Break_Into) {
        return 0;
    }

    WIN_X = 0;
    Scene_Cut = Cut_Cut_Cut();
    Win_Jmp_Tbl[M_No[0]]();

    if ((Check_Exit_Check() == 0) && (Debug_w[0x18] == -1)) {
        WIN_X = 0;
    }

    return WIN_X;
}

void Win_1st() {
    s16 ix;

    Switch_Screen(0);
    Play_Mode = 1;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
    M_No[0] += 1;
    M_No[1] = 0;
    Game_pause = 0;
    BGM_Request(55);
    Cover_Timer = 23;
    All_Clear_Suicide();
    base_y_pos = 40;

    for (ix = 0; ix < 4; ix++) {
        Unsubstantial_BG[ix] = 0;
    }

    System_all_clear_Level_B();
    Purge_mmtm_area(4);
    Make_texcash_of_list(4);
    load_any_texture_patnum(0x7F30, 0xC, 0);
    Setup_BG(0, 0x200, 0);
    bg_etc_write(PL_Color_Data[My_char[Winner_id]]);
    Setup_BG(2, 0x300, 0);
    Setup_BG(1, 0x200, 0);
    Setup_BG(3, 0x2C0, 0);

    if (Play_Type == 0) {
        Last_Selected_EM[Winner_id] = 1;
    }

    pulpul_stop();
    setup_pos_remake_key(5);
}

void Win_2nd() {
#if defined(TARGET_PS2)
    s32 effect_B8_init(s32 WIN_PL_NO, s16 timer);
#endif

    Switch_Screen(0);
    M_No[0] += 1;

    Order[0x37] = 1;
    Order_Timer[0x37] = 1;
    effect_76_init(0x37);

    Order[0x35] = 3;
    Order_Timer[0x35] = 1;
    effect_76_init(0x35);

    Order[0x34] = 3;
    Order_Timer[0x34] = 1;
    effect_76_init(0x34);

    Order[0x2B] = 3;
    Order_Timer[0x2B] = 1;
    effect_76_init(0x2B);

    Order[0x3A] = 3;
    Order_Timer[0x3A] = 1;
    effect_76_init(0x3A);

    Order[0x2C] = 3;
    Order_Timer[0x2C] = 1;
    effect_76_init(0x2C);

    Order[0x2D] = 1;
    Order_Dir[0x2D] = 4;
    Order_Timer[0x2D] = 0x1E;
    effect_76_init(0x2D);

    Order[0x38] = 6;
    Order_Timer[0x38] = 1;
    effect_76_init(0x38);

    WGJ_Score = Continue_Coin[Winner_id] + Score[Winner_id][Play_Type];
    WGJ_Win = Win_Record[Winner_id];

    effect_L1_init(1);
    effect_L1_init(2);
    effect_L1_init(3);
    effect_L1_init(4);
    effect_L1_init(5);
    effect_L1_init(6);

    Setup_Wins_OBJ();
    effect_B8_init(WINNER, 0x3C);
}

void Win_3rd() {
    switch (M_No[1]) {
    case 0:
        Switch_Screen(0);
        M_No[1] += 1;
        Clear_Flash_No();
        Switch_Screen_Init(1);

        break;

    case 1:
        if (Switch_Screen_Revival(1) != 0) {
            M_No[0] += 1;
            M_Timer = 90;
            Forbid_Break = -1;
            Ignore_Entry[LOSER] = 0;
            Target_BG_X[2] = bg_w.bgw[2].wxy[0].disp.pos - 384;
            Offset_BG_X[2] = 0;
            Next_Step = 0;
            bg_mvxy.a[0].sp = -0x100000;
            bg_mvxy.d[0].sp = 0x800;

            effect_58_init(0xE, 0x14, 2);

            if (Debug_w[0x1D]) {
                My_char[0] = Debug_w[0x1D] - 1;
            }

            if (Debug_w[0x1E]) {
                My_char[1] = Debug_w[0x1E] - 1;
            }

            if (Mode_Type == 0) {
                Push_LDREQ_Queue_Player(Winner_id, My_char[Winner_id]);
            }
        }

        break;
    }
}

void Win_4th() {
    if (--M_Timer == 0) {
        M_No[0] += 1;
        M_No[1] = 0;
        M_Timer = 0xAA;
        Forbid_Break = 0;
    }
}

void Win_5th() {
    switch (M_No[1]) {
    case 0:
        if (Scene_Cut) {
            M_Timer = 9;
        }

        if (M_Timer < 10) {
            M_Timer = 9;
            M_No[1] += 1;

            if (Mode_Type == 0) {
                SsBgmFadeOut(0x1000);
            }
        }

        break;
    }

    if (--M_Timer == 0) {
        M_No[0] += 1;
        WIN_X = 1;
    }
}

void Win_6th() {
    WIN_X = 1;
}

s32 Loser_Scene() {
    void (*Lose_Jmp_Tbl[6])() = { Win_1st, Lose_2nd, Lose_3rd, Win_4th, Win_5th, Win_6th };

    WIN_X = 0;
    Scene_Cut = Cut_Cut_Loser();
    Lose_Jmp_Tbl[M_No[0]]();

    if ((Check_Exit_Check() == 0) && (Debug_w[0x18] == -1)) {
        WIN_X = 0;
    }

    if (Break_Into) {
        return 0;
    }

    return WIN_X;
}

void Lose_2nd() {
#if defined(TARGET_PS2)
    s32 effect_B8_init(s32 WIN_PL_NO, s16 timer);
#endif

    Switch_Screen(0);
    M_No[0] += 1;

    Order[0x37] = 1;
    Order_Timer[0x37] = 1;
    effect_76_init(0x37);

    Order[0x40] = 3;
    Order_Timer[0x40] = 1;
    effect_76_init(0x40);

    Order[0x36] = 3;
    Order_Timer[0x36] = 1;
    effect_76_init(0x36);

    Order[0x39] = 3;
    Order_Timer[0x39] = 1;
    effect_76_init(0x39);

    Order[0x2D] = 1;
    Order_Dir[0x2D] = 4;
    Order_Timer[0x2D] = 30;
    effect_76_init(0x2D);

    effect_B8_init(WINNER, 0x3C);
}

void Lose_3rd() {
    switch (M_No[1]) {
    case 0:
        Switch_Screen(0);
        M_No[1] += 1;
        Clear_Flash_No();
        Switch_Screen_Init(1);
        break;

    case 1:
        if (Switch_Screen_Revival(1) != 0) {
            M_No[0] += 1;
            M_Timer = 90;
            Forbid_Break = -1;
            Ignore_Entry[LOSER] = 0;
        }

        break;
    }
}

void Setup_Wins_OBJ() {
    if (Mode_Type == 1) {
        WGJ_Win = VS_Win_Record[Winner_id];
    } else {
        WGJ_Win = Win_Record[Winner_id];
    }

    if ((WGJ_Win == 0) || (Mode_Type == 2)) {
        return;
    }

    effect_L1_init(0);

    if (WGJ_Win > 1) {
        effect_76_init(0x2F);
        Order[0x2F] = 3;
        Order_Timer[0x2F] = 1;

        effect_76_init(0x31);
        Order[0x31] = 3;
        Order_Timer[0x31] = 1;
    } else {
        effect_76_init(0x2E);
        Order[0x2E] = 3;
        Order_Timer[0x2E] = 1;

        effect_76_init(0x30);
        Order[0x30] = 3;
        Order_Timer[0x30] = 1;
    }
}

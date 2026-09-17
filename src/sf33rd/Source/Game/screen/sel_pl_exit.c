/**
 * @file sel_pl_exit.c
 * The exit and handicap screens reached from character select.
 *
 * Split out of sel_pl.c, which was over the Lines of Code threshold.
 */

#include "sf33rd/Source/Game/screen/sel_pl.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/demo/demo_dat.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff50.h"
#include "sf33rd/Source/Game/effect/eff52.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff69.h"
#include "sf33rd/Source/Game/effect/eff70.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff79.h"
#include "sf33rd/Source/Game/effect/eff93.h"
#include "sf33rd/Source/Game/effect/eff99.h"
#include "sf33rd/Source/Game/effect/effa5.h"
#include "sf33rd/Source/Game/effect/effd8.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/Game/screen/sel_pl_exit.h"

void Exit_1st();
void Exit_2nd();
void Exit_3rd();
void Exit_4th();
void Exit_5th();
void Exit_6th();
void Exit_7th();
void Handicap_1st();
void Handicap_2nd();
void Handicap_3rd();
void Handicap_Control();
void Handicap_1();
void Handicap_2();
void Handicap_3();
void Handicap_4();
u16 Handicap_Vital_Move_Sub(u16 sw, s16 PL_id);
void Handicap_Stage_Move_Sub(u16 sw);

void Handicap_Vital_Select(s16 PL_id);
void Handicap_Stage_Select(s16 PL_id);

/* Defined in sel_pl.c, which this screen returns to. */
void Go_Away_Red_Lines();
u8 Setup_Battle_Country();
extern u8 SEL_PL_X;

u8 Decide_Stage;
u8 hc3alpha;

void Check_Exit() {
    void (*Sel_Exit_Tbl[10])() = { Exit_1st, Exit_2nd, Exit_3rd,     Exit_4th,     Exit_5th,
                                   Exit_6th, Exit_7th, Handicap_1st, Handicap_2nd, Handicap_3rd };
    Sel_Exit_Tbl[Exit_No]();
}

void Exit_1st() {
    if (plw[0].wu.operator != 0 && Sel_Arts_Complete[0] >= 0) {
        return;
    }

    if (plw[1].wu.operator != 0 && Sel_Arts_Complete[1] >= 0) {
        return;
    }

    Go_Away_Red_Lines();
    Order[4] = 4;
    Order_Timer[4] = 1;
    Order[7] = 4;
    Order[8] = 4;
    Order_Timer[7] = 1;
    Order_Timer[8] = 1;
    Setup_Training_Difficulty();

    if (Mode_Type == MODE_VERSUS && save_w[Present_Mode].Handicap != 0) {
        Exit_No = 7;
    } else {
        Exit_No++;
    }

    if (Demo_Flag) {
        E_No[0] = 3;
        E_No[1] = 0;
        E_No[2] = 0;
        E_No[3] = 0;
    }
}

void Exit_2nd() {
    s16 xx;

    S_No[1] = 0;

    if (Select_Status[0] == 3) {
        Exit_No = 3;
        Last_My_char[0] = My_char[0];
        Last_My_char[1] = My_char[1];
        Battle_Country = Setup_Battle_Country();
        bg_w.stage = Battle_Country;
        bg_w.area = 0;

#if DEBUG
        if (debug_config.stage_override) {
            Battle_Country = bg_w.stage = debug_config.stage_override - 1;
        }
#endif

        Push_LDREQ_Queue_BG(bg_w.stage);
        return;
    }

    if (Scene_Cut) {
        Exit_Timer = 1;
    } else {
        Exit_Timer = 60;
    }

    Exit_No++;
    Last_My_char[Player_id] = My_char[Player_id];
    Time_Stop = 2;

    for (xx = 0; xx < 4; xx++) {
        SC_No[xx] = 0;
    }
}

void Exit_3rd() {
    if (!Select_CPU_First()) {
        return;
    }

    Exit_No++;
    S_No[1] = 0;
    Suicide[3] = 1;

    if (VS_Index[Player_id] >= 9) {
        EM_Rank = 1;
        return;
    }

    EM_Rank = 0;
}

void Exit_4th() {
    FadeInit();
    FadeIn(0, 4, 8);
    Exit_No++;
    Forbid_Break = 0;
    Suicide[0] = 1;
    Menu_Suicide[0] = 1;
    bgPalCodeOffset[0] = 144;
    BGM_Request(51);
    Exit_Timer = 240;
    effect_58_init(17, 2, 0);

    if (Select_Status[0] != 3) {
        effect_K6_init(0, 35, 35, 2);
        Order[35] = 3;
        Order_Timer[35] = 1;
        effect_K6_init(1, 36, 35, 2);
        Order[36] = 3;
        Order_Timer[36] = 1;
        effect_39_init(0, 17, My_char[0], 2, 0);
        Order[17] = 3;
        Order_Timer[17] = 1;
        effect_39_init(1, 18, My_char[1], 2, 0);
        Order[18] = 3;
        Order_Timer[18] = 1;
        effect_K6_init(0, 29, 29, 2);
        Order[29] = 3;
        Order_Timer[29] = 1;
        effect_K6_init(1, 30, 29, 2);
        Order[30] = 3;
        Order_Timer[30] = 1;
    } else if (Win_Record[Champion]) {
        effect_76_init(72);
        Order[72] = 3;
        Order_Timer[72] = 1;
        effect_76_init(73);
        Order[73] = 3;
        Order_Timer[73] = 1;
    }

    effect_43_init(2, 2);
    Order[42] = 2;
    Order_Timer[42] = 1;
    Order_Dir[42] = 5;
}

void Exit_5th() {
    Exit_Timer--;

    if (!FadeIn(0, 4, 8)) {
        return;
    }

    Exit_No++;

    if (Exit_Timer < 0) {
        Exit_Timer = 1;
    }
}

void Exit_6th() {
    if (!Check_PL_Load()) {
        return;
    }

    if (!Check_LDREQ_Queue_BG(bg_w.stage)) {
        return;
    }

    // We shouldn't skip VS screen in network mode, because that can lead to IO race conditions
    if (Scene_Cut && (Mode_Type != MODE_NETWORK)) {
        Exit_Timer = 1;
    }

    if ((Exit_Timer -= 1) == 0) {
        Exit_No++;
        init_omop();
    }
}

void Exit_7th() {
    bg_w.stage = Battle_Country;
    bg_w.area = 0;
    SEL_PL_X = 1;
}

void Handicap_1st() {
    Exit_No++;
    Decide_Stage = 0;
    Menu_Common_Init();
    Setup_Training_Difficulty();
    SP_No[0][2] = 0;
    SP_No[1][2] = 0;
    effect_66_init(138, 31, 0, 2, -1, -1, -0x7FF8);
    Order[138] = 3;
    Order_Timer[138] = 1;
    effect_66_init(139, 35, 0, 2, 71, 20, 0);
    Order[139] = 5;
    effect_66_init(140, 36, 0, 2, 71, 20, 0);
    Order[140] = 5;
    effect_66_init(141, 37, 0, 2, 71, 21, 0);
    Order[141] = 5;
    effect_66_init(91, 28, 0, 2, 71, 15, 0);
    Order[91] = 3;
    Order_Timer[91] = 1;
    effect_66_init(92, 29, 0, 2, 71, 16, 0);
    Order[92] = 3;
    Order_Timer[92] = 1;
    effect_66_init(93, 30, 0, 2, 71, 17, 0);
    Order[93] = 3;
    Order_Timer[93] = 1;
    effect_66_init(120, 32, 0, 2, 71, 18, 0);
    Order[120] = 2;
    Order_Timer[120] = 1;
    effect_66_init(121, 33, 0, 2, 71, 18, 0);
    Order[121] = 2;
    Order_Timer[121] = 1;
    effect_66_init(122, 34, 0, 2, 71, 19, 0);
    Order[122] = 5;
    effect_99_init(&(Effect99Init){0, 0, 0x7047, 0, 0, 0});
    effect_99_init(&(Effect99Init){1, 0, 0x7047, 1, 1, 0});
    effect_99_init(&(Effect99Init){255, 1, 0x7047, 2, 2, 0});
    effect_99_init(&(Effect99Init){255, 1, 0x70A7, 3, 3, 0});
    effect_99_init(&(Effect99Init){255, 1, 0x70A7, 4, 4, 0});
}

void Handicap_2nd() {
    ID2 = 0;
    Handicap_Control();
    ID2 = 1;
    Handicap_Control();
}

void Handicap_3rd() {
    if (S_Timer == 9) {
        SsBgmFadeOut(0x1000);
    }

    if ((S_Timer -= 1) == 0) {
        Exit_No = 1;
    }
}

void Handicap_Control() {
    void (*Handicap_Jmp_Tbl[4])() = { Handicap_1, Handicap_2, Handicap_3, Handicap_4 };
    Handicap_Jmp_Tbl[SP_No[ID2][2]]();
}

void Handicap_1() {
    Handicap_Vital_Select(ID2);

    if (!(IO_Result & 0x100)) {
        return;
    }

    SE_selected();
    Order[ID2 + 120] = 5;
    Order[ID2 + 139] = 6;
    Order_Timer[ID2 + 139] = 1;

    if (SP_No[ID2 ^ 1][2] == 2) {
        SP_No[ID2][2] = 1;
        return;
    }

    SP_No[ID2][2] = 2;

    if (SP_No[ID2 ^ 1][2] < 3) {
        Order[122] = 2;
        Order_Timer[122] = 1;
    }
}

void Handicap_2() {
    u16 sw;

    if (ID2 == 0) {
        sw = ~p1sw_1 & p1sw_0;
    } else {
        sw = ~p2sw_1 & p2sw_0;
    }

    if (sw & SWK_EAST && Decide_Stage == 0) {
        SP_No[ID2][2] = 0;
        SE_selected();
        Order[ID2 + 139] = 5;
        Order[ID2 + 120] = 2;
        Order_Timer[ID2 + 120] = 1;
        return;
    }

    if (SP_No[ID2 ^ 1][2] == 0) {
        SP_No[ID2][2] = 2;
        Order[122] = 2;
        Order_Timer[122] = 1;
    }
}

u8 hc3alphaadd = { 1 };

void Handicap_3() {
    Handicap_Stage_Select(ID2);

    if (IO_Result & 0x100) {
        SP_No[ID2][2]++;
        SE_selected();
        Order[141] = 6;
        Order_Timer[141] = 1;
        Order[122] = 5;
        Decide_Stage = 1;
        return;
    }

    if (IO_Result & 0x200 && Decide_Stage == 0) {
        SP_No[ID2][2] = 0;
        SE_selected();
        Order[122] = 5;
        Order[ID2 + 139] = 5;
        Order[ID2 + 120] = 2;
        Order_Timer[ID2 + 120] = 1;
    }

    hc3alpha += hc3alphaadd;
    hc3alpha &= 0xF;

    if (hc3alpha == 0) {
        if (hc3alphaadd == 1) {
            hc3alpha = 16;
        }

        hc3alphaadd = -hc3alphaadd;
    }

    if (Decide_Stage != 0) {
        return;
    }

    if (ID2) {
        f32 dmypos[8] = { 296.0f, 90.0f, 296.0f, 98.0f, 284.0f, 90.0f, 268.0f, 112.0f };
        njdp2d_sort(dmypos, PrioBase[2], (hc3alpha + 48) * 0x1000000 | 0xFFFFFF, 0);
    } else {
        f32 dmypos[8] = { 88.0f, 90.0f, 88.0f, 98.0f, 100.0f, 90.0f, 116.0f, 112.0f };
        njdp2d_sort(dmypos, PrioBase[2], (hc3alpha + 48) * 0x1000000 | 0xFFFFFF, 0);
    }
}

void Handicap_4() {
    if (SP_No[0][2] > 0 && SP_No[1][2] > 0) {
        Exit_No = 9;
        S_Timer = 60;
    }
}

void Handicap_Vital_Select(s16 PL_id) {
    Setup_Pad_or_Stick();
    IO_Result = Check_Menu_Lever(PL_id, 0);
    Handicap_Vital_Move_Sub(IO_Result, PL_id);
}

u16 Handicap_Vital_Move_Sub(u16 sw, s16 PL_id) {
    if (PL_id == 0) {
        switch (sw) {
        case SWK_LEFT:
            if ((Vital_Handicap[Present_Mode][PL_id] += 1) > 7) {
                Vital_Handicap[Present_Mode][PL_id] = 7;
            } else {
                SE_dir_cursor_move();
            }

            return SWK_LEFT;

        case SWK_RIGHT:
            if ((Vital_Handicap[Present_Mode][PL_id] -= 1) < 0) {
                Vital_Handicap[Present_Mode][PL_id] = 0;
            } else {
                SE_dir_cursor_move();
            }

            return SWK_RIGHT;
        }
    } else {
        switch (sw) {
        case SWK_LEFT:
            if ((Vital_Handicap[Present_Mode][PL_id] -= 1) < 0) {
                Vital_Handicap[Present_Mode][PL_id] = 0;
            } else {
                SE_dir_cursor_move();
            }

            return SWK_LEFT;

        case SWK_RIGHT:
            if ((Vital_Handicap[Present_Mode][PL_id] += 1) > 7) {
                Vital_Handicap[Present_Mode][PL_id] = 7;
            } else {
                SE_dir_cursor_move();
            }

            return SWK_RIGHT;
        }
    }

    return 0;
}

void Handicap_Stage_Select(s16 PL_id) {
    Setup_Pad_or_Stick();
    IO_Result = Check_Menu_Lever(PL_id, 0);
    Handicap_Stage_Move_Sub(IO_Result);
}

void Handicap_Stage_Move_Sub(u16 sw) {
    switch (sw) {
    case SWK_LEFT:
        if ((VS_Stage -= 1) < 0) {
            VS_Stage = 20;
        }

        if (VS_Stage == 17) {
            VS_Stage = 16;
        }

        SE_dir_cursor_move();
        break;

    case SWK_RIGHT:
        if ((VS_Stage += 1) > 20) {
            VS_Stage = 0;
        }

        if (VS_Stage == 17) {
            VS_Stage = 18;
        }

        SE_dir_cursor_move();
        break;
    }
}

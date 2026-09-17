/**
 * @file next_cpu_setup.c
 * Setting the next fight up: the opponent list, the fighter and its colours, the
 * VS-screen objects, the bonus stage and the stage itself.
 *
 * Split out of next_cpu.c, which was over the Lines of Code threshold.
 */

#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff98.h"
#include "sf33rd/Source/Game/effect/effa5.h"
#include "sf33rd/Source/Game/effect/effa9.h"
#include "sf33rd/Source/Game/effect/effe0.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
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
#include "sf33rd/Source/Game/screen/next_cpu_setup.h"

void Setup_EM_List() {
    if (My_char[Player_id] == 0) {
        EM_Candidate[Player_id][0][9] = 1;
        EM_Candidate[Player_id][1][9] = 1;
    } else {
        EM_Candidate[Player_id][0][9] = 0;
        EM_Candidate[Player_id][1][9] = 0;
    }

    EM_List[Player_id][0] = EM_Candidate[Player_id][0][VS_Index[Player_id]];
    EM_List[Player_id][1] = EM_Candidate[Player_id][1][VS_Index[Player_id]];
}

void Setup_Next_Fighter() {
    paring_counter[COM_id] = 0;
    paring_bonus_r[COM_id] = 0;
    My_char[COM_id] = EM_id;

    if (EM_id == 17) {
        Battle_Country = Q_Country;
        bg_w.stage = Q_Country;
    } else {
        Battle_Country = EM_id;

        if (My_char[Player_id] == 0 && EM_id == 1) {
            Battle_Country = 0;
        }

        bg_w.stage = Battle_Country;
    }

#if DEBUG
    if (debug_config.stage_override) {
        Battle_Country = bg_w.stage = debug_config.stage_override - 1;
    }
#endif

    Push_LDREQ_Queue_BG(bg_w.stage);
    bg_w.area = 0;
    Super_Arts[COM_id] = Stock_Com_Arts[Player_id] = Setup_Com_Arts();

#if DEBUG
    if (debug_config.cpu_sa) {
        Super_Arts[COM_id] = debug_config.cpu_sa - 1;
    }
#endif

    Setup_Com_Color();
    Setup_PL_Color(COM_id, Com_Color_Shot);
}

const u8 Arts_Rnd_Data[8] = { 0, 0, 0, 1, 1, 1, 2, 2 };

s8 Setup_Com_Arts() {
    if (EM_id == 0) {
        return 1;
    }

    if (Stock_Com_Arts[Player_id] == -1) {
        return Arts_Rnd_Data[random_16() & 7];
    }

    return Stock_Com_Arts[Player_id];
}

void Setup_Com_Color() {
    Com_Color_Shot = Stock_Com_Color[Player_id];

    if (Break_Com[Player_id][EM_id]) {
        Com_Color_Shot = 1024;
        return;
    }

    Com_Color_Shot = 16;
}

/* Every colour arm makes the same choice: take the colour the button asks for, unless
 * the other player is already on it with the same character, in which case take the
 * alternate. The two colours are written out at each call site. */
static void Take_Player_Color(s16 PL_id, s8 wanted, s8 alternate, s32 same_char) {
    if (Player_Color[PL_id ^ 1] == wanted && same_char) {
        Player_Color[PL_id] = alternate;
    } else {
        Player_Color[PL_id] = wanted;
    }
}

/* Gill has two palettes only. */
static void Setup_Gill_Color(s16 PL_id, u16 sw, s32 same_char) {
    switch (sw) {
    case SWK_WEST:
    case SWK_NORTH:
    case SWK_RIGHT_SHOULDER:
        Take_Player_Color(PL_id, 0, 1, same_char);
        break;

    default:
        Take_Player_Color(PL_id, 1, 0, same_char);
        break;
    }
}

/* With start held, the second bank of palettes. */
static void Setup_Start_Held_Color(s16 PL_id, u16 sw, s32 same_char) {
    switch (sw) {
    case SWK_WEST:
        Take_Player_Color(PL_id, 7, 10, same_char);
        break;

    case SWK_NORTH:
        Take_Player_Color(PL_id, 8, 11, same_char);
        break;

    case SWK_RIGHT_SHOULDER:
        Take_Player_Color(PL_id, 9, 12, same_char);
        break;

    case SWK_SOUTH:
        Take_Player_Color(PL_id, 10, 7, same_char);
        break;

    case SWK_EAST:
        Take_Player_Color(PL_id, 11, 8, same_char);
        break;

    default:
        Take_Player_Color(PL_id, 12, 9, same_char);
        break;
    }
}

/* The ordinary palette bank. */
static void Setup_Normal_Color(s16 PL_id, u16 sw, s32 same_char) {
    switch (sw) {
    case SWK_WEST | SWK_RIGHT_SHOULDER | SWK_EAST:
        Take_Player_Color(PL_id, 6, 0, same_char);
        break;

    case SWK_WEST:
        Take_Player_Color(PL_id, 0, 3, same_char);
        break;

    case SWK_NORTH:
        Take_Player_Color(PL_id, 1, 4, same_char);
        break;

    case SWK_RIGHT_SHOULDER:
        Take_Player_Color(PL_id, 2, 5, same_char);
        break;

    case SWK_SOUTH:
        Take_Player_Color(PL_id, 3, 0, same_char);
        break;

    case SWK_EAST:
        Take_Player_Color(PL_id, 4, 1, same_char);
        break;

    default:
        Take_Player_Color(PL_id, 5, 2, same_char);
        break;
    }
}

void Setup_PL_Color(s16 PL_id, u16 sw) {
    s8 id_0;
    s8 id_1;
    u16 sw_new = 0;

    if (plw[PL_id ^ 1].wu.operator == 0) {
        id_0 = -1;
        id_1 = 1;
    } else {
        id_0 = My_char[PL_id];
        id_1 = My_char[PL_id ^ 1];
    }

    if (Sel_PL_Complete[PL_id ^ 1] == 0) {
        id_0 = 127;
    }

    if (plw[PL_id].wu.operator != 0 && My_char[PL_id] == CHAR_GILL) {
        sw_new = 0;
    } else {
        if (PL_id == 0) {
            sw_new = p1sw_0;
        } else {
            sw_new = p2sw_0;
        }
    }

    if (My_char[PL_id] == CHAR_GILL) {
        Setup_Gill_Color(PL_id, sw, id_0 == id_1);
    } else if (sw_new & SWK_START) {
        Setup_Start_Held_Color(PL_id, sw, id_0 == id_1);
    } else {
        Setup_Normal_Color(PL_id, sw, id_0 == id_1);
    }
}

void Setup_Regular_OBJ(s16 PL_id) {
    s16 em_id;

    if (VS_Index[Player_id] < 8) {
        Regular_OBJ_Sub(PL_id, 2);
        Regular_OBJ_Sub(PL_id, 1);
        effect_A9_init(16, 5, 10, 0);
        effect_42_init(9);
        effect_42_init(10);
        Order[9] = 0;
        Order[10] = 0;
        Order_Timer[9] = 1;
        Order_Timer[10] = 1;
        return;
    }

    effect_A9_init(33, EM_List[PL_id][1], 5, 0);
    effect_A9_init(12, EM_List[PL_id][1], 21, 0);
    effect_A9_init(57, 0, 22, 0);
    em_id = EM_List[PL_id][1];

    if (chkNameAkuma(em_id, 1)) {
        em_id = 23;
    }

    effect_A9_init(34, em_id, 20, 0);
}

void Regular_OBJ_Sub(s16 PL_id, s16 Dir) {
    s16 ix = Dir - 1;
    s16 x;

    effect_A9_init(33, EM_List[PL_id][ix], ix + 4, 0);
    x = chkNameAkuma(EM_List[PL_id][ix], 9);
    effect_A9_init(34, x + EM_List[PL_id][ix], ix + 6, 0);
    effect_A9_init(12, EM_List[PL_id][ix], ix + 8, 0);
    effect_E0_init(Dir, 0, 0);
    effect_E0_init(Dir, 1, 0);
}

/* One grade column of the history strip. An unplayed stage reads as -1 and is shown
 * as grade 0. */
static void Show_Grade_Column(s16 grade) {
    if (grade == -1) {
        grade = 0;
    }

    effect_A9_init(80, grade, 17, 0);
    Offset_BG_X[3] += 88;
}

void Setup_History_OBJ() {
    s16 q_index = Break_Com[Player_id][17];
    s16 xx;
    s16 ix;
    s16 grade;

    effect_A9_init(79, 12, 11, 0);
    Offset_BG_X[3] = 88;
    effect_A9_init(79, 13, 12, 0);
    Offset_BG_X[3] += 80;

    for (xx = 0; xx < VS_Index[Player_id]; xx++) {
        effect_A9_init(79, 13, 12, 0);
        effect_A9_init(79, xx, 13, 0);
        effect_A9_init(79, 10, 14, 0);
        ix = chkNameAkuma(EM_History[Player_id][xx], 6);
        effect_A9_init(81, ix + EM_History[Player_id][xx], 15, 0);
        effect_A9_init(12, EM_History[Player_id][xx], 16, 0);
        grade = judge_final[Player_id][0].vs_cpu_grade[xx];

        Show_Grade_Column(grade);

        if (q_index == 0 || (q_index - 1) != xx) {
            continue;
        }

        effect_A9_init(79, 13, 12, 0);
        effect_A9_init(81, 17, 15, 0);
        effect_A9_init(12, 17, 16, 0);
        grade = judge_final[Player_id]->vs_cpu_grade[15];

        Show_Grade_Column(grade);
    }

    Offset_BG_X[3] -= 40;
}

void Setup_VS_OBJ(s16 Option) {
    effect_38_init(0, 11, My_char[0], 1, 0);
    Order[11] = 3;
    Order_Timer[11] = 1;
    effect_38_init(1, 12, My_char[1], 1, 0);
    Order[12] = 3;
    Order_Timer[12] = 1;
    effect_K6_init(0, 35, 35, 0);
    Order[35] = 3;
    Order_Timer[35] = 1;
    effect_K6_init(1, 36, 35, 0);
    Order[36] = 3;
    Order_Timer[36] = 1;
    effect_39_init(0, 17, My_char[0], 0, 0);
    Order[17] = 3;
    Order_Timer[17] = 1;
    effect_39_init(1, 18, My_char[1], 0, 0);
    Order[18] = 3;
    Order_Timer[18] = 1;
    effect_K6_init(0, 29, 29, 0);
    Order[29] = 3;
    Order_Timer[29] = 1;
    effect_K6_init(1, 30, 29, 0);
    Order[30] = 3;
    Order_Timer[30] = 1;

    if (My_char[0] != 20) {
        effect_75_init(42, 3, 0);
    }

    Order[42] = 3;
    Order_Timer[42] = 1;
    Order_Dir[42] = 5;

    if (Option == 0) {
        effect_43_init(1, 0);
    }
}

s8 Check_Bonus_Stage() {
    Setup_ID();
    Bonus_Type = Check_Bonus_Type();

    if (Bonus_Type == 0) {
        return 0;
    }

    bg_w.stage = Bonus_Type;
    bg_w.area = 0;

    if (Bonus_Type == 21) {
        My_char[COM_id] = 0xC;
    } else {
        My_char[COM_id] = My_char[Player_id];
    }

    Setup_Com_Color();
    Setup_PL_Color(COM_id, Com_Color_Shot);
    Push_LDREQ_Queue_Player(COM_id, My_char[COM_id]);
    Push_LDREQ_Queue_BG(Bonus_Type);
    return Completion_Bonus[Player_id][Bonus_Type - 20] = 1;
}

#if DEBUG
/* Which bonus stage the debug override asks for, once it is known to be set. */
static s8 Debug_Bonus_Type() {
    if (debug_config.bonus_stage_override == 1) {
        Completion_Bonus[Player_id][0] = 0;
        return 20;
    }

    if (debug_config.bonus_stage_override == 2) {
        Completion_Bonus[Player_id][1] = 0;
        return 21;
    }

    return 0;
}
#endif

s8 Check_Bonus_Type() {
#if DEBUG
    if (debug_config.bonus_stage_override != 0) {
        return Debug_Bonus_Type();
    }
#endif

    if (save_w[Present_Mode].extra_option.contents[0][5] == 0) {
        return 0;
    }

    if (VS_Index[Player_id] >= 6) {
        if (Completion_Bonus[Player_id][1] & 0x80) {
            return 0;
        }

        return 21;
    }

    if (VS_Index[Player_id] >= 3) {
        if (Completion_Bonus[Player_id][0] & 0x80) {
            return 0;
        }

        return 20;
    }

    return 0;
}

void Setup_Next_Stage(s16 dir_step) {
    s16 ix;

    for (ix = 0; ix < 4; ix++) {
        effect_A9_init(dir_step, ix, ix + 23, 0);
    }
}

void Check_Auto_Cut() {
    if (!Auto_Cut_Sub()) {
        return;
    }

    if ((Cut_Scroll -= 1) < 0) {
        Cut_Scroll = 0;
    }
}

s32 Auto_Cut_Sub() {
    if (plw[0].wu.operator && ~p1sw_1 & p1sw_0 & 0xFF0) {
        return 1;
    }

    if (plw[1].wu.operator && ~p2sw_1 & p2sw_0 & 0xFF0) {
        return 1;
    }

    return 0;
}

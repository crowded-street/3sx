/**
 * @file com_sub_attack.c
 * CPU Controlled Character ground attacks and guard.
 *
 * Split out of com_sub.c unchanged - no function body was edited.
 */

#include "sf33rd/Source/Game/com/com_sub.h"
#include "common.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/active/ac0000.h"
#include "sf33rd/Source/Game/com/active/ac0001.h"
#include "sf33rd/Source/Game/com/active/ac0002.h"
#include "sf33rd/Source/Game/com/active/ac0003.h"
#include "sf33rd/Source/Game/com/active/ac0004.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_datu.h"
#include "sf33rd/Source/Game/com/com_pl.h"
#include "sf33rd/Source/Game/com/follow/fl_com00.h"
#include "sf33rd/Source/Game/com/follow/fl_com02.h"
#include "sf33rd/Source/Game/com/passive/pass0000.h"
#include "sf33rd/Source/Game/com/passive/pass0001.h"
#include "sf33rd/Source/Game/com/passive/pass0002.h"
#include "sf33rd/Source/Game/com/passive/pass0003.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/pls03.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/screen/vs_shell.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"
#include "sf33rd/Source/Game/com/com_sub_internal.h"

void Forced_Guard(PLW* wk, s16 Guard_Type) {
    WORK* em;
    s16 xx;

    em = (WORK*)wk->wu.target_adrs;

    if (Attack_Flag[wk->wu.id] == 0) {
        Next_Be_Free(wk);
    }
    xx = Hit_Range_Data[em->hit_range];
    xx += Com_Width_Data[wk->wu.id];

    if (PL_Distance[wk->wu.id] > xx) {
        Next_Be_Free(wk);
    }

    Next_Be_Guard(wk, em, Guard_Type);
    Lever_Buff[wk->wu.id] |= Lever_Squat[wk->wu.id];
}

/* Latch the hit and hand the attack over to the reaction handler. Every attack
 * function in this file ends its last state on these two lines. */
static void Attack_Reaction_Exit(PLW* wk, s16 Reaction) {
    Stock_Hit_Flag[wk->wu.id] = wk->wu.hf.hit.player;
    Reaction_Sub(wk, Reaction, 0);
}

/* Start the taunt. Non-zero when the state advanced and the arm state runs in
 * the same frame; each early exit here broke out of the switch instead. */
static s32 Provoke_Begin(PLW* wk, s16 Lever) {
    if (Check_Passive(wk) != 0) {
        return 0;
    }

    if (wk->spmv_ng_flag & DIP_TAUNT_DISABLED) {
        Next_Be_Free(wk);
        return 0;
    }

    CP_Index[wk->wu.id][1]++;
    if (Lever != -1) {
        Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, Lever & 1);
        Lever_LR[wk->wu.id] |= Lever & 2;
    }

    return 1;
}

/* Wait for the taunt to be permitted, then send the taunt input. */
static void Provoke_Arm(PLW* wk) {
    if (wk->permited_koa & 0x80) {
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] = 0x440;
    }
}

/* Hold the lever until the taunt animation is over. */
static void Provoke_Wait(PLW* wk) {
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
    if ((wk->wu.routine_no[1] != 4) || (wk->wu.routine_no[2] != 0x1E)) {
        Reaction_Exit_Sub(wk);
    }
}

void Provoke(PLW* wk, s16 Lever) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (!Provoke_Begin(wk, Lever)) {
            break;
        }
        /* fallthrough */

    case 1:
        Provoke_Arm(wk);
        break;

    default:
        Provoke_Wait(wk);
        break;
    }
}

/* Set the guard lever up and check the attack may start. Non-zero when the
 * state advanced and the wind-up runs in the same frame; each early exit here
 * broke out of the switch instead. */
static s32 Normal_Attack_Begin(PLW* wk, s16 Reaction, u16 Lever_Data) {
    if (Check_Passive(wk) != 0) {
        return 0;
    }

    if (Lever_Data & 2) {
        Lever_LR[wk->wu.id] = Setup_Guard_Lever(wk, 1);
    } else {
        Lever_LR[wk->wu.id] = 0;
    }

    Lever_LR[wk->wu.id] |= Lever_Data & 2;
    Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];

    if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) != 0) {
        return 0;
    }

    CP_Index[wk->wu.id][1]++;
    Check_First_Menu(wk);

    return 1;
}

/* Count the combo delay down, and press the attack once it runs out. */
static void Normal_Attack_Wind_Up(PLW* wk, u16 Lever_Data) {
    if (Check_Passive(wk) != 0) {
        return;
    }
    if (--Combo_Speed[wk->wu.id] == 0) {
        CP_Index[wk->wu.id][1]++;
        Lever_Buff[wk->wu.id] = Lever_Data;
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    } else {
        Lever_Buff[wk->wu.id] |= Lever_LR[wk->wu.id];
    }
}

void Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (!Normal_Attack_Begin(wk, Reaction, Lever_Data)) {
            break;
        }
        /* fallthrough */

    case 1:
        Normal_Attack_Wind_Up(wk, Lever_Data);
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Attack_Reaction_Exit(wk, Reaction);
        break;
    }
}

s32 Small_Jump_Measure(PLW* wk) {
    if (Lever_Squat[wk->wu.id] & 2) {
        return Setup_Guard_Lever(wk, 1);
    }
    return 0;
}

/* Hold the attack lever down while the SP timer runs, and step on when it
 * expires. Normal_Attack_SP and Lever_Attack_SP share this state. */
static void Attack_SP_Hold(PLW* wk, u16 Lever_Data) {
    if (--Timer_00[wk->wu.id]) {
        Lever_Buff[wk->wu.id] = Lever_Data;
        Lever_Squat[wk->wu.id] = Lever_Data & 2;
    } else {
        CP_Index[wk->wu.id][1]++;
    }
}

void Normal_Attack_SP(PLW* wk, s16 Reaction, u16 Lever_Data, s16 Time) {
    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Timer_00[wk->wu.id] = Time;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            Lever_Buff[wk->wu.id] = Lever_Data;
            Lever_Squat[wk->wu.id] = Lever_Data & 2;
            CP_Index[wk->wu.id][1]++;
            Timer_00[wk->wu.id]--;
        } else {
            Lever_Buff[wk->wu.id] = Lever_Squat[wk->wu.id];
        }
        break;

    case 2:
        Attack_SP_Hold(wk, Lever_Data);
        break;

    default:
        Attack_Reaction_Exit(wk, Reaction);
        break;
    }
}

void Adjust_Attack(PLW* wk, s16 Reaction, u16 Lever_Data) {
    u16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Normal_Attack(wk, Reaction, Lever_Data) != 0) {
            break;
        }

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id] == 0) {
            xx = Check_Squat(wk);
            Lever_Buff[wk->wu.id] = Lever_Data | xx;
            Lever_LR[wk->wu.id] = xx;
            Lever_Buff[wk->wu.id] |= Small_Jump_Measure(wk);
            CP_Index[wk->wu.id][1]++;
        } else {
            Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
            Lever_Buff[wk->wu.id] |= Small_Jump_Measure(wk);
        }
        break;

    default:
        Lever_Buff[wk->wu.id] = Lever_LR[wk->wu.id];
        Attack_Reaction_Exit(wk, Reaction);
        break;
    }
}

s32 Check_Squat(PLW* wk) {
    if (((WORK*)wk->wu.target_adrs)->pat_status == 0x20) {
        return 0;
    }

    return 2;
}

s32 Check_Start_Normal_Attack(PLW* wk, s16 Reaction, u16 Lever_Data) {
    if (((wk->wu.routine_no[1]) != 4) || ((wk->wu.cg_type) == 0x40)) {
        return 0;
    }

    if (wk->wu.cg_cancel & 4) {
        return 0;
    }

    if (wk->permited_koa & 0x10) {
        return 0;
    }

    if ((wk->wu.cg_cancel & 8) && (Reaction == 0xE)) {
        return 0;
    }

    return 1;
}

void Lever_Attack(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Lever_Attack(wk, Lever, Lever_Data) != 0) {
            break;
        }
        dash_flag_clear(wk->wu.id);

        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* falltrhough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }

        xx = Setup_Guard_Lever(wk, Lever);
        Lever_Buff[wk->wu.id] = (Lever_Data | xx);
        CP_Index[wk->wu.id][1]++;
        break;

    default:
        if (wk->wu.routine_no[1] == 2) {
            Be_Catch(wk);
        } else {
            Attack_Reaction_Exit(wk, Reaction);
        }
        break;
    }
}

void Lever_Attack_SP(PLW* wk, s16 Reaction, u16 Lever, u16 Lever_Data, s16 Time) {
    s16 xx;

    switch (CP_Index[wk->wu.id][1]) {
    case 0:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (Check_Start_Lever_Attack(wk, Lever, Lever_Data) != 0) {
            break;
        }
        dash_flag_clear(wk->wu.id);

        Timer_00[wk->wu.id] = Time;
        CP_Index[wk->wu.id][1]++;
        Check_First_Menu(wk);
        /* fallthrough */

    case 1:
        if (Check_Passive(wk) != 0) {
            break;
        }
        if (--Combo_Speed[wk->wu.id]) {
            break;
        }

        xx = Setup_Guard_Lever(wk, Lever);
        Lever_Buff[wk->wu.id] = (Lever_Data | xx);

        Timer_00[wk->wu.id]--;
        CP_Index[wk->wu.id][1]++;
        break;

    case 2:
        Attack_SP_Hold(wk, Lever_Data);
        break;

    default:
        Attack_Reaction_Exit(wk, Reaction);
        break;
    }
}

s32 Setup_Guard_Lever(PLW* wk, u16 Lever) {
    s32 rnum = 0;

    switch (Lever) {
    case 0:
        if (wk->wu.rl_waza == 0) {
            rnum = 4;
        } else {
            rnum = 8;
        }
        break;
    case 1:
        if (wk->wu.rl_waza == 1) {
            rnum = 4;
        } else {
            rnum = 8;
        }
        break;
    }
    return rnum;
}

s32 Check_Start_Lever_Attack(PLW* wk, u16 Lever, u16 Lever_Data) {
    if (Check_Free_To_Act(wk)) {
        return 0;
    }

    if (wk->wu.cg_cancel & 4) {
        return 0;
    }

    if (wk->wu.cg_cancel & 8) {
        return 0;
    }

    return 1;
}

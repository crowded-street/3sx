/**
 * @file com_pl_flip.c
 * CPU Controlled Character
 * Floating, flipping and turning to face the other way.
 */

#include "sf33rd/Source/Game/com/com_pl.h"
#include "common.h"
#include "sf33rd/Source/Game/com/active/active00.h"
#include "sf33rd/Source/Game/com/active/active01.h"
#include "sf33rd/Source/Game/com/active/active02.h"
#include "sf33rd/Source/Game/com/active/active03.h"
#include "sf33rd/Source/Game/com/active/active04.h"
#include "sf33rd/Source/Game/com/active/active05.h"
#include "sf33rd/Source/Game/com/active/active06.h"
#include "sf33rd/Source/Game/com/active/active07.h"
#include "sf33rd/Source/Game/com/active/active08.h"
#include "sf33rd/Source/Game/com/active/active09.h"
#include "sf33rd/Source/Game/com/active/active10.h"
#include "sf33rd/Source/Game/com/active/active11.h"
#include "sf33rd/Source/Game/com/active/active12.h"
#include "sf33rd/Source/Game/com/active/active13.h"
#include "sf33rd/Source/Game/com/active/active14.h"
#include "sf33rd/Source/Game/com/active/active15.h"
#include "sf33rd/Source/Game/com/active/active16.h"
#include "sf33rd/Source/Game/com/active/active17.h"
#include "sf33rd/Source/Game/com/active/active18.h"
#include "sf33rd/Source/Game/com/active/active19.h"
#include "sf33rd/Source/Game/com/ck_pass.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/com/com_sub.h"
#include "sf33rd/Source/Game/com/follow/follow02.h"
#include "sf33rd/Source/Game/com/passive/pass00.h"
#include "sf33rd/Source/Game/com/passive/pass01.h"
#include "sf33rd/Source/Game/com/passive/pass02.h"
#include "sf33rd/Source/Game/com/passive/pass03.h"
#include "sf33rd/Source/Game/com/passive/pass04.h"
#include "sf33rd/Source/Game/com/passive/pass05.h"
#include "sf33rd/Source/Game/com/passive/pass06.h"
#include "sf33rd/Source/Game/com/passive/pass07.h"
#include "sf33rd/Source/Game/com/passive/pass08.h"
#include "sf33rd/Source/Game/com/passive/pass09.h"
#include "sf33rd/Source/Game/com/passive/pass10.h"
#include "sf33rd/Source/Game/com/passive/pass11.h"
#include "sf33rd/Source/Game/com/passive/pass12.h"
#include "sf33rd/Source/Game/com/passive/pass13.h"
#include "sf33rd/Source/Game/com/passive/pass14.h"
#include "sf33rd/Source/Game/com/passive/pass15.h"
#include "sf33rd/Source/Game/com/passive/pass16.h"
#include "sf33rd/Source/Game/com/passive/pass17.h"
#include "sf33rd/Source/Game/com/passive/pass18.h"
#include "sf33rd/Source/Game/com/passive/pass19.h"
#include "sf33rd/Source/Game/com/shell/shell00.h"
#include "sf33rd/Source/Game/com/shell/shell01.h"
#include "sf33rd/Source/Game/com/shell/shell03.h"
#include "sf33rd/Source/Game/com/shell/shell04.h"
#include "sf33rd/Source/Game/com/shell/shell05.h"
#include "sf33rd/Source/Game/com/shell/shell07.h"
#include "sf33rd/Source/Game/com/shell/shell11.h"
#include "sf33rd/Source/Game/com/shell/shell12.h"
#include "sf33rd/Source/Game/com/shell/shell13.h"
#include "sf33rd/Source/Game/com/shell/shell14.h"
#include "sf33rd/Source/Game/engine/cmd_data.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/getup.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/com/com_pl_internal.h"

static s32 Check_Shell_Flip(PLW* wk);
static s32 Check_Flip_Attack(PLW* wk);


void Float_3rd(PLW* wk) {
    if (wk->wu.routine_no[1] != 1) {
        Next_Be_Free(wk);
    }

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        CP_No[wk->wu.id][2]++;
        Timer_00[wk->wu.id] = 4;
        Lever_Pool[wk->wu.id] = Setup_Guard_Lever(wk, 0);
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;

    default:
        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Timer_00[wk->wu.id] = 3;
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;
    }
}

void Float_4th(PLW* wk) {
    if (wk->wu.routine_no[1] != 1) {
        Next_Be_Free(wk);
    }

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        CP_No[wk->wu.id][2]++;
        Timer_00[wk->wu.id] = 4;
        Lever_Pool[wk->wu.id] = Setup_Guard_Lever(wk, 1);
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;

    default:
        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Timer_00[wk->wu.id] = 3;
        Lever_Buff[wk->wu.id] = Lever_Pool[wk->wu.id];
        break;
    }
}

void Flip_Zero(PLW* wk) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    switch (CP_No[wk->wu.id][2]) {
    case 0:
        if (em->routine_no[1] != 4) {
            Exit_Damage_Sub(wk);
            break;
        }

        if (!Check_Flip_GO(wk, 0)) {
            break;
        }

        CP_No[wk->wu.id][2]++;
        Timer_00[wk->wu.id] = 9;
        break;

    case 1:
        if (Check_Flip(wk)) {
            break;
        }

        if (--Timer_00[wk->wu.id] != 0) {
            break;
        }

        Exit_Damage_Sub(wk);
        break;
    }
}

s32 Check_Flip_GO(PLW* wk, s16 xx) {
    WORK* em = (WORK*)wk->wu.target_adrs;

    if (em->att_hit_ok || xx) {
        if (em->pat_status == 0x21 || em->pat_status == 0x20) {
            Lever_Buff[wk->wu.id] = 2;
        } else {
            Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 0);
        }

        if (xx == 0 && Resume_Lever[wk->wu.id][0] == Lever_Buff[wk->wu.id]) {
            Next_Be_Guard(wk, em, 0);
            Flip_Counter[wk->wu.id] = 255;
            return 0;
        }

        Flip_Counter[wk->wu.id]++;
        return 1;
    }

    return 0;
}

void Flip_1st(PLW* wk) {
    if (wk->wu.xyz[1].disp.pos <= 0) {
        Exit_Damage_Sub(wk);
    }
}

void Flip_2nd(PLW* wk) {
    if (PL_Damage_Data[wk->wu.routine_no[2]] != 0) {
        return;
    }

    if (Check_Flip_Attack(wk) != 0) {
        if (Select_Passive(wk) == -1) {
            Exit_Damage_Sub(wk);
        }
    } else {
        Exit_Damage_Sub(wk);
    }
}

void Flip_3rd(PLW* wk) {
    s16 next_disposal;

    if (PL_Damage_Data[wk->wu.routine_no[2]] == 0) {
        return;
    }

    next_disposal = Check_Shell_Flip(wk);

    switch (next_disposal) {
    case 0:
        CP_No[wk->wu.id][1] = 2;
        return;

    case 1:
        Timer_00[wk->wu.id] = 15;
        /* fallthrough */

    case 3:
        CP_No[wk->wu.id][1] = 4;
        return;

    case 2:
        CP_No[wk->wu.id][0] = 9;
        CP_No[wk->wu.id][1] = 0;
        CP_No[wk->wu.id][2] = 0;
        CP_No[wk->wu.id][3] = 0;
        Timer_00[wk->wu.id] = 10;
        Flip_Counter[wk->wu.id] = 255;
        dash_flag_clear(wk->wu.id);
        Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 1);

        if (((WORK*)wk->wu.dmg_adrs)->att.guard & 0x10) {
            break;
        }

        Lever_Buff[wk->wu.id] |= 2;
        break;

    default:
        Flip_Counter[wk->wu.id] = 255;
        Next_Be_Free(wk);
        break;
    }
}

void Flip_4th(PLW* wk) {
    if (--Timer_00[wk->wu.id] != 0) {
        return;
    }

    if (SetShellFlipLever(wk) == 0) {
        Flip_Counter[wk->wu.id] = 255;
        Next_Be_Free(wk);
        return;
    }

    CP_No[wk->wu.id][1] = 0;
    CP_No[wk->wu.id][2] = 1;
    Timer_00[wk->wu.id] = 9;
}

s32 SetShellFlipLever(PLW* wk) {
    WORK* tmw;

    Lever_Buff[wk->wu.id] = 0;
    tmw = (WORK*)Shell_Address[wk->wu.id];

    if (tmw == NULL) {
        return 0;
    }

    if (tmw->be_flag == 0 || tmw->id != 13) {
        return 0;
    }

    if (!(tmw->att.guard & 3)) {
        return 0;
    }

    Lever_Buff[wk->wu.id] = 2;

    if (tmw->att.guard & 2) {
        Lever_Buff[wk->wu.id] = Setup_Guard_Lever(wk, 0);
    }

    return 1;
}

static s32 Check_Shell_Flip(PLW* wk) {
    WORK* shell;
    s32 Rnd;
    s32 Lv;
    s32 xx;
    s32 res;

    res = 0;
    Flip_Counter[wk->wu.id]++;

    if (Timer_01[wk->wu.id] != 8) {
        return 0;
    }

    shell = (WORK*)wk->wu.dmg_adrs;

    if (shell == NULL) {
        res = 1;
    } else if (shell->be_flag != 0 && shell->id == 13) {
        // do nothing
    } else {
        res = 1;
    }

    if (res || shell->vital_new < 256) {
        if ((xx = Check_Shell_Another_in_Flip(wk)) == 0) {
            if (res) {
                return -1;
            }

            return 0;
        }

        if (xx > 16) {
            return 0;
        }

        res = 1;
        shell = (WORK*)Shell_Address[wk->wu.id];
        wk->wu.dmg_adrs = shell;
    }

    Rnd = random_32_com();
    Rnd -= Flip_Term_Correct(wk);
    Lv = emLevelRemake(Setup_Lv08(0), 8, 0);

    if (Rnd >= Shell_Renzoku_Flip_Data[wk->player_number][Lv]) {
        return 2;
    }

    if (Flip_Counter[wk->wu.id] < emGetMaxBlocking()) {
        if (res == 0) {
            return 1;
        }

        xx -= 8;

        if (xx > 0) {
            Timer_00[wk->wu.id] = xx;
            return 3;
        }
    }

    return 0;
}

s32 Check_Flip(PLW* wk) {
    if (Flip_Flag[wk->wu.id]) {
        return 0;
    }

    if (wk->wu.routine_no[1] != 0) {
        return 0;
    }

    if (PL_Damage_Data[wk->wu.routine_no[2]] == 0) {
        return 0;
    }

    if (Flip_Counter[wk->wu.id] == 0xFF) {
        return 0;
    }

    CP_No[wk->wu.id][0] = 12;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Timer_00[wk->wu.id] = 15;

    if (Timer_01[wk->wu.id] == 8) {
        CP_No[wk->wu.id][1] = 3;
    } else {
        CP_No[wk->wu.id][1] = 2;
    }

    if (wk->wu.xyz[1].disp.pos > 0) {
        CP_No[wk->wu.id][1] = 1;
    }

    return 1;
}

static s32 Check_Flip_Attack(PLW* wk) {
    s16 Lv = Setup_Lv08(0);
    s16 Rnd;
    s16 xx;

    if (Break_Into_CPU == 2) {
        Lv = 7;
    }

    if (Demo_Flag == 0 && Weak_PL == wk->wu.id) {
        Lv = 0;
    }

    Rnd = random_32_com();
    Rnd -= Flip_Term_Correct(wk);
    xx = Setup_EM_Rank_Index(wk);

    if (Rnd >= Flip_Attack_Data[xx][emLevelRemake(Lv, 8, 0)]) {
        return 0;
    }

    Flip_Flag[wk->wu.id] = 0;
    VS_Tech[wk->wu.id] = 13;
    Counter_Attack[wk->wu.id] = 1;
    return 1;
}

void Next_Be_Float(PLW* wk) {
    s16 Rnd;
    s16 Lv;

    CP_No[wk->wu.id][0] = 11;
    CP_No[wk->wu.id][2] = 0;
    CP_No[wk->wu.id][3] = 0;
    Clear_Com_Flag(wk);
    Lv = Setup_Lv04(0);
    Rnd = random_16_com();
    CP_No[wk->wu.id][1] = Float_Attack_Data[emLevelRemake(Lv, 4, 0)][Rnd];
}

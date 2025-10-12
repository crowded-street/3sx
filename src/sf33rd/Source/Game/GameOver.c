#include "common.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/Sel_Data.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/bg_sub.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect_init.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"

u8 GAME_OVER_X; // size: 0x1, address: 0x579078

void GameOver_1st();
void GameOver_2nd();
void GameOver_3rd();
void Setup_Result_OBJ();

s16 Game_Over() {
    void (*GameOver_Jmp_Tbl[3])() = { GameOver_1st, GameOver_2nd, GameOver_3rd };

    GAME_OVER_X = 0;
    Scene_Cut = Cut_Cut_Loser();
    GameOver_Jmp_Tbl[GO_No[0]]();

    if ((Check_Exit_Check() == 0) && (Debug_w[0x18] == -1)) {
        GAME_OVER_X = 0;
    }

    if (Break_Into) {
        return 0;
    }

    return GAME_OVER_X;
}

void GameOver_1st() {
    switch (GO_No[1]) {
    case 0:
        GO_No[1] += 1;
        Unsubstantial_BG[3] = 1;
        Target_BG_X[3] = bg_w.bgw[3].wxy[0].disp.pos + 466;
        Offset_BG_X[3] = 0;
        Target_BG_X[1] = bg_w.bgw[1].wxy[0].disp.pos + 458;
        Offset_BG_X[1] = 0;
        bg_mvxy.a[0].sp = 0xE0000;
        bg_mvxy.d[0].sp = 0;
        effect_A9_init(0x20, 5, 0x12, 0);
        BGM_Request(59);
        Next_Step = 0;

        effect_58_init(0xC, 1, 3);
        effect_58_init(0xC, 1, 1);
        effect_58_init(0xF, 5, 2);
        effect_58_init(0x10, 5, 2);

        if (Break_Com[WINNER][0]) {
            effect_76_init(0x38);
            Order[0x38] = 3;
            Order_Timer[0x38] = 1;
            return;
        }

        break;

    case 1:
        if (Next_Step) {
            GO_No[1] += 1;
            G_Timer = 420; // 7 minutes?
            return;
        }

        break;

    case 2:
        if (Scene_Cut) {
            G_Timer = 1;
        }
        // fallthrough

    default:
        if (--G_Timer == 0) {
            GO_No[0] += 1;
            GO_No[1] = 0;
        }

        break;
    }
}

void GameOver_2nd() {
    s16 temp_v1_3;
    s16 temp_v1_4;
    s8 temp_v1_2;
    u32 temp_v1;

    switch (GO_No[1]) {
    case 0:
        GO_No[1] += 1;
        /* fallthrough */

    case 1:
        GO_No[1] += 1;
        Forbid_Break = 0;
        FadeInit();
        return;

    case 2:
        if (FadeOut(1, 8, 8) != 0) {
            GO_No[1] += 1;
            Cover_Timer = 5;
            Suicide[3] = 1;
            Suicide[2] = 0;

            if (Break_Com[WINNER][0]) {
                Setup_BG(0, 0x200, 0);
                bg_etc_write(PL_Color_Data[My_char[Winner_id]]);
            }

            Setup_Result_OBJ();
            effect_76_init(0x41);
            Order[0x41] = 3;
            Order_Timer[0x41] = 1;
            return;
        }

        break;

    case 3:
        FadeOut(1, 8, 8);

        if (--Cover_Timer == 0) {
            GO_No[1] += 1;
            Forbid_Break = -1;
            FadeInit();
            return;
        }

        break;

    case 4:
        if (FadeIn(1, 8, 8) != 0) {
            Forbid_Break = 0;
            BGM_Request(54);
            Ignore_Entry[LOSER] = 0;

            if ((E_Number[0][0] != 2) && (E_Number[1][0] != 2)) {
                GO_No[1] += 2;
                G_Timer = 60;
                return;
            }

            GO_No[1] += 1;
            return;
        }

        break;

    case 5:
        if ((E_Number[0][0] != 2) && (E_Number[1][0] != 2)) {
            GO_No[1] += 1;
            G_Timer = 60;
            return;
        }

        break;

    case 6:
        if (--G_Timer == 0) {
            GO_No[1] += 1;
            G_Timer = Result_Timer[Player_id];
            return;
        }

        break;

    case 7:
        if (Scene_Cut) {
            G_Timer = 1;
        }
        /* fallthrough */

    default:
        if (--G_Timer == 0) {
            GO_No[0] += 1;
            SsBgmFadeOut(0x222);
            GAME_OVER_X = 1;
        }

        break;
    }
}

void GameOver_3rd() {
    GAME_OVER_X = 1;
}

void Setup_Result_OBJ() {
    effect_76_init(0x32);
    Order[0x32] = 3;
    Order_Timer[0x32] = 1;

    effect_76_init(0x33);
    Order[0x33] = 3;
    Order_Timer[0x33] = 1;

    effect_L1_init(7);
    effect_L1_init(8);
    effect_L1_init(9);
    effect_L1_init(0xA);
    effect_L1_init(0xB);
    effect_L1_init(0xC);
    effect_L1_init(0xD);
    effect_L1_init(0xE);
}

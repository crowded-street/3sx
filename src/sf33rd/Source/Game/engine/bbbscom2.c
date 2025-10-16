/**
 * @file bbbscom2.c
 * "Crush the Car!" Bonus Stage
 */

#include "sf33rd/Source/Game/engine/bbbscom2.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff16.h"
#include "sf33rd/Source/Game/effect/effc2.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/texcash.h"

void bbbs_com_execute2(PLW* wk) {
    switch (Bonus_Stage_RNO[0]) {
    case 0:
        if (Bonus_Stage_RNO[1]) {
            if (!Allow_a_battle_f) {
                break;
            }

            Bonus_Stage_RNO[0] = 1;
            Bonus_Stage_RNO[1] = 0;
            wk->zettai_muteki_flag = 1;

            break;
        }

        if (wk->wu.id) {
            purge_texcash_work(4);
            wk->wu.my_mts = 3;

        } else {
            purge_texcash_work(3);
            wk->wu.my_mts = 4;
        }

        make_texcash_work(5);
        wk->wu.xyz[0].disp.pos = 468;
        wk->wu.xyz[1].disp.pos = 0;
        effect_C2_init(&wk->wu, 0);
        effect_16_init(wk, 0);
        effect_16_init(wk, 1);
        Bonus_Stage_RNO[1] = 1;

        break;

    case 1:
        if (((WORK*)wk->wu.my_effadrs)->routine_no[0] == 2 && ((WORK*)wk->wu.my_effadrs)->routine_no[1] == 9) {
            Bonus_Stage_RNO[0] = 2;
            Bonus_Stage_RNO[1] = 0;
            Allow_a_battle_f = 0;
            pcon_dp_flag = 1;
        }

        wk->wu.xyz[0].disp.pos = 468;

        break;

    case 2:
        break;
    }
}

/**
 * @file eff84.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/eff84.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff56.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"

const u8 Time_Data[5] = { 80, 90, 50, 50, 50 };

void effect_84_move(WORK_Other* ewk) {
    if (Suicide[0]) {
        push_effect_work(&ewk->wu);
        return;
    }

    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (request_message) {
            ewk->wu.routine_no[0]++;
            ewk->wu.dir_timer = Time_Data[request_message];
        }

        break;

    case 1:
        switch (ewk->wu.routine_no[1]) {
        case 0:
            switch (message_index) {
            case 0:
                Game_pause = 1;
                ewk->wu.routine_no[1]++;
                effect_56_init(0, 0);
                break;

            case 1:
                Game_pause = 1;
                ewk->wu.routine_no[1]++;
                effect_56_init(1, 0);
                break;

            case 2:
                Game_pause = 1;
                ewk->wu.routine_no[1]++;

                if (Bonus_Game_Flag == 20 && bg_w.stage == 20) {
                    ewk->wu.dir_timer = 90;
                }

                effect_56_init(2, 0);
                break;

            case 3:
                ewk->wu.routine_no[1]++;
                effect_56_init(3, 3);
                break;

            case 4:
            default:
                ewk->wu.routine_no[1]++;
                effect_56_init(4, 2);
                break;
            }

            break;

        case 1:
            if ((ewk->wu.dir_timer -= 1) != 0) {
                break;
            }

            switch (message_index) {
            case 4:
                Message_Suicide[2] = 1;
                break;

            case 3:
                Message_Suicide[3] = 1;
                break;

            default:
                Message_Suicide[0] = 1;
                break;
            }

            dead_voice_request();
            request_message = 0;
            Game_pause = 0;
            ewk->wu.routine_no[0] = ewk->wu.routine_no[1] = 0;
            break;
        }

        break;

    case 2:
        break;
    }
}

s32 effect_84_init() {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 84;
    return 0;
}

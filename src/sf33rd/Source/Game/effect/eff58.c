/**
 * @file eff58.c
 * Shared timed transition effect dispatcher.
 * Depending on `id`, it performs screen wipes, tone fades, background
 * movement, audio requests, and logo presentation sequencing.
 */

#include "sf33rd/Source/Game/effect/eff58.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

s32 SF33rd_Logo(WORK_Other* ewk);
void EFF58_Type_01(WORK_Other* ewk);
void Fade_In_58_Sub(WORK_Other* ewk);

static void move_background_to_target(WORK_Other* ewk, s32 speed) {
    if (0 < speed) {
        if (Target_BG_X[ewk->wu.direction] + Offset_BG_X[ewk->wu.direction] <=
            bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos) {
            Next_Step |= 1;
            bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos =
                Target_BG_X[ewk->wu.direction] + Offset_BG_X[ewk->wu.direction];
            ewk->wu.routine_no[0]++;
        }
    } else if (Target_BG_X[ewk->wu.direction] + Offset_BG_X[ewk->wu.direction] >=
               bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos) {
        Next_Step |= 1;
        bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos =
            Target_BG_X[ewk->wu.direction] + Offset_BG_X[ewk->wu.direction];
        ewk->wu.routine_no[0]++;
    }
}

static void move_background_58(WORK_Other* ewk) {
    bg_w.bgw[ewk->wu.direction].wxy[0].cal += bg_w.bgw[ewk->wu.direction].speed_x;
    move_background_to_target(ewk, bg_w.bgw[ewk->wu.direction].speed_x);
}

static void request_sound_58(WORK_Other* ewk) {
    if (Demo_Flag != 0) {
        SsRequest(ewk->wu.direction);
    }

    ewk->wu.routine_no[0]++;
}

static void set_next_step_58(WORK_Other* ewk) {
    Next_Step = 1;
    ewk->wu.routine_no[0]++;
}

static void request_bgm_58(WORK_Other* ewk) {
    if (Demo_Flag != 0 && PB_Music_Off == 0) {
        BGM_Request(ewk->wu.direction);
    }

    ewk->wu.routine_no[0]++;
}

static void fade_in_bgm_58(WORK_Other* ewk) {
    if (Demo_Flag != 0 && PB_Music_Off == 0) {
        SsBgmFadeIn(ewk->wu.direction, 0x222);
    }

    ewk->wu.routine_no[0]++;
}

static void show_logo_58(WORK_Other* ewk) {
    SF3_logo(SF33rd_Logo(ewk));
}

static void move_cut_background_3_58(WORK_Other* ewk) {
    s16 xx;

    if (Cut_Scroll == 0) {
        xx = 3;
    } else {
        xx = Cut_Cut_Sub(3);
    }

    bg_w.bgw[ewk->wu.direction].wxy[0].cal += bg_mvxy.a[0].sp * xx;
    bg_mvxy.a[0].sp += bg_mvxy.d[0].sp;
    move_background_to_target(ewk, bg_mvxy.a[0].sp);
}

static void move_cut_background_5_58(WORK_Other* ewk) {
    s16 xx;

    if (Cut_Scroll == 0) {
        xx = 5;
    } else {
        xx = Cut_Cut_Sub(5);
    }

    bg_w.bgw[ewk->wu.direction].wxy[0].cal += bg_mvxy.a[0].sp * xx;
    bg_mvxy.a[0].sp += bg_mvxy.d[0].sp;
    move_background_to_target(ewk, bg_mvxy.a[0].sp);
}

static void move_accelerating_background_58(WORK_Other* ewk) {
    bg_w.bgw[ewk->wu.direction].wxy[0].cal += bg_mvxy.a[0].sp;
    bg_mvxy.a[0].sp += bg_mvxy.d[0].sp;
    move_background_to_target(ewk, bg_mvxy.a[0].sp);
}

static void mark_suicide_58(WORK_Other* ewk) {
    Suicide[ewk->wu.direction] = 1;
    ewk->wu.routine_no[0]++;
}

static void offset_background_y_58(WORK_Other* ewk) {
    bg_w.bgw[ewk->wu.direction].wxy[1].disp.pos += 256;
    Setup_BG(ewk->wu.direction,
             bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos,
             bg_w.bgw[ewk->wu.direction].wxy[1].disp.pos);
    ewk->wu.routine_no[0]++;
}

static void offset_background_x_58(WORK_Other* ewk) {
    bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos += 512;
    Setup_BG(ewk->wu.direction,
             bg_w.bgw[ewk->wu.direction].wxy[0].disp.pos,
             bg_w.bgw[ewk->wu.direction].wxy[1].disp.pos);
    ewk->wu.routine_no[0]++;
}

static void setup_background_58(WORK_Other* ewk) {
    Setup_BG(2, 0x480, 0);
    ewk->wu.routine_no[0]++;
}

static void dispatch_effect_58_action(WORK_Other* ewk) {
    void (*const actions[])(WORK_Other*) = {
        [0] = move_background_58,
        [1] = EFF58_Type_01,
        [4] = Fade_In_58_Sub,
        [6] = request_sound_58,
        [7] = set_next_step_58,
        [8] = request_bgm_58,
        [9] = fade_in_bgm_58,
        [10] = show_logo_58,
        [12] = move_cut_background_3_58,
        [13] = move_cut_background_5_58,
        [14] = move_accelerating_background_58,
        [15] = mark_suicide_58,
        [16] = offset_background_y_58,
        [17] = offset_background_x_58,
        [18] = setup_background_58,
    };
    void (*action)(WORK_Other*);

    if (ewk->wu.routine_no[1] >= sizeof(actions) / sizeof(actions[0])) {
        return;
    }

    action = actions[ewk->wu.routine_no[1]];

    if (action != NULL) {
        action(ewk);
    }
}

void effect_58_move(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[0]) {
    case 0:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[0]++;
        }

        break;

    case 1:
        dispatch_effect_58_action(ewk);
        break;

    default:
        push_effect_work(&ewk->wu);
        break;
    }
}

s32 effect_58_init(s16 id, s16 time0, s16 option) {
    WORK_Other* ewk;
    s16 ix;

    if ((ix = pull_effect_work(4)) == -1) {
        return -1;
    }

    ewk = (WORK_Other*)frw[ix];
    ewk->wu.be_flag = 1;
    ewk->wu.id = 58;
    ewk->wu.work_id = 16;
    ewk->wu.dir_timer = time0;
    ewk->wu.dir_old = time0;
    ewk->wu.direction = option;
    ewk->wu.routine_no[1] = id;
    return 0;
}

static void initialize_logo_58(WORK_Other* ewk) {
    ewk->wu.routine_no[2]++;
    ewk->wu.dir_timer = 1;
    Disappear_LOGO = 0;
}

static s32 advance_logo_frame_58(WORK_Other* ewk) {
    if (--ewk->wu.dir_timer != 0) {
        return 0;
    }

    ewk->wu.dir_timer = 2;
    ewk->wu.direction++;
    return 2;
}

static void reveal_logo_58(WORK_Other* ewk) {
    if (!advance_logo_frame_58(ewk)) {
        return;
    }

    if (ewk->wu.direction >= 8) {
        ewk->wu.direction = 8;
        ewk->wu.routine_no[2]++;
    }
}

static void wait_to_hide_logo_58(WORK_Other* ewk) {
    if (Disappear_LOGO) {
        ewk->wu.routine_no[2]++;
        ewk->wu.dir_timer = 1;
    }
}

static void hide_logo_58(WORK_Other* ewk) {
    if (!advance_logo_frame_58(ewk)) {
        return;
    }

    if (ewk->wu.direction > 16) {
        ewk->wu.direction = 16;
        sort_push_request4(&ewk->wu);
    }
}

s32 SF33rd_Logo(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        initialize_logo_58(ewk);
        /* fallthrough */

    case 1:
        reveal_logo_58(ewk);
        break;

    case 2:
        wait_to_hide_logo_58(ewk);
        break;

    default:
        hide_logo_58(ewk);
        break;
    }

    return ewk->wu.direction;
}

void EFF58_Type_01(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        Switch_Screen(1);

        if (!--Cover_Timer) {
            ewk->wu.routine_no[2]++;
            Switch_Screen_Init(1);
        }

        break;

    case 1:
        if (Switch_Screen_Revival(1)) {
            ewk->wu.routine_no[0] = 99;
        }

        break;
    }
}

void Fade_In_58_Sub(WORK_Other* ewk) {
    switch (ewk->wu.routine_no[2]) {
    case 0:
        ewk->wu.routine_no[2]++;
        ewk->wu.dir_timer = 10;
        ewk->wu.dir_old = 0xFF;
        /* fallthrough */

    case 1:
        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.routine_no[2]++;
            ewk->wu.dir_timer = 4;
        }

        ToneDown(ewk->wu.dir_old, ewk->wu.direction);
        break;

    default:
        if (ewk->wu.dir_old < 0) {
            ewk->wu.dir_old = 0;
            ewk->wu.routine_no[0]++;
        }

        ToneDown(ewk->wu.dir_old, ewk->wu.direction);

        if (--ewk->wu.dir_timer == 0) {
            ewk->wu.dir_old -= 16;
            ewk->wu.dir_timer = 4;
        }

        break;
    }
}

/**
 * @file end_main.c
 * Ending Movie Controller
 */

#include "sf33rd/Source/Game/ending/end_main.h"
#include "common.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/effe9.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/ending/end_00.h"
#include "sf33rd/Source/Game/ending/end_01.h"
#include "sf33rd/Source/Game/ending/end_02.h"
#include "sf33rd/Source/Game/ending/end_03.h"
#include "sf33rd/Source/Game/ending/end_04.h"
#include "sf33rd/Source/Game/ending/end_05.h"
#include "sf33rd/Source/Game/ending/end_06.h"
#include "sf33rd/Source/Game/ending/end_07.h"
#include "sf33rd/Source/Game/ending/end_08.h"
#include "sf33rd/Source/Game/ending/end_09.h"
#include "sf33rd/Source/Game/ending/end_10.h"
#include "sf33rd/Source/Game/ending/end_11.h"
#include "sf33rd/Source/Game/ending/end_12.h"
#include "sf33rd/Source/Game/ending/end_13.h"
#include "sf33rd/Source/Game/ending/end_14.h"
#include "sf33rd/Source/Game/ending/end_16.h"
#include "sf33rd/Source/Game/ending/end_17.h"
#include "sf33rd/Source/Game/ending/end_18.h"
#include "sf33rd/Source/Game/ending/end_19.h"
#include "sf33rd/Source/Game/ending/end_20.h"
#include "sf33rd/Source/Game/ending/end_data.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/screen/n_input.h"
#include "sf33rd/Source/Game/screen/staff.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"

void normal_ending(s16 pl_num);
void end_main_move(s16 pl_num);
void end_reset_etc();
void end_fade_bgm();
s32 Cut_Cut_Cut_t();

void (*end_main_jp[20])(s16) = { end_00000, end_01000, end_02000, end_03000, end_04000, end_05000, end_06000,
                                 end_07000, end_08000, end_09000, end_10000, end_11000, end_12000, end_13000,
                                 end_14000, end_16000, end_17000, end_18000, end_19000, end_20000 };

void Ending_init() {
    end_w.r_no_0 = 0;
    end_w.r_no_1 = 0;
    end_w.r_no_2 = 0;
    end_w.type = 0;
    end_w.end_flag = 0;
    Game_timer = 0;
    ending_all_end = 0;
    end_fade_timer = 0;
    end_fade_flag = 0;
    end_no_cut = 0;
    staff_r_no = 0;
    end_staff_flag = 0;
}

s8 Ending_main(s16 pl_num) {
    Game_timer++;
    normal_ending(pl_num);
    return ending_all_end;
}

void normal_ending(s16 pl_num) {
    switch (end_w.r_no_0) {
    case 0:
        end_w.r_no_0++;
        Switch_Screen(1);
        Bg_Off_R(7);
        System_all_clear_Level_B();
        Cover_Timer = 29;
        end_main_jp[pl_num](pl_num);
        break;

    case 1:
        Switch_Screen(1);

        if (!(Cover_Timer -= 1)) {
            end_w.r_no_0++;
            Switch_Screen_Init(1);
            end_main_jp[pl_num](pl_num);
        }

        break;

    case 2:
        if (Switch_Screen_Revival(1)) {
            end_w.r_no_0++;
            Ignore_Entry[LOSER] = 0;
            Forbid_Break = -1;
        }

        break;

    case 3:
        end_main_move(pl_num);

        if (end_w.end_flag) {
            end_w.r_no_0++;
            end_no_cut = 1;
            effect_work_kill(4, 0x9F);
            SsBgmFadeOut(0x111);
        } else if (Cut_Cut_Cut_t()) {
            end_w.timer = 0;
            effect_work_kill(4, 0x9F);
            end_main_move(pl_num);
            end_fade_flag = 0;
        }

        Forbid_Break = -1;
        break;

    case 4:
        if (end_no_cut == 0) {
            fadeout_to_staff_roll();
        } else {
            end_w.r_no_0++;
        }

        Forbid_Break = -1;
        break;

    case 5:
        if (end_fade_complete()) {
            end_w.r_no_0++;
            end_w.r_no_2++;
            overwrite_panel(0xFF000000, 0x12);

            if (end_w.type == 4) {
                Zoomf_Init();
            }

            end_w.r_no_0++;
            end_no_cut = 1;
            bg_w.bgw[0].xy[0].disp.pos = 256;
            bg_w.bgw[0].abs_x = 512;
            bg_w.bgw[0].xy[1].disp.pos = 0;
            bg_w.bgw[0].abs_y = 0;
            end_scn_pos_set2();
            end_bg_pos_hosei2();
            end_fam_set2();
            Bg_Off_W(0xF);
        }

        Forbid_Break = -1;
        break;

    case 6:
        overwrite_panel(0xFF000000, 0x12);
        end_w.r_no_0++;
        break;

    case 7:
        end_w.r_no_0++;
        overwrite_panel(0xFF000000, 0x12);
        Request_Fade(6);
        end_no_cut = 1;
        Forbid_Break = -1;
        break;

    case 8:
        if (end_fade_complete()) {
            end_w.r_no_0++;
            end_no_cut = 0;
            Bg_Close();
        }

        Forbid_Break = -1;
        break;

    case 9:
        scr_calc(5);

        if (staff_credits(end_staff_flag)) {
            end_w.r_no_0++;
        }

        break;

    case 10:
        end_w.r_no_0++;

        if (name_wk[WINNER].timer >= 1) {
            name_wk[WINNER].timer = 0;
            end_name_cut[WINNER] = 1;
            end_w.timer = 180;

            if (bgm_play_status() == 2) {
                SsBgmFadeOut(0xB6);
            }

            break;
        }

        end_w.timer = 60;

        if (bgm_play_status() == 2) {
            SsBgmFadeOut(0x222);
        }

        break;

    case 11:
        end_w.timer--;

        if (end_w.timer < 0) {
            end_w.r_no_0++;
            ending_all_end = 1;
        }

        break;

    case 12:
        ending_all_end = 1;
        break;
    }
}

void end_main_move(s16 pl_num) {
    end_main_jp[pl_num](pl_num);
    end_fade_bgm();
}

void fadeout_to_staff_roll() {
    Request_Fade(7);
    end_no_cut = 1;
    Forbid_Break = -1;
}

void common_end_init00(s16 pl_num) {
    s16 i;

    Family_Init();
    Scrn_Pos_Init();
    Zoomf_Init();
    bg_w.bg_opaque = ending_opaque[pl_num];
    Screen_Switch = 0;
    Screen_Switch_Buffer = 0;
    bg_disp_off = 0;
    end_w.type = pl_num;
    bg_w.scno = end_use_scr[end_w.type];
    bg_w.scrno = end_use_real_scr[end_w.type];
    Bg_Texture_Load_Ending(end_w.type);
    ending_obj_load();
    bg_w.pos_offset = 192;
    base_y_pos = 40;

    for (i = 0; i < 7; i++) {
        bg_w.bgw[i].r_no_0 = 0;
        bg_w.bgw[i].r_no_1 = 0;
        bg_w.bgw[i].r_no_2 = 0;
        bg_w.bgw[i].pos_x_work = bg_w.bgw[i].pos_y_work = 0;
        bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].xy[0].cal = 0;
        bg_w.bgw[i].xy[1].cal = 0;
        bg_w.bgw[i].wxy[0].cal = 0;
        bg_w.bgw[i].wxy[1].cal = 0;
        bg_w.bgw[i].hos_xy[0].cal = 0;
        bg_w.bgw[i].hos_xy[1].cal = 0;
        bg_w.bgw[i].rewrite_flag = 0;
    }

    for (i = 0; i < bg_w.scno; i++) {
        bg_w.bgw[i].r_no_1 = bg_w.bgw[i].r_no_2 = 0;
        bg_w.bgw[i].fam_no = i;
    }
}

void common_end_init01() {
    bg_w.scr_stop = 0;
    bg_w.frame_flag = 0;
    bg_w.bg_f_x = 9;
    bg_w.bg_f_y = 9;
    bg_w.bg2_sp_x2 = bg_w.bg2_sp_x = 0;
    bg_w.max_x = 6;
    bg_w.old_chase_flag = bg_w.chase_flag = 0;
    bg_w.quake_x_index = 0;
    bg_w.quake_y_index = 0;
    end_etc_flag = 0;
    end_reset_etc();
    bg_w.bgw[3].wxy[0].cal = bg_w.bgw[3].xy[0].cal = 0x2000000;
    bg_w.bgw[3].xy[1].cal = 0;
    bg_w.bgw[3].position_x = 512 - bg_w.pos_offset;
    bg_w.bgw[3].position_y = 0;
    end_fam_set(3);
    Scrn_Move_Set(3, bg_w.bgw[3].position_x, bg_w.bgw[3].position_y);
    effect_E9_init();
    effect_F9_init(end_w.type);
}

void end_fam_set(s16 i) {
    s16 pos_work_x = bg_w.bgw[i].position_x;
    s16 pos_work_y = bg_w.bgw[i].position_y;

    pos_work_x = pos_work_x & 0xFFFF;
    pos_work_y = (pos_work_y + 16) & 0xFFFF;

    Family_Set_W(i + 1, pos_work_x, pos_work_y);
}

void end_fam_set2() {
    s16 i;
    s16 pos_work_x;
    s16 pos_work_y;

    for (i = 0; i < bg_w.scno; i++) {
        pos_work_x = bg_w.bgw[i].position_x;
        pos_work_y = bg_w.bgw[i].position_y;
        pos_work_x = pos_work_x & 0xFFFF;
        pos_work_y = (pos_work_y + 16) & 0xFFFF;
        Family_Set_W(i + 1, pos_work_x, pos_work_y);
    }
}

void end_bg_pos_hosei(s16 bg_no) {
    s16 pos_work = bg_w.bgw[bg_no].abs_x & 0xFFFF;
    pos_work -= bg_w.pos_offset;
    bg_w.bgw[bg_no].position_x = pos_work & 0xFFFF;
    pos_work = bg_w.bgw[bg_no].abs_y & 0xFFFF;
    bg_w.bgw[bg_no].position_y = pos_work;
}

void end_bg_pos_hosei2() {
    s16 bg_no;
    u16 pos_work;

    for (bg_no = 0; bg_no < bg_w.scno; bg_no++) {
        pos_work = bg_w.bgw[bg_no].abs_x & 0xFFFF;
        pos_work -= bg_w.pos_offset;
        bg_w.bgw[bg_no].position_x = pos_work & 0xFFFF;
        pos_work = bg_w.bgw[bg_no].abs_y & 0xFFFF;
        bg_w.bgw[bg_no].position_y = pos_work;
    }
}

void end_scn_pos_set2() {
    s16 bg_no;

    for (bg_no = 0; bg_no < bg_w.scno; bg_no++) {
        Scrn_Move_Set(bg_w.bgw[bg_no].fam_no,
                      (bg_w.bgw[bg_no].xy[0].disp.pos & 0xFFFF) - bg_w.pos_offset,
                      bg_w.bgw[bg_no].xy[1].disp.pos);
        bg_w.bgw[bg_no].wxy[0].cal = bg_w.bgw[bg_no].xy[0].cal;
        bg_w.bgw[bg_no].wxy[1].cal = bg_w.bgw[bg_no].xy[1].cal;
    }
}

void end_reset_etc() {
    s16 i;

    for (i = 0; i < bg_w.scno; i++) {
        bg_w.bgw[i].r_no_1 = 0;
        bg_w.bgw[i].abs_x = bg_w.bgw[i].xy[0].disp.pos = 512;
        bg_w.bgw[i].abs_y = bg_w.bgw[i].xy[1].disp.pos = 0;
    }
}

void end_X_com01() {
    switch (bgw_ptr->r_no_1) {
    case 0:
        bgw_ptr->r_no_1++;
        Bg_Off_W(1 << bgw_ptr->fam_no);
        break;

    case 1:
        break;
    }
}

void end_fade_bgm() {
    if (end_fade_flag != 0) {
        end_fade_timer -= 1;

        if (end_fade_timer < 0) {
            end_fade_flag = 0;
        }
    }
}

s16 end_fade_complete() {
    if (Check_Fade_Complete()) {
        Forbid_Break = -1;
        return 1;
    }

    return 0;
}

s32 Cut_Cut_Cut_t() {
    u16 sw_w;

    if (end_no_cut == 0) {
        if (WINNER) {
            sw_w = p2sw_0 & ~p2sw_1;
        } else {
            sw_w = p1sw_0 & ~p1sw_1;
        }

        if (sw_w & 0xFF0) {
            return 1;
        }
    }

    return 0;
}

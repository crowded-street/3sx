/**
 * @file menu.c
 * Menus
 */

#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "core/app.h"
#include "main.h"
#include "platform/app/sdl/sdl_app.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/animation/appear.h"
#include "sf33rd/Source/Game/effect/eff04.h"
#include "sf33rd/Source/Game/effect/eff10.h"
#include "sf33rd/Source/Game/effect/eff18.h"
#include "sf33rd/Source/Game/effect/eff23.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff40.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff45.h"
#include "sf33rd/Source/Game/effect/eff51.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/eff63.h"
#include "sf33rd/Source/Game/effect/eff64.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff91.h"
#include "sf33rd/Source/Game/effect/effa0.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effa8.h"
#include "sf33rd/Source/Game/effect/effc4.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/io/vm_sub.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/menu/ex_data.h"
#include "sf33rd/Source/Game/message/en/msgtable_en.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/screen/entry.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/pause.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/reset.h"
#include "sf33rd/Source/Game/system/saver.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/count.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/PS2/mc/savesub.h"
#include "structs.h"

#if NETPLAY_ENABLED
#include "platform/netplay/fistbump.h"
#include "platform/netplay/netplay.h"
#include "port/sdl/netplay_screen.h"
#include "sf33rd/Source/Game/menu/netplay_menu.h"
#endif

void Default_Training_Option();
void Dummy_Move_Sub(struct _TASK* task_ptr, s16 PL_id, s16 id, s16 type, s16 max);
void Return_Pause_Sub(struct _TASK* task_ptr);
void Dummy_Move_Sub_LR(u16 sw, s16 id, s16 type, s16 cursor_id);
void Return_VS_Result_Sub(struct _TASK* task_ptr);
void Exit_Replay_Save(struct _TASK* task_ptr);
void Setup_NTr_Data(s16 ix);
s32 Check_Pad_in_Pause(struct _TASK* task_ptr);
void Next_Be_Tr_Menu(struct _TASK* task_ptr);
void Yes_No_Cursor_Exit_Training(struct _TASK* task_ptr, s16 cursor_id);
void Check_Skip_Recording();
void Check_Skip_Replay(s16 ix);
void Setup_Tr_Pause(struct _TASK* task_ptr);
void Control_Player_Tr();
s32 Pause_Check_Tr(s16 PL_id);
void Setup_Win_Lose_OBJ();
s32 Pause_in_Normal_Tr(struct _TASK* task_ptr);
void Training_Disp_Sub(struct _TASK* task_ptr);

// forward decls
void After_Title(struct _TASK* task_ptr);
void In_Game(struct _TASK* task_ptr);
void Wait_Load_Save(struct _TASK* task_ptr);
void Wait_Replay_Check(struct _TASK* task_ptr);
void Suspend_Menu();
void Wait_Replay_Load();
void Training_Menu(struct _TASK* task_ptr);
void After_Replay(struct _TASK* task_ptr);
void Wait_Pause_in_Tr(struct _TASK* task_ptr);
void Reset_Training(struct _TASK* task_ptr);
void Reset_Replay(struct _TASK* task_ptr);
void End_Replay_Menu(struct _TASK* task_ptr);
void Mode_Select(struct _TASK* task_ptr);
void Option_Select(struct _TASK* task_ptr);
void Training_Mode(struct _TASK* task_ptr);
void System_Direction(struct _TASK* task_ptr);
void Load_Replay(struct _TASK* task_ptr);
void toSelectGame(struct _TASK* task_ptr);
void Game_Option(struct _TASK* task_ptr);
void Button_Config(struct _TASK* task_ptr);
void Screen_Adjust(struct _TASK* task_ptr);
void Sound_Test(struct _TASK* task_ptr);
void Extra_Option(struct _TASK* task_ptr);
void VS_Result(struct _TASK* task_ptr);
void Save_Replay(struct _TASK* task_ptr);
void Direction_Menu(struct _TASK* task_ptr);
void Setup_VS_Mode(struct _TASK* task_ptr);
void Setup_Next_Page(struct _TASK* task_ptr, u8 /* unused */);
void Load_Replay_Sub(struct _TASK* task_ptr);
void Button_Exit_Check(struct _TASK* task_ptr, s16 PL_id);
void Back_to_Mode_Select(struct _TASK* task_ptr);
void Flash_1P_or_2P(struct _TASK* task_ptr);

void bg_etc_write_ex(s16 type);
void Decide_PL(s16 PL_id);
void imgSelectGameButton();
void jmpRebootProgram();
s32 Check_Pause_Term_Tr(s16 PL_id);

void Menu_in_Sub(struct _TASK* task_ptr);
s32 Exit_Sub(struct _TASK* task_ptr, s16 cursor_ix, s16 next_routine);
u16 MC_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max, s16 cansel_menu);
s32 Menu_Sub_case1(struct _TASK* task_ptr);
void System_Dir_Move_Sub(s16 PL_id);
void System_Dir_Move_Sub_LR(u16 sw, s16 cursor_id);
u16 Dir_Move_Sub2(u16 sw);
void Dir_Move_Sub_LR(u16 sw, s16 /* unused */);
void Ex_Move_Sub_LR(u16 sw, s16 PL_id);
u16 Game_Option_Sub(s16 PL_id);
u16 GO_Move_Sub_LR(u16 sw, s16 cursor_id);
void Button_Config_Sub(s16 PL_id);
void Button_Move_Sub_LR(u16 sw, s16 cursor_id);
void Return_Option_Mode_Sub(struct _TASK* task_ptr);
void Screen_Adjust_Sub(s16 PL_id);
void Screen_Exit_Check(struct _TASK* task_ptr, s16 PL_id);
void Screen_Move_Sub_LR(u16 sw);
u16 Sound_Cursor_Sub(s16 PL_id);
u16 SD_Move_Sub_LR(u16 sw);
u16 After_VS_Move_Sub(u16 sw, s16 cursor_id, s16 menu_max);
s32 VS_Result_Move_Sub(struct _TASK* task_ptr, s16 PL_id);
void Training_Init(struct _TASK* task_ptr);
void Menu_Select(struct _TASK* task_ptr);
void Button_Config_in_Game(struct _TASK* task_ptr);
void Character_Change(struct _TASK* task_ptr);
void Pad_Come_Out(struct _TASK* task_ptr);
void Normal_Training(struct _TASK* task_ptr);
void Blocking_Training(struct _TASK* task_ptr);
void Dummy_Setting(struct _TASK* task_ptr);
void Training_Option(struct _TASK* task_ptr);
void Button_Config_Tr(struct _TASK* task_ptr);
void Blocking_Tr_Option(struct _TASK* task_ptr);
void Training_Init_Sub(struct _TASK* task_ptr);
void Training_Exit_Sub(struct _TASK* task_ptr);
void Menu_Init(struct _TASK* task_ptr);
s32 Check_Pad_in_Pause(struct _TASK* task_ptr);
s32 Yes_No_Cursor_Move_Sub(struct _TASK* task_ptr);
void Setup_Button_Sub(s16 x, s16 y, s16 master_player);
void Button_Exit_Check_in_Game(struct _TASK* task_ptr, s16 PL_id);
void Setup_Save_Replay_1st(struct _TASK* task_ptr);
s32 Save_Replay_MC_Sub(struct _TASK* task_ptr, s16 /* unused */);
void Button_Exit_Check_in_Tr(struct _TASK* task_ptr, s16 PL_id);
s32 VS_Result_Select_Sub(struct _TASK* task_ptr, s16 PL_id);
void Setup_Replay_Sub(s16 type, MenuHeader char_type, s16 master_player);

void Load_Replay(struct _TASK* task_ptr) {
    Menu_Cursor_X[1] = Menu_Cursor_X[0];
    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        Menu_in_Sub(task_ptr);
        Menu_Cursor_X[0] = 0;
        Setup_BG(1, 0x200, 0);
        Setup_Replay_Sub(0x6E, MENU_HEADER_REPLAY, 1);
        Clear_Flash_Init(4);
        Message_Data->kind_req = 5;
        break;

    case 1:
        if (Menu_Sub_case1(task_ptr) != 0) {
            SaveInit(SAVE_FILE_REPLAY, SAVE_MODE_LOAD);
        }

        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            task_ptr->free[3] = 0;
            Menu_Cursor_X[0] = Setup_Final_Cursor_Pos(0, 8);
        }

        break;

    case 3:
        switch (SaveMove()) {
        case 0:
            Decide_ID = 0;

            if (Interface_Type[0] == 0) {
                Decide_ID = 1;
            }

            task_ptr->r_no[2] += 1;
            task_ptr->r_no[3] = 0;
            break;

        case -1:
            IO_Result = 0x200;
            Load_Replay_MC_Sub(task_ptr, 0);
            break;
        }

        break;

    case 4:
        Load_Replay_Sub(task_ptr);
        break;
    }
}

static bool is_replay_load_complete(void) {
    return Check_PL_Load() && Check_LDREQ_Queue_BG(bg_w.stage) && (adx_now_playend() != 0);
}

void Load_Replay_Sub(struct _TASK* task_ptr) {
    s32 ix;

    switch (task_ptr->r_no[3]) {
    case 0:
        task_ptr->r_no[3] += 1;
        Rep_Game_Infor[0xA] = Replay_w.game_infor;
        cpExitTask(TASK_ENTRY);
        Play_Mode = 3;
        break;

    case 1:
        task_ptr->r_no[3] += 1;
        FadeInit();
        FadeOut(0, 0xFF, 8);
        Play_Type = 1;
        Mode_Type = MODE_REPLAY;
        Present_Mode = 3;
        Bonus_Game_Flag = 0;

        for (ix = 0; ix < 2; ix++) {
            plw[ix].wu.operator = Replay_w.game_infor.player_infor[ix].player_type;
            Operator_Status[ix] = Replay_w.game_infor.player_infor[ix].player_type;
            My_char[ix] = Replay_w.game_infor.player_infor[ix].my_char;
            Super_Arts[ix] = Replay_w.game_infor.player_infor[ix].sa;
            Player_Color[ix] = Replay_w.game_infor.player_infor[ix].color;
            Vital_Handicap[3][ix] = Replay_w.game_infor.Vital_Handicap[ix];
        }

        Direction_Working[3] = Replay_w.game_infor.Direction_Working;
        bg_w.stage = Replay_w.game_infor.stage;
        bg_w.area = 0;
        save_w[3].Time_Limit = Replay_w.mini_save_w.Time_Limit;
        save_w[3].Battle_Number[0] = Replay_w.mini_save_w.Battle_Number[0];
        save_w[3].Battle_Number[1] = Replay_w.mini_save_w.Battle_Number[1];
        save_w[3].Damage_Level = Replay_w.mini_save_w.Damage_Level;
        save_w[3].extra_option = Replay_w.mini_save_w.extra_option;
        system_dir[3] = Replay_w.system_dir;
        save_w[3].extra_option = Replay_w.mini_save_w.extra_option;
        save_w[3].Pad_Infor[0] = Replay_w.mini_save_w.Pad_Infor[0];
        save_w[3].Pad_Infor[1] = Replay_w.mini_save_w.Pad_Infor[1];
        save_w[3].Pad_Infor[0].Vibration = 0;
        save_w[3].Pad_Infor[1].Vibration = 0;
        cpExitTask(TASK_SAVER);
        break;

    case 2:
        FadeOut(0, 0xFF, 8);
        task_ptr->r_no[3] += 1;
        task_ptr->timer = 0xA;
        System_all_clear_Level_B();
        pulpul_stop();
        init_pulpul_work();
        bg_etc_write(2);
        bg_w.bgw[0].wxy[0].disp.pos += 0x200;
        Setup_BG(0, bg_w.bgw[0].wxy[0].disp.pos, bg_w.bgw[0].wxy[1].disp.pos);
        effect_38_init(0, 0xB, My_char[0], 1, 0);
        Order[0xB] = 3;
        Order_Timer[0xB] = 1;
        effect_38_init(1, 0xC, My_char[1], 1, 0);
        Order[0xC] = 3;
        Order_Timer[0xC] = 1;
        effect_K6_init(0, 0x23, 0x23, 0);
        Order[0x23] = 3;
        Order_Timer[0x23] = 1;
        effect_K6_init(1, 0x24, 0x23, 0);
        Order[0x24] = 3;
        Order_Timer[0x24] = 1;
        effect_39_init(0, 0x11, My_char[0], 0, 0);
        Order[0x11] = 3;
        Order_Timer[0x11] = 1;
        effect_39_init(1, 0x12, My_char[1], 0, 0);
        Order[0x12] = 3;
        Order_Timer[0x12] = 1;
        effect_K6_init(0, 0x1D, 0x1D, 0);
        Order[0x1D] = 3;
        Order_Timer[0x1D] = 1;
        effect_K6_init(1, 0x1E, 0x1D, 0);
        Order[0x1E] = 3;
        Order_Timer[0x1E] = 1;
        effect_43_init(2, 0);
        effect_75_init(0x2A, 3, 0);
        Order[0x2A] = 3;
        Order_Timer[0x2A] = 1;
        Order_Dir[0x2A] = 5;
        break;

    case 3:
        FadeOut(0, 0xFF, 8);

        if (--task_ptr->timer <= 0) {
            task_ptr->r_no[3] += 1;
            bgPalCodeOffset[0] = 0x90;
            BGM_Request(51);
            Purge_memory_of_kind_of_key(0xC);
            Push_LDREQ_Queue_Player(0, My_char[0]);
            Push_LDREQ_Queue_Player(1, My_char[1]);
            Push_LDREQ_Queue_BG(bg_w.stage);
        }

        break;

    case 4:
        if (FadeIn(0, 4, 8) != 0) {
            task_ptr->r_no[3] += 1;
        }

        break;

    case 5:
        if (is_replay_load_complete()) {
            task_ptr->r_no[3] += 1;
            Switch_Screen_Init(0);
            init_omop();
        }

        break;

    case 6:
        if (Switch_Screen(0) != 0) {
            Game01_Sub();
            Cover_Timer = 5;
            appear_type = APPEAR_TYPE_ANIMATED;
            set_hitmark_color();
            Purge_texcash_of_list(3);
            Make_texcash_of_list(3);
            G_No[1] = 2;
            G_No[2] = 0;
            G_No[3] = 0;
            E_No[0] = 4;
            E_No[1] = 0;
            E_No[2] = 0;
            E_No[3] = 0;

            if (plw->wu.operator != 0) {
                Sel_Arts_Complete[0] = -1;
            }

            if (plw[1].wu.operator != 0) {
                Sel_Arts_Complete[1] = -1;
            }

            task_ptr->r_no[2] = 0;
            cpExitTask(TASK_MENU);
        }

        break;

    default:
        break;
    }
}

s32 Load_Replay_MC_Sub(struct _TASK* task_ptr, s16 PL_id) {
    u16 sw = IO_Result;

    switch (sw) {
    case 0x100:
        if ((Menu_Cursor_X[0] == -1) || (vm_w.Connect[Menu_Cursor_X[0]] == 0)) {
            break;
        }

        Pause_ID = PL_id;
        vm_w.Drive = (u8)Menu_Cursor_X[0];

        if (VM_Access_Request(6, Menu_Cursor_X[0]) == 0) {
            break;
        }

        SE_selected();
        task_ptr->free[1] = 0;
        task_ptr->free[2] = 0;
        task_ptr->r_no[0] = 3;
        return 1;

    case 0x200:
        if (task_ptr->r_no[1] == 6) {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            task_ptr->r_no[1] = 1;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[0x6E] = 4;
            Order_Timer[0x6E] = 4;
        } else {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 0;
            Menu_Suicide[2] = 1;
            task_ptr->r_no[1] = 5;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[0x70] = 4;
            Order_Timer[0x70] = 4;
        }

        break;
    }

    return 0;
}


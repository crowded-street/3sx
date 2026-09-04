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
static void apply_training_hitbox_display(bool force_off);
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

typedef struct {
    s16 pos_x;
    s8* menu;
} LetterData;

extern u8 control_player;
extern u8 control_pl_rno;
extern const LetterData training_letter_data[];

static bool is_training_hitbox_display_enabled() {
    return Training[0].contents[0][1][TRAINING_OPTION_HITBOXES] != 0;
}

static bool should_disable_training_hitbox_display(bool force_off) {
    return force_off || Mode_Type != MODE_NORMAL_TRAINING || !is_training_hitbox_display_enabled();
}

static void apply_training_hitbox_display(bool force_off) {
    if (should_disable_training_hitbox_display(force_off)) {
        Set_Training_Hitbox_Display(false);
    } else {
        Set_Training_Hitbox_Display(true);
    }
}

static void handle_training_pause_state(struct _TASK* task_ptr) {
    u16 ix;
    if (Interface_Type[Pause_ID] == 0) {
        Setup_Tr_Pause(task_ptr);
        task_ptr->r_no[1] = 3;
        return;
    }

    if (Pause_Down) {
        Flash_1P_or_2P(task_ptr);
    }

    switch (Pause_in_Normal_Tr(task_ptr)) {
    case 1:
        task_ptr->r_no[1] = 0;
        SE_selected();
        Game_pause = 0;
        Pause = 0;
        Pause_Down = 0;
        Disp_Attack_Data = Training->contents[0][1][TRAINING_OPTION_ATTACK_DATA];

        for (ix = 0; ix < 4; ix++) {
            Menu_Suicide[ix] = 1;
        }

        pulpul_request_again();
        SsBgmHalfVolume(0);
        break;

    case 2:
        Next_Be_Tr_Menu(task_ptr);
        break;
    }
}

void Wait_Pause_in_Tr(struct _TASK* task_ptr) {
    u16 ans;
    u16 ix;

    Training_Data_Disp();
    Control_Player_Tr();

    if (End_Training) {
        Next_Be_Tr_Menu(task_ptr);
        return;
    }

    switch (task_ptr->r_no[1]) {
    case 0:
        if (Allow_a_battle_f) {
            task_ptr->r_no[1]++;

            if (Present_Mode == 4) {
                Disp_Attack_Data = Training->contents[0][1][TRAINING_OPTION_ATTACK_DATA];
            } else {
                Disp_Attack_Data = 0;
            }
        } else {
            Disp_Attack_Data = 0;
        }

        /* fallthrough */

    case 1:
        if (Allow_a_battle_f == 0 || Extra_Break != 0) {
            return;
        }

        ans = 0;

        if (Check_Pause_Term_Tr(0)) {
            ans = Pause_Check_Tr(0);
        }

        if (ans == 0 && Check_Pause_Term_Tr(1)) {
            ans = Pause_Check_Tr(1);
        }

        switch (ans) {
        case 1:
            Setup_Tr_Pause(task_ptr);
            break;

        case 2:
            Setup_Tr_Pause(task_ptr);
            task_ptr->r_no[1] = 3;
            break;
        }

        break;

    case 2:
        handle_training_pause_state(task_ptr);
        break;

    case 3:
        if (Interface_Type[Pause_ID] == 0) {
            dispControllerWasRemovedMessage(132, 82, 16);
            break;
        }

        Setup_Tr_Pause(task_ptr);
        break;
    }
}

void Control_Player_Tr() {
    switch (control_pl_rno) {
    case 0:
        if (control_player) {
            p2sw_0 = 0;
        } else {
            p1sw_0 = 0;
        }

        break;

    case 1:
        if (control_player) {
            p2sw_0 = SWK_DOWN;
        } else {
            p1sw_0 = SWK_DOWN;
        }

        break;

    case 2:
        if (control_player) {
            p2sw_0 = SWK_UP;
        } else {
            p1sw_0 = SWK_UP;
        }

        break;
    }
}

void Next_Be_Tr_Menu(struct _TASK* task_ptr) {
    s16 ix;

    apply_training_hitbox_display(true);
    task_ptr->r_no[0] = 11;
    task_ptr->r_no[1] = 0;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    Allow_a_battle_f = 0;

    for (ix = 0; ix < 4; ix++) {
        Menu_Suicide[ix] = 1;
    }

    SsBgmHalfVolume(0);
}

s32 Check_Pause_Term_Tr(s16 PL_id) {
    if (Mode_Type == MODE_PARRY_TRAINING) {
        if (PL_id == Champion) {
            return 1;
        }

        return 0;
    }

    if (PL_id == Champion) {
        return 1;
    }

    if (Training->contents[0][1][TRAINING_OPTION_DIFFICULTY] == 2) {
        return 0;
    }

    if (Training->contents[0][0][0] == 4) {
        return 1;
    }

    return 0;
}

s32 Pause_Check_Tr(s16 PL_id) {
    u16 sw;

    if (plw[PL_id].wu.operator == 0) {
        return 0;
    }

    sw = ~(PLsw[PL_id][1]) & PLsw[PL_id][0];

    if (sw & SWK_START) {
        Pause_ID = PL_id;
        return 1;
    }

    if (Interface_Type[PL_id] == 0) {
        Pause_ID = PL_id;
        return 2;
    }

    return 0;
}

void Setup_Tr_Pause(struct _TASK* task_ptr) {
    task_ptr->r_no[1] = 2;
    task_ptr->r_no[2] = 1;
    task_ptr->r_no[3] = 0;
    task_ptr->free[0] = 60;
    Cursor_Y_Pos[0][0] = 0;
    Disp_Attack_Data = 0;
    Game_pause = 0x81;
    Pause_Down = 1;
    Menu_Suicide[0] = 1;
    Menu_Suicide[1] = 1;
    Menu_Suicide[2] = 0;
    Order[138] = 3;
    Order_Timer[138] = 1;
    effect_66_init(138, 9, 2, 7, -1, -1, -0x3FFC);
    SsBgmHalfVolume(1);
    spu_all_off();
}

void Flash_1P_or_2P(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[3]) {
    case 0:
        if (--task_ptr->free[0]) {
            if (Pause_ID == 0) {
                SSPutStr2(20, 9, 9, "1P PAUSE");
                break;
            } else {
                SSPutStr2(20, 9, 9, "2P PAUSE");
                break;
            }
        }

        task_ptr->r_no[3] = 1;
        task_ptr->free[0] = 0x1E;
        break;

    case 1:
        if (--task_ptr->free[0] == 0) {
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0x3C;
        }

        break;
    }
}

static s32 handle_normal_training_pause_input(struct _TASK* task_ptr) {
    u16 sw;

    if (Pause_Down) {
        IO_Result = MC_Move_Sub(Check_Menu_Lever(Pause_ID, 0), 0, 2, 0xFF);
    } else {
        sw = ~PLsw[Pause_ID][1] & PLsw[Pause_ID][0];

        if (sw & SWK_ATTACKS) {
            IO_Result = SWK_WEST;
        } else {
            return 3;
        }
    }

    switch (IO_Result) {
    case SWK_START:
    case SWK_EAST:
        task_ptr->r_no[2] = 0x63;
        Exit_Menu = 1;
        Menu_Suicide[0] = 1;
        return 1;

    case SWK_SOUTH:
        switch (Menu_Cursor_Y[0]) {
        case 0: // CONTINUE
            task_ptr->r_no[2] = 0x63;
            Exit_Menu = 1;
            Menu_Suicide[0] = 1;
            return 1;

        case 1: // TRAINING MENU
            Cursor_Y_Pos[0][0] = 0;
            return 2;

        case 2: // EXIT
            task_ptr->r_no[2]++;
            SE_selected();
            Menu_Suicide[0] = 1;
            Menu_Cursor_Y[0] = 1;
            effect_10_init(0, 0, 3, 6, 1, 17, 12);
            effect_10_init(0, 1, 0, 0, 1, 20, 15);
            effect_10_init(0, 1, 1, 1, 1, 26, 15);
            break;
        }

        break;
    }

    return 0;
}

s32 Pause_in_Normal_Tr(struct _TASK* task_ptr) {
    s16 ix;
    u16 sw;

    Control_Player_Tr();

    switch (task_ptr->r_no[2]) {
    case 0:
    case 1:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = Cursor_Y_Pos[0][0];

        for (ix = 0; ix < 4; ix++) {
            Menu_Suicide[ix] = 0;
        }

        effect_10_init(0, 6, 0, 0, 0, 20, 12);
        effect_10_init(0, 6, 1, 1, 0, 18, 14);
        effect_10_init(0, 6, 2, 2, 0, 22, 16);
        break;

    case 2:
        return handle_normal_training_pause_input(task_ptr);

    case 3:
        sw = ~plsw_01[Pause_ID] & plsw_00[Pause_ID];
        (void)sw;

        if (Pause_Down) {
            Yes_No_Cursor_Move_Sub(task_ptr);
        }

        break;
    }

    return 0;
}

void Reset_Training(struct _TASK* task_ptr) {
    s16 ix;

    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1]++;
        task_ptr->timer = 10;
        Game_pause = 0x81;
        break;

    case 1:
        if (--task_ptr->timer != 0) {
            break;
        }

        if (!Check_LDREQ_Break()) {
            task_ptr->r_no[1]++;
            Switch_Screen_Init(0);
            break;
        }

        task_ptr->timer = 1;
        break;

    case 2:
        if (!Switch_Screen(0)) {
            break;
        }

        task_ptr->r_no[1]++;
        task_ptr->timer = 2;
        effect_work_kill(6, -1);
        move_effect_work(6);

        for (ix = 0; ix < 4; ix++) {
            C_No[ix] = 0;
        }

        C_No[0] = 1;
        G_No[2] = 5;
        G_No[3] = 0;
        seraph_flag = 0;
        BGM_No[0] = 1;
        BGM_Timer[0] = 1;
        G_Timer = 10;
        Cover_Timer = 5;
        Suicide[0] = 1;
        Suicide[6] = 1;
        judge_flag = 0;
        Lever_LR[0] = 0;
        Lever_LR[1] = 0;
        break;

    default:
        Switch_Screen(0);

        if (--task_ptr->timer != 0) {
            break;
        }

        for (ix = 0; ix < 4; ix++) {
            task_ptr->r_no[ix] = 0;
        }

        task_ptr->r_no[0] = 7;
        break;
    }
}

void Reset_Replay(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[1]) {
    case 0:
        task_ptr->r_no[1]++;
        task_ptr->timer = 10;
        Game_pause = 0x81;
        break;

    case 1:
        if (--task_ptr->timer != 0) {
            break;
        }

        if (!Check_LDREQ_Break()) {
            task_ptr->r_no[1]++;
            Switch_Screen_Init(0);
            break;
        }

        task_ptr->timer = 1;
        break;

    case 2:
        if (!Switch_Screen(0)) {
            break;
        }

        task_ptr->r_no[1]++;
        task_ptr->timer = 2;
        G_No[2] = 2;
        G_No[3] = 0;
        seraph_flag = 0;
        G_Timer = 10;
        Cover_Timer = 5;
        effect_work_kill_mod_plcol();
        move_effect_work(6);
        Suicide[0] = 1;
        Suicide[6] = 1;
        judge_flag = 0;
        cpExitTask(TASK_PAUSE);
        break;

    default:
        Switch_Screen(0);

        if (--task_ptr->timer == 0) {
            cpExitTask(TASK_MENU);
        }

        break;
    }
}

void Training_Menu(struct _TASK* task_ptr) {
    void (*Training_Jmp_Tbl[8])() = { Training_Init,   Normal_Training,  Blocking_Training, Dummy_Setting,
                                      Training_Option, Button_Config_Tr, Character_Change,  Blocking_Tr_Option };
    Training_Jmp_Tbl[task_ptr->r_no[1]](task_ptr);
    Akaobi();
    ToneDown(0xAA, 2);
    SSPutStr_Bigger(
        training_letter_data[Training_Index].pos_x, 0x18, 9, training_letter_data[Training_Index].menu, 1, 2, 1
    );
}

void Training_Init(struct _TASK* task_ptr) {
    ToneDown(0x80, 2);
    Menu_Init(task_ptr);
    task_ptr->r_no[1] = Mode_Type - 2;
    Pause_Down = 1;
    End_Training = 0;
    Demo_Time_Stop = 0;
    Disp_Cockpit = 0;

    if (Mode_Type == MODE_NORMAL_TRAINING) {
        control_player = Champion;
        control_pl_rno = 0x63;
    } else {
        control_player = Champion;
        control_pl_rno = 0;
    }

    Round_num = 0;
    PL_Wins[0] = 0;
    PL_Wins[1] = 0;
    Play_Mode = 0;
    Replay_Status[0] = 0;
    Replay_Status[1] = 0;
}

void start_normal_training_mode(struct _TASK* task_ptr) {
    if (Interface_Type[Champion ^ 1] == 0 && Training[2].contents[0][0][0] == 4) {
        Training[2].contents[0][0][0] = 0;
    }

    task_ptr->r_no[0] = 10;
    task_ptr->r_no[1] = 0;
    task_ptr->r_no[2] = 0;
    task_ptr->r_no[3] = 0;
    Menu_Suicide[0] = 1;
    Game_pause = 0;
    Pause_Down = 0;
    Training_Disp_Work_Clear();
    CP_No[0][0] = 0;
    CP_No[1][0] = 0;
    plw[New_Challenger].wu.operator = 1;
    Operator_Status[New_Challenger] = 1;
    Setup_NTr_Data(Menu_Cursor_Y[0]);
    count_cont_init(0);

    switch (Training[0].contents[0][0][0]) {
    case 0:
        control_pl_rno = 0;
        control_player = New_Challenger;
        break;

    case 1:
        control_pl_rno = 1;
        control_player = New_Challenger;
        break;

    case 2:
        control_pl_rno = 2;
        control_player = New_Challenger;
        break;

    case 3:
        control_pl_rno = 99;
        plw[New_Challenger].wu.operator = 0;
        Operator_Status[New_Challenger] = 0;
        break;

    case 4:
        control_pl_rno = 99;
        break;
    }

    All_Clear_Timer();
    Check_Replay();
    Training[0].contents[0][1][TRAINING_OPTION_DIFFICULTY] = Menu_Cursor_Y[0];
    init_omop();
    set_init_A4_flag();
    setup_vitality(&plw[0].wu, My_char[0] + 0);
    setup_vitality(&plw[1].wu, My_char[1] + 0);
    Setup_Training_Difficulty();
    Training_Cursor = Menu_Cursor_Y[0];
}

void Setup_NTr_Data(s16 ix) {
    switch (ix) {
    case 0:
        Play_Mode = 0;
        Replay_Status[0] = 0;
        Replay_Status[1] = 0;
        save_w[Present_Mode].Time_Limit = -1;
        save_w[Present_Mode].Damage_Level = Training[2].contents[0][1][TRAINING_OPTION_DAMAGE];
        Training[0] = Training[2];
        break;

    case 1:
        Record_Data_Tr = 1;
        Play_Mode = 1;
        Replay_Status[0] = 1;
        Replay_Status[1] = 1;
        save_w[Present_Mode].Time_Limit = 60;
        save_w[Present_Mode].Damage_Level = Training[2].contents[0][1][TRAINING_OPTION_DAMAGE];
        Training[0] = Training[2];
        Training[1] = Training[2];
        break;

    case 2:
        Play_Mode = 3;
        Replay_Status[0] = 3;
        Replay_Status[1] = 3;
        save_w[Present_Mode].Time_Limit = 60;
        save_w[Present_Mode].Damage_Level = Training[1].contents[0][1][TRAINING_OPTION_DAMAGE];
        Training[0] = Training[1];
        break;
    }

    apply_training_hitbox_display(false);
}

void Check_Skip_Replay(s16 ix) {
    if (Menu_Cursor_Y[0] != ix) {
        return;
    }

    if (Record_Data_Tr != 0) {
        return;
    }

    if (Menu_Cursor_Y[0] >= Menu_Cursor_Y[1]) {
        Menu_Cursor_Y[0]++;
        return;
    }

    Menu_Cursor_Y[0]--;
    Check_Skip_Recording();
}

void Check_Skip_Recording() {
    if (Menu_Cursor_Y[0] != 1) {
        return;
    }

    if (Training->contents[0][0][0] != 3) {
        return;
    }

    if (Menu_Cursor_Y[0] >= Menu_Cursor_Y[1]) {
        Menu_Cursor_Y[0]++;
        Check_Skip_Replay(2);
        return;
    }

    Menu_Cursor_Y[0]--;
}

void Yes_No_Cursor_Exit_Training(struct _TASK* task_ptr, s16 cursor_id) {
    u16 sw = ~(plsw_01[Decide_ID]) & plsw_00[Decide_ID];

    switch (sw) {
    case 0x4:
        Menu_Cursor_Y[0]--;

        if (Menu_Cursor_Y[0] < 0) {
            Menu_Cursor_Y[0] = 0;
            break;
        }

        SE_dir_cursor_move();
        break;

    case 0x8:
        Menu_Cursor_Y[0]++;

        if (Menu_Cursor_Y[0] > 1) {
            Menu_Cursor_Y[0] = 1;
            break;
        }

        SE_dir_cursor_move();
        break;

    case 0x200:
    case 0x100:
        SE_selected();

        if (Menu_Cursor_Y[0] || sw == 0x200) {
            task_ptr->r_no[2] = 0;
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            Cursor_Y_Pos[0][0] = cursor_id;
            break;
        }

        Soft_Reset_Sub();
        break;
    }
}

void Button_Config_Tr(struct _TASK* task_ptr) {
    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Menu_Suicide[0] = 1;
        Training_Index = 5;
        Copy_Key_Disp_Work();
        Setup_Button_Sub(6, 5, 1);
        pp_operator_check_flag(0);
        break;

    case 1:
        Button_Config_Sub(0);
        Button_Exit_Check_in_Tr(task_ptr, 0);
        Button_Config_Sub(1);
        Button_Exit_Check_in_Tr(task_ptr, 1);
        Save_Game_Data();
        break;
    }
}

void Button_Exit_Check_in_Tr(struct _TASK* task_ptr, s16 PL_id) {
    if (IO_Result & 0x200) {
        goto ten;
    }

    if (!(IO_Result & 0x100)) {
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 10) {
    ten:
        SE_selected();
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;

        if (Mode_Type == MODE_NORMAL_TRAINING) {
            task_ptr->r_no[1] = 1;
        } else {
            task_ptr->r_no[1] = 2;
        }

        pp_operator_check_flag(1);
        return;
    }

    if (Menu_Cursor_Y[PL_id] == 9) {
        SE_selected();
        Setup_IO_ConvDataDefault(PL_id);
    }
}

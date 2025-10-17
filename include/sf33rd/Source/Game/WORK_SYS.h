#ifndef WORK_SYS_H
#define WORK_SYS_H

#include "structs.h"
#include "types.h"

extern u32 current_task_num;
extern struct _SYSTEM_W sys_w;
extern struct _VM_W vm_w;
extern _EXTRA_OPTION ck_ex_option;

/// Player 1 current inputs
extern u16 p1sw_0;

/// Player 1 previous inputs
extern u16 p1sw_1;

/// Player 2 current inputs
extern u16 p2sw_0;

/// Player 2 previous inputs
extern u16 p2sw_1;

/// Player 3 current inputs (unused)
extern u16 p3sw_0;

/// Player 3 previous inputs (unused)
extern u16 p3sw_1;

/// Player 4 current inputs (unused)
extern u16 p4sw_0;

/// Player 4 previous inputs (unused)
extern u16 p4sw_1;

extern u8 Process_Counter;
extern u32 system_timer;
extern u8 Interface_Type[2];
extern s32 X_Adjust;
extern s32 Y_Adjust;
extern s32 X_Adjust_Buff[3];
extern s32 Y_Adjust_Buff[3];
extern u8 Disp_Size_H;
extern u8 Disp_Size_V;
extern u8 No_Trans;
extern u8 Turbo;
extern u8 Turbo_Timer;
extern s16 Correct_X[4];
extern s16 Correct_Y[4];
extern u8 Interrupt_Flag;

/// Controller 1 inputs
extern u16 p1sw_buff;

/// Controller 2 inputs
extern u16 p2sw_buff;

/// Controller 3 inputs (unused)
extern u16 p3sw_buff;

/// Controller 4 inputs (unused)
extern u16 p4sw_buff;

extern u32 Interrupt_Timer;
extern s8 Gill_Appear_Flag;

/// Current and previous inputs for controllers 1 and 2
/// `PLsw[i][0]` holds current inputs for controller `i`. `PLsw[i][1]` – previous button presses
extern u16 PLsw[2][2];

extern u8 Screen_PAL;
extern BG_POS bg_pos[8];
extern FM_POS fm_pos[8];
extern BackgroundParameters bg_prm[8];
extern s32 sca_x;
extern s32 sca_y;
extern f32 scr_sc;
extern f32 Screen_Zoom_X;
extern f32 Screen_Zoom_Y;
extern f32 SA_Zoom_X;
extern f32 SA_Zoom_Y;
extern f32 Frame_Zoom_X;
extern f32 Frame_Zoom_Y;
extern s32 Zoom_Base_Position_X;
extern s32 Zoom_Base_Position_Y;
extern s32 Zoom_Base_Position_Z;

extern struct _SAVE_W save_w[6];
extern Permission permission_player[6];
extern SystemDir system_dir[6];
extern _REPLAY_W Replay_w;
extern struct _REP_GAME_INFOR Rep_Game_Infor[11];
extern struct _TASK task[11];
extern MTX BgMATRIX[9];
extern TrainingData Training[3];

#endif

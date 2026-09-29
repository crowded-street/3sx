/**
 * @file work_sys.c
 * Global System State Variables
 */

#include "common.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "structs.h"

struct _SYSTEM_W sys_w;
struct _VM_W vm_w;
TrainingData Training[3];
_EXTRA_OPTION ck_ex_option;
u16 p1sw_0;
u16 p1sw_1;
u16 p2sw_0;
u16 p2sw_1;
u16 p3sw_0;
u16 p3sw_1;
u16 p4sw_0;
u16 p4sw_1;
u32 system_timer;
u8 Interface_Type[2];
s32 X_Adjust;
s32 Y_Adjust;
s32 X_Adjust_Buff[3];
s32 Y_Adjust_Buff[3];
u8 Disp_Size_H;
u8 Disp_Size_V;
u8 No_Trans;
u16 p1sw_buff;
u16 p2sw_buff;
u16 p3sw_buff;
u16 p4sw_buff;
u32 Interrupt_Timer;
s8 Gill_Appear_Flag;
u16 PLsw[2][2];
BG_POS bg_pos[BG_SCROLL_SPACE_COUNT];
FM_POS fm_pos[BG_SCROLL_SPACE_COUNT];
BackgroundParameters bg_prm[BG_SCROLL_SPACE_COUNT];
f32 scr_sc;

MTX BgMATRIX[BG_SCROLL_SPACE_COUNT + 1];
struct _TASK task[11];
struct _REP_GAME_INFOR Rep_Game_Infor[11];
_REPLAY_W Replay_w;
SystemDir system_dir[6];
Permission permission_player[6];
struct _SAVE_W save_w[6];

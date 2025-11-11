#ifndef BG_H
#define BG_H

#include "structs.h"
#include "types.h"

typedef struct {
    s8 be_flag;
    s8 disp_flag;
    s16 fam_no;
    s16 r_no_0;
    s16 r_no_1;
    s16 r_no_2;
    s16 position_x;
    s16 position_y;
    s32 speed_x;
    s32 speed_y;
    XY xy[2];
    XY wxy[2];
    u16* bg_address;
    u16* suzi_adrs;
    s16 old_pos_x;
    s32 zuubun;
    s16 no_suzi_line;
    u16* start_suzi;
    s16 u_line;
    s16 d_line;
    s16 bg_adrs_c_no;
    s16 suzi_c_no;
    s16 pos_x_work;
    s16 pos_y_work;
    s8 rewrite_flag;
    s8 suzi_base_flag;
    XY hos_xy[2];
    XY chase_xy[2];
    s16 free;
    s16 frame_deff;
    s16 r_limit;
    s16 r_limit2;
    s16 l_limit;
    s16 l_limit2;
    s16 y_limit;
    s16 y_limit2;
    u16* suzi_adrs2;
    u16* start_suzi2;
    s16 suzi_c_no2;
    s32 max_x_limit;
    s16* deff_rl;
    s16* deff_plus;
    s16* deff_minus;
    s16 abs_x;
    s16 abs_y;
} BGW;

typedef struct {
    s8 bg_routine;
    s8 bg_r_1;
    s8 bg_r_2;
    s8 stage;
    s8 area;
    s8 compel_flag;
    s32 scroll_cg_adr;
    s32 ake_cg_adr;
    u8 scno;
    u8 scrno;
    s16 bg2_sp_x;
    s16 bg2_sp_y;
    s16 scr_stop;
    s8 frame_flag;
    s8 chase_flag;
    s8 old_chase_flag;
    s8 old_frame_flag;
    s16 pos_offset;
    s16 quake_x_index;
    s16 quake_y_index;
    s16 bg_f_x;
    s16 bg_f_y;
    s16 bg2_sp_x2;
    s16 bg2_sp_y2;
    s16 frame_deff;
    s16 center_x;
    s16 center_y;
    s16 bg_index;
    s8 frame_vol;
    s16 max_x;
    u8 bg_opaque;
    BGW bgw[7];
} BG;

// MARK: - Serialized

extern BG bg_w;

// MARK: - Unhandled

extern s32 bgPalCodeOffset[8];
extern u16 Screen_Switch_Buffer;
extern u16 Screen_Switch;

extern u8 bg_disp_off;

void Bg_TexInit();
void Bg_Kakikae_Set();
void Ed_Kakikae_Set(s16 type);
void Bg_Close();
void Bg_Texture_Load_EX();
void Bg_Texture_Load2(u8 type);
void Bg_Texture_Load_Ending(s16 type);
void scr_trans(u8 bgnm);
void scr_trans_sub2(s32 x, s32 y, s32 suzi);
void scr_calc(u8 bgnm);
void scr_calc2(u8 bgnm);
void Pause_Family_On();
void Zoomf_Init();
void Zoom_Value_Set(u16 zadd);
void Frame_Up(u16 x, u16 y, u16 add);
void Frame_Down(u16 x, u16 y, u16 add);
void Frame_Adgjust(u16 pos_x, u16 pos_y);
void Scrn_Pos_Init();
void Scrn_Move_Set(s8 bgnm, s16 x, s16 y);
void Family_Init();
void Family_Set_R(s8 fmnm, s16 x, s16 y);
void Family_Set_W(s8 fmnm, s16 x, s16 y);
void Bg_On_R(u16 s_prm);
void Bg_On_W(u16 s_prm);
void Bg_Off_R(u16 s_prm);
void Bg_Off_W(u16 s_prm);
void Scrn_Renew();
void Irl_Family();
void Irl_Scrn();
void Family_Move();
void Ending_Family_Move();
void Bg_Disp_Switch(u8 on_off);

#endif

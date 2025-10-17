#ifndef BG_H
#define BG_H

#include "structs.h"
#include "types.h"

extern s32 bgPalCodeOffset[8];
extern u16 Screen_Switch_Buffer;
extern u16 Screen_Switch;

extern BG bg_w;
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

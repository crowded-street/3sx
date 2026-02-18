#ifndef SC_SUB_H
#define SC_SUB_H

#include "structs.h"
#include "types.h"

typedef struct {
    s16 fade;
    s16 fade_kind;
    u8 fade_prio;
} FadeData;

typedef struct {
    u8 atr;
    u8 page;
    u8 cx;
    u8 cy;
} SAFrame;

// MARK: - Unhandled

extern SAFrame sa_frame[3][48];
extern Polygon scrscrntex[4];
extern s16 Hnc_Num;
extern FadeData fd_dat;

// MARK: - Serialized

extern u8 FadeLimit;
extern u8 WipeLimit;

void Scrscreen_Init();
void Sa_frame_Clear();
void Sa_frame_Clear2(u8 pl);
void Sa_frame_Write();
void SSPutStr(u16 x, u16 y, u8 atr, const s8* str);
s32 SSPutStrPro(u16 flag, u16 x, u16 y, u8 atr, u32 vtxcol, const char* str);
void SSPutStr2(u16 x, u16 y, u8 atr, const s8* str);
void SSPutStr_Bigger(u16 x, u16 y, u8 atr, s8* str, f32 sc, u8 gr, u16 priority);
void SSPutDec(u16 x, u16 y, u8 atr, u8 dec, u8 size);
void scfont_put(u16 x, u16 y, u8 atr, u8 page, u8 cx, u8 cy, u16 priority);
void scfont_put2(u16 x, u16 y, u8 atr, u8 page, u8 cx, u8 cy);
void scfont_sqput(u16 x, u16 y, u8 atr, u8 page, u8 cx1, u8 cy1, u8 cx2, u8 cy2, u16 priority);
void scfont_sqput2(u16 x, u16 y, u8 atr, u8 inverse, u8 page, u8 cx1, u8 cy1, u8 cx2, u8 cy2);
void sc_clear(u16 sposx, u16 sposy, u16 eposx, u16 eposy);
void vital_put(u8 Pl_Num, s8 atr, s16 vital, u8 kind, u16 priority);
void silver_vital_put(u8 Pl_Num);
void vital_base_put(u8 Pl_Num);
void spgauge_base_put(u8 Pl_Num, s16 len);
void stun_put(u8 Pl_Num, u8 stun);
void stun_base_put(u8 Pl_Num, s16 len);
void WipeInit();
s32 WipeOut(u8 type);
s32 WipeIn(u8 type);
void FadeInit();
s32 FadeOut(u8 type, u8 step, u8 priority);
s32 FadeIn(u8 type, u8 step, u8 priority);
void ToneDown(u8 tone, u8 priority);
void player_name();
void stun_mark_write(u8 Pl_Num, s16 Len);
void max_mark_write(s8 Pl_Num, u8 Gauge_Len, u8 Mchar, u8 Mass_Len);
void SF3_logo(u8 step);
void player_face_init();
void player_face();
void hnc_set(u8 num, u8 atr);
void hnc_wipeinit(u8 atr);
s32 hnc_wipeout(u8 atr);
void ci_set(u8 type, u8 atr);
void nw_set(u8 PL_num, u8 atr);
void score8x16_put(u16 x, u16 y, u8 atr, u8 chr);
void score16x24_put(u16 x, u16 y, u8 atr, u8 chr);
void combo_message_set(u8 pl, u8 kind, u8 x, u8 num, u8 hi, u8 low);
void combo_pts_set(u8 pl, u8 x, u8 num, s8* pts, s8 digit);
void naming_set(u8 pl, s16 place, u16 atr, u16 chr);
void stun_gauge_waku_write(s16 p1len, s16 p2len);
void sc_ram_to_vram_opc(s8 sc_num, s8 x, s8 y, u16 atr);
void overwrite_panel(u32 color, u8 priority);
void sa_stock_trans(s16 St_Num, s16 Spg_Col, s8 Stpl_Num);
void sa_fullstock_trans(s16 St_Num, s16 Spg_Col, s8 Stpl_Num);
void sa_number_write(s8 Stpl_Num, u16 x);
void sc_ram_to_vram(s8 sc_num);
void sq_paint_chenge(u16 x, u16 y, u16 sx, u16 sy, u16 atr);
void fade_cont_init();
void fade_cont_main();
void Akaobi();
void Training_Disp_Work_Clear();
void Training_Damage_Set(s16 damage, s16 /* unused */, u8 kezuri);
void Training_Data_Disp();
void dispButtonImage(s32 px, s32 py, s32 pz, s32 sx, s32 sy, s32 cl, s32 ix);
void dispButtonImage2(s32 px, s32 py, s32 pz, s32 sx, s32 sy, s32 cl, s32 ix);
void dispSaveLoadTitle(void* ewk);

#endif

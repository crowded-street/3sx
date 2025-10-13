#ifndef BG_DATA_H
#define BG_DATA_H

#include "structs.h"
#include "types.h"

typedef struct {
    u32 bg_num;
    u32 rwgbix;
    const s16* rw_ptr;
} bgrw_data_tbl_elem;

extern const u8 use_scr[22];
extern const u8 use_real_scr[22];
extern const u8 use_family[22];
extern const u8 rewrite_scr[22];
extern const u8 use_scr2[7];
extern const u8 stage_bgw_number[22][3];
extern const s32 msp[22][3][2];
extern const s32 msp2[][3][2];
extern const u16 stage000_map[64];
extern const u16 stage001_map[64];
extern const u16 stage010_map[64];
extern const u16 stage020_map[64];
extern const u16 stage021_map[64];
extern const u16 stage022_map[64];
extern const u16 stage030_map[64];
extern const u16 stage031_map[64];
extern const u16 stage040_map[64];
extern const u16 stage050_map[64];
extern const u16 stage051_map[64];
extern const u16 stage060_map[64];
extern const u16 stage070_map[64];
extern const u16 stage071_map[64];
extern const u16 stage072_map[64];
extern const u16 stage080_map[64];
extern const u16 stage081_map[64];
extern const u16 stage090_map[64];
extern const u16 stage091_map[64];
extern const u16 stage100_map[64];
extern const u16 stage101_map[64];
extern const u16 stage110_map[64];
extern const u16 stage120_map[64];
extern const u16 stage121_map[64];
extern const u16 stage130_map[64];
extern const u16 stage131_map[64];
extern const u16 stage140_map[64];
extern const u16 stage141_map[64];
extern const u16 stage142_map[64];
extern const u16 stage150_map[64];
extern const u16 stage151_map[64];
extern const u16 stage160_map[64];
extern const u16 stage161_map[64];
extern const u16 stage180_map[64];
extern const u16 stage181_map[64];
extern const u16 stage190_map[64];
extern const u16 bonus010_map[64];
extern const u16 bonus011_map[64];
extern const u16 bonus020_map[64];
extern const u16 select_map[64];
extern const u16 rank_map[64];
extern const u16 win_lose_map[64];
extern const u32 bgtex_stage_gbix[22][3];
extern const u32 bgtex_etc_gbix[7];
extern const u32 stage_priority[22];
extern const u32 etc_bg_priority[7];
extern const u8 stage_opaque[22];
extern const s8 bgrw_on[22][8];
extern const s16 rw30[113];
extern const u32 rw30col[8];
extern const s16 rw31[57];
extern const u32 rw31col[4];
extern const s16 rw190[26];
extern const s16 rw191[11];
extern const s16 rw192[11];

#if defined(TARGET_PS2)
extern const u32 bgrw_data_tbl[20][3];
#else
extern const bgrw_data_tbl_elem bgrw_data_tbl[20];
#endif

extern const s32 stage03rw_data_tbl[13];
extern const u8 stage03_flash_tbl[16];
extern const s32 stage19rw_data_tbl[4];
extern const u8 stage19_loop_tbl1[16];
extern const u8 stage19_loop_tbl2[16];
extern const u8 ake_bg_off[20];
extern const s16 limit_tbl3[22][3][4];
extern const s8 bg_index_tbl[22][3];
extern const s32 bg_pos_tbl2[7][3][2];
extern const s8 quake_x_tbl[130];
extern const s8 quake_y_tbl[130];
extern const u16* bg_map_tbl[][3];
extern const u16* bg_map_tbl2[];

extern s16 y_sitei_pos;
extern u8 y_sitei_flag;
extern u8 c_number;
extern u8 c_kakikae;
extern u8 g_number[2];
extern u8 g_kakikae[2];
extern u8 nosekae;
extern const u16* scr_bcm[4];
extern s16 scrn_adgjust_y;
extern s16 scrn_adgjust_x;
extern u16 zoom_add;
extern s16 ls_cnt1;
extern s8 bg_app;
extern s8 sa_pa_flag;
extern s8 aku_flag;
extern s8 seraph_flag;
extern s8 akebono_flag;
extern MVXY bg_mvxy;
extern s16 chase_time_y;
extern s16 chase_time_x;
extern s16 chase_y;
extern s16 chase_x;
extern s8 demo_car_flag[2];
extern Ideal_W ideal_w;
extern BGW* bgw_ptr;
extern s8 bg_app_stop;
extern s16 bg_stop;
extern s16 base_y_pos;
extern s32 etcBgPalCnvTable[7];
extern u8 etcBgGixCnvTable[7][16];

#endif

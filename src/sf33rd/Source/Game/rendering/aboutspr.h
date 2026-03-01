#ifndef ABOUTSPR_H
#define ABOUTSPR_H

#include "structs.h"
#include "types.h"

#define MULTITEXTURE_MAX 24

extern MultiTexture mts[MULTITEXTURE_MAX];
extern MTS_OK mts_ok[MULTITEXTURE_MAX];
extern WORK dmwk_moji;
extern WORK dmwk_kage;
extern const u16 effk8k9_pattern[18];
extern const u32 judge_area_attr[17][2];
extern const u32 box_color_attr[4][2];
extern const char gkw_table[64];

void Init_load_on_memory_data();
void shadow_drawing(WORK* wk, s16 bsy);
void reset_dma_group(u16 num);
s32 sort_push_request(WORK* wk);
s32 sort_push_request4(WORK* wk);
s8 get_kage_width(s16 cpy);
void shadow_setup(WORK* wk, s16 bsy);
s32 sort_push_requestB(WORK* wk);
s32 sort_push_requestA(WORK* wk);
s32 sort_push_request8(WORK* wk);
s32 sort_push_request3(WORK* wk);
s32 sort_push_request2(WORK_Other* wk);
s16 exchange_current_colcd(WORK* wk);
void Mtrans_use_trans_mode(WORK* wk, s16 bsy);
void all_cgps_put_back(WORK* wk);
s32 set_conn_sprite(WORK_Other_CONN* wk, s16 bsy);
void set_judge_area_sprite(WORK_Other_JUDGE* wk, s16 bsy);
void draw_hit_judge_line(f32 px, f32 py, f32 sx, f32 sy, u32 col, u32 attr);
void setup_dma_group(u16 num, u32 /* unused */);
void setup_bonus_car_parts();
s32 setup_GILL_Opening_Ceremony();
s32 setup_GILL_exsa_obj();

#endif

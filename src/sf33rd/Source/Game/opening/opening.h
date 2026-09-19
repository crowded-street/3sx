#ifndef OPENING_H
#define OPENING_H

#include "structs.h"
#include "types.h"

extern s16 op_obj_disp;
extern s8 op_scrn_end;
extern s16 title_tex_flag;
extern s16 op_timer0;
extern OP_W op_w;
extern s16 music_scene;
extern s16 music_time;
extern s16 op_plmove_timer;
extern OPBW* opw_ptr;
extern s16 op_end_flag;
extern s16 op_demo_index;
extern s16 op_sound_status;
extern MVXY op_bg_mvxy[3];
extern void (*opening_move_jp[19])();
extern const s16 optsr_tbl[59];
extern const s16 op_quake_y_tbl0[16];

void TITLE_Init();
s16 TITLE_Move(u16 type);
s16 opening_demo();
void OPBG_Init();
s16 OPBG_Move(s32 /* unused */);
void opening_init();
void sound_trg_init();
void sound_trg_move();
void OPBG_Trans();
void op_work_clear();
s16 oh_opening_demo();
void Bg_Family_Set_op();
void opening_init2();
void opening_move();
void opening_title();
void op_100_move();
void op_101_move();
void op_102_move();
void op_103_move();
void op_104_move();
void op_105_move();
void op_106_move();
void op_107_move();
void op_108_move();
void op_109_move();
void op_110_move();
void op_111_move();
void op_112_move();
void op_113_move();
void op_114_move();
void op_115_move();
void op_116_move();
void op_117_move();
void op_118_move();
void opning_init_00000();
void opning_init_01000();
void opning_init_02000();
void op_bg_move(s16 r_index);
void op_bg0_move(s16 r_index);
void op_bg1_move(s16 r_index);
void op_bg2_move(s16 r_index);
void op_bg0_0000(s16 /* unused */);
void op_bg0_0001(s16 /* unused */);
void op_bg0_0002(s16 /* unused */);
void op_bg0_0003(s16 /* unused */);
void op_bg0_0004(s16 /* unused */);
void op_bg0_0005(s16 /* unused */);
void op_bg0_0006(s16 /* unused */);
void op_bg0_0007(s16 /* unused */);
void op_bg0_0008(s16 /* unused */);
void op_bg0_0010(s16 /* unused */);
void op_bg0_0011(s16 /* unused */);
void op_bg0_0012(s16 /* unused */);
void op_bg0_0013(s16 /* unused */);
void op_bg0_0014(s16 /* unused */);
void op_bg0_0015(s16 /* unused */);
void op_bg0_0016(s16 /* unused */);
void op_bg1_0003(s16 r_index);
void op_scrn_pos_set2(s16 bg_no);
/* Where an opening background block goes and how it is drawn. The field order
 * is the parameter order the oh_bg_blk_* family took. */
typedef struct {
    s32 blk_no;
    s16 mapx;
    s16 mapy;
    s32 trans;
} Op_Bg_Blk_Args;

void oh_bg_blk_w(OPBW* opbw, const Op_Bg_Blk_Args* b);
void oh_bg_blk_wh(OPBW* opbw, const Op_Bg_Blk_Args* b);
void oh_bg_blk_wv(OPBW* opbw, const Op_Bg_Blk_Args* b);
void oh_bg_blk_whv(OPBW* opbw, const Op_Bg_Blk_Args* b);
void op_bg1_0000(s16 /* unused */);
void op_bg1_0001(s16 r_index);
void op_bg1_0002(s16 r_index);
void op_bg1_0003(s16 r_index);
void op_bg2_0000();
void op_bg2_0001();
void op_bg2_0002();
void op_bg2_0003();
void opening_title_01();

#endif // OPENING_H

#ifndef OPENING_H
#define OPENING_H

#include "structs.h"
#include "types.h"

extern s16 op_obj_disp;
extern s8 op_scrn_end;
extern s16 title_tex_flag;
extern s16 op_timer0;
extern OP_W op_w;

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
void oh_bg_blk_w(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans);
void oh_bg_blk_wh(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans);
void oh_bg_blk_wv(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans);
void oh_bg_blk_whv(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans);
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

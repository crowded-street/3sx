#ifndef COLOR3RD_H
#define COLOR3RD_H

#include "structs.h"
#include "types.h"

extern u16 ColorRAM[512][64];
extern Col3rd_W col3rd_w;

void q_ldreq_color_data(REQ* curr);
void load_any_color(u16 ix, u8 kokey);
void set_hitmark_color();
void init_trans_color_ram(s16 id, s16 key, u8 type, u16 data);
void init_color_trans_req();
void push_color_trans_req(s16 from_col, s16 to_col);
void palCopyGhostDC(s32 ofs, s32 cnt, void* data);
u16 palConvSrcToRam(u16 col);
void palCreateGhost();
Palette* palGetChunkGhostDC();
Palette* palGetChunkGhostCP3();
void palUpdateGhostDC();
void palUpdateGhostCP3(s32 pal, s32 nums);
void palConvRowTim2CI8Clut(u16* src, u16* dst, s32 size);

#endif

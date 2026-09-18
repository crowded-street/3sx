#ifndef BG_STATE_H
#define BG_STATE_H

#include "structs.h"
#include "types.h"

/* State defined in bg.c and shared with the files split out of it. These are
 * the file's own globals, not part of its public interface - bg.h stays the
 * interface the rest of the game uses. */

extern u8 bg_priority[4];
extern u8 rw_num;
extern u8 rw_bg_flag[4];
extern u8 tokusyu_stage;
extern s32 rw_gbix[13];
extern s8 stage_flash;
extern s8 stage_ftimer;
extern s32 yang_ix_plus;
extern s8 yang_ix;
extern s8 yang_timer;
extern u8 ending_flag;
extern u8 gouki_end_gbix[16];
extern const u32* rw3col_ptr;
extern RW_DATA rw_dat[20];

#endif

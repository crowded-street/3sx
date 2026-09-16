#ifndef EFF64_H
#define EFF64_H

#include "structs.h"
#include "types.h"

void effect_64_move(WORK_Other_CONN* ewk);
/* The nine values effect_64_init is driven by. The field order is the parameter
 * order it used to take. */
typedef struct {
    u8 dir_old;
    s16 sync_bg;
    s16 master_player;
    s16 letter_type;
    s16 cursor_index;
    u16 char_offset;
    s16 pos_index;
    s16 convert_id;
    s16 convert_id2;
} Effect64Init;

s32 effect_64_init(const Effect64Init* p);

#endif

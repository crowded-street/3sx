#ifndef EFF57_H
#define EFF57_H

#include "structs.h"
#include "types.h"

typedef enum MenuHeader {
    MENU_HEADER_MODE_MENU,
    MENU_HEADER_OPTION_MENU,
    MENU_HEADER_BUTTON_CONFIG,
    MENU_HEADER_SCREEN_ADJUST,
    MENU_HEADER_SOUND,
    MENU_HEADER_EXTRA_OPTION = 6,
    MENU_HEADER_GAME_OPTION,
    MENU_HEADER_NETWORK,
    MENU_HEADER_REPLAY,
    MENU_HEADER_SYSTEM_DIRECTION,
    MENU_HEADER_TRAINING
} MenuHeader;

typedef struct {
    s16 direction;
    MenuHeader header;
    s16 target_bg;
    s16 character_index;
    s16 option;
} Effect57Init;

void effect_57_move(WORK_Other* ewk);
void EFF57_KILL(WORK_Other* ewk);
s32 effect_57_init_params(Effect57Init params);

#define effect_57_init(dir_old, ID, Target_BG, char_ix, option)                                                        \
    effect_57_init_params((Effect57Init) { dir_old, ID, Target_BG, char_ix, option })

#endif

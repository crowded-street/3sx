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

void effect_57_move(WORK_Other* ewk);
void EFF57_KILL(WORK_Other* ewk);
s32 effect_57_init(s16 dir_old, MenuHeader ID, s16 Target_BG, s16 char_ix, s16 option);

#endif

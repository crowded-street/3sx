#ifndef MAIN_H
#define MAIN_H

#include "structs.h"
#include "types.h"

#include <stdbool.h>

#define INIT_TASK_NUM 0
#define ENTRY_TASK_NUM 1
#define RESET_TASK_NUM 2
#define MENU_TASK_NUM 3
#define PAUSE_TASK_NUM 4
#define GAME_TASK_NUM 5
#define SAVER_TASK_NUM 6
#define DEBUG_TASK_NUM 9

extern MPP mpp_w;
extern s32 system_init_level;

bool get_game_initialized();

void cpInitTask();
void cpReadyTask(u16 num, void* func_adrs);
void cpExitTask(u16 num);
s32 mppGetFavoritePlayerNumber();

#endif

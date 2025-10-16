#ifndef CMD_DATA_H
#define CMD_DATA_H

#include "structs.h"
#include "types.h"

typedef const s16* const_s16_arr;

extern WORK_CP wcp[2];
extern T_PL_LVR t_pl_lvr[2];
extern WAZA_WORK waza_work[2][56];

extern s16 cmd_id;
extern s16* cmd_tbl_ptr;
extern u16 sw_work;
extern T_PL_LVR* chk_pl;
extern s16 waza_type[2];
extern WAZA_WORK* waza_ptr;
extern PLW* cmd_pl;

extern const s16 ukemi_time_tbl[17];
extern const s16 pl_cmd_num[20][7];
extern const s16 lever_gacha_tbl[16];
extern const u16 chk22_tbl[8];

extern void* pl_cmd[];
extern void* pl_CMD[];
extern const_s16_arr* player_CMD[];
extern const_s16_arr* player_cmd[];
extern s16 lvr_chk_tbl[2][4];

#endif

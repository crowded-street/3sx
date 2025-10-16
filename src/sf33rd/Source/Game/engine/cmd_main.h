#ifndef CMD_MAIN_H
#define CMD_MAIN_H

#include "structs.h"
#include "types.h"

void waza_check(PLW* pl);
void key_thru(PLW* pl);
void cmd_data_set(PLW* /* unused */, s16 i);
void cmd_init(PLW* pl);
void cmd_move();
void check_init();
void check_next();
void check_0();
void check_1();
void check_2();
void check_3();
void check_4();
void check_5();
void check_6();
void check_7();
void check_9();
void paring_miss_init();
void check_10();
void check_11();
void check_12();
void check_13();
void check_14();
void check_15();
void check_16();
void check_18();
void check_19();
void check_20();
void check_21();
void check_22();
void check_23();
void check_24();
void check_25();
void check_26();
void command_ok();
void command_ok_move(s16 waza_num);
s32 dead_lvr_check();
void pl_lvr_set();
void sw_pick_up();
void dash_flag_clear(s16 pl_id);
void hi_jump_flag_clear(s16 pl_id);
void waza_flag_clear_only_1(s16 pl_id, s16 wznum);
void waza_compel_init(s16 pl_id, s16 num, intptr_t* adrs);
void waza_compel_all_init(PLW* pl);
void waza_compel_all_init2(PLW* pl);
u16 processed_lvbt(u16 lv_data);

#endif

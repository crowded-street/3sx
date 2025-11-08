#ifndef COUNT_H
#define COUNT_H

#include "structs.h"
#include "types.h"

#include <stdbool.h>

extern s8 round_timer;
extern s8 flash_timer;
extern s8 flash_r_num;
extern s8 flash_col;
extern s8 math_counter_hi;
extern s8 math_counter_low;
extern u8 counter_color;
extern bool mugen_flag;
extern s8 hoji_counter;

void count_cont_init(u8 type);
void count_cont_main();
void counter_control();
void counter_write(u8 atr);
void bcounter_write();
void counter_flash(s8 Flash_Num);
void bcount_cont_init();
void bcount_cont_main();
void bcounter_control();
s16 bcounter_down(u8 kind);

#endif

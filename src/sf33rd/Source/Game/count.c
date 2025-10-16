#include "sf33rd/Source/Game/count.h"
#include "common.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/sc_data.h"
#include "sf33rd/Source/Game/sc_sub.h"

Round_Timer round_timer;
s8 flash_timer;
s8 flash_r_num;
s8 flash_col;
s8 math_counter_hi;
s8 math_counter_low;
u8 counter_color;
s8 mugen_flag;
s8 hoji_counter;

void count_cont_init(u8 type) {
    Counter_hi = save_w[Present_Mode].Time_Limit;

    if (Counter_hi == -1) {
        mugen_flag = 1;
        round_timer.size.half.h = 1;

        if (type == 0) {
            counter_write(4);
        }
    } else {
        mugen_flag = 0;
        hoji_counter = 60;
        Counter_low = hoji_counter;
        round_timer.size.half.h = Counter_hi;
        math_counter_hi = Counter_hi;
        math_counter_hi /= 10;
        math_counter_low = Counter_hi - (math_counter_hi * 10);

        if (type == 0) {
            counter_write(4);
        }

        round_timer.size.half.l = 0;
    }

    flash_r_num = 0;
    flash_col = 0;
    counter_color = 4;
}

void count_cont_main() {
#if defined(TARGET_PS2)
    void counter_write(s32 atr);
#endif

    if (Bonus_Game_Flag) {
        return;
    }

    if (count_end) {
        counter_write(4);
        return;
    }

    if (Debug_w[24]) {
        counter_write(counter_color);
        return;
    }

    if (Allow_a_battle_f == 0 || Demo_Time_Stop != 0) {
        counter_write(counter_color);
        return;
    }

    if (Break_Into) {
        counter_write(counter_color);
        return;
    }

    if (sa_stop_check() != 0) {
        counter_write(counter_color);
        return;
    }

    if (mugen_flag == 1) {
        counter_write(4);
        return;
    }

    if (!EXE_flag && !Game_pause) {
        counter_control();
        return;
    }

    counter_write(counter_color);
}

void counter_control() {
#if defined(TARGET_PS2)
    void counter_write(s32 atr);
#endif

    if (Counter_hi == 0) {
        if (No_Trans == 0) {
            counter_write(counter_color);
        }
        return;
    }

    if (flash_r_num) {
        if (Counter_hi == 10 && Counter_low == hoji_counter) {
            flash_timer = 0;
            counter_flash(1);
        } else if (Counter_hi < 11) {
            counter_flash(1);
        } else {
            counter_flash(0);
        }
    } else if (Counter_hi == 30 && Counter_low == hoji_counter) {
        flash_r_num = 1;
        flash_timer = 0;
        counter_flash(0);
    }

    if (Counter_low != 0) {
        Counter_low -= 1;

        if (No_Trans == 0) {
            counter_write(counter_color);
        }

        return;
    }

    Counter_low = hoji_counter;
    Counter_hi -= 1;

    if (Counter_hi == 0) {
        counter_color = 4;
    }

    round_timer.size.half.h = Counter_hi;
    math_counter_hi = Counter_hi;
    math_counter_hi /= 10;
    math_counter_low = Counter_hi - (math_counter_hi * 10);

    if (No_Trans == 0) {
        counter_write(counter_color);
    }
}

void counter_write(u8 atr) {
    u8 i;

    if (omop_cockpit != 0) {
        if (omop_round_timer == 0) {
            for (i = 0; i < 4; i++) {
                scfont_sqput(i + 22, 1, 9, 2, 31, 2, 1, 3, 2);
            }
        } else if (mugen_flag == 0) {
            scfont_sqput(22, 0, atr, 2, math_counter_hi << 1, 2, 2, 4, 2);
            scfont_sqput(24, 0, atr, 2, math_counter_low << 1, 2, 2, 4, 2);
        } else {
            scfont_sqput(22, 0, 4, 2, 28, 28, 4, 4, 2);
        }

        scfont_sqput(21, 1, 9, 0, 12, 6, 1, 4, 2);
        scfont_sqput(26, 1, 137, 0, 12, 6, 1, 4, 2);
        scfont_sqput(22, 4, 9, 0, 3, 18, 4, 1, 2);
    }
}

void bcounter_write() {
    if (!No_Trans) {
        scfont_put(21, 4, 0x8F, 2, 20, 6, 2);
        scfont_sqput(22, 2, 15, 2, math_counter_hi << 1, 6, 2, 3, 2);
        scfont_sqput(24, 2, 15, 2, math_counter_low << 1, 6, 2, 3, 2);
        scfont_put(26, 4, 15, 2, 20, 6, 2);
    }
}

void counter_flash(s8 Flash_Num) {
    flash_timer--;

    if (flash_timer < 0) {
        flash_timer = flash_timer_tbl[Flash_Num];
        counter_color = flash_color_tbl[flash_col];
        flash_col++;

        if (flash_col == 4) {
            flash_col = 0;
        }
    }
}

void bcount_cont_init() {
    Counter_hi = 50;
    hoji_counter = 60;
    Counter_low = hoji_counter;
    round_timer.size.half.h = Counter_hi;
    math_counter_hi = 5;
    math_counter_low = 0;
    bcounter_write();
    round_timer.size.half.l = 0;
    Time_Stop = 0;
}

void bcount_cont_main() {
    if (Break_Into != 0 || sa_stop_check() || Time_Stop != 0 || Allow_a_battle_f == 0) {
        return;
    }

    if (!Debug_w[24] && !EXE_flag && !Game_pause) {
        bcounter_control();
    }
}

void bcounter_control() {
    if (Counter_hi != 0) {
        if (Counter_low != 0) {
            Counter_low -= 1;
            return;
        }

        hoji_counter = 60;
        Counter_low = hoji_counter;
        Counter_hi -= 1;
        round_timer.size.half.h = Counter_hi;
        math_counter_hi = Counter_hi;
        math_counter_hi /= 10;
        math_counter_low = Counter_hi - (math_counter_hi * 10);

        if (Counter_hi == 0) {
            math_counter_hi = math_counter_low = 0;
            Allow_a_battle_f = 0;
            Time_Over = 1;
        }
    }
}

s16 bcounter_down(u8 kind) {
    if (Counter_hi == 0) {
        math_counter_hi = math_counter_low = 0;
        return 0;
    }

    Counter_hi -= 1;

    if (kind) {
        Counter_hi = 0;
    }

    math_counter_hi = Counter_hi;
    math_counter_hi /= 10;
    math_counter_low = Counter_hi - (math_counter_hi * 10);

    if (Counter_hi == 0) {
        math_counter_hi = math_counter_low = 0;
    }

    return Counter_hi;
}

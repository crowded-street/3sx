/**
 * @file count.c
 * Game Clock
 */

#include "sf33rd/Source/Game/ui/count.h"
#include "common.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_data.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

void count_cont_init(u8 type) {
    gs.Counter_hi = save_w[Present_Mode].Time_Limit; // FIXME: use a consistent value in netplay

    if (gs.Counter_hi == -1) {
        gs.mugen_flag = true;
        gs.round_timer = 1;

        if (type == 0) {
            counter_write(4);
        }
    } else {
        gs.mugen_flag = false;
        gs.hoji_counter = 60;
        gs.Counter_low = gs.hoji_counter;
        gs.round_timer = gs.Counter_hi;
        gs.math_counter_hi = gs.Counter_hi;
        gs.math_counter_hi /= 10;
        gs.math_counter_low = gs.Counter_hi - (gs.math_counter_hi * 10);

        if (type == 0) {
            counter_write(4);
        }
    }

    gs.flash_r_num = 0;
    gs.flash_col = 0;
    gs.counter_color = 4;
}

void count_cont_main() {
    if (Bonus_Game_Flag) {
        return;
    }

    if (count_end) {
        counter_write(4);
        return;
    }

    if (Debug_w[24]) {
        counter_write(gs.counter_color);
        return;
    }

    if (Allow_a_battle_f == 0 || Demo_Time_Stop != 0) {
        counter_write(gs.counter_color);
        return;
    }

    if (Break_Into) {
        counter_write(gs.counter_color);
        return;
    }

    if (sa_stop_check() != 0) {
        counter_write(gs.counter_color);
        return;
    }

    if (gs.mugen_flag) {
        counter_write(4);
        return;
    }

    if (!EXE_flag && !Game_pause) {
        counter_control();
        return;
    }

    counter_write(gs.counter_color);
}

void counter_control() {
    if (gs.Counter_hi == 0) {
        if (No_Trans == 0) {
            counter_write(gs.counter_color);
        }
        return;
    }

    if (gs.flash_r_num) {
        if (gs.Counter_hi == 10 && gs.Counter_low == gs.hoji_counter) {
            gs.flash_timer = 0;
            counter_flash(1);
        } else if (gs.Counter_hi < 11) {
            counter_flash(1);
        } else {
            counter_flash(0);
        }
    } else if (gs.Counter_hi == 30 && gs.Counter_low == gs.hoji_counter) {
        gs.flash_r_num = 1;
        gs.flash_timer = 0;
        counter_flash(0);
    }

    if (gs.Counter_low != 0) {
        gs.Counter_low -= 1;

        if (No_Trans == 0) {
            counter_write(gs.counter_color);
        }

        return;
    }

    gs.Counter_low = gs.hoji_counter;
    gs.Counter_hi -= 1;

    if (gs.Counter_hi == 0) {
        gs.counter_color = 4;
    }

    gs.round_timer = gs.Counter_hi;
    gs.math_counter_hi = gs.Counter_hi;
    gs.math_counter_hi /= 10;
    gs.math_counter_low = gs.Counter_hi - (gs.math_counter_hi * 10);

    if (No_Trans == 0) {
        counter_write(gs.counter_color);
    }
}

void counter_write(u8 atr) {
    u8 i;

    if (omop_cockpit != 0) {
        if (omop_round_timer == 0) {
            for (i = 0; i < 4; i++) {
                scfont_sqput(i + 22, 1, 9, 2, 31, 2, 1, 3, 2);
            }
        } else if (!gs.mugen_flag) {
            scfont_sqput(22, 0, atr, 2, gs.math_counter_hi << 1, 2, 2, 4, 2);
            scfont_sqput(24, 0, atr, 2, gs.math_counter_low << 1, 2, 2, 4, 2);
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
        scfont_sqput(22, 2, 15, 2, gs.math_counter_hi << 1, 6, 2, 3, 2);
        scfont_sqput(24, 2, 15, 2, gs.math_counter_low << 1, 6, 2, 3, 2);
        scfont_put(26, 4, 15, 2, 20, 6, 2);
    }
}

void counter_flash(s8 Flash_Num) {
    gs.flash_timer--;

    if (gs.flash_timer < 0) {
        gs.flash_timer = flash_timer_tbl[Flash_Num];
        gs.counter_color = flash_color_tbl[gs.flash_col];
        gs.flash_col++;

        if (gs.flash_col == 4) {
            gs.flash_col = 0;
        }
    }
}

void bcount_cont_init() {
    gs.Counter_hi = 50;
    gs.hoji_counter = 60;
    gs.Counter_low = gs.hoji_counter;
    gs.round_timer = gs.Counter_hi;
    gs.math_counter_hi = 5;
    gs.math_counter_low = 0;
    bcounter_write();
    gs.Time_Stop = 0;
}

void bcount_cont_main() {
    if (Break_Into != 0 || sa_stop_check() || gs.Time_Stop != 0 || Allow_a_battle_f == 0) {
        return;
    }

    if (!Debug_w[24] && !EXE_flag && !Game_pause) {
        bcounter_control();
    }
}

void bcounter_control() {
    if (gs.Counter_hi == 0) {
        return;
    }

    if (gs.Counter_low != 0) {
        gs.Counter_low -= 1;
        return;
    }

    gs.hoji_counter = 60;
    gs.Counter_low = gs.hoji_counter;
    gs.Counter_hi -= 1;
    gs.round_timer = gs.Counter_hi;
    gs.math_counter_hi = gs.Counter_hi;
    gs.math_counter_hi /= 10;
    gs.math_counter_low = gs.Counter_hi - (gs.math_counter_hi * 10);

    if (gs.Counter_hi == 0) {
        gs.math_counter_hi = gs.math_counter_low = 0;
        Allow_a_battle_f = 0;
        Time_Over = true;
    }
}

s16 bcounter_down(u8 kind) {
    if (gs.Counter_hi == 0) {
        gs.math_counter_hi = gs.math_counter_low = 0;
        return 0;
    }

    gs.Counter_hi -= 1;

    if (kind) {
        gs.Counter_hi = 0;
    }

    gs.math_counter_hi = gs.Counter_hi;
    gs.math_counter_hi /= 10;
    gs.math_counter_low = gs.Counter_hi - (gs.math_counter_hi * 10);

    if (gs.Counter_hi == 0) {
        gs.math_counter_hi = gs.math_counter_low = 0;
    }

    return gs.Counter_hi;
}

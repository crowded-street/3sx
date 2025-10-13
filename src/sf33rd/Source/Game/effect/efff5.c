/**
 * @file efff5.c
 * TODO: identify what this effect does
 */

#include "sf33rd/Source/Game/effect/efff5.h"
#include "common.h"

void efff5_0000(WORK_Other* /* unused */);
void efff5_1000(WORK_Other* /* unused */);
void efff5_2000(WORK_Other* /* unused */);
void efff5_3000(WORK_Other* /* unused */);
void efff5_4000(WORK_Other* /* unused */);
void efff5_5000(WORK_Other* /* unused */);
void efff5_6000(WORK_Other* /* unused */);
void efff5_7000(WORK_Other* /* unused */);
void efff5_8000(WORK_Other* /* unused */);
void efff5_9000(WORK_Other* /* unused */);
void efff5_A000(WORK_Other* /* unused */);
void efff5_B000(WORK_Other* /* unused */);

void effect_F5_move(WORK_Other* ewk) {
    void (*efff5_jp[12])(WORK_Other*) = { efff5_0000, efff5_1000, efff5_2000, efff5_3000, efff5_4000, efff5_5000,
                                          efff5_6000, efff5_7000, efff5_8000, efff5_9000, efff5_A000, efff5_B000 };

    efff5_jp[ewk->wu.routine_no[0]](ewk);
}

void efff5_0000(WORK_Other* /* unused */) {}

void efff5_1000(WORK_Other* /* unused */) {}

void efff5_2000(WORK_Other* /* unused */) {}

void efff5_3000(WORK_Other* /* unused */) {}

void efff5_4000(WORK_Other* /* unused */) {}

void efff5_5000(WORK_Other* /* unused */) {}

void efff5_6000(WORK_Other* /* unused */) {}

void efff5_7000(WORK_Other* /* unused */) {}

void efff5_8000(WORK_Other* /* unused */) {}

void efff5_9000(WORK_Other* /* unused */) {}

void efff5_A000(WORK_Other* /* unused */) {}

void efff5_B000(WORK_Other* /* unused */) {}

s32 effect_F5_init(s16 /* unused */) {
    return 0;
}

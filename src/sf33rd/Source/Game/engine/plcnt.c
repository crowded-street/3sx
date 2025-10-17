/**
 * @file plcnt.c
 * Character Controller
 */

#include "sf33rd/Source/Game/engine/plcnt.h"
#include "common.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/animation/win_pl.h"
#include "sf33rd/Source/Game/count.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/eff00.h"
#include "sf33rd/Source/Game/effect/eff01.h"
#include "sf33rd/Source/Game/effect/eff33.h"
#include "sf33rd/Source/Game/effect/effc9.h"
#include "sf33rd/Source/Game/effect/effd3.h"
#include "sf33rd/Source/Game/effect/effe3.h"
#include "sf33rd/Source/Game/effect/effe4.h"
#include "sf33rd/Source/Game/effect/effe5.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/effj7.h"
#include "sf33rd/Source/Game/effect/effk5.h"
#include "sf33rd/Source/Game/effect/effk7.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/charset.h"
#include "sf33rd/Source/Game/engine/cmd_main.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/hitcheck.h"
#include "sf33rd/Source/Game/engine/manage.h"
#include "sf33rd/Source/Game/engine/plmain.h"
#include "sf33rd/Source/Game/engine/plpdm.h"
#include "sf33rd/Source/Game/engine/pls01.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/slowf.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/main.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"

#if defined(DEBUG)
#include "sf33rd/Source/Game/debug/debug_config.h"
#endif

#include <SDL3/SDL.h>

void pli_0000();
void pli_1000();
void move_player_work();
void move_P1_move_P2();
void move_P2_move_P1();
void check_damage_hosei();
void check_damage_hosei_nage(PLW* as, PLW* ds);
void check_damage_hosei_dageki(PLW* w1, PLW* w2);
s32 time_over_check();
s32 will_die();
void setup_settle_rno(s16 kos);
void settle_check();
s32 check_sa_resurrection(PLW* wk);
s16 nekorobi_check(s8 ix);
s16 footwork_check(s8 ix);
void setup_gouki_wins();
void setup_any_data();
void set_base_data(PLW* wk, s16 ix);
void set_base_data_metamorphose(PLW* wk, s16 dmid);
void set_base_data_tiny(PLW* wk);
void setup_other_data(PLW* wk);
s16 remake_sa_store_max(s16 ix, s16 store_max);
s16 remake_sa_gauge_len(s16 ix, s16 gauge_len);
void clear_super_arts_point(PLW* wk);
void set_scrrrl();

// bss
ComboType combo_type[2]; // FIXME: move to PLW instead?
ZanzouTableEntry zanzou_table[2][48];
ComboType remake_power[2]; // FIXME: move to PLW instead?

// sbss
s16 pcon_rno[4];
s16 appear_type;
u8 round_slow_flag;
u8 pcon_dp_flag;
u8 win_sp_flag;
char dead_voice_flag;
SA_WORK super_arts[2];     // FIXME: move to game state (or to PLW?)
PiyoriType piyori_type[2]; // FIXME: move to game state (or to PLW?)
RAMBOD rambod[2];
RAMHAN ramhan[2];
u32 omop_spmv_ng_table[2];
u32 omop_spmv_ng_table2[2];
u16 vital_inc_timer;
u16 vital_dec_timer;
char cmd_sel[2];
s8 vib_sel[2];
s16 sag_inc_timer[2];
char no_sa[2];

void plcnt_init();
void plcnt_move();
void plcnt_die();

void (*const player_main_process[3])() = { plcnt_init, plcnt_move, plcnt_die };

void init_app_10000();
void init_app_20000();
void init_app_30000();

void (*const appear_initalize[4])() = { init_app_10000, init_app_10000, init_app_20000, init_app_30000 };

void settle_type_00000();
void settle_type_10000();
void settle_type_20000();
void settle_type_30000();
void settle_type_40000();

void (*const settle_process[5])() = {
    settle_type_00000, settle_type_10000, settle_type_20000, settle_type_30000, settle_type_40000
};

const s8 plid_data[20] = { 6, 3, 5, 1, 2, 9, 7, 4, 10, 8, 12, 13, 14, 15, 16, 18, 19, 20, 21, 22 };

const s8 weight_lv_table[20] = { 2, 2, 1, 0, 1, 2, 3, 0, 1, 0, 0, 1, 1, 2, 1, 1, 1, 3, 2, 1 };

const s16 quake_table[64] = { 0, -1, 0, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -2, 1, -2, 1, -2, 1, -2, 1, -2,
                              1, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -2, 2, -3, 2, -3,
                              2, -3, 2, -3, 2, -3, 2, -3, 2, -3, 2, -3, 2, -3, 2, -3, 3, -3, 3, -3 };

const s16 kage_base[20][2] = { { 0, 21 },  { 0, 30 },  { 0, 21 },  { -4, 16 }, { 4, 21 }, { 6, 20 }, { -4, 26 },
                               { -4, 20 }, { 0, 25 },  { -3, 22 }, { -4, 16 }, { 0, 21 }, { 0, 21 }, { 0, 21 },
                               { 0, 21 },  { -8, 21 }, { 0, 23 },  { 0, 24 },  { 6, 25 }, { -6, 21 } };

const SA_DATA super_arts_data[20][4] = { { { 20, 24, 25, 0, 0, 0, 0, 3, 128, 1, 65536 },
                                           { 21, 24, 25, 0, 0, 0, 0, 3, 128, 1, 65536 },
                                           { 22, 24, 25, 0, 0, 0, 0, 3, 128, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 128, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 96, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 80, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 112, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 128, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 96, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65536 } },
                                         { { 22, 0, 0, 0, 0, 0, 0, 0, 120, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 88, 3, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 1, 72, 1, 16384 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 96, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 112, 1, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 80, 3, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 88, 1, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 104, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 72, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 128, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 112, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 88, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 0, 0, 0, 38, 0, 0, 0, 0, 88, 3, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 1, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 0, 80, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 80, 3, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 104, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 128, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 22, 0, 0, 0, 0, 0, 1, 1, 104, 1, 16384 },
                                           { 21, 24, 0, 0, 0, 0, 0, 0, 88, 3, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 1, 1, 112, 1, 13107 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 0, 65536 } },
                                         { { 21, 0, 0, 0, 0, 0, 0, 0, 120, 1, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 104, 2, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 1, 64, 1, 13107 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 112, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 104, 1, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 80, 3, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 72, 3, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 96, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 104, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 80, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 88, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 24, 25, 38, 0, 0, 0, 0, 112, 2, 65536 },
                                           { 21, 24, 25, 0, 0, 0, 0, 0, 112, 2, 65536 },
                                           { 22, 24, 25, 39, 0, 0, 0, 0, 112, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 88, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 104, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 72, 3, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 22, 0, 0, 0, 0, 0, 0, 0, 120, 1, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 0, 88, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 1, 96, 1, 10922 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 96, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 1, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 1, 112, 1, 10922 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 96, 2, 65536 },
                                           { 0, 0, 0, 38, 0, 0, 0, 0, 112, 1, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 1, 128, 1, 6553 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 104, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 80, 1, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } } };

const SA_DATA super_arts_DATA[20][4] = { { { 20, 24, 25, 0, 0, 0, 0, 3, 120, 2, 65536 },
                                           { 21, 24, 25, 0, 0, 0, 0, 3, 120, 2, 65536 },
                                           { 22, 24, 25, 0, 0, 0, 0, 3, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65536 } },
                                         { { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 1, 120, 2, 16384 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 0, 0, 0, 38, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 22, 24, 0, 0, 0, 0, 1, 1, 120, 2, 16384 },
                                           { 21, 24, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 20, 24, 0, 0, 0, 0, 1, 1, 120, 2, 13107 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 0, 65536 } },
                                         { { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 1, 120, 2, 13107 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 3, 64, 1, 65536 } },
                                         { { 20, 24, 25, 38, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 24, 25, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 24, 25, 39, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 3, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 1, 120, 2, 10922 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 1, 120, 2, 10922 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 38, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 1, 120, 2, 6553 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } },
                                         { { 20, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 22, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 21, 0, 0, 0, 0, 0, 0, 0, 120, 2, 65536 },
                                           { 0, 0, 0, 0, 0, 0, 0, 0, 64, 1, 65536 } } };

const s16 pl_piyo_tbl[20] = { 72, 72, 64, 64, 72, 64, 72, 64, 64, 72, 64, 64, 64, 64, 56, 64, 64, 72, 64, 56 };

const s32 pl_nr_piyo_tbl[20] = { 3276, 2849, 2978, 2730, 2978, 2849, 3120, 2730, 2978, 2730,
                                 2730, 2978, 2849, 3120, 2849, 2978, 3276, 2978, 2849, 3120 };

const s16 tsuujyou_dageki_00[16] = { 150, 150, 130, 130, 130, 110, 110, 110, 110, 110, 90, 90, 90, 90, 90, 90 };
const s16 tsuujyou_dageki_01[16] = { 150, 150, 150, 150, 130, 130, 130, 130, 110, 110, 110, 110, 90, 90, 90, 90 };
const s16 tsuujyou_dageki_02[16] = { 150, 150, 150, 150, 150, 150, 130, 130, 130, 130, 130, 110, 110, 110, 90, 90 };

const s16* tsuujyou_dageki[4] = { tsuujyou_dageki_00, tsuujyou_dageki_01, tsuujyou_dageki_02, tsuujyou_dageki_02 };

const s16 hissatsu_dageki_00[16] = { 210, 210, 190, 190, 190, 170, 170, 170, 170, 170, 150, 150, 150, 150, 150, 150 };
const s16 hissatsu_dageki_01[16] = { 210, 210, 210, 210, 190, 190, 190, 190, 170, 170, 170, 170, 150, 150, 150, 150 };
const s16 hissatsu_dageki_02[16] = { 210, 210, 210, 210, 210, 210, 190, 190, 190, 190, 190, 170, 170, 170, 150, 150 };

const s16* hissatsu_dageki[4] = {
    hissatsu_dageki_00,
    hissatsu_dageki_01,
    hissatsu_dageki_02,
    hissatsu_dageki_02,
};

const s16 tsuujyou_nage_00[16] = { 180, 180, 160, 160, 160, 140, 140, 140, 140, 140, 120, 120, 120, 120, 120, 120 };
const s16 tsuujyou_nage_01[16] = { 180, 180, 180, 180, 160, 160, 160, 160, 140, 140, 140, 140, 120, 120, 120, 120 };
const s16 tsuujyou_nage_02[16] = { 180, 180, 180, 180, 180, 180, 160, 160, 160, 160, 160, 140, 140, 140, 120, 120 };

const s16* tsuujyou_nage[4] = { tsuujyou_nage_00, tsuujyou_nage_01, tsuujyou_nage_02, tsuujyou_nage_02 };

const s16 hissatsu_nage_00[16] = { 240, 240, 200, 200, 200, 160, 160, 160, 160, 160, 120, 120, 120, 120, 120, 120 };
const s16 hissatsu_nage_01[16] = { 240, 240, 240, 240, 200, 200, 200, 200, 160, 160, 160, 160, 120, 120, 120, 120 };
const s16 hissatsu_nage_02[16] = { 240, 240, 240, 240, 240, 240, 200, 200, 200, 200, 200, 160, 160, 160, 120, 120 };

const s16* hissatsu_nage[4] = { hissatsu_nage_00, hissatsu_nage_01, hissatsu_nage_02, hissatsu_nage_02 };

const s16 super_arts_dageki_00[16] = { 240, 240, 200, 200, 200, 160, 160, 160, 160, 160, 120, 120, 120, 120, 120, 120 };
const s16 super_arts_dageki_01[16] = { 240, 240, 240, 240, 200, 200, 200, 200, 160, 160, 160, 160, 120, 120, 120, 120 };
const s16 super_arts_dageki_02[16] = { 240, 240, 240, 240, 240, 240, 200, 200, 200, 200, 200, 160, 160, 160, 120, 120 };

const s16* super_arts_dageki[4] = {
    super_arts_dageki_00, super_arts_dageki_01, super_arts_dageki_02, super_arts_dageki_02
};

const s16 super_arts_nage_00[16] = { 270, 270, 230, 230, 230, 190, 190, 190, 190, 190, 150, 150, 150, 150, 150, 150 };
const s16 super_arts_nage_01[16] = { 270, 270, 270, 270, 230, 230, 230, 230, 190, 190, 190, 190, 150, 150, 150, 150 };
const s16 super_arts_nage_02[16] = { 270, 270, 270, 270, 270, 270, 230, 230, 230, 230, 230, 190, 190, 190, 150, 150 };

const s16* super_arts_nage[4] = { super_arts_nage_00, super_arts_nage_01, super_arts_nage_02, super_arts_nage_02 };

const s16** kizetsu_timer_table[9] = { tsuujyou_dageki,   hissatsu_dageki,   tsuujyou_nage,
                                       hissatsu_nage,     super_arts_dageki, super_arts_nage,
                                       super_arts_dageki, super_arts_nage,   super_arts_dageki };

void Player_control() {
    pulpul_scene = 1;

    if (pcon_rno[0] + pcon_rno[1] != 0) {
        if (Game_pause || EXE_flag) {
            goto end;
        } else {
            if (pcon_dp_flag == 0) {
                if (--vital_inc_timer > 50) {
                    vital_inc_timer = 50;
                }

                if (--vital_dec_timer > 40) {
                    vital_dec_timer = 40;
                }
            } else {
                vital_inc_timer = 50;
                vital_dec_timer = 40;
                sag_inc_timer[0] = sag_inc_timer[1] = 20;
            }
        }
    }

    players_timer++;
    players_timer &= 0x7FFF;
    set_scrrrl();
    player_main_process[pcon_rno[0]]();
    check_body_touch();
    check_damage_hosei();
    set_quake(&gs.plw[0]);
    set_quake(&gs.plw[1]);

    if (!gs.plw[0].zuru_flag && !gs.plw[0].zettai_muteki_flag) {
        hit_push_request(&gs.plw[0].wu);
    }

    if (!gs.plw[1].zuru_flag && !gs.plw[1].zettai_muteki_flag) {
        hit_push_request(&gs.plw[1].wu);
    }

    add_next_position(&gs.plw[0]);
    add_next_position(&gs.plw[1]);
    check_cg_zoom();

end:
    if (Game_pause != 0x81) {
        store_player_after_image_data();
    }
}

void reqPlayerDraw() {
    if (Debug_w[15] == 0) {
        move_effect_work(6);
        sort_push_request(&gs.plw[0].wu);
        sort_push_request(&gs.plw[1].wu);
    }
}

void plcnt_init() {
    gs.plw[0].reserv_add_y = gs.plw[1].reserv_add_y = 0;
    appear_initalize[appear_type]();
    move_player_work();
}

void init_app_10000() {
    switch (pcon_rno[1]) {
    case 0:
        pli_0000();
        pcon_rno[1] = 2;
        pcon_dp_flag = 0;
        round_slow_flag = 0;
        dead_voice_flag = 0;
        another_bg[0] = another_bg[1] = 0;
        gs.plw[0].scr_pos_set_flag = gs.plw[1].scr_pos_set_flag = 1;

        if (Play_Type == 0) {
            if (gs.plw[0].wu.operator) {
                mpp_w.useChar[My_char[0]]++;
            }

            if (gs.plw[1].wu.operator) {
                mpp_w.useChar[My_char[1]]++;
            }
        }

        break;

    case 1:
        pli_1000();
        break;

    case 2:
        pcon_rno[1] = 3;

        if (gs.plw[0].wu.operator) {
            paring_ctr_vs[0][0] = paring_ctr_ori[0];
        } else {
            paring_ctr_vs[0][0] = 0;
        }

        if (gs.plw[1].wu.operator) {
            paring_ctr_vs[0][1] = paring_ctr_ori[1];
            return;
        }

        paring_ctr_vs[0][1] = 0;
        break;

    case 3:
        pcon_rno[1] = 1;
        pli_0002();
        break;
    }
}

void init_app_20000() {
    s16 i;

    switch (pcon_rno[1]) {
    case 0:
        pcon_rno[1]++;
        round_slow_flag = 0;
        dead_voice_flag = 0;
        pcon_dp_flag = 0;
        another_bg[0] = another_bg[1] = 0;

        for (i = 0; i < 8; i++) {
            gs.plw[0].wu.routine_no[i] = gs.plw[1].wu.routine_no[i] = 0;
        }

        setup_any_data();
        gs.plw[0].do_not_move = gs.plw[1].do_not_move = 0;
        gs.plw[0].scr_pos_set_flag = gs.plw[1].scr_pos_set_flag = 1;
        break;

    case 1:
        pli_1000();
        break;
    }
}

void init_app_30000() {
    s16 i;

    switch (pcon_rno[1]) {
    case 0:
        pcon_rno[1]++;
        round_slow_flag = 0;
        dead_voice_flag = 0;

        for (i = 1; i < 8; i++) {
            gs.plw[0].wu.routine_no[i] = gs.plw[1].wu.routine_no[i] = 0;
        }

        gs.plw[0].wu.routine_no[0] = gs.plw[1].wu.routine_no[0] = 1;
        another_bg[0] = another_bg[1] = 0;
        gs.plw[0].do_not_move = gs.plw[1].do_not_move = 0;
        K7_muriyari_metamor_rebirth(&gs.plw[0]);
        K7_muriyari_metamor_rebirth(&gs.plw[1]);
        break;

    case 1:
        if (gs.plw[0].wu.routine_no[0] != 3 || gs.plw[1].wu.routine_no[0] != 3) {
            break;
        }

        pcon_rno[0] = 2;
        pcon_rno[1] = 3;
        pcon_rno[2] = 1;
        setup_EJG_index();
        effect_C9_init(gs.plw, 0);
        effect_C9_init(gs.plw, 1);
        effect_C9_init(gs.plw, 2);
        load_any_color(0x3F, 0);
        load_any_texture_patnum(0x71E0, 0xE, 0);
        effect_work_kill(4, 0xD9);

        if (gs.plw[0].player_number == 6) {
            effect_33_init(&gs.plw[0].wu);
        }

        if (gs.plw[1].player_number == 6) {
            effect_33_init(&gs.plw[1].wu);
        }

        break;
    }
}

void pli_0000() {
    pcon_rno[1]++;
    round_slow_flag = 0;
    SDL_zeroa(gs.plw);
    setup_base_and_other_data();
}

void pli_1000() {
    if (gs.plw[0].wu.routine_no[0] != 3) {
        return;
    }

    if (gs.plw[1].wu.routine_no[0] != 3) {
        return;
    }

    if (!Allow_a_battle_f) {
        return;
    }

    pcon_rno[0] = 1;
    pcon_rno[1] = 0;
    gs.plw[0].wu.routine_no[0] = 4;
    gs.plw[1].wu.routine_no[0] = 4;
    ca_check_flag = 1;
}

void pli_0002() {
    // Do nothing
}

void plcnt_move() {
    if (time_over_check() != 0) {
        return;
    }

#if defined(DEBUG)
    if (debug_config.player_1_invincible) {
        gs.plw[0].wu.dm_vital = 0;
    }

    if (debug_config.player_2_invincible) {
        gs.plw[1].wu.dm_vital = 0;
    }

    if (debug_config.player_1_one_hit_ko) {
        gs.plw[0].wu.vital_new = 0;
    }

    if (debug_config.player_2_one_hit_ko) {
        gs.plw[1].wu.vital_new = 0;
    }
#endif

    if (No_Death) {
        gs.plw[0].wu.dm_vital = gs.plw[1].wu.dm_vital = 0;
    }

    if (Break_Into) {
        gs.plw[0].wu.dm_vital = gs.plw[1].wu.dm_vital = 0;
    }

    if (Mode_Type == MODE_NORMAL_TRAINING && Training->contents[0][1][3] == 0) {
        gs.plw[0].wu.dm_nodeathattack = 1;
        gs.plw[1].wu.dm_nodeathattack = 1;
    }

    move_player_work();

    if (aiuchi_flag) {
        subtract_dm_vital_aiuchi(&gs.plw[0]);
        subtract_dm_vital_aiuchi(&gs.plw[1]);

        if ((gs.plw[0].dead_flag != 0) && (gs.plw[1].dead_flag != 0)) {
            gs.plw[0].wu.hit_stop = gs.plw[1].wu.hit_stop = 2;
            gs.plw[0].wu.dm_stop = gs.plw[1].wu.dm_stop = 0;
            gs.plw[0].wu.hit_quake = gs.plw[1].wu.hit_quake = 4;
            gs.plw[0].wu.dm_quake = gs.plw[1].wu.dm_quake = 0;
        } else if ((gs.plw[0].dead_flag != 0) || (gs.plw[1].dead_flag != 0)) {
            gs.plw[0].wu.hit_stop = gs.plw[1].wu.hit_stop = 4;
            gs.plw[0].wu.dm_stop = gs.plw[1].wu.dm_stop = 0;
            gs.plw[0].wu.hit_quake = gs.plw[1].wu.hit_quake = 8;
            gs.plw[0].wu.dm_quake = gs.plw[1].wu.dm_quake = 0;
        }
    }

    settle_check();

    if (pcon_rno[0] == 2) {
        if (Round_Result & 0x980) {
            if ((Round_Result & 0x800) && gouki_wins) {
                effect_D3_init(1);
            } else {
                effect_D3_init(0);
            }
        }

        if ((gs.plw[0].kezurijini_flag == 1) || (gs.plw[1].kezurijini_flag == 1)) {
            Round_Result |= 0x200;
        }

        if (Winner_id != Loser_id) {
            grade_store_vitality(Winner_id + 0);
        }
    }

    grade_check_tairyokusa();
}

void plcnt_die() {
    gs.plw[0].wu.dm_vital = gs.plw[1].wu.dm_vital = 0;
    settle_process[pcon_rno[1]]();
    move_player_work();

    if (pcon_rno[1] == 3) {
        gs.plw[0].scr_pos_set_flag = gs.plw[1].scr_pos_set_flag = 0;
    }
}

void settle_type_00000() {
    switch (pcon_rno[2]) {
    case 0:
        gs.plw[Winner_id].wu.dir_timer = 60;
        pcon_rno[2]++;
        /* fallthrough */

    case 1:
        if (nekorobi_check(Loser_id)) {
            pcon_rno[2]++;
            gs.plw[Winner_id].wkey_flag = 1;
        }

        if (--gs.plw[Winner_id].wu.dir_timer == 0) {
            gs.plw[Winner_id].wkey_flag = 1;
        }

        break;

    case 2:
        if (footwork_check(Winner_id)) {
            grade_set_round_result(Winner_id + 0);
            pcon_rno[2]++;
            gs.plw[Winner_id].wu.routine_no[2] = 40;
            gs.plw[Winner_id].wu.routine_no[3] = 0;
            gs.plw[Loser_id].wu.routine_no[1] = 0;
            gs.plw[Loser_id].wu.routine_no[2] = 41;
            gs.plw[Loser_id].wu.routine_no[3] = 0;
            gs.plw[0].wu.cg_type = gs.plw[1].wu.cg_type = 0;
            gs.plw[0].image_setup_flag = gs.plw[1].image_setup_flag = 0;
            complete_victory_pause();
        }

        break;

    case 3:
        if (gs.plw[Winner_id].wu.routine_no[3] == 9) {
            pcon_rno[2]++;
        }

        break;
    }
}

void settle_type_10000() {
    switch (pcon_rno[2]) {
    case 0:
        if (nekorobi_check(0) && nekorobi_check(1)) {
            pcon_rno[2]++;
        }

        break;

    case 1:
        complete_victory_pause();
        pcon_rno[2]++;
        gs.plw[0].image_setup_flag = gs.plw[1].image_setup_flag = 0;
        break;
    }
}

void settle_type_20000() {
    switch (pcon_rno[2]) {
    case 0:
        gs.plw[0].wkey_flag = gs.plw[1].wkey_flag = 1;
        gs.plw[0].image_setup_flag = gs.plw[1].image_setup_flag = 0;
        pcon_rno[2]++;
        /* fallthrough */

    case 1:
        if (footwork_check(0) && footwork_check(1)) {
            pcon_rno[2]++;
        }

        break;

    case 2:
        complete_victory_pause();

        if (gs.plw[0].wu.vital_new == gs.plw[1].wu.vital_new) {
            pcon_rno[2] = 4;
            return;
        }

        grade_set_round_result(Winner_id + 0);
        gs.plw[Winner_id].wu.routine_no[2] = 40;
        gs.plw[Loser_id].wu.routine_no[2] = 41;
        gs.plw[0].wu.routine_no[1] = gs.plw[1].wu.routine_no[1] = 0;
        gs.plw[0].wu.routine_no[3] = gs.plw[1].wu.routine_no[3] = 0;
        gs.plw[0].wu.cg_type = gs.plw[1].wu.cg_type = 0;
        pcon_rno[2]++;
        break;

    case 3:
        if ((gs.plw[0].wu.routine_no[3] == 9) && (gs.plw[1].wu.routine_no[3] == 9)) {
            pcon_rno[2]++;
        }

        break;
    }
}

void settle_type_30000() {
    switch (pcon_rno[2]) {
    case 0:
        break;

    case 1:
        if ((Event_Judge_Gals == -1) && Complete_Judgement) {
            gs.plw[Winner_id].wu.routine_no[2] = 40;
            gs.plw[Loser_id].wu.routine_no[2] = 41;
            gs.plw[0].wu.routine_no[3] = gs.plw[1].wu.routine_no[3] = 0;
            gs.plw[0].wu.cg_type = gs.plw[1].wu.cg_type = 0;
            grade_set_round_result(Winner_id + 0);
            complete_victory_pause();
            pcon_rno[2]++;
        }

        break;

    case 2:
        if ((gs.plw[0].wu.routine_no[3] == 9) && (gs.plw[1].wu.routine_no[3] == 9)) {
            pcon_rno[2]++;
        }

        break;
    }
}

void settle_type_40000() {
    switch (pcon_rno[2]) {
    case 0:
        gs.plw[Winner_id].wkey_flag = 1;
        pcon_rno[2]++;
        /* fallthrough */

    case 1:
        if (nekorobi_check(Loser_id) != 0) {
            pcon_rno[2]++;
        case 2:
            if (footwork_check(Winner_id)) {
                pcon_rno[2]++;
                gs.plw[Winner_id].wu.routine_no[2] = 40;
                gs.plw[Winner_id].wu.routine_no[3] = 0;
                gs.plw[Loser_id].wu.routine_no[1] = 0;
                gs.plw[Loser_id].wu.routine_no[2] = 41;
                gs.plw[Loser_id].wu.routine_no[3] = 0;
                gs.plw[Winner_id].wu.cg_type = 0;
                grade_set_round_result(Winner_id + 0);
                gs.plw[0].image_setup_flag = gs.plw[1].image_setup_flag = 0;
                gs.plw[Winner_id].wu.dir_timer = 60;
                set_conclusion_slow();
                return;
            }
        }

        break;

    case 3:
        if (--gs.plw[Winner_id].wu.dir_timer <= 0) {
            complete_victory_pause();
            pcon_rno[2]++;
        }

        break;

    case 4:
        if (gs.plw[Winner_id].wu.routine_no[3] == 9) {
            pcon_rno[2]++;
        }

        break;
    }
}

void move_player_work() {
    if (gs.plw[0].reserv_add_y) {
        gs.plw[0].wu.xyz[1].disp.pos += gs.plw[0].reserv_add_y;
        gs.plw[0].reserv_add_y = 0;
    }

    if (gs.plw[1].reserv_add_y) {
        gs.plw[1].wu.xyz[1].disp.pos += gs.plw[1].reserv_add_y;
        gs.plw[1].reserv_add_y = 0;
    }

    ichikannkei = check_work_position(&gs.plw[0].wu, &gs.plw[1].wu);
    set_rl_waza(&gs.plw[0]);
    set_rl_waza(&gs.plw[1]);
    Timer_Freeze = 0;

    switch (gs.plw[0].tsukami_f + (gs.plw[1].tsukami_f * 2)) {
    case 1:
        move_P1_move_P2();
        break;

    case 2:
        move_P2_move_P1();
        break;

    default:
        switch (gs.plw[0].wu.operator + (gs.plw[1].wu.operator * 2)) {
        case 1:
            move_P1_move_P2();
            break;

        case 2:
            move_P2_move_P1();
            break;

        default:
            switch ((gs.plw[0].wu.routine_no[1] == 4) + ((gs.plw[1].wu.routine_no[1] == 4) * 2)) {
            case 1:
                move_P1_move_P2();
                break;

            case 2:
                move_P2_move_P1();
                break;

            default:
                if (Game_timer & 1) {
                    move_P1_move_P2();
                    break;
                }

                move_P2_move_P1();
                break;
            }

            break;
        }

        break;
    }
}

void move_P1_move_P2() {
    if (gs.plw[0].do_not_move == 0) {
        Player_move(&gs.plw[0], processed_lvbt(Convert_User_Setting(0)));
    }

    if (bg_app_stop == 0 && bg_app == 0 && set_field_hosei_flag(&gs.plw[0], scrr, 1) != 0) {
        set_field_hosei_flag(&gs.plw[0], scrl, 0);
    }

    if (gs.plw[1].do_not_move == 0) {
        Player_move(&gs.plw[1], processed_lvbt(Convert_User_Setting(1)));
    }

    if (bg_app_stop == 0 && bg_app == 0 && set_field_hosei_flag(&gs.plw[1], scrr, 1) != 0) {
        set_field_hosei_flag(&gs.plw[1], scrl, 0);
    }
}

void move_P2_move_P1() {
    if (gs.plw[1].do_not_move == 0) {
        Player_move(&gs.plw[1], processed_lvbt(Convert_User_Setting(1)));
    }

    if (bg_app_stop == 0 && bg_app == 0 && set_field_hosei_flag(&gs.plw[1], scrr, 1) != 0) {
        set_field_hosei_flag(&gs.plw[1], scrl, 0);
    }

    if (gs.plw[0].do_not_move == 0) {
        Player_move(&gs.plw[0], processed_lvbt(Convert_User_Setting(0)));
    }

    if (bg_app_stop == 0 && bg_app == 0 && set_field_hosei_flag(&gs.plw[0], scrr, 1) != 0) {
        set_field_hosei_flag(&gs.plw[0], scrl, 0);
    }
}

void store_player_after_image_data() {
    s16 i;

    for (i = 47; i > 0; i--) {
        zanzou_table[0][i] = zanzou_table[0][i - 1];
        zanzou_table[1][i] = zanzou_table[1][i - 1];
    }

    for (i = 0; i < 2; i++) {
        zanzou_table[i]->pos_x = gs.plw[i].wu.position_x;
        zanzou_table[i]->pos_y = gs.plw[i].wu.position_y;
        zanzou_table[i]->pos_z = gs.plw[i].wu.position_z;
        zanzou_table[i]->cg_num = gs.plw[i].wu.cg_number;
        zanzou_table[i]->renew = gs.plw[i].wu.renew_attack;
        zanzou_table[i]->hit_ix = gs.plw[i].wu.cg_hit_ix;
        zanzou_table[i]->flip = gs.plw[i].wu.rl_flag;
        zanzou_table[i]->cg_flp = gs.plw[i].wu.cg_flip;
        zanzou_table[i]->kowaza = gs.plw[i].wu.kind_of_waza;
    }
}

void check_damage_hosei() {
    gs.plw[0].muriyari_ugoku = gs.plw[0].hosei_amari;
    gs.plw[1].muriyari_ugoku = gs.plw[1].hosei_amari;

    if (gs.plw[0].tsukami_f && gs.plw[1].tsukamare_f) {
        check_damage_hosei_nage(&gs.plw[0], &gs.plw[1]);
    } else if (gs.plw[1].tsukami_f && gs.plw[0].tsukamare_f) {
        check_damage_hosei_nage(&gs.plw[1], &gs.plw[0]);
    } else {
        switch ((gs.plw[0].hosei_amari != 0) + ((gs.plw[1].hosei_amari != 0) * 2)) {
        case 1:
            check_damage_hosei_dageki(&gs.plw[0], &gs.plw[1]);
            break;
        case 2:
            check_damage_hosei_dageki(&gs.plw[1], &gs.plw[0]);
            break;
        }
    }

    gs.plw[0].hosei_amari = gs.plw[1].hosei_amari = 0;
}

void check_damage_hosei_nage(PLW* as, PLW* ds) {
    if (as->kind_of_catch) {
        if (ds->hosei_amari != 0) {
            as->wu.xyz[0].disp.pos += ds->hosei_amari;
            as->muriyari_ugoku += ds->hosei_amari;
            return;
        }

        if (bg_app_stop == 0 && bg_app == 0 && set_field_hosei_flag(as, scrr, 1) != 0) {
            set_field_hosei_flag(as, scrl, 0);
        }

        if (as->hosei_amari != 0) {
            ds->wu.xyz[0].disp.pos += as->hosei_amari;
            ds->muriyari_ugoku += as->hosei_amari;
        }
    } else if (ds->hosei_amari != 0) {
        as->wu.xyz[0].disp.pos += ds->hosei_amari;
        as->muriyari_ugoku += ds->hosei_amari;
    }
}

void check_damage_hosei_dageki(PLW* w1, PLW* w2) {
    if ((w1->dm_hos_flag != 0) && (w2->wu.hit_stop == 0)) {
        w2->wu.xyz[0].disp.pos += w1->hosei_amari;
        w2->muriyari_ugoku += w1->hosei_amari;
    }
}

s32 time_over_check() {
    if ((will_die() != 0) && (round_timer.size.full == 0)) {
        Winner_id = 0;
        Loser_id = 1;

        if (gs.plw[0].wu.vital_new < gs.plw[1].wu.vital_new) {
            Winner_id = 1;
            Loser_id = 0;
        }

        Conclusion_Flag = 1;
        Conclusion_Type = 2;
        setup_settle_rno(2);

        if (Demo_Flag) {
            request_center_message(2);
        }

        gs.plw[0].wu.dm_vital = gs.plw[1].wu.dm_vital = 0;
        Round_Result |= 1;
        return 1;
    }

    return 0;
}

s32 will_die() {
    if (gs.plw[0].wu.dm_vital > gs.plw[0].wu.vital_new) {
        return 0;
    }

    if (gs.plw[1].wu.dm_vital > gs.plw[1].wu.vital_new) {
        return 0;
    }

    return 1;
}

void setup_settle_rno(s16 kos) {
    pcon_rno[0] = 2;
    pcon_rno[1] = kos;
    pcon_rno[2] = 0;
    ca_check_flag = 0;
    pcon_dp_flag = 1;
}

void settle_check() {
    while (1) {
        switch ((gs.plw[0].dead_flag) + (gs.plw[1].dead_flag * 2)) {
        case 1:
            if (1) {
                Winner_id = 1;
                Loser_id = 0;
            } else {
                /* fallthrough */

            case 2:
                Winner_id = 0;
                Loser_id = 1;
            }

            if (check_sa_resurrection(&gs.plw[Loser_id]) == 0) {
                setup_gouki_wins();
                Round_Result |= gs.plw[Loser_id].wu.dm_koa;

                if ((Round_Result & 0x800) && gouki_wins) {
                    Forbid_Break = -1;
                    Shin_Gouki_BGM = 1;
                    Control_Music_Fade(0x96);
                    setup_settle_rno(4);
                    break;
                }

                setup_settle_rno(0);
                Conclusion_Flag = 1;
                Conclusion_Type = 0;

                if (Demo_Flag) {
                    request_center_message(0);
                }
            }

            break;

        case 3:
            if ((check_sa_resurrection(&gs.plw[0]) == 0) && (check_sa_resurrection(&gs.plw[1]) == 0)) {
                Conclusion_Flag = 1;
                Conclusion_Type = 1;
                setup_settle_rno(1);

                if (Demo_Flag) {
                    request_center_message(1);
                }
            } else {
                continue;
            }

            break;

        default:
            break;
        }

        break;
    }
}

s32 check_sa_resurrection(PLW* wk) {
    if (check_sa_type_rebirth(wk) == 0) {
        return 0;
    }

    wk->kezurijini_flag = 0;
    wk->dead_flag = 0;
    wk->resurrection_resv = 1;
    return 1;
}

s32 check_sa_type_rebirth(PLW* wk) {
    if ((wk->spmv_ng_flag & 0x40000000) || (wk->spmv_ng_flag & 0x80000000)) {
        return 0;
    }

    if (wk->sa->gauge_type != 3) {
        return 0;
    }

    if (wk->sa->ok != 1) {
        return 0;
    }

    return 1;
}

s16 nekorobi_check(s8 ix) {
    s16 rnum = 0;

    if ((gs.plw[ix].wu.routine_no[1] == 1) && (gs.plw[ix].wu.routine_no[2] == 0) && (gs.plw[ix].wu.routine_no[3] > 2)) {
        rnum = 1;
    }

    return rnum;
}

s16 footwork_check(s8 ix) {
    s16 rnum = 0;

    if (gs.plw[ix].wu.routine_no[1] == 0 && gs.plw[ix].wu.routine_no[2] == 1) {
        rnum = 1;
    }

    return rnum;
}

void set_quake(PLW* wk) {
    if (wk->wu.hit_quake) {
        wk->wu.hit_quake--;
        wk->wu.next_x = quake_table[wk->wu.hit_quake];

        if (wk->wu.rl_flag) {
            wk->wu.next_x = -wk->wu.next_x;
        }
    } else {
        wk->wu.next_x = 0;
    }
}

void add_next_position(PLW* wk) {
    wk->wu.position_x = wk->wu.xyz[0].disp.pos + wk->wu.next_x;
    wk->wu.position_y = wk->wu.xyz[1].disp.pos + wk->wu.next_y;
    wk->wu.position_z = wk->wu.next_z;
    wk->wu.next_y = 0;
}

void setup_gouki_wins() {
    gouki_wins = 0;

    if (gs.plw[Winner_id].player_number == 14) {
        gouki_wins = 1;
    }
}

void erase_extra_plef_work() {
    effect_work_list_init(0, 0);
    effect_work_list_init(1, 1);
    effect_work_list_init(3, 0x91);
    effect_work_list_init(3, 0x93);
    effect_work_list_init(3, 0x94);
    effect_work_list_init(4, 0x81);
    effect_work_list_init(4, 0x25);
    effect_work_list_init(4, 0xAC);
    effect_work_list_init(6, -1);
}

void setup_base_and_other_data() {
    make_texcash_work(3);
    make_texcash_work(4);
    make_texcash_work(6);
    gs.plw[0].wu.my_mts = 3;
    gs.plw[1].wu.my_mts = 4;
    set_base_data(&gs.plw[0], 0);
    set_base_data(&gs.plw[1], 1);
    gs.plw[0].sa = &super_arts[0];
    gs.plw[1].sa = &super_arts[1];
    gs.plw[0].py = &piyori_type[0];
    gs.plw[1].py = &piyori_type[1];
    setup_other_data(&gs.plw[0]);
    setup_other_data(&gs.plw[1]);
    effect_work_list_init(6, 0xC5);
    gs.plw[0].gill_ccch_go = gs.plw[1].gill_ccch_go = 0;
    effect_J7_init(&gs.plw[0]);
    effect_J7_init(&gs.plw[1]);
    effect_E5_init(&gs.plw[0]);
    effect_E5_init(&gs.plw[1]);

    if (gs.plw[0].wu.my_priority == gs.plw[1].wu.my_priority) {
        gs.plw[0].the_same_players = gs.plw[1].the_same_players = 1;
    }

    poison_flag[0] = 0;
    poison_flag[1] = 0;

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        effect_E3_init(&gs.plw[0]);
        effect_E3_init(&gs.plw[1]);
        effect_E4_init(&gs.plw[0]);
        effect_E4_init(&gs.plw[1]);
    }
}

void setup_any_data() {
    set_base_data_tiny(&gs.plw[0]);
    set_base_data_tiny(&gs.plw[1]);
    setup_other_data(&gs.plw[0]);
    setup_other_data(&gs.plw[1]);
    effect_work_list_init(6, 0xC5);
    gs.plw[0].gill_ccch_go = gs.plw[1].gill_ccch_go = 0;
    effect_J7_init(&gs.plw[0]);
    effect_J7_init(&gs.plw[1]);
    effect_E5_init(&gs.plw[0]);
    effect_E5_init(&gs.plw[1]);

    if (gs.plw[0].wu.my_priority == gs.plw[1].wu.my_priority) {
        gs.plw[0].the_same_players = gs.plw[1].the_same_players = 1;
    }
}

void set_base_data(PLW* wk, s16 ix) {
    wk->wu.be_flag = 1;
    wk->wu.disp_flag = 0;
    wk->wu.blink_timing = ix;
    wk->wu.id = ix;
    wk->wu.work_id = 1;
    wk->wu.operator = Operator_Status[ix];
    wk->wu.charset_id = plid_data[My_char[ix]];
    wk->wkey_flag = wk->dead_flag = 0;
    set_char_base_data(&wk->wu);
    wk->wu.target_adrs = (u32*)&gs.plw[(ix + 1) & 1];
    wk->player_number = My_char[ix];
    wk->wu.hit_adrs = wk->wu.target_adrs;
    wk->wu.dmg_adrs = wk->wu.target_adrs;
    cmd_init(wk);
    wk->cb = &combo_type[ix];
    wk->rp = &remake_power[ix];

    if (ix) {
        wk->wu.my_col_code |= 0x10;
    }

    wk->spmv_ng_flag = omop_spmv_ng_table[wk->wu.id];
    wk->spmv_ng_flag2 = omop_spmv_ng_table2[wk->wu.id];
    wk->wu.weight_level = weight_lv_table[wk->player_number];
    set_player_shadow(wk);
    wk->wu.cg_olc_ix = wk->wu.cg_hit_ix = 0;
    wk->wu.cg_olc = wk->wu.olc_ix_table[wk->wu.cg_olc_ix];
    wk->wu.cg_ja = wk->wu.hit_ix_table[wk->wu.cg_hit_ix];

    set_jugde_area(&wk->wu);
}

void set_base_data_metamorphose(PLW* wk, s16 dmid) {
    set_char_base_data(&wk->wu);

    if (wk->wu.id) {
        wk->wu.my_col_code |= 0x10;
    }

    cmd_init(wk);
    wk->spmv_ng_flag = omop_spmv_ng_table[dmid];
    wk->spmv_ng_flag2 = omop_spmv_ng_table2[dmid];
    set_player_shadow(wk);
}

void set_base_data_tiny(PLW* wk) {
    wk->wu.charset_id = plid_data[My_char[wk->wu.id]];
    wk->player_number = My_char[wk->wu.id];
    set_char_base_data(&wk->wu);

    if (wk->wu.id) {
        wk->wu.my_col_code |= 0x10;
    }

    wk->wu.be_flag = 1;
    wk->wu.disp_flag = 0;
    wk->wkey_flag = wk->dead_flag = 0;
    cmd_init(wk);
    wk->spmv_ng_flag = omop_spmv_ng_table[wk->wu.id];
    wk->spmv_ng_flag2 = omop_spmv_ng_table2[wk->wu.id];
    wk->wu.weight_level = weight_lv_table[wk->player_number];
    set_player_shadow(wk);
}

void set_player_shadow(PLW* wk) {
    wk->wu.kage_flag = 1;
    wk->wu.kage_prio = 68;
    wk->wu.kage_hx = kage_base[wk->player_number][0];
    wk->wu.kage_char = kage_base[wk->player_number][1];
}

void setup_other_data(PLW* wk) {
    s16 i;

    if (wk->player_number == 0) {
        setup_GILL_exsa_obj();
    }

    for (i = 0; i < 4; i++) {
        effect_01_init(&wk->wu, i);
    }

    effect_K5_init(wk);
    effect_00_init(&wk->wu);
}

void clear_chainex_check(s16 ix) {
    s16 i;

    for (i = 0; i < 36; i++) {
        chainex_check[ix][i] = 0;
    }
}

void set_kizetsu_status(s16 ix) {
    s16 plnum = My_char[ix];

    piyori_type[ix].flag = 0;
    piyori_type[ix].time = 0;
    piyori_type[ix].now.timer = 0;
    piyori_type[ix].store = 0;
    piyori_type[ix].recover = pl_nr_piyo_tbl[plnum];
    piyori_type[ix].genkai = pl_piyo_tbl[plnum] + stun_gauge_len_omake[omop_stun_gauge_len[ix]];

    if (piyori_type[ix].genkai < 56) {
        piyori_type[ix].genkai = 56;
    }

    if (piyori_type[ix].genkai > 72) {
        piyori_type[ix].genkai = 72;
    }
}

void clear_kizetsu_point(PLW* wk) {
    wk->py->flag = 0;
    wk->py->time = 0;
    wk->py->now.timer = 0;
    wk->py->store = 0;
    wk->py->recover = pl_nr_piyo_tbl[wk->player_number];
}

void set_super_arts_status(s16 ix) {
    const SA_DATA* saptr;

    if (cmd_sel[ix] || no_sa[ix]) {
        saptr = &super_arts_DATA[My_char[ix]][Super_Arts[ix]];
    } else {
        saptr = &super_arts_data[My_char[ix]][Super_Arts[ix]];
    }

    super_arts[ix].kind_of_arts = Super_Arts[ix];
    super_arts[ix].nmsa_g_ix = saptr->nmsa_g_ix;
    super_arts[ix].exsa_g_ix = saptr->exsa_g_ix;
    super_arts[ix].exs2_g_ix = saptr->exs2_g_ix;
    super_arts[ix].nmsa_a_ix = saptr->nmsa_a_ix;
    super_arts[ix].exsa_a_ix = saptr->exsa_a_ix;
    super_arts[ix].exs2_a_ix = saptr->exs2_a_ix;
    super_arts[ix].ex4th_full = saptr->ex4th_full;
    super_arts[ix].gauge_type = saptr->gauge_type;
    super_arts[ix].gt2 = saptr->gauge_type;
    super_arts[ix].gauge_len = remake_sa_gauge_len(ix, saptr->gauge_len);
    super_arts[ix].store_max = remake_sa_store_max(ix, saptr->store_max);
    super_arts[ix].dtm = saptr->dtm;
    super_arts[ix].dtm_mul = 1;
    super_arts[ix].store = 0;
    super_arts[ix].gauge.s.h = 0;
    super_arts[ix].gauge.s.l = -1;
    super_arts[ix].sa_rno = 0;
    super_arts[ix].ok = 0;
}

s16 remake_sa_store_max(s16 ix, s16 store_max) {
    s16 num = store_max + sag_stock_omake[omop_sag_max_ix[ix]];

    if (num <= 0) {
        num = 1;
    }

    if (num > 9) {
        num = 9;
    }

    return num;
}

s16 remake_sa_gauge_len(s16 ix, s16 gauge_len) {
    s16 num = gauge_len + sag_length_omake[omop_sag_len_ix[ix]] * 8;

    if (num < 0x40) {
        num = 0x40;
    }

    if (num > 0x80) {
        num = 0x80;
    }

    return num;
}

void set_super_arts_status_dc(s16 ix) {
    const SA_DATA* saptr;

    if (cmd_sel[ix] || no_sa[ix]) {
        saptr = &super_arts_DATA[My_char[ix]][Super_Arts[ix]];
    } else {
        saptr = &super_arts_data[My_char[ix]][Super_Arts[ix]];
    }

    super_arts[ix].kind_of_arts = Super_Arts[ix];
    super_arts[ix].nmsa_g_ix = saptr->nmsa_g_ix;
    super_arts[ix].exsa_g_ix = saptr->exsa_g_ix;
    super_arts[ix].exs2_g_ix = saptr->exs2_g_ix;
    super_arts[ix].nmsa_a_ix = saptr->nmsa_a_ix;
    super_arts[ix].exsa_a_ix = saptr->exsa_a_ix;
    super_arts[ix].exs2_a_ix = saptr->exs2_a_ix;
    super_arts[ix].ex4th_full = saptr->ex4th_full;
    super_arts[ix].gauge_type = saptr->gauge_type;
    super_arts[ix].gauge_len = remake_sa_gauge_len(ix, saptr->gauge_len);
    super_arts[ix].store_max = remake_sa_store_max(ix, saptr->store_max);
    super_arts[ix].dtm = saptr->dtm;
    super_arts[ix].dtm_mul = 1;
}

void clear_super_arts_point(PLW* wk) {
    wk->sa->store = 0;
    wk->sa->gauge.s.h = 0;
    wk->sa->gauge.s.l = -1;
    wk->sa->mp_rno = 0;
    wk->sa->mp_rno2 = 0;
    wk->sa->sa_rno = 0;
    wk->sa->sa_rno2 = 0;
    wk->sa->ex_rno = 0;
    wk->sa->mp = 0;
    wk->sa->ok = 0;
    wk->sa->ex = 0;
    wk->sa->bacckup_g_h = 0;
}

s16 check_combo_end(s16 ix) {
    s16 rnum;

    if (gs.plw[ix].py->flag) {
        return 1;
    }

    if (gs.plw[ix].tsukamare_f) {
        return 1;
    }

    if (pcon_rno[0] == 2 && pcon_rno[1] == 0 && pcon_rno[2] == 2) {
        return 0;
    }

    if (gs.plw[ix].wu.cg_ja.boix == 0 && gs.plw[ix].wu.cg_ja.cuix == 0 && gs.plw[ix].wu.pat_status == 38) {
        return 0;
    }

    if (gs.plw[ix].zuru_flag) {
        return 0;
    }

    if (gs.plw[ix].wu.routine_no[1] != 1 && gs.plw[ix].wu.routine_no[1] != 3) {
        return 0;
    }

    if (gs.plw[ix].old_gdflag != gs.plw[ix].guard_flag) {
        if (gs.plw[ix].guard_flag == 0) {
            rnum = 0;
        } else {
            rnum = 1;
        }
    } else if (gs.plw[ix].guard_flag == 0) {
        rnum = 0;
    } else {
        rnum = 1;
    }

    return rnum;
}

void set_scrrrl() {
    s16 scrc = get_center_position();

    scrr = scrc + 192;
    scrl = scrc - 192;
}

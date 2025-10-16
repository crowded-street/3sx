/**
 * @file effxx.c
 * Effect Move and Init Jumptables
 */

// Uncomment once effmovejptbl is fully decompiled
// #include "sf33rd/Source/Game/effect/effxx.h"
#include "common.h"
#include "sf33rd/Source/Game/effect/eff00.h"
#include "sf33rd/Source/Game/effect/eff01.h"
#include "sf33rd/Source/Game/effect/eff02.h"
#include "sf33rd/Source/Game/effect/eff03.h"
#include "sf33rd/Source/Game/effect/eff04.h"
#include "sf33rd/Source/Game/effect/eff05.h"
#include "sf33rd/Source/Game/effect/eff06.h"
#include "sf33rd/Source/Game/effect/eff07.h"
#include "sf33rd/Source/Game/effect/eff08.h"
#include "sf33rd/Source/Game/effect/eff09.h"
#include "sf33rd/Source/Game/effect/eff10.h"
#include "sf33rd/Source/Game/effect/eff11.h"
#include "sf33rd/Source/Game/effect/eff12.h"
#include "sf33rd/Source/Game/effect/eff13.h"
#include "sf33rd/Source/Game/effect/eff14.h"
#include "sf33rd/Source/Game/effect/eff15.h"
#include "sf33rd/Source/Game/effect/eff16.h"
#include "sf33rd/Source/Game/effect/eff17.h"
#include "sf33rd/Source/Game/effect/eff18.h"
#include "sf33rd/Source/Game/effect/eff19.h"
#include "sf33rd/Source/Game/effect/eff20.h"
#include "sf33rd/Source/Game/effect/eff21.h"
#include "sf33rd/Source/Game/effect/eff22.h"
#include "sf33rd/Source/Game/effect/eff23.h"
#include "sf33rd/Source/Game/effect/eff24.h"
#include "sf33rd/Source/Game/effect/eff25.h"
#include "sf33rd/Source/Game/effect/eff26.h"
#include "sf33rd/Source/Game/effect/eff27.h"
#include "sf33rd/Source/Game/effect/eff29.h"
#include "sf33rd/Source/Game/effect/eff30.h"
#include "sf33rd/Source/Game/effect/eff31.h"
#include "sf33rd/Source/Game/effect/eff32.h"
#include "sf33rd/Source/Game/effect/eff33.h"
#include "sf33rd/Source/Game/effect/eff34.h"
#include "sf33rd/Source/Game/effect/eff35.h"
#include "sf33rd/Source/Game/effect/eff36.h"
#include "sf33rd/Source/Game/effect/eff37.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff40.h"
#include "sf33rd/Source/Game/effect/eff41.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff44.h"
#include "sf33rd/Source/Game/effect/eff45.h"
#include "sf33rd/Source/Game/effect/eff46.h"
#include "sf33rd/Source/Game/effect/eff47.h"
#include "sf33rd/Source/Game/effect/eff48.h"
#include "sf33rd/Source/Game/effect/eff49.h"
#include "sf33rd/Source/Game/effect/eff50.h"
#include "sf33rd/Source/Game/effect/eff51.h"
#include "sf33rd/Source/Game/effect/eff52.h"
#include "sf33rd/Source/Game/effect/eff53.h"
#include "sf33rd/Source/Game/effect/eff54.h"
#include "sf33rd/Source/Game/effect/eff55.h"
#include "sf33rd/Source/Game/effect/eff56.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff59.h"
#include "sf33rd/Source/Game/effect/eff60.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/eff62.h"
#include "sf33rd/Source/Game/effect/eff63.h"
#include "sf33rd/Source/Game/effect/eff64.h"
#include "sf33rd/Source/Game/effect/eff65.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff67.h"
#include "sf33rd/Source/Game/effect/eff68.h"
#include "sf33rd/Source/Game/effect/eff69.h"
#include "sf33rd/Source/Game/effect/eff70.h"
#include "sf33rd/Source/Game/effect/eff71.h"
#include "sf33rd/Source/Game/effect/eff72.h"
#include "sf33rd/Source/Game/effect/eff73.h"
#include "sf33rd/Source/Game/effect/eff74.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff77.h"
#include "sf33rd/Source/Game/effect/eff78.h"
#include "sf33rd/Source/Game/effect/eff79.h"
#include "sf33rd/Source/Game/effect/eff80.h"
#include "sf33rd/Source/Game/effect/eff81.h"
#include "sf33rd/Source/Game/effect/eff82.h"
#include "sf33rd/Source/Game/effect/eff83.h"
#include "sf33rd/Source/Game/effect/eff84.h"
#include "sf33rd/Source/Game/effect/eff85.h"
#include "sf33rd/Source/Game/effect/eff86.h"
#include "sf33rd/Source/Game/effect/eff90.h"
#include "sf33rd/Source/Game/effect/eff91.h"
#include "sf33rd/Source/Game/effect/eff92.h"
#include "sf33rd/Source/Game/effect/eff93.h"
#include "sf33rd/Source/Game/effect/eff94.h"
#include "sf33rd/Source/Game/effect/eff95.h"
#include "sf33rd/Source/Game/effect/eff96.h"
#include "sf33rd/Source/Game/effect/eff97.h"
#include "sf33rd/Source/Game/effect/eff98.h"
#include "sf33rd/Source/Game/effect/eff99.h"
#include "sf33rd/Source/Game/effect/effa0.h"
#include "sf33rd/Source/Game/effect/effa1.h"
#include "sf33rd/Source/Game/effect/effa2.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effa5.h"
#include "sf33rd/Source/Game/effect/effa6.h"
#include "sf33rd/Source/Game/effect/effa7.h"
#include "sf33rd/Source/Game/effect/effa8.h"
#include "sf33rd/Source/Game/effect/effa9.h"
#include "sf33rd/Source/Game/effect/effb0.h"
#include "sf33rd/Source/Game/effect/effb1.h"
#include "sf33rd/Source/Game/effect/effb2.h"
#include "sf33rd/Source/Game/effect/effb3.h"
#include "sf33rd/Source/Game/effect/effb4.h"
#include "sf33rd/Source/Game/effect/effb5.h"
#include "sf33rd/Source/Game/effect/effb6.h"
#include "sf33rd/Source/Game/effect/effb7.h"
#include "sf33rd/Source/Game/effect/effb8.h"
#include "sf33rd/Source/Game/effect/effb9.h"
#include "sf33rd/Source/Game/effect/effc0.h"
#include "sf33rd/Source/Game/effect/effc1.h"
#include "sf33rd/Source/Game/effect/effc2.h"
#include "sf33rd/Source/Game/effect/effc3.h"
#include "sf33rd/Source/Game/effect/effc4.h"
#include "sf33rd/Source/Game/effect/effc5.h"
#include "sf33rd/Source/Game/effect/effc6.h"
#include "sf33rd/Source/Game/effect/effc7.h"
#include "sf33rd/Source/Game/effect/effc8.h"
#include "sf33rd/Source/Game/effect/effc9.h"
#include "sf33rd/Source/Game/effect/effd0.h"
#include "sf33rd/Source/Game/effect/effd1.h"
#include "sf33rd/Source/Game/effect/effd3.h"
#include "sf33rd/Source/Game/effect/effd4.h"
#include "sf33rd/Source/Game/effect/effd5.h"
#include "sf33rd/Source/Game/effect/effd6.h"
#include "sf33rd/Source/Game/effect/effd7.h"
#include "sf33rd/Source/Game/effect/effd8.h"
#include "sf33rd/Source/Game/effect/effd9.h"
#include "sf33rd/Source/Game/effect/effe0.h"
#include "sf33rd/Source/Game/effect/effe1.h"
#include "sf33rd/Source/Game/effect/effe2.h"
#include "sf33rd/Source/Game/effect/effe3.h"
#include "sf33rd/Source/Game/effect/effe4.h"
#include "sf33rd/Source/Game/effect/effe5.h"
#include "sf33rd/Source/Game/effect/effe6.h"
#include "sf33rd/Source/Game/effect/effe7.h"
#include "sf33rd/Source/Game/effect/effe8.h"
#include "sf33rd/Source/Game/effect/effe9.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/effect/efff0.h"
#include "sf33rd/Source/Game/effect/efff2.h"
#include "sf33rd/Source/Game/effect/efff5.h"
#include "sf33rd/Source/Game/effect/efff6.h"
#include "sf33rd/Source/Game/effect/efff8.h"
#include "sf33rd/Source/Game/effect/efff9.h"
#include "sf33rd/Source/Game/effect/effg0.h"
#include "sf33rd/Source/Game/effect/effg3.h"
#include "sf33rd/Source/Game/effect/effg4.h"
#include "sf33rd/Source/Game/effect/effg5.h"
#include "sf33rd/Source/Game/effect/effg6.h"
#include "sf33rd/Source/Game/effect/effg7.h"
#include "sf33rd/Source/Game/effect/effg8.h"
#include "sf33rd/Source/Game/effect/effg9.h"
#include "sf33rd/Source/Game/effect/effh0.h"
#include "sf33rd/Source/Game/effect/effh1.h"
#include "sf33rd/Source/Game/effect/effh2.h"
#include "sf33rd/Source/Game/effect/effh6.h"
#include "sf33rd/Source/Game/effect/effh9.h"
#include "sf33rd/Source/Game/effect/effi0.h"
#include "sf33rd/Source/Game/effect/effi3.h"
#include "sf33rd/Source/Game/effect/effi4.h"
#include "sf33rd/Source/Game/effect/effi6.h"
#include "sf33rd/Source/Game/effect/effi7.h"
#include "sf33rd/Source/Game/effect/effi8.h"
#include "sf33rd/Source/Game/effect/effi9.h"
#include "sf33rd/Source/Game/effect/effj0.h"
#include "sf33rd/Source/Game/effect/effj2.h"
#include "sf33rd/Source/Game/effect/effj4.h"
#include "sf33rd/Source/Game/effect/effj6.h"
#include "sf33rd/Source/Game/effect/effj7.h"
#include "sf33rd/Source/Game/effect/effj8.h"
#include "sf33rd/Source/Game/effect/effj9.h"
#include "sf33rd/Source/Game/effect/effk2.h"
#include "sf33rd/Source/Game/effect/effk3.h"
#include "sf33rd/Source/Game/effect/effk4.h"
#include "sf33rd/Source/Game/effect/effk5.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/effect/effk7.h"
#include "sf33rd/Source/Game/effect/effk8.h"
#include "sf33rd/Source/Game/effect/effk9.h"
#include "sf33rd/Source/Game/effect/effl0.h"
#include "sf33rd/Source/Game/effect/effl1.h"
#include "sf33rd/Source/Game/effect/effl2.h"
#include "sf33rd/Source/Game/effect/effl3.h"
#include "sf33rd/Source/Game/effect/effl4.h"
#include "sf33rd/Source/Game/effect/effl5.h"
#include "sf33rd/Source/Game/effect/effl6.h"
#include "sf33rd/Source/Game/effect/effl7.h"
#include "sf33rd/Source/Game/effect/effl8.h"
#include "sf33rd/Source/Game/effect/effl9.h"
#include "sf33rd/Source/Game/effect/effm0.h"
#include "sf33rd/Source/Game/effect/effm1.h"
#include "sf33rd/Source/Game/effect/effm2.h"
#include "sf33rd/Source/Game/effect/effm3.h"
#include "sf33rd/Source/Game/effect/effm5.h"
#include "sf33rd/Source/Game/effect/effm6.h"
#include "sf33rd/Source/Game/effect/effm7.h"
#include "sf33rd/Source/Game/effect/effm8.h"
#include "sf33rd/Source/Game/engine/plpat09.h"

s32 effect_dummy_init() {
    return -1;
}

void effect_dummy_move() {
    // Do nothing
}

const void (*effmovejptbl[229])() = {
    effect_00_move,    effect_01_move,    effect_02_move,    effect_03_move,    effect_04_move,    effect_05_move,
    effect_06_move,    effect_07_move,    effect_08_move,    effect_09_move,    effect_10_move,    effect_11_move,
    effect_12_move,    effect_13_move,    effect_14_move,    effect_15_move,    effect_16_move,    effect_17_move,
    effect_18_move,    effect_19_move,    effect_20_move,    effect_21_move,    effect_22_move,    effect_23_move,
    effect_24_move,    effect_25_move,    effect_26_move,    effect_27_move,    effect_dummy_move, effect_29_move,
    effect_30_move,    effect_31_move,    effect_32_move,    effect_33_move,    effect_34_move,    effect_35_move,
    effect_36_move,    effect_37_move,    effect_38_move,    effect_39_move,    effect_40_move,    effect_41_move,
    effect_42_move,    effect_43_move,    effect_44_move,    effect_45_move,    effect_46_move,    effect_47_move,
    effect_48_move,    effect_49_move,    effect_50_move,    effect_51_move,    effect_52_move,    effect_53_move,
    effect_54_move,    effect_55_move,    effect_56_move,    effect_57_move,    effect_58_move,    effect_59_move,
    effect_60_move,    effect_61_move,    effect_62_move,    effect_63_move,    effect_64_move,    effect_65_move,
    effect_66_move,    effect_67_move,    effect_68_move,    effect_69_move,    effect_70_move,    effect_71_move,
    effect_72_move,    effect_73_move,    effect_74_move,    effect_75_move,    effect_76_move,    effect_77_move,
    effect_78_move,    effect_79_move,    effect_80_move,    effect_81_move,    effect_82_move,    effect_83_move,
    effect_84_move,    effect_85_move,    effect_86_move,    effect_dummy_move, effect_dummy_move, effect_dummy_move,
    effect_90_move,    effect_91_move,    effect_92_move,    effect_93_move,    effect_94_move,    effect_95_move,
    effect_96_move,    effect_97_move,    effect_98_move,    effect_99_move,    effect_A0_move,    effect_A1_move,
    effect_A2_move,    effect_A3_move,    effect_dummy_move, effect_A5_move,    effect_A6_move,    effect_A7_move,
    effect_A8_move,    effect_A9_move,    effect_B0_move,    effect_B1_move,    effect_B2_move,    effect_B3_move,
    effect_B4_move,    effect_B5_move,    effect_B6_move,    effect_B7_move,    effect_B8_move,    effect_B9_move,
    effect_C0_move,    effect_C1_move,    effect_C2_move,    effect_C3_move,    effect_C4_move,    effect_C5_move,
    effect_C6_move,    effect_C7_move,    effect_C8_move,    effect_C9_move,    effect_D0_move,    effect_D1_move,
    effect_dummy_move, effect_D3_move,    effect_D4_move,    effect_D5_move,    effect_D6_move,    effect_D7_move,
    effect_D8_move,    effect_D9_move,    effect_E0_move,    effect_E1_move,    effect_E2_move,    effect_E3_move,
    effect_E4_move,    effect_E5_move,    effect_E6_move,    effect_E7_move,    effect_E8_move,    effect_E9_move,
    effect_F0_move,    effect_dummy_move, effect_F2_move,    effect_dummy_move, effect_dummy_move, effect_F5_move,
    effect_F6_move,    effect_dummy_move, effect_F8_move,    effect_F9_move,    effect_G0_move,    effect_dummy_move,
    effect_dummy_move, effect_G3_move,    effect_G4_move,    effect_G5_move,    effect_G6_move,    effect_G7_move,
    effect_G8_move,    effect_G9_move,    effect_H0_move,    effect_H1_move,    effect_H2_move,    effect_dummy_move,
    effect_dummy_move, effect_dummy_move, effect_H6_move,    effect_dummy_move, effect_dummy_move, effect_H9_move,
    effect_I0_move,    effect_dummy_move, effect_dummy_move, effect_I3_move,    effect_I4_move,    effect_dummy_move,
    effect_I6_move,    effect_I7_move,    effect_I8_move,    effect_I9_move,    effect_J0_move,    effect_dummy_move,
    effect_J2_move,    effect_dummy_move, effect_J4_move,    effect_dummy_move, effect_J6_move,    effect_J7_move,
    effect_J8_move,    effect_J9_move,    effect_dummy_move, effect_dummy_move, effect_K2_move,    effect_K3_move,
    effect_K4_move,    effect_K5_move,    effect_K6_move,    effect_K7_move,    effect_K8_move,    effect_K9_move,
    effect_L0_move,    effect_L1_move,    effect_L2_move,    effect_L3_move,    effect_L4_move,    effect_L5_move,
    effect_L6_move,    effect_L7_move,    effect_L8_move,    effect_L9_move,    effect_M0_move,    effect_M1_move,
    effect_M2_move,    effect_M3_move,    effect_dummy_move, effect_M5_move,    effect_M6_move,    effect_M7_move,
    effect_M8_move,
};

const s32 (*effinitjptbl[59])() = {
    NULL,
    effect_03_init,
    effect_13_init,
    effect_09_init,
    effect_G7_init,
    effect_C0_init,
    effect_C7_init,
    effect_D0_init,
    effect_D1_init,
    effect_dummy_init,
    effect_34_init,
    effect_37_init,
    effect_09_init2,
    effect_41_init,
    effect_D4_init,
    set_tenguiwa,
    effect_D9_init,
    setup_accessories,
    setup_after_images,
    erase_after_images,
    effect_dummy_init,
    clear_caution_flag,
    setup_status_flag,
    reset_extra_bg_flag,
    flip_my_rl_flag,
    effect_F8_init,
    clear_caution_flag,
    effect_G3_init,
    effect_G4_init,
    setup_ase_extra,
    effect_G6_init,
    setup_meoshi_hit_flag,
    exec_char_asxy,
    set_caution_flag,
    setup_my_clear_level,
    setup_my_bright_level,
    effect_dummy_init,
    setup_free_program,
    setup_bg_quake_x,
    setup_bg_quake_y,
    effect_47_init,
    setup_koishi_extra,
    effect_77_init,
    setup_exdm_ix,
    setup_dmv_use_flag,
    effect_D5_init,
    effect_D6_init,
    setup_disp_flag,
    setup_command_number,
    effect_I7_init,
    effect_dummy_init,
    setup_sa_shadow,
    effect_73_init,
    effect_K8_init,
    effect_K9_init,
    effect_L7_init,
    effect_M2_init,
    effect_M8_init,
    effect_F0_init,
};

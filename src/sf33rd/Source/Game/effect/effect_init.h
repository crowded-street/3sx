#ifndef EFFECT_INIT_H
#define EFFECT_INIT_H

#include "types.h"

// TODO: get rid of this file

s32 effect_04_init(s16 Death_Type, s16 cg_type, s16 sync_bg, s16 priority);        // Range: 0x1C56A0 -> 0x1C5818
s32 effect_18_init(s16 disp_index, s16 cursor_id, s16 sync_bg, s16 master_player); // Range: 0x1D36B0 -> 0x1D37E0
s32 effect_23_init(s16 id, u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type, s16 cursor_index,
                   u16 char_offset, s16 pos_index, s16 type); // Range: 0x1D5230 -> 0x1D5390
s32 effect_35_init(s16 wait_timer, s16 c_type);               // Range: 0x1DBE80 -> 0x1DC170
s32 effect_36_init(u8 typenum);
s32 effect_38_init(s16 PL_id, s16 dir_old, s16 Your_Char, s16 Play_Status,
                   s16 Target_BG);                                                    // Range: 0x1DEC30 -> 0x1DF620
s32 effect_39_init(s16 PL_id, s16 dir_old, s16 Your_Char, s16 Target_BG, s16 Option); // Range: 0x1E04E0 -> 0x1E0718
s32 effect_40_init(s16 id, s16 type, s16 char_ix, s16 sync_bg, s16 master_player,
                   s16 master_priority);                   // Range: 0x1E0B90 -> 0x1E0E78
s32 effect_43_init(s16 Time, s16 Target_BG);               // Range: 0x1E2720 -> 0x1E28A8
s32 effect_45_init(u8 id, s16 sync_bg, s16 master_player); // Range: 0x1E30D0 -> 0x1E3228
s32 effect_48_init(s16 type);
s32 effect_49_init(s16 vital_new);                                               // Range: 0x1E52B0 -> 0x1E54D8
s32 effect_51_init(s16 letter_type, s16 cursor_index, s16 master_player);        // Range: 0x1E5E80 -> 0x1E6040
s32 effect_57_init(s16 dir_old, s16 ID, s16 Target_BG, s16 char_ix, s16 option); // Range: 0x1E85B0 -> 0x1E8740
s32 effect_58_init(s16 id, s16 time0, s16 option);                               // Range: 0x1E97D0 -> 0x1E98A0
s32 effect_61_init(s16 master, u8 dir_old, s16 sync_bg, s16 master_player, s16 char_ix, s16 cursor_index,
                   u16 letter_type); // Range: 0x1EB350 -> 0x1EB68C
s32 effect_63_init(u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type,
                   s16 cursor_index); // Range: 0x1EBEC0 -> 0x1EC050
s32 effect_64_init(u8 dir_old, s16 sync_bg, s16 master_player, s16 letter_type, s16 cursor_index, u16 char_offset,
                   s16 pos_index, s16 convert_id, s16 convert_id2); // Range: 0x1EC830 -> 0x1EC990
s32 effect_66_init(s16 order_index, s16 id, s16 master_player, s16 target_bg, s16 char_ix, s16 char_ix2,
                   s16 option);                         // Range: 0x1EDB60 -> 0x1EDEA4
s32 effect_75_init(s16 dir_old, s16 ID, s16 Target_BG); // Range: 0x1F1910 -> 0x1F1A28
s32 effect_76_init(s16 dir_old);                        // Range: 0x1F2670 -> 0x1F27A0
s32 effect_95_init(s16 vital_new);                      // Range: 0x1FCFD0 -> 0x1FD218
s32 effect_A2_init(s16 /* unused */);                   // Range: 0x1FF350 -> 0x1FF408
s32 effect_A8_init(s16 id, u8 dir_old, s16 sync_bg, s16 master_player, s16 cursor_index, s16 char_ix,
                   s16 pos_index);                                                // Range: 0x2012C0 -> 0x201540
s32 effect_A9_init(s16 Char_Index, s16 Option, s16 Pos_Index, s16 Option2);       // Range: 0x201A80 -> 0x201C78
s32 effect_B8_init(s8 WIN_PL_NO, s16 timer);                                      // Range: 0x205500 -> 0x2056B8
s32 effect_C4_init(s16 id, s16 letter_type, s16 cursor_index, s16 master_player); // Range: 0x209D40 -> 0x209F4C
s32 effect_E1_init(s16 id, s16 Time, s16 /* unused */);
s32 effect_F5_init(s16 /* unused */);
s32 effect_F6_init(u8 typenum);
s32 effect_J2_init(s16 delay);                                           // Range: 0x227340 -> 0x227478
s32 effect_K6_init(s16 PL_id, s16 dir_old, s16 dm_vital, s16 Target_BG); // Range: 0x22EEC0 -> 0x22F020
s32 effect_L1_init(s16 flag);                                            // Range: 0x232820 -> 0x2328E8

#endif

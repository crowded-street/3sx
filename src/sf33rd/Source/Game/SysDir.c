#include "sf33rd/Source/Game/SysDir.h"
#include "common.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"

u8 chainex_check[2][36];
s16 omop_use_ex_gauge_ix[2];
s16 omop_guard_distance_ix[2];
s16 omop_sa_gauge_ix[2];
s16 omop_stun_gauge_add[2];
s16 omop_stun_gauge_rcv[2];
s16 omop_stun_gauge_len[2];
s16 omop_otedama_ix[2];
s16 omop_b_block_ix[2];
s16 omop_r_block_ix[2];
s16 omop_vital_ix[2];
s16 omop_sag_max_ix[2];
s16 omop_sag_len_ix[2];
s16 omop_vital_init[2];
s16 omop_vt_bar_disp[2];
s16 omop_st_bar_disp[2];
s16 omop_sa_bar_disp[2];
s16 omop_cockpit;
s16 omop_round_timer;
s16 omop_dokidoki;

const u32 omop_guard_type[4] = { 16777408, 16777344, 192, 16777280 };

const u32 sysdir_base_move[20] = { 786432, 786432, 786432, 786432, 786432, 786432, 786432, 786432, 786432, 262144,
                                   786432, 786432, 786432, 786432, 786432, 524288, 786432, 786432, 786432, 786432 };

const s16 use_ex_gauge[4] = { 0, 20, 40, 60 };

const s16 guard_distance[4] = { 48, 112, 256, 512 };

const s16 sa_gauge_omake[4] = { 0, 24, 32, 44 };

const s16 stun_gauge_omake[4] = { 0, 24, 32, 44 };

const s16 stun_gauge_r_omake[4] = { 0, 20, 32, 44 };

const s16 stun_gauge_len_omake[5] = { -16, -8, 0, 8, 16 };

const s16 blok_b_omake[4] = { -2, 0, 2, 4 };

const s16 blok_r_omake[4] = { -1, 0, 1, 2 };

const s16 sag_stock_omake[11] = { -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

const s16 sag_length_omake[17] = { -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

const s16 base_vital_omake[7] = { 0xfe3e, 0xfed4, 0xff6a, 0x0000, 0x0096, 0x012c, 0x01c2 };

u32 sag_ikinari_max() {
    return ((omop_spmv_ng_table2[0] & 0x40000) + (omop_spmv_ng_table2[1] & 0x40000)) == 0x80000;
}

u32 check_use_all_SA() {
    if (Direction_Working[Present_Mode] != 0) {
        return system_dir[Present_Mode].contents[9][0];
    }

    return 0;
}

u32 check_without_SA() {
    if (Direction_Working[Present_Mode] != 0) {
        return system_dir[Present_Mode].contents[5][0] == 0;
    }

    return 0;
}

void init_omop() {
    omop_spmv_ng_table2[0] = 0;
    omop_spmv_ng_table[0] = 0;
    omop_spmv_ng_table2[1] = 0;
    omop_spmv_ng_table[1] = 0;
    omop_spmv_ng_table2[0] |= 0x400000;
    omop_spmv_ng_table2[0] |= 0x800000;

    if (Mode_Type == MODE_NETWORK) {
        get_system_direction_parameter((DirData*)&system_dir[2]);
        get_extra_option_parameter(&save_w[2].extra_option);
    } else if (Demo_Flag == 0) {
        get_system_direction_parameter((DirData*)&system_dir[0]);
        get_extra_option_parameter(&save_w->extra_option);
    } else {
        if (Direction_Working[Present_Mode]) {
            get_system_direction_parameter((DirData*)&system_dir[Present_Mode]);
        } else {
            get_system_direction_parameter((DirData*)&system_dir[0]);
        }

        get_extra_option_parameter(&save_w[Present_Mode].extra_option);
    }

    omop_spmv_ng_table[0] |= sysdir_base_move[My_char[0]];
    omop_spmv_ng_table[1] |= sysdir_base_move[My_char[1]];
    cmd_sel[0] = (omop_spmv_ng_table[0] & 0x08000000) == 0;
    cmd_sel[1] = (omop_spmv_ng_table[1] & 0x08000000) == 0;
    no_sa[0] = (omop_spmv_ng_table[0] & 0xC0000000) != 0;
    no_sa[1] = (omop_spmv_ng_table[1] & 0xC0000000) != 0;
    vib_sel[0] = 1;
    vib_sel[1] = 1;
}

void get_extra_option_parameter(_EXTRA_OPTION* omop_extra) {
    omop_vital_ix[0] = omop_extra->contents[0][0];
    omop_vital_ix[1] = omop_extra->contents[0][0];
    omop_vital_init[0] = omop_extra->contents[0][1];
    omop_vital_init[1] = omop_extra->contents[0][2];
    omop_spmv_ng_table[0] |= omop_guard_type[omop_extra->contents[0][3]];
    omop_spmv_ng_table[1] |= omop_guard_type[omop_extra->contents[0][3]];

    switch (omop_extra->contents[1][0]) {
    case 1:
        omop_spmv_ng_table2[0] |= 0x80000;
        break;

    case 2:
        omop_spmv_ng_table2[0] |= 0x10000;
        break;

    default:
        omop_spmv_ng_table2[0] |= 0x90000;
        break;
    }

    switch (omop_extra->contents[1][1]) {
    case 1:
        omop_spmv_ng_table2[1] |= 0x80000;
        break;

    case 2:
        omop_spmv_ng_table2[1] |= 0x10000;
        break;

    default:
        omop_spmv_ng_table2[1] |= 0x90000;
        break;
    }

    omop_sag_max_ix[0] = omop_extra->contents[1][2];
    omop_sag_max_ix[1] = omop_extra->contents[1][3];
    omop_sag_len_ix[0] = omop_extra->contents[1][4];
    omop_sag_len_ix[1] = omop_extra->contents[1][5];
    omop_sa_gauge_ix[0] = omop_extra->contents[1][6];
    omop_sa_gauge_ix[1] = omop_extra->contents[1][6];
    omop_stun_gauge_len[0] = omop_extra->contents[2][0];
    omop_stun_gauge_len[1] = omop_extra->contents[2][1];
    omop_stun_gauge_add[0] = omop_extra->contents[2][2];
    omop_stun_gauge_add[1] = omop_extra->contents[2][2];
    omop_stun_gauge_rcv[0] = omop_extra->contents[2][3];
    omop_stun_gauge_rcv[1] = omop_extra->contents[2][3];
    omop_cockpit = omop_extra->contents[3][0];
    omop_vt_bar_disp[0] = omop_extra->contents[3][1];
    omop_vt_bar_disp[1] = omop_extra->contents[3][1];
    omop_round_timer = omop_extra->contents[3][2];
    omop_st_bar_disp[0] = omop_extra->contents[3][3];
    omop_st_bar_disp[1] = omop_extra->contents[3][3];
    omop_sa_bar_disp[0] = omop_extra->contents[3][4];
    omop_sa_bar_disp[1] = omop_extra->contents[3][4];
    omop_dokidoki = 0;
}

void get_system_direction_parameter(DirData* sysdir_data) {
    if (sysdir_data->contents[0][0] == 0) {
        omop_spmv_ng_table[0] |= 0x300;
    }

    if (sysdir_data->contents[0][1] == 0) {
        omop_spmv_ng_table[0] |= 0x800;
    }

    if (sysdir_data->contents[0][2] == 0) {
        omop_spmv_ng_table[0] |= 0x400;
    }

    omop_b_block_ix[0] = sysdir_data->contents[0][3];

    if (sysdir_data->contents[0][4] == 0) {
        omop_spmv_ng_table[0] |= 0x1000;
    }

    omop_r_block_ix[0] = sysdir_data->contents[0][5];

    if (sysdir_data->contents[1][0] == 0) {
        omop_spmv_ng_table[0] |= 0x10;
    }

    if (sysdir_data->contents[1][1] == 0) {
        omop_spmv_ng_table[0] |= 0x2000;
    }

    omop_guard_distance_ix[0] = sysdir_data->contents[1][2];

    if (sysdir_data->contents[1][3] != 0) {
        omop_spmv_ng_table[0] |= 0x8000;
    }

    if (sysdir_data->contents[1][4] == 0) {
        omop_spmv_ng_table2[0] |= 0x10000000;
    }

    if (save_w[Present_Mode].GuardCheck) {
        omop_spmv_ng_table[0] |= 0x04000000;
    }

    if (sysdir_data->contents[2][0] == 0) {
        omop_spmv_ng_table[0] |= 4;
    }

    if (sysdir_data->contents[2][1] == 0) {
        omop_spmv_ng_table[0] |= 8;
    }

    if (sysdir_data->contents[2][2] == 0) {
        omop_spmv_ng_table[0] |= 0x10000;
    }

    if (sysdir_data->contents[2][3] == 0) {
        omop_spmv_ng_table[0] |= 0x20000;
    }

    if (sysdir_data->contents[2][4] == 0) {
        omop_spmv_ng_table2[0] |= 0x200;
    }

    if (sysdir_data->contents[3][0] == 0) {
        omop_spmv_ng_table2[0] |= 0x100;
    }

    if (sysdir_data->contents[3][1] == 0) {
        omop_spmv_ng_table2[0] |= 0x400;
    }

    if (sysdir_data->contents[3][2] != 0) {
        omop_spmv_ng_table2[0] |= 0x800;
    }

    if (sysdir_data->contents[4][0] == 0) {
        omop_spmv_ng_table2[0] |= 0x10;
    }

    if (sysdir_data->contents[4][1] == 0) {
        omop_spmv_ng_table2[0] |= 0x20;
    }

    if (sysdir_data->contents[4][2] == 0) {
        omop_spmv_ng_table[0] |= 1;
    }

    if (sysdir_data->contents[4][3] == 0) {
        omop_spmv_ng_table[0] |= 2;
    }

    if (sysdir_data->contents[5][0] == 0) {
        omop_spmv_ng_table[0] |= 0xC0000000;
    }

    if (sysdir_data->contents[5][1] == 0) {
        omop_spmv_ng_table[0] |= 0x30000000;
    }

    if (sysdir_data->contents[5][2] == 0) {
        omop_spmv_ng_table2[0] |= 0x1000;
    }

    omop_use_ex_gauge_ix[0] = sysdir_data->contents[5][3];

    if (sysdir_data->contents[6][0] == 0) {
        omop_spmv_ng_table2[0] |= 1;
    }

    if (sysdir_data->contents[6][1] == 0) {
        omop_spmv_ng_table2[0] |= 0x40;
    }

    if (sysdir_data->contents[6][2] == 0) {
        omop_spmv_ng_table2[0] |= 0x80;
    }

    if (sysdir_data->contents[6][3] == 0) {
        omop_spmv_ng_table[0] |= 0x400000;
    }

    if (sysdir_data->contents[6][4] == 1) {
        omop_spmv_ng_table[0] |= 0x800000;
    }

    if (sysdir_data->contents[7][0] == 0) {
        omop_spmv_ng_table[0] |= 0x20;
    }

    if (sysdir_data->contents[7][1] == 0) {
        omop_spmv_ng_table[0] |= 0x200000;
    }

    if (sysdir_data->contents[7][2] == 0) {
        omop_spmv_ng_table[0] |= 0x02000000;
    }

    if (sysdir_data->contents[7][3] == 0) {
        omop_spmv_ng_table[0] |= 0x4000;
    }

    if (sysdir_data->contents[7][4] == 0) {
        omop_spmv_ng_table2[0] |= 0x40000;
    }

    if (sysdir_data->contents[7][5] == 0) {
        omop_spmv_ng_table2[0] |= 0x20000;
    }

    if (sysdir_data->contents[8][0] == 0) {
        omop_spmv_ng_table2[0] |= 0x100000;
    }

    if (sysdir_data->contents[8][1] == 0) {
        omop_spmv_ng_table2[0] |= 0x200000;
    }

    if (sysdir_data->contents[8][2] == 0) {
        omop_spmv_ng_table2[0] |= 4;
    }

    if (sysdir_data->contents[8][3] == 0) {
        omop_spmv_ng_table2[0] |= 0x01000000;
    }

    if (sysdir_data->contents[8][4] == 0) {
        omop_spmv_ng_table2[0] |= 0x02000000;
    }

    if (sysdir_data->contents[8][5] == 0) {
        omop_spmv_ng_table2[0] |= 2;
    }

    if (sysdir_data->contents[9][0] == 0) {
        omop_spmv_ng_table[0] |= 0x08000000;
    }

    if (sysdir_data->contents[9][1] == 0) {
        omop_spmv_ng_table2[0] |= 8;
    }

    omop_otedama_ix[0] = sysdir_data->contents[9][2];

    if (sysdir_data->contents[9][3] == 0) {
        omop_spmv_ng_table2[0] |= 0x20000000;
    }

    omop_spmv_ng_table[1] = omop_spmv_ng_table[0];
    omop_spmv_ng_table2[1] = omop_spmv_ng_table2[0];
    omop_b_block_ix[1] = omop_b_block_ix[0];
    omop_r_block_ix[1] = omop_r_block_ix[0];
    omop_guard_distance_ix[1] = omop_guard_distance_ix[0];
    omop_use_ex_gauge_ix[1] = omop_use_ex_gauge_ix[0];
    omop_otedama_ix[1] = omop_otedama_ix[0];
}

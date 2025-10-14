/**
 * @file opening.c
 * Opening
 */

#include "sf33rd/Source/Game/opening/opening.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/AcrUtil.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/EFFE1.h"
#include "sf33rd/Source/Game/MTRANS.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/aboutspr.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/bg_data.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect_init.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"

#include "sf33rd/Source/Game/demo/demo00.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/opening/op_sub.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"

typedef const f32* ro_f32_ptr;

static const f32 title00[25] = { 0.0f, 0.0f, 0.75f, 0.75f, -192.0f, -96.0f, 384.0f, 192.0f, -1.0f,
                                 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,    0.0f,   0.0f,   0.0f,   0.0f,
                                 0.0f, 0.0f, 0.0f,  0.0f,  0.0f,    0.0f,   0.0f };

static ro_f32_ptr title[2] = { title00, title00 };

const s16 optsr_tbl[59] = { 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
                            54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
                            74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, -1 };

void (*opening_move_jp[19])() = { op_100_move, op_101_move, op_102_move, op_103_move, op_104_move,
                                  op_105_move, op_106_move, op_107_move, op_108_move, op_109_move,
                                  op_110_move, op_111_move, op_112_move, op_113_move, op_114_move,
                                  op_115_move, op_116_move, op_117_move, op_118_move };

s16 op_obj_disp;
s8 op_scrn_end;
s16 op_timer0;
s16 title_tex_flag;
s16 music_scene;
s16 music_time;
s16 op_plmove_timer;
OPBW* opw_ptr;
s16 op_end_flag;
s16 op_demo_index;
s16 op_sound_status;
MVXY op_bg_mvxy[3];
OP_W op_w;

s16 opening_demo() {
    switch (D_No[3]) {
    case 0:
        D_No[3] += 1;
        OPBG_Init();
        break;

    case 1:
        if (OPBG_Move(0)) {
            D_No[3] += 1;
            reset_dma_group(0x8C40);
            purge_texcash_work(9);
            TexRelease_OP();
            TITLE_Init();
            FadeInit();
        }

        break;

    case 2:
        TITLE_Move(0);

        if (FadeIn(0, 4, 8) != 0) {
            D_No[3] += 1;
            op_timer0 = 300;
        }

        break;

    case 3:
        if (!Game_pause) {
            if (--op_timer0 == 0) {
                D_No[3] = 99;
            }
        }

        TITLE_Move(1);
        Disp_Copyright();
        break;

    default:
        TITLE_Move(1);
        Disp_Copyright();
        return 1;
    }

    return 0;
}

void TITLE_Init() {
    void* loadAdrs;
    u32 loadSize;
    s16 key;

    mmDebWriteTag("\nMAIN TITLE\n\n");
    Opening_Now = 0;
    ppgTitleList.tex = &ppgTitleTex;
    ppgTitleList.pal = NULL;
    ppgSetupCurrentDataList(&ppgTitleList);
    loadSize = load_it_use_any_key2(78, &loadAdrs, &key, 2, 1); // TitleTM.ppg

    if (loadSize == 0) {
        // Main title texture could not be loaded.
        flLogOut("メインタイトルのテクスチャが読み込めませんでした。\n");
        while (1) {}
    }

    ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, 601, 1, 0, 0);
    ppgSetupTexChunk_2nd(NULL, 601);
    ppgSetupTexChunk_3rd(NULL, 601, 1);
    Push_ramcnt_key(key);
    ppgSourceDataReleased(NULL);
    title_tex_flag = 1;
    op_w.r_no_0 = 0;
}

s16 TITLE_Move(u16 type) {
    ppgSetupCurrentDataList(&ppgTitleList);

    switch (type) {
    case 0:
        switch (op_w.r_no_0) {
        case 0:
            op_w.r_no_0 += 1;
            Zoom_Value_Set(0x40);
            Frame_Up(192, 112, 0x13);
            op_timer0 = 10;
            break;

        case 1:
            op_timer0 -= 1;

            if (op_timer0 <= 0) {
                op_w.r_no_0 += 1;
                op_timer0 = 19;
            }

            break;

        case 2:
            if (!Game_pause) {
                if (op_timer0-- >= 0) {
                    Frame_Down(0xC0, 0x70, 1);
                } else {
                    op_w.r_no_0 += 1;
                }
            }

            break;

        default:
            Zoom_Value_Set(0x40);
            break;
        }

        Put_char(title[type], 601, 9, 192, 96, scr_sc, scr_sc);
        return 0;

    case 1:
        Put_char(title[type], 601, 9, 192, 96, 1.0f, 1.0f);
        return 0;

    default:
        return 0;
    }
}

void OPBG_Init() {
    void* loadAdrs;
    size_t loadSize;
    s16 i;
    s16 key;

    mmDebWriteTag("\nOPENING\n\n");
    ppgOpnBgList.tex = &ppgOpnBgTex;
    ppgOpnBgList.pal = palGetChunkGhostCP3();
    ppgSetupCurrentDataList(&ppgOpnBgList);

    if ((key = Search_ramcnt_type(0x1D)) == 0) {
        // Opening demo texture has not been loaded.
        flLogOut("オープニングデモテクスチャが読み込まれていません。\n");
        while (1) {}
    }

    loadSize = Get_size_data_ramcnt_key(key);
    loadAdrs = (void*)Get_ramcnt_address(key);
    ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, 602, 91, 0, 0);

    for (i = 0; i < ppgOpnBgTex.textures; i++) {
        ppgSetupTexChunk_2nd(NULL, i + 602);
        ppgSetupTexChunk_3rd(NULL, i + 602, 1);
    }

    Opening_Now = 1;
    make_texcash_work(9);
    mlt_obj_melt2(&mts[9], 0x8C40);
    sound_trg_init();
    opening_init();
    Zoom_Value_Set(0x40);
}

s16 OPBG_Move(s32 /* unused */) {
    s16 flag = 0;

    flag = oh_opening_demo();
    OPBG_Trans();
    return flag;
}

void sound_trg_init() {
    music_scene = music_time = 0;
}

const s16 sound_time_tbl[257] = {
    4,    11,   19,   26,   34,   41,   49,   56,   64,   71,   79,   86,   94,   101,  109,  116,  124,  131,  139,
    146,  154,  161,  169,  176,  184,  191,  199,  206,  214,  221,  229,  236,  244,  251,  259,  267,  274,  282,
    289,  297,  304,  312,  319,  327,  334,  342,  349,  357,  364,  372,  379,  387,  394,  402,  409,  417,  425,
    432,  440,  447,  456,  463,  471,  478,  486,  494,  501,  509,  516,  524,  531,  539,  546,  554,  561,  569,
    576,  584,  591,  599,  606,  614,  621,  629,  636,  644,  651,  659,  666,  674,  681,  689,  697,  704,  712,
    719,  727,  734,  742,  749,  757,  764,  772,  779,  787,  794,  802,  809,  817,  824,  832,  839,  847,  854,
    862,  869,  877,  884,  892,  899,  907,  914,  922,  929,  937,  944,  952,  959,  967,  974,  982,  989,  997,
    1004, 1012, 1019, 1027, 1035, 1042, 1050, 1057, 1065, 1072, 1080, 1086, 1094, 1101, 1109, 1116, 1123, 1131, 1138,
    1145, 1153, 1160, 1168, 1175, 1182, 1190, 1197, 1204, 1212, 1219, 1227, 1234, 1242, 1249, 1257, 1264, 1272, 1279,
    1287, 1294, 1302, 1309, 1317, 1325, 1332, 1340, 1347, 1355, 1362, 1366, 1370, 1385, 1393, 1400, 1408, 1416, 1423,
    1431, 1438, 1446, 1454, 1461, 1469, 1476, 1484, 1491, 1499, 1506, 1514, 1521, 1529, 1536, 1544, 1552, 1559, 1567,
    1574, 1582, 1589, 1596, 1603, 1611, 1618, 1625, 1633, 1640, 1648, 1655, 1662, 1670, 1677, 1684, 1692, 1699, 1707,
    1714, 1722, 1729, 1737, 1744, 1752, 1759, 1767, 1774, 1782, 1789, 1797, 1804, 1812, 1819, 1827, 1834, 1842, 1849,
    1857, 1864, 1872, 1880, 1887, 1895, 1902, 1910, 1917, -1
};

const s16 sound_trg_tbl[257] = {
    101, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 102, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    103, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 104, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    105, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 106, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    107, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 108, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    109, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 110, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    111, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 112, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    113, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 114, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    115, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 116, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    -1
};

void sound_trg_move() {
    s16 buff;

    if (op_plmove_timer >= sound_time_tbl[music_scene]) {
        if ((buff = sound_trg_tbl[music_scene]) >= 0) {
            music_scene += 1;
            gSeqStatus[0] = buff;
        }
    }
}

void OPBG_Trans() {
    s16 i;
    s16 j;
    s16 k;

    if (No_Trans) {
        return;
    }

    ppgSetupCurrentDataList(&ppgOpnBgList);
    Scrn_Renew();
    Irl_Family();
    Irl_Scrn();
    scr_calc(0);
    scr_calc(1);
    scr_calc(2);

    if (Screen_Switch & 1) {
        opbg_trans(&op_w.bgw[0], bg_prm[0].bg_h_shift, bg_prm[0].bg_v_shift);
    }

    if (Screen_Switch & 2) {
        opbg_trans(&op_w.bgw[1], bg_prm[1].bg_h_shift, bg_prm[1].bg_v_shift);
    }

    if (Screen_Switch & 4) {
        opbg_trans(&op_w.bgw[2], bg_prm[2].bg_h_shift, bg_prm[2].bg_v_shift);
    }

    if (Debug_w[0x30] & 1) {
        for (k = 0; k < 3; k++) {
            for (j = 0; j < 4; j++) {
                for (i = 0; i < 4; i++) {
                    flPrintL(i * 5 + 20, 23 - j + k * 5, "%04d", op_w.bgw[k].map[i][j].g_no);
                }
            }

            flPrintL(46, k * 5 + 20, "%04x , %04x", bg_w.bgw[k].wxy[0].disp.pos, bg_w.bgw[k].xy[1].disp.pos);
        }
    }
}

s16 oh_tsr_ck(s32 blk_no) {
    s16 i;

    for (i = 0; optsr_tbl[i] != -1; i++) {
        if (optsr_tbl[i] == (s16)blk_no) {
            return 1;
        }
    }

    return 0;
}

void oh_reload_tex(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy) {
    if (!oh_tsr_ck(blk_no) && (opbw->map[mapx][mapy].g_no != (blk_no + 0x259))) {
        if (opbw->map[mapx][mapy].g_no && !oh_tsr_ck(opbw->map[mapx][mapy].g_no - 0x259)) {
            ppgReleaseTextureHandle(&ppgOpnBgTex, opbw->map[mapx][mapy].g_no);
        }

        opbw->map[mapx][mapy].ok = 1;
    }
}

void oh_bg_blk_w(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans) {
    oh_reload_tex(opbw, blk_no, mapx, mapy);
    opbw->map[mapx][mapy].g_no = blk_no + 0x259;
    opbw->map[mapx][mapy].hv = 0;
    opbw->map[mapx][mapy].trans = trans;
    opbw->blk_no = blk_no;
    opbw->map[mapx][mapy].col.full = 0xFFFFFFFF;
}

void oh_bg_blk_wh(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans) {
    oh_reload_tex(opbw, blk_no, mapx, mapy);
    opbw->map[mapx][mapy].g_no = blk_no + 0x259;
    opbw->map[mapx][mapy].hv = 1;
    opbw->map[mapx][mapy].trans = trans;
    opbw->blk_no = blk_no;
    opbw->map[mapx][mapy].col.full = 0xFFFFFFFF;
}

void oh_bg_blk_wv(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans) {
    oh_reload_tex(opbw, blk_no, mapx, mapy);
    opbw->map[mapx][mapy].g_no = blk_no + 0x259;
    opbw->map[mapx][mapy].hv = 2;
    opbw->map[mapx][mapy].trans = trans;
    opbw->blk_no = blk_no;
    opbw->map[mapx][mapy].col.full = 0xFFFFFFFF;
}

void oh_bg_blk_whv(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy, s32 trans) {
    oh_reload_tex(opbw, blk_no, mapx, mapy);
    opbw->map[mapx][mapy].g_no = blk_no + 0x259;
    opbw->map[mapx][mapy].hv = 3;
    opbw->map[mapx][mapy].trans = trans;
    opbw->blk_no = blk_no;
    opbw->map[mapx][mapy].col.full = 0xFFFFFFFF;
}

void opening_init() {
    s16 i;
    s16 j;
    s16 k;

    op_w.r_no_0 = 0;
    op_w.r_no_1 = 0;
    op_w.r_no_2 = 0;
    op_w.index = 0;
    op_w.mv_ctr = 0;
    op_w.bgw[0].blk_no = op_w.bgw[1].blk_no = op_w.bgw[2].blk_no = 0;
    op_w.bgw[0].prio = 75;
    op_w.bgw[1].prio = 80;
    op_w.bgw[2].prio = 85;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 4; k++) {
                op_w.bgw[i].map[j][k].g_no = op_w.bgw[i].map[j][k].trans = op_w.bgw[i].map[j][k].hv =
                    op_w.bgw[i].map[j][k].ok = 0;
            }
        }
    }
}

void op_work_clear() {
    s16 i;

    for (i = 0; i < 3; i++) {
        op_w.bgw[i].r_no_0 = 0;
        op_w.bgw[i].r_no_1 = 0;
    }
}

s16 oh_opening_demo() {
    void (*opening_demo_jp[3])() = { opening_init2, opening_move, opening_title };

    Game_timer += 1;
    opening_demo_jp[op_w.r_no_0]();
    Bg_Family_Set_op();
    return op_end_flag;
}

void opening_init2() {
    Game_timer = 0;

    switch (op_w.r_no_1) {
    case 0:
        opning_init_00000();
        break;

    case 1:
        opning_init_01000();
        break;

    case 2:
        opning_init_02000();
        break;
    }
}

void opning_init_00000() {
    s16 i;

    op_w.r_no_1++;
    op_end_flag = 0;
    Family_Init();
    Scrn_Pos_Init();
    Zoomf_Init();
    Zoom_Value_Set(64);
    bg_w.scno = 3;
    bg_w.pos_offset = 192;

    for (i = 0; i < 6; i++) {
        bg_w.bgw[i].pos_x_work = bg_w.bgw[i].pos_y_work = 0;
        bg_w.bgw[i].rewrite_flag = 0;
        bg_w.bgw[i].fam_no = 0;
        bg_w.bgw[i].zuubun = 0;
        bg_w.bgw[i].wxy[0].cal = 0x02000000; // Isn't this supposed to set xy?
        bg_w.bgw[i].xy[1].cal = 0;
        bg_w.bgw[i].wxy[0].cal = 0x02000000;
        bg_w.bgw[i].wxy[1].cal = 0;
        bg_w.bgw[i].hos_xy[0].cal = 0x02000000;
        bg_w.bgw[i].hos_xy[1].cal = 0;
        bg_w.bgw[i].position_x = 512 - bg_w.pos_offset;
        bg_w.bgw[i].position_y = 0;
    }

    for (i = 0; i < 3; i++) {
        bg_w.bgw[i].fam_no = i;
        op_w.bgw[i].r_no_0 = 0;
        op_w.bgw[i].r_no_1 = 0;
        op_w.bgw[i].bg_no = i;
    }

    op_w.r_no_2 = 0;
    op_end_flag = 0;
    bg_stop = 0;
    akebono_flag = 0;
    aku_flag = 0;
    sa_pa_flag = 0;
    bg_app = 0;
    bg_app_stop = 0;
    bg_w.chase_flag = 0;
}

void opning_init_01000() {
    op_w.r_no_1++;
    Bg_Off_R(0xF);
    Bg_Off_W(0xF);
    op_w.free_work = 8;
    Scrn_Move_Set(0, 512 - bg_w.pos_offset, 512);
    base_y_pos = 40;
}

void opning_init_02000() {
    op_w.free_work--;

    if (op_w.free_work < 0) {
        op_w.r_no_0++;
        op_w.r_no_1 = 0;
        op_w.r_no_2 = 0;
    }

    Scrn_Move_Set(0, 512 - bg_w.pos_offset, 512);
    op_demo_index = 0;
    gSeqStatus[0] = 0;
    op_w.index = 0;
    op_sound_status = 0;
    op_plmove_timer = 2;
}

const s16 op_change_sound_tbl[18] = { 101, 102, 103, 104, 105, 106, 107, 108, 109,
                                      110, 111, 112, 113, 114, 115, 116, 117, 16 };

const s16 op_quake_y_tbl0[16] = { 4, -8, 2, 1, -6, -3, 9, -3, 8, -2, 6, 3, -4, -9, 3, -1 };

void opening_move() {
    s16 work2;

    if (Debug_w[0x30]) {
        flPrintColor(0xFFFFFF8F);
        flPrintL(2, 1, "BAR %d", op_w.r_no_1);
    }

    op_plmove_timer += 1;
    sound_trg_move();

    if (op_w.r_no_1 < 18) {
        work2 = gSeqStatus[0];

        if (op_sound_status != work2) {
            if (work2 == op_change_sound_tbl[op_w.r_no_1]) {
                op_w.r_no_1 += 1;
                op_w.r_no_2 = 0;
                op_work_clear();
            }
        }
    }

    opening_move_jp[op_w.r_no_1]();
    op_sound_status = gSeqStatus[0];
}

void op_100_move() {
    op_w.r_no_1 += 1;
    Go_BGM();
    Disp_Sound_Code();
    op_101_move();
}

const s16 op_101_sound[2] = { 0, 11 };

void op_101_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        ToneDown(0xFF, 0);
        op_bg_move(0);
        effect_F6_init(0);
        effect_F6_init(1);
        op_obj_disp = 0;
        effect_48_init(0);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_101_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x65)) {
            op_w.r_no_2 += 1;
            op_w.index = 1;
            op_obj_disp = 1;
            op_work_clear();
            break;
        }

        op_bg_move(0);
        break;

    default:
        op_bg_move(1);
        break;
    }
}

const s16 op_102_sound[3] = { 0, 9, 12 };

void op_102_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_w.index = 2;
        effect_F6_init(2);
        effect_F6_init(3);
        effect_F6_init(4);
        op_obj_disp = 0;
        effect_48_init(1);
        op_work_clear();
        op_bg_move(2);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_102_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x66)) {
            op_w.r_no_2 += 1;
            op_w.index = 3;
            op_obj_disp = 1;
            op_work_clear();
            op_bg_move(3);
            return;
        }

        op_bg_move(2);
        break;

    case 2:
        if (gSeqStatus[0] >= op_102_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_w.index = 4;
            op_work_clear();
            op_bg_move(4);
            return;
        }

        op_bg_move(3);
        break;

    default:
        op_bg_move(4);
        break;
    }
}

const s16 op_103_sound[12] = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13 };

void op_103_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_w.index = 5;
        effect_F6_init(5);
        effect_F6_init(6);
        effect_F6_init(7);
        effect_F6_init(8);
        effect_F6_init(9);
        effect_F6_init(10);
        op_work_clear();
        op_bg_move(5);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x67)) {
            op_w.r_no_2 += 1;
            op_w.index = 6;
            op_work_clear();
            effect_F6_init(11);
            effect_F6_init(12);
            effect_F6_init(13);
            effect_F6_init(14);
            effect_F6_init(15);
            effect_F6_init(16);
            return;
        }

        op_bg_move(5);
        break;

    case 2:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 7;
            op_obj_disp = 0;
            effect_48_init(8);
            return;
        }

        op_bg_move(6);
        break;

    case 3:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 8;
            op_obj_disp = 1;
            op_scrn_end = 0;
            effect_36_init(22);
            return;
        }

        op_bg_move(7);
        break;

    case 4:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 9;
            return;
        }

        op_bg_move(8);
        break;

    case 5:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 10;
            return;
        }

        op_bg_move(9);
        break;

    case 6:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 11;
            return;
        }

        op_bg_move(10);
        break;

    case 7:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 12;
            op_obj_disp = 0;
            effect_48_init(9);
            return;
        }

        op_bg_move(11);
        break;

    case 8:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 13;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(12);
        break;

    case 9:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 14;
            op_scrn_end = 0;
            effect_36_init(23);
            return;
        }

        op_bg_move(13);
        break;

    case 10:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 15;
        }

        op_bg_move(14);
        break;

    case 11:
        if (gSeqStatus[0] >= op_103_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 16;
            return;
        }

        op_bg_move(15);
        break;

    default:
        op_bg_move(16);
        break;
    }
}

s16 op_104_sound[7] = { 0, 5, 6, 7, 9, 10, 11 };

void op_104_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 17;
        op_bg_move(17);
        effect_F6_init(17);
        effect_F6_init(18);
        effect_F6_init(19);
        effect_F6_init(20);
        effect_F6_init(21);
        effect_F6_init(22);
        effect_F6_init(23);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x68)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 18;
            return;
        }

        op_bg_move(17);
        break;

    case 2:
        if (gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 19;
            op_obj_disp = 0;
            effect_48_init(10);
            return;
        }

        op_bg_move(18);
        break;

    case 3:
        if (gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 20;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(19);
        break;

    case 4:
        if (gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 21;
            op_obj_disp = 0;
            effect_48_init(11);
            return;
        }

        op_bg_move(20);
        break;

    case 5:
        if (gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 22;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(21);
        break;

    case 6:
        if (gSeqStatus[0] >= op_104_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 23;
            return;
        }

        op_bg_move(22);
        break;

    case 7:
    default:
        op_bg_move(23);
        break;
    }
}

void op_105_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 24;
        op_bg_move(24);
        effect_F6_init(24);
        op_obj_disp = 0;
        effect_48_init(12);
        break;

    case 1:
    default:
        op_bg_move(24);
        break;
    }
}

const s16 op_106_sound[4] = { 0, 1, 3, 7 };

void op_106_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 25;
        op_obj_disp = 1;
        op_bg_move(25);
        effect_F6_init(25);
        effect_F6_init(26);
        effect_F6_init(27);
        effect_F6_init(28);
        effect_36_init(18);
        effect_36_init(19);
        effect_36_init(20);
        effect_36_init(21);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_106_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6A)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 26;
            return;
        }

        op_bg_move(25);
        break;

    case 2:
        if (gSeqStatus[0] >= op_106_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 27;
            return;
        }

        op_bg_move(26);
        break;

    case 3:
        if (gSeqStatus[0] >= op_106_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 28;
            op_obj_disp = 0;
            effect_48_init(15);
            return;
        }

        op_bg_move(27);
        break;

    default:
        op_bg_move(28);
        break;
    }
}

const s16 op_107_sound[12] = { 0, 1, 2, 3, 4, 5, 7, 8, 9, 11, 12, 13 };

void op_107_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_plmove_timer += 1;
        op_obj_disp = 1;
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 29;
        op_bg_move(29);
        effect_F6_init(29);
        effect_F6_init(30);
        effect_F6_init(31);
        effect_F6_init(32);
        effect_F6_init(33);
        effect_F6_init(34);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6B)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 30;
            return;
        }

        op_bg_move(29);
        break;

    case 2:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 31;
            return;
        }

        op_bg_move(30);
        break;

    case 3:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 32;
            op_obj_disp = 0;
            effect_48_init(13);
            return;
        }

        op_bg_move(31);
        break;

    case 4:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 33;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(32);
        break;

    case 5:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 34;
            effect_F6_init(35);
            effect_F6_init(36);
            effect_F6_init(37);
            effect_F6_init(38);
            effect_F6_init(39);
            effect_F6_init(40);
            return;
        }

        op_bg_move(33);
        break;

    case 6:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 35;
            return;
        }

        op_bg_move(34);
        return;

    case 7:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 36;
            op_obj_disp = 0;
            effect_48_init(14);
            return;
        }

        op_bg_move(35);
        break;

    case 8:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 37;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(36);
        break;

    case 9:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 38;
            return;
        }

        op_bg_move(37);
        break;

    case 10:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 39;
            return;
        }

        op_bg_move(38);
        break;

    case 11:
        if (gSeqStatus[0] >= op_107_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 40;
            return;
        }

        op_bg_move(39);
        break;

    default:
        op_bg_move(40);
        break;
    }
}

const s16 op_108_sound[13] = { 0, 4, 20, 24, 28, 32, 48, 52, 60, 64, 76, 80, 86 };

void op_108_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 41;
        op_bg_move(41);
        op_w.mv_ctr = 0;
        effect_36_init(0);
        effect_36_init(1);
        effect_36_init(2);
        effect_36_init(3);
        effect_36_init(4);
        effect_36_init(5);
        effect_36_init(6);
        effect_36_init(7);
        break;

    case 1:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 42;
            op_bg_move(42);
            return;
        }

        op_bg_move(41);
        break;

    case 2:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 43;
            op_bg_move(43);
            effect_36_init(8);
            effect_36_init(9);
            effect_36_init(10);
            effect_36_init(11);
            effect_36_init(12);
            effect_36_init(13);
            effect_36_init(14);
            effect_36_init(15);
            return;
        }

        break;

    default:
        break;

    case 3:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 44;
            op_bg_move(44);
            return;
        }

        op_bg_move(43);
        break;

    case 4:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 45;
            op_bg_move(45);
            return;
        }

        break;

    case 5:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 46;
            op_bg_move(46);
            return;
        }

        op_bg_move(45);
        break;

    case 6:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 47;
            op_bg_move(47);
            return;
        }

        break;

    case 7:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 48;
            op_bg_move(48);
            return;
        }

        op_bg_move(47);
        break;

    case 8:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 49;
            op_bg_move(49);
            return;
        }

        break;

    case 9:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 50;
            op_bg_move(50);
            return;
        }

        op_bg_move(49);
        break;

    case 10:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 51;
            op_bg_move(51);
            return;
        }

        break;

    case 11:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 52;
            op_bg_move(52);
            return;
        }

        op_bg_move(51);
        break;

    case 12:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_108_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 53;
            op_bg_move(53);
        }

        break;
    }
}

const s16 op_109_sound[5] = { 0, 3, 5, 7, 11 };

void op_109_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_scrn_end = 0;
        op_work_clear();
        op_w.index = 54;
        op_bg_move(54);
        op_obj_disp = 0;
        effect_48_init(16);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6D)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 55;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(54);
        break;

    case 2:
        if (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 56;
            return;
        }

        op_bg_move(55);
        break;

    case 3:
        if (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_scrn_end = 0;
            op_work_clear();
            op_w.index = 57;
            op_obj_disp = 0;
            effect_48_init(17);
            return;
        }

        op_bg_move(56);
        break;

    case 4:
        if (gSeqStatus[0] >= op_109_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 58;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(57);
        break;

    default:
        op_bg_move(58);
        break;
    }
}

const s16 op_110_sound[6] = { 0, 0, 3, 4, 7, 9 };

void op_110_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        Zoom_Value_Set(64);
        op_work_clear();
        op_w.index = 59;
        op_bg_move(59);
        op_obj_disp = 0;
        effect_48_init(2);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6E)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 60;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(59);
        break;

    case 2:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 61;
            op_obj_disp = 0;
            effect_48_init(3);
            return;
        }

        op_bg_move(60);
        break;

    case 3:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 62;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(61);
        break;

    case 4:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 63;
            op_obj_disp = 0;
            effect_48_init(4);
            return;
        }

        op_bg_move(62);
        break;

    case 5:
        if (gSeqStatus[0] >= op_110_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 64;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(63);
        break;

    default:
        op_bg_move(64);
        break;
    }
}

const s16 op_111_sound[5] = { 0, 2, 4, 7, 11 };

void op_111_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 65;
        op_bg_move(65);
        effect_F6_init(41);
        effect_F6_init(42);
        effect_F6_init(43);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x6F)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 66;
            return;
        }

        op_bg_move(65);
        break;

    case 2:
        if (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 67;
            return;
        }

        op_bg_move(66);
        break;

    case 3:
        if (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 68;
            return;
        }

        op_bg_move(67);
        break;

    case 4:
        if (gSeqStatus[0] >= op_111_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 69;
            return;
        }

        op_bg_move(68);
        break;

    default:
        op_bg_move(69);
        break;
    }
}

const s16 op_112_sound[9] = { 0, 8, 8, 14, 19, 26, 34, 40, 44 };

void op_112_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 70;
        op_bg_move(70);
        effect_F6_init(54);
        effect_F6_init(44);
        effect_F6_init(45);
        effect_F6_init(46);
        effect_F6_init(47);
        effect_F6_init(48);
        effect_F6_init(49);
        effect_F6_init(50);
        effect_F6_init(51);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_112_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x70)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 71;
            op_w.mv_ctr = 0;
            return;
        }

        op_bg_move(70);
        break;

    case 2:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 72;
            return;
        }

        op_bg_move(71);
        break;

    case 3:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 73;
            op_obj_disp = 0;
            effect_48_init(18);
            return;
        }

        op_bg_move(72);
        break;

    case 4:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 74;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(73);
        break;

    case 5:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 75;
            return;
        }

        op_bg_move(74);
        break;

    case 6:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 76;
            op_obj_disp = 0;
            effect_48_init(19);
            return;
        }

        op_bg_move(75);
        break;

    case 7:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 77;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(76);
        break;

    case 8:
        op_w.mv_ctr += 1;

        if (op_w.mv_ctr >= op_112_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 78;
            return;
        }

        op_bg_move(77);
        break;

    default:
        op_bg_move(78);
        break;
    }
}

const s16 op_113_sound[4] = { 0, 3, 7, 11 };

void op_113_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_scrn_end = 0;
        op_work_clear();
        op_w.index = 79;
        op_bg_move(79);
        effect_F6_init(52);
        effect_F6_init(53);
        op_obj_disp = 0;
        effect_48_init(20);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_113_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x71)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 80;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(79);
        break;

    case 2:
        if (gSeqStatus[0] >= op_113_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_scrn_end = 0;
            op_work_clear();
            op_w.index = 81;
            op_obj_disp = 0;
            effect_48_init(21);
            return;
        }

        op_bg_move(80);
        break;

    case 3:
        if (gSeqStatus[0] >= op_113_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 82;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(81);
        break;

    default:
        op_bg_move(82);
        break;
    }
}

const s16 op_114_sound[6] = { 0, 2, 3, 4, 7, 9 };

void op_114_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 83;
        op_bg_move(83);
        op_obj_disp = 0;
        effect_48_init(5);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x72)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 84;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(83);
        break;

    case 2:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 85;
            op_obj_disp = 0;
            effect_48_init(6);
            return;
        }

        op_bg_move(84);
        break;

    case 3:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 86;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(85);
        break;

    case 4:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            Zoom_Value_Set(64);
            op_work_clear();
            op_w.index = 87;
            op_obj_disp = 0;
            effect_48_init(7);
            return;
        }

        op_bg_move(86);
        break;

    case 5:
        if (gSeqStatus[0] >= op_114_sound[op_w.r_no_2]) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 88;
            op_obj_disp = 1;
            return;
        }

        op_bg_move(87);
        break;

    default:
        op_bg_move(88);
        break;
    }
}

const s16 op_115_sound[2] = { 0, 7 };

void op_115_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        Zoom_Value_Set(64);
        op_work_clear();
        op_w.index = 89;
        op_bg_move(89);
        effect_36_init(16);
        break;

    case 1:
        if ((gSeqStatus[0] >= op_115_sound[op_w.r_no_2]) && (gSeqStatus[0] != 0x73)) {
            op_w.r_no_2 += 1;
            op_work_clear();
            op_w.index = 90;
            return;
        }

        op_bg_move(89);
        break;

    default:
        op_bg_move(90);
        break;
    }
}

void op_116_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_work_clear();
        op_w.index = 91;
        setup_pos_remake_key(2);
        op_bg_move(91);
        effect_36_init(17);
        effect_36_init(28);
        op_w.mv_ctr = 88;
        FadeInit();
        break;

    case 1:
        op_bg_move(91);

        if (op_w.mv_ctr < 4) {
            FadeOut(0, 8, 8);
        }

        if (--op_w.mv_ctr <= 0) {
            op_w.r_no_2 += 1;
            op_w.mv_ctr = 16;
            return;
        }

        break;

    case 2:
        if ((FadeOut(0, 8, 8) != 0) && (--op_w.mv_ctr <= 0)) {
            op_w.index = 0x8000 - 1;
            op_end_flag = 1;
            op_w.r_no_2 += 1;
        }

        op_bg_move(91);
        break;

    default:
        op_bg_move(91);
        break;
    }
}

void op_117_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        Zoomf_Init();
        op_work_clear();
        op_w.index = 92;
        op_bg_move(92);
        effect_E1_init(1, 0, 1);
        effect_E1_init(0, 0, 1);
        effect_F5_init(16);
        effect_F5_init(17);
        effect_F5_init(18);
        effect_F5_init(9);
        op_bg_move(92);
        op_w.r_no_1 += 1;
        op_w.r_no_2 = 0;
        op_work_clear();
        break;
    }
}

void op_118_move() {
    switch (op_w.r_no_2) {
    case 0:
        op_w.r_no_2 += 1;
        op_w.mv_ctr = 60;
        break;

    case 1:
        op_w.mv_ctr -= 1;

        if (op_w.mv_ctr < 0) {
            op_w.r_no_2 += 1;
            return;
        }

        break;

    case 2:
        if (Request_Fade(0x28) != 0) {
            op_w.r_no_2 += 1;
            return;
        }

        break;

    case 3:
        if (Check_Fade_Complete() != 0) {
            op_w.r_no_2 += 1;
            Disp_Copyright();
            op_w.mv_ctr = 240;
        }

        op_w.index = 93;
        op_bg_move(93);
        break;

    case 4:
        op_w.mv_ctr -= 1;

        if (op_w.mv_ctr < 0) {
            op_w.r_no_2 += 1;
            return;
        }

        break;

    case 5:
        op_end_flag = 1;
        Bg_Off_W(14);
        op_bg_move(93);
        break;
    }
}

void op_bg_move(s16 r_index) {
    op_bg0_move(r_index);
    op_bg1_move(r_index);
    op_bg2_move(r_index);
}

void op_bg0_move(s16 r_index) {
    void (*op_bg0_move_jp[94])(
        s16) = { op_bg0_0000, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001,
                 op_bg0_0015, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0015, op_bg0_0001,
                 op_bg0_0000, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000,
                 op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0000, op_bg0_0001, op_bg0_0001,
                 op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001,
                 op_bg0_0000, op_bg0_0002, op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0002,
                 op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0002, op_bg0_0003, op_bg0_0000, op_bg0_0004, op_bg0_0001,
                 op_bg0_0001, op_bg0_0004, op_bg0_0005, op_bg0_0002, op_bg0_0001, op_bg0_0002, op_bg0_0001, op_bg0_0002,
                 op_bg0_0006, op_bg0_0001, op_bg0_0001, op_bg0_0001, op_bg0_0007, op_bg0_0008, op_bg0_0000, op_bg0_0000,
                 op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0001, op_bg0_0001, op_bg0_0000, op_bg0_0000, op_bg0_0004,
                 op_bg0_0001, op_bg0_0004, op_bg0_0001, op_bg0_0002, op_bg1_0003, op_bg0_0002, op_bg0_0010, op_bg0_0002,
                 op_bg0_0011, op_bg0_0012, op_bg0_0013, op_bg0_0014, op_bg0_0002, op_bg0_0016 };

    opw_ptr = &op_w.bgw[0];
    bgw_ptr = &bg_w.bgw[0];
    op_bg0_move_jp[r_index](r_index);
}

void op_bg0_0000(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_Off_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0001(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 0x1:
            oh_bg_blk_w(op_w.bgw, 0x30, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x31, 2, 0, 1);
            break;

        case 0x3:
            oh_bg_blk_wv(op_w.bgw, 0x45, 1, 0, 1);
            oh_bg_blk_wv(op_w.bgw, 0x46, 2, 0, 1);
            op_w.bgw[0].map[1][0].col.full = -0x01000000;
            op_w.bgw[0].map[2][0].col.full = -0x01000000;
            break;

        case 0x4:
        case 0x16:
        case 0x21:
            oh_bg_blk_wh(op_w.bgw, 0x35, 1, 0, 1);
            oh_bg_blk_wh(op_w.bgw, 0x34, 2, 0, 1);
            break;

        case 0x7:
            oh_bg_blk_w(op_w.bgw, 0x3F, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x40, 2, 0, 1);
            break;

        case 0x9:
        case 0x48:
            oh_bg_blk_w(op_w.bgw, 0x36, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x37, 2, 0, 1);
            break;

        case 0xB:
        case 0x1E:
            oh_bg_blk_wh(op_w.bgw, 0x33, 1, 0, 1);
            oh_bg_blk_wh(op_w.bgw, 0x32, 2, 0, 1);
            break;

        case 0xC:
            oh_bg_blk_w(op_w.bgw, 0x43, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x44, 2, 0, 1);
            break;

        case 0xF:
            oh_bg_blk_wh(op_w.bgw, 0x3C, 2, 0, 1);
            oh_bg_blk_wh(op_w.bgw, 0x3D, 1, 0, 1);
            break;

        case 0x12:
            oh_bg_blk_wv(op_w.bgw, 0x32, 1, 0, 1);
            oh_bg_blk_wv(op_w.bgw, 0x33, 2, 0, 1);
            break;

        case 0x13:
            oh_bg_blk_w(op_w.bgw, 0x47, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x48, 2, 0, 1);
            break;

        case 0x15:
            oh_bg_blk_w(op_w.bgw, 0x38, 1, 0, 1);
            op_w.bgw[0].map[2][0].g_no = 0;
            break;

        case 0x1F:
            oh_bg_blk_w(op_w.bgw, 0x49, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x4A, 2, 0, 1);
            break;

        case 0x20:
            oh_bg_blk_w(op_w.bgw, 0x4B, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x4C, 2, 0, 1);
            break;

        case 0x23:
            oh_bg_blk_w(op_w.bgw, 0x59, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x5A, 2, 0, 1);
            break;

        case 0x24:
            oh_bg_blk_w(op_w.bgw, 0x22, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x23, 2, 0, 1);
            break;

        case 0x26:
            oh_bg_blk_w(op_w.bgw, 0x3D, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x3E, 2, 0, 1);
            break;

        case 0x27:
            oh_bg_blk_w(op_w.bgw, 0x3D, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x3E, 2, 0, 1);
            break;

        case 0x37:
        case 0x38:
            oh_bg_blk_w(op_w.bgw, 0x4D, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x4E, 2, 0, 1);
            break;

        case 0x3C:
        case 0x3E:
        case 0x50:
        case 0x52:
            oh_bg_blk_w(op_w.bgw, 0x57, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x58, 2, 0, 1);
            break;

        case 0x41:
            oh_bg_blk_w(op_w.bgw, 0x4F, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x50, 2, 0, 1);
            break;

        case 0x42:
            oh_bg_blk_w(op_w.bgw, 0x51, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x52, 2, 0, 1);
            break;

        case 0x49:
            oh_bg_blk_w(op_w.bgw, 0x53, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x54, 2, 0, 1);
            break;

        case 0x4B:
            op_w.bgw[0].map[1][0].g_no = 0;
            op_w.bgw[0].map[2][0].g_no = 0;
            oh_bg_blk_whv(op_w.bgw, 0x53, 2, 0, 1);
            oh_bg_blk_whv(op_w.bgw, 0x54, 1, 0, 1);
            break;

        case 0x4C:
            oh_bg_blk_w(op_w.bgw, 0x55, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x56, 2, 0, 1);
            break;
        }

        /* fallthrough */

    case 1:
        bgw_ptr->free -= 1;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->free = 1;
            bgw_ptr->frame_deff += 1;
            bgw_ptr->frame_deff &= 0xF;
            bgw_ptr->xy[1].disp.pos += op_quake_y_tbl0[bgw_ptr->frame_deff];
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0002(s16 r_index) {
    PAL_CURSOR beta_poly;
    PAL_CURSOR_P beta_p[4];
    PAL_CURSOR_COL beta_col[4];

    beta_poly.p = beta_p;
    beta_poly.col = beta_col;
    beta_poly.num = 4;
    beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color = 0xFF000000;
    beta_p[0].x = beta_p[2].x = 0.0f;
    beta_p[1].x = beta_p[3].x = 384.0f * Frame_Zoom_X;
    beta_p[0].y = beta_p[1].y = 0.0f;
    beta_p[2].y = beta_p[3].y = 224.0f * Frame_Zoom_Y;

    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;

        Bg_Off_W(1);

        if (r_index == 87) {
            Zoomf_Init();
        }

        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        if (!No_Trans) {
            njDrawPolygon2D(&beta_poly, 4, PrioBase[75], 0x20);
        }

        break;

    case 1:
        if (!No_Trans) {
            njDrawPolygon2D(&beta_poly, 4, PrioBase[75], 0x20);
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0003(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 0x2A:
            oh_bg_blk_w(op_w.bgw, 0x24, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x25, 2, 0, 1);
            break;

        case 0x2C:
            oh_bg_blk_w(op_w.bgw, 0x26, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x27, 2, 0, 1);
            break;

        case 0x2E:
            oh_bg_blk_w(op_w.bgw, 0x28, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x29, 2, 0, 1);
            break;

        case 0x30:
            oh_bg_blk_w(op_w.bgw, 0x2A, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x2B, 2, 0, 1);
            break;

        case 0x32:
            oh_bg_blk_w(op_w.bgw, 0x2C, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x2D, 2, 0, 1);
            break;

        case 0x34:
            oh_bg_blk_w(op_w.bgw, 0x2E, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x2F, 2, 0, 1);
            break;
        }

        break;

    case 1:
        break;
    }

    op_scrn_pos_set2(0);
}

const s32 ot_bg0_0004_tbl[6] = { 0xFF00A0B0, 0xFF005888, 0xFF00A0B0, 0xFF005888, 0xFF000058, 0xFF000000 };

void op_bg0_0004(s16 r_index) {
    PAL_CURSOR beta_poly;
    PAL_CURSOR_P beta_p[4];
    PAL_CURSOR_COL beta_col[4];

    beta_poly.p = beta_p;
    beta_poly.col = beta_col;
    beta_poly.num = 4;
    beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color = 0xFF000000;
    beta_p[0].x = beta_p[2].x = 0.0f;
    beta_p[1].x = beta_p[3].x = 384.0f * Frame_Zoom_X;
    beta_p[0].y = beta_p[1].y = 0.0f;
    beta_p[2].y = beta_p[3].y = 224.0f * Frame_Zoom_Y;

    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_Off_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        if (!No_Trans) {
            njDrawPolygon2D(&beta_poly, 4, PrioBase[75], 0x20);
        }

        break;

    case 1:
        if (!No_Trans) {
            njDrawPolygon2D(&beta_poly, 4, PrioBase[75], 0x20);
        }

        if (!op_scrn_end) {
            break;
        }

        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->l_limit = 0;

        /* fallthrough */

    case 2:
        bgw_ptr->free -= 1;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->l_limit += 1;

            if (bgw_ptr->l_limit >= 6) {
                opw_ptr->r_no_0 += 1;
            } else {
                bgw_ptr->free = 1;
                beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color =
                    ot_bg0_0004_tbl[bgw_ptr->l_limit];
            }
        }

        if (!No_Trans) {
            njDrawPolygon2D(&beta_poly, 4, PrioBase[75], 0x20);
        }

        break;

    case 3:
        beta_col[0].color = beta_col[1].color = beta_col[2].color = beta_col[3].color = ot_bg0_0004_tbl[5];

        if (!No_Trans) {
            njDrawPolygon2D(&beta_poly, 4, PrioBase[75], 0x20);
        }

        break;
    }

    op_scrn_pos_set2(0);
}

const s16 op_bg0_0005_tbl[16] = { 0x0008, 0xFFF8, 0x0007, 0xFFF9, 0x0005, 0xFFFB, 0x0004, 0xFFFC,
                                  0x0002, 0xFFFE, 0x0001, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000 };

void op_bg0_0005(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, 5, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 6, 2, 0, 0);
        Zoom_Value_Set(0x40);
        bgw_ptr->frame_deff = 12;
        Frame_Up(0xC0, 0xE0, bgw_ptr->frame_deff);
        break;

    case 1:
        bgw_ptr->frame_deff -= 2;

        if (bgw_ptr->frame_deff <= 0) {
            opw_ptr->r_no_0 += 1;
            Zoomf_Init();
            Zoom_Value_Set(0x40);
            bgw_ptr->free = 0;
            bgw_ptr->wxy[0].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
            bgw_ptr->xy[1].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
        } else {
            Frame_Down(0xC0, 0xE0, 2);
        }

        break;

    case 2:
        bgw_ptr->free += 1;

        if (bgw_ptr->free >= 0x10) {
            opw_ptr->r_no_0 += 1;
        } else {
            bgw_ptr->wxy[0].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
            bgw_ptr->xy[1].disp.pos += op_bg0_0005_tbl[bgw_ptr->free];
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0006(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0xC00000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, 0xD, 0, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0xE, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0xF, 2, 0, 0);
        break;

    case 1:
        bgw_ptr->wxy[0].cal += 0x40000;

        if (bgw_ptr->wxy[0].disp.pos > 0x2C0) {
            opw_ptr->r_no_0 += 1;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0007(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x1D00000;
        bgw_ptr->xy[1].cal = 0xFFF00000;
        oh_bg_blk_w(op_w.bgw, 0x16, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0x17, 2, 0, 0);
        break;

    case 1:
        if (bgw_ptr->wxy[0].disp.pos < 0x200) {
            bgw_ptr->wxy[0].cal += 0x7FFF + 0x4001;
        } else {
            bgw_ptr->wxy[0].cal = 0x2000000;
        }

        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].cal += 0x4000;
        } else {
            bgw_ptr->xy[1].cal = 0;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0008(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2300000;
        bgw_ptr->xy[1].cal = 0xFFF00000;
        oh_bg_blk_w(op_w.bgw, 0x18, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0x19, 2, 0, 0);
        /* fallthrough */

    case 1:
        if (bgw_ptr->wxy[0].disp.pos > 0x200) {
            bgw_ptr->wxy[0].cal -= 0x8000 + 0x4000;
        } else {
            bgw_ptr->wxy[0].cal = 0x2000000;
        }

        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].cal += 0x4000;
        } else {
            bgw_ptr->xy[1].cal = 0;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0010(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        Zoomf_Init();
        Zoom_Value_Set(0x40);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, 0x12, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0x13, 2, 0, 0);
        break;

    case 1:
        opw_ptr->r_no_0 += 1;
        break;

    case 2:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->frame_deff = 12;
        Frame_Up(0xC0, 0xE0, bgw_ptr->frame_deff);
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0011(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1);
        Zoomf_Init();
        Zoom_Value_Set(0x40);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        oh_bg_blk_w(op_w.bgw, 0x14, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0x15, 2, 0, 0);
        break;

    case 1:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->frame_deff = 0x38;
        Frame_Up(0xC0, 0x40, bgw_ptr->frame_deff);
        bgw_ptr->xy[1].cal = 0xFFE00000;
        /* fallthrough */

    case 2:
        if (bgw_ptr->xy[1].disp.pos < 0) {
            bgw_ptr->xy[1].cal += 0x20000;
        }

        if (bgw_ptr->frame_deff > 0) {
            bgw_ptr->frame_deff -= 1;
            Frame_Down(0xC0, 0x40, 1);
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0012(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        Bg_On_W(1);
        Zoomf_Init();
        Zoom_Value_Set(0x40);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0x1000000;
        oh_bg_blk_w(op_w.bgw, 0x1E, 1, 1, 0);
        oh_bg_blk_w(op_w.bgw, 0x1F, 2, 1, 0);
        oh_bg_blk_w(op_w.bgw, 0x20, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0x21, 2, 0, 0);
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0xFFFE0000;

        if (bgw_ptr->xy[1].disp.pos < 0x61) {
            opw_ptr->r_no_0 += 1;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0013(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->frame_deff = 0;
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0x600000;
        oh_bg_blk_w(op_w.bgw, 0x1A, 1, 1, 0);
        oh_bg_blk_w(op_w.bgw, 0x1B, 2, 1, 0);
        oh_bg_blk_w(op_w.bgw, 0x1C, 1, 0, 0);
        oh_bg_blk_w(op_w.bgw, 0x1D, 2, 0, 0);
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0x20000;

        if (bgw_ptr->xy[1].disp.pos > 0x100) {
            opw_ptr->r_no_0 += 1;
        }

        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0014(s16 r_index) {
    op_bg0_0000(r_index);
}

const s32 ot_bg0_0015_tbl[6] = { 0xFF00A0B0, 0xFF005888, 0xFF00A0B0, 0xFF005888, 0xFF000058, 0xFF000000 };

void op_bg0_0015(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->free = 1;
        bgw_ptr->l_limit = 0;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 8:
            oh_bg_blk_w(op_w.bgw, 0x41, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x42, 2, 0, 1);
            break;

        case 14:
            oh_bg_blk_w(op_w.bgw, 0x45, 1, 0, 1);
            oh_bg_blk_w(op_w.bgw, 0x46, 2, 0, 1);
            break;
        }

        op_w.bgw[0].map[1][0].col.full = 0xFF000000;
        op_w.bgw[0].map[2][0].col.full = 0xFF000000;
        break;

    case 1:
        op_w.bgw[0].map[1][0].col.full = 0xFF000000;
        op_w.bgw[0].map[2][0].col.full = 0xFF000000;
        bgw_ptr->free -= 1;

        if (bgw_ptr->free <= 0) {
            bgw_ptr->l_limit += 1;

            if (bgw_ptr->l_limit >= 6) {
                opw_ptr->r_no_0 += 1;
            } else {
                bgw_ptr->free = 1;
                op_w.bgw[0].map[1][0].col.full = ot_bg0_0015_tbl[bgw_ptr->l_limit];
                op_w.bgw[0].map[2][0].col.full = ot_bg0_0015_tbl[bgw_ptr->l_limit];
            }
        }

        break;

    case 2:
        op_w.bgw[0].map[1][0].col.full = 0xFF000000;
        op_w.bgw[0].map[2][0].col.full = 0xFF000000;
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg0_0016(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1);
        bgw_ptr->wxy[0].cal = 0x2000000;
        bgw_ptr->xy[1].cal = 0;
        op_scrn_end = 0;
        bgw_ptr->frame_deff = 0x13;
        Frame_Up(0xC0, 0x70, bgw_ptr->frame_deff);
        bgw_ptr->free = 0xA;
        break;

    case 1:
        bgw_ptr->free -= 1;

        if (bgw_ptr->free <= 0) {
            opw_ptr->r_no_0 += 1;
        }

        break;

    case 2:
        bgw_ptr->frame_deff -= 1;

        if (bgw_ptr->frame_deff >= 0) {
            Frame_Down(0xC0, 0x70, 1);
        } else {
            opw_ptr->r_no_0 += 1;
            op_scrn_end = 1;
        }

        break;

    case 3:
        break;
    }

    op_scrn_pos_set2(0);
}

void op_bg1_move(s16 r_index) {
    opw_ptr = &op_w.bgw[1];
    bgw_ptr = &bg_w.bgw[1];

    switch (r_index) {
    case 55:
    case 56:
        op_bg1_0001(r_index);
        break;

    case 60:
        op_bg1_0002(r_index);
        break;

    case 62:
        op_bg1_0003(r_index);
        break;

    default:
        op_bg1_0000(r_index);
        break;
    }

    op_scrn_pos_set2(1);
}

void op_bg1_0000(s16 /* unused */) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 0;
        Bg_Off_W(2);
        break;

    case 1:
    case 2:
        break;
    }
}

void op_bg1_0001(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1 << bgw_ptr->fam_no);
        bgw_ptr->wxy[0].cal = 0x02000000;
        bgw_ptr->xy[1].cal = 0;

        switch (r_index) {
        case 0x37:
            oh_bg_blk_w(&op_w.bgw[1], 1, 1, 0, 0);
            oh_bg_blk_w(&op_w.bgw[1], 2, 2, 0, 0);
            break;

        case 0x38:
            oh_bg_blk_w(&op_w.bgw[1], 3, 1, 0, 0);
            oh_bg_blk_w(&op_w.bgw[1], 4, 2, 0, 0);
            break;
        }

        break;

    case 1:
        break;
    }
}

void op_bg1_0002(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1 << bgw_ptr->fam_no);

        switch (r_index) {
        case 0x3C:
            bgw_ptr->wxy[0].cal = 0x01000000;
            bgw_ptr->xy[1].cal = 0;
            oh_bg_blk_w(&op_w.bgw[1], 7, 0, 0, 0);
            oh_bg_blk_w(&op_w.bgw[1], 8, 1, 0, 0);
            oh_bg_blk_w(&op_w.bgw[1], 9, 2, 0, 0);
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp = 0xC0000;
            op_bg_mvxy[bgw_ptr->fam_no].d[0].sp = 0;
            bgw_ptr->r_limit = 0x1E0;
            break;

        default:
            break;
        }

        break;

    case 1:
        op_bg_mvxy[bgw_ptr->fam_no].a[0].sp += op_bg_mvxy[bgw_ptr->fam_no].d[0].sp;
        bgw_ptr->wxy[0].cal += op_bg_mvxy[bgw_ptr->fam_no].a[0].sp;

        if (bgw_ptr->wxy[0].disp.pos >= bgw_ptr->r_limit) {
            opw_ptr->r_no_0 += 1;
        }

        break;

    case 2:
        break;
    }
}

void op_bg1_0003(s16 r_index) {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        Bg_On_W(1 << bgw_ptr->fam_no);

        switch (r_index) {
        case 83:
            bgw_ptr->wxy[0].cal = 0x2200000;
            bgw_ptr->xy[1].cal = 0;
            oh_bg_blk_w(op_w.bgw, 0x10, 1, 0, 0);
            oh_bg_blk_w(op_w.bgw, 0x11, 2, 0, 0);
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp = 0x80000;
            op_bg_mvxy[bgw_ptr->fam_no].d[0].sp = -0x8000;
            bgw_ptr->r_limit = 0x200;
            break;

        case 62:
            bgw_ptr->wxy[0].cal = 0x1E00000; // low = 0, pos = 480
            bgw_ptr->xy[1].cal = 0;
            oh_bg_blk_w(&op_w.bgw[1], 0xA, 0, 0, 0);
            oh_bg_blk_w(&op_w.bgw[1], 0xB, 1, 0, 0);
            oh_bg_blk_w(&op_w.bgw[1], 0xC, 2, 0, 0);
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp = 0xFFF80000;
            op_bg_mvxy[bgw_ptr->fam_no].d[0].sp = 0;
            break;
        }

        break;

    case 1:
        op_bg_mvxy[bgw_ptr->fam_no].a[0].sp += op_bg_mvxy[bgw_ptr->fam_no].d[0].sp;
        bgw_ptr->wxy[0].cal +=
            op_bg_mvxy[bgw_ptr->fam_no].a[0].sp; // Move background horizontally by the specified offset/speed

        if (bgw_ptr->wxy[0].disp.pos <= bgw_ptr->l_limit) {
            opw_ptr->r_no_0 += 1;
        }

        break;

    case 2:
        break;
    }
}

void op_bg2_move(s16 r_index) {
    opw_ptr = &op_w.bgw[2];
    bgw_ptr = &bg_w.bgw[2];

    switch (r_index) {
    case 0:
    case 24:
        op_bg2_0000();
        break;

    case 2:
        op_bg2_0002();
        break;

    case 28:
        op_bg2_0003();
        break;

    default:
        op_bg2_0001();
        break;
    }

    op_scrn_pos_set2(2);
}

void op_bg2_0000() {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        bgw_ptr->wxy[0].cal -= (0x8000 + 0x8000);
        break;

    case 2:
        break;
    }
}

void op_bg2_0001() {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 512;
        bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        break;
    }
}

void op_bg2_0002() {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 0x200;
        bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        bgw_ptr->xy[1].cal += 0x10000;
        break;

    case 2:
        break;
    }
}

void op_bg2_0003() {
    switch (opw_ptr->r_no_0) {
    case 0:
        opw_ptr->r_no_0 += 1;
        bgw_ptr->wxy[0].disp.pos = 0x200;
        bgw_ptr->xy[1].disp.pos = 0;
        break;

    case 1:
        bgw_ptr->wxy[0].cal += 0x10000;
        break;

    case 2:
        break;
    }
}

void opening_title() {
    switch (op_w.r_no_1) {
    case 0:
        op_w.r_no_1 += 1;
        break;

    case 1:
        op_w.free_work -= 1;

        if (op_w.free_work <= 0) {
            opening_title_01();
            op_w.r_no_1 += 1;
        }

        break;

    case 2:
        break;
    }
}

void opening_title_01() {
    s16 pos_work_x;
    s16 pos_work_y;

    effect_E1_init(1, 0, 1);
    effect_E1_init(0, 0, 1);
    effect_F5_init(0x10);
    effect_F5_init(0x11);
    effect_F5_init(0x12);
    effect_F5_init(9);
    Disp_Copyright();
    Bg_Off_W(0xFU);
    Scrn_Move_Set(0, 0x200 - bg_w.pos_offset, 0);
    Scrn_Move_Set(1, 0x200 - bg_w.pos_offset, 0);
    pos_work_x = -(0x200 - bg_w.pos_offset);
    pos_work_y = 0x300;
    Family_Set_W(1, pos_work_x, pos_work_y);
    op_end_flag = 1;
    bg_stop = 0;
    akebono_flag = 0;
    aku_flag = 0;
    sa_pa_flag = 0;
    bg_app = 0;
    bg_w.chase_flag = 0;
}

void op_scrn_pos_set2(s16 bg_no) {
    s16 pos_x = bg_w.bgw[bg_no].wxy[0].disp.pos;
    s16 pos_y = bg_w.bgw[bg_no].xy[1].disp.pos;
    Scrn_Move_Set(bg_no, pos_x - bg_w.pos_offset, pos_y);
}

void Bg_Family_Set_op() {
    s16 pos_work_x;
    s16 pos_work_y;
    s16 i;

    for (i = 0; i < 4; i++) {
        bg_w.bgw[i].xy[0].cal = bg_w.bgw[i].wxy[0].cal;
        bg_w.bgw[i].position_y = bg_w.bgw[i].xy[1].disp.pos;
        bg_w.bgw[i].position_x = bg_w.bgw[i].wxy[0].disp.pos - bg_w.pos_offset;
        pos_work_x = -bg_w.bgw[i].position_x;
        pos_work_y = bg_w.bgw[i].position_y;
        pos_work_y = 768 - (pos_work_y & 0x3FF);
        Family_Set_W(i + 1, pos_work_x, pos_work_y);
    }

    bg_w.bgw[5].position_y = bg_w.bgw[5].xy[1].disp.pos;
    bg_w.bgw[5].position_x = bg_w.bgw[5].wxy[0].disp.pos - bg_w.pos_offset;
    pos_work_x = -bg_w.bgw[5].position_x;
    pos_work_y = bg_w.bgw[5].position_y;
    pos_work_y = 768 - (pos_work_y & 0x3FF);
    Family_Set_W(6, pos_work_x, pos_work_y);
}

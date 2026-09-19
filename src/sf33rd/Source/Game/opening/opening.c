/**
 * @file opening.c
 * Opening
 */

#include "sf33rd/Source/Game/opening/opening.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/demo/demo00.h"
#include "sf33rd/Source/Game/effect/eff36.h"
#include "sf33rd/Source/Game/effect/eff48.h"
#include "sf33rd/Source/Game/effect/effe1.h"
#include "sf33rd/Source/Game/effect/efff5.h"
#include "sf33rd/Source/Game/effect/efff6.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/opening/op_sub.h"
#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

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

static void tick_opening_title_timeout() {
    if (!Game_pause) {
        if (--op_timer0 == 0) {
            D_No[3] = 99;
        }
    }
}

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
        tick_opening_title_timeout();
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

    ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 601, 1, 0, 0});
    ppgSetupTexChunk_2nd(NULL, 601);
    ppgSetupTexChunk_3rd(NULL, 601, 1);
    Push_ramcnt_key(key);
    ppgSourceDataReleased(NULL);
    title_tex_flag = 1;
    op_w.r_no_0 = 0;
}

/* The title screen's own three-step sequence: zoom in, hold, then frame down.
 * Anything past it holds the zoom where the sequence left it. */
static void advance_title_frame(void) {
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
        if (Game_pause) {
            break;
        }

        if (op_timer0-- >= 0) {
            Frame_Down(0xC0, 0x70, 1);
        } else {
            op_w.r_no_0 += 1;
        }

        break;

    default:
        Zoom_Value_Set(0x40);
        break;
    }
}

s16 TITLE_Move(u16 type) {
    ppgSetupCurrentDataList(&ppgTitleList);

    if (type == 1) {
        Put_char(title[type], 601, 9, 192, 96, 1.0f, 1.0f);
        return 0;
    }

    if (type != 0) {
        return 0;
    }

    advance_title_frame();

    Put_char(title[type], 601, 9, 192, 96, scr_sc, scr_sc);
    return 0;
}

void OPBG_Init() {
    void* loadAdrs;
    size_t loadSize;
    s16 i;
    s16 key;

    ppgOpnBgList.tex = &ppgOpnBgTex;
    ppgOpnBgList.pal = palGetChunkGhostCP3();
    ppgSetupCurrentDataList(&ppgOpnBgList);

    if ((key = Search_ramcnt_type(0x1D)) == 0) {
        // Opening demo texture has not been loaded.
        flLogOut("オープニングデモテクスチャが読み込まれていません。\n");
        while (1) {}
    }

    loadSize = Get_size_data_ramcnt_key(key);
    loadAdrs = Get_ramcnt_pointer(key);
    ppgSetupTexChunk_1st(NULL, &(PPGTexChunk1stArgs){loadAdrs, loadSize, 602, 91, 0, 0});

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

static bool should_reload_opening_texture(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy) {
    return !oh_tsr_ck(blk_no) && (opbw->map[mapx][mapy].g_no != (blk_no + 0x259));
}

static bool should_release_opening_texture(OPBW* opbw, s16 mapx, s16 mapy) {
    return opbw->map[mapx][mapy].g_no && !oh_tsr_ck(opbw->map[mapx][mapy].g_no - 0x259);
}

void oh_reload_tex(OPBW* opbw, s32 blk_no, s16 mapx, s16 mapy) {
    if (should_reload_opening_texture(opbw, blk_no, mapx, mapy)) {
        if (should_release_opening_texture(opbw, mapx, mapy)) {
            ppgReleaseTextureHandle(&ppgOpnBgTex, opbw->map[mapx][mapy].g_no);
        }

        opbw->map[mapx][mapy].ok = 1;
    }
}

void oh_bg_blk_w(OPBW* opbw, const Op_Bg_Blk_Args* b) {
    oh_reload_tex(opbw, b->blk_no, b->mapx, b->mapy);
    opbw->map[b->mapx][b->mapy].g_no = b->blk_no + 0x259;
    opbw->map[b->mapx][b->mapy].hv = 0;
    opbw->map[b->mapx][b->mapy].trans = b->trans;
    opbw->blk_no = b->blk_no;
    opbw->map[b->mapx][b->mapy].col.full = 0xFFFFFFFF;
}

void oh_bg_blk_wh(OPBW* opbw, const Op_Bg_Blk_Args* b) {
    oh_reload_tex(opbw, b->blk_no, b->mapx, b->mapy);
    opbw->map[b->mapx][b->mapy].g_no = b->blk_no + 0x259;
    opbw->map[b->mapx][b->mapy].hv = 1;
    opbw->map[b->mapx][b->mapy].trans = b->trans;
    opbw->blk_no = b->blk_no;
    opbw->map[b->mapx][b->mapy].col.full = 0xFFFFFFFF;
}

void oh_bg_blk_wv(OPBW* opbw, const Op_Bg_Blk_Args* b) {
    oh_reload_tex(opbw, b->blk_no, b->mapx, b->mapy);
    opbw->map[b->mapx][b->mapy].g_no = b->blk_no + 0x259;
    opbw->map[b->mapx][b->mapy].hv = 2;
    opbw->map[b->mapx][b->mapy].trans = b->trans;
    opbw->blk_no = b->blk_no;
    opbw->map[b->mapx][b->mapy].col.full = 0xFFFFFFFF;
}

void oh_bg_blk_whv(OPBW* opbw, const Op_Bg_Blk_Args* b) {
    oh_reload_tex(opbw, b->blk_no, b->mapx, b->mapy);
    opbw->map[b->mapx][b->mapy].g_no = b->blk_no + 0x259;
    opbw->map[b->mapx][b->mapy].hv = 3;
    opbw->map[b->mapx][b->mapy].trans = b->trans;
    opbw->blk_no = b->blk_no;
    opbw->map[b->mapx][b->mapy].col.full = 0xFFFFFFFF;
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

/**
 * @file sel_pl_faces.c
 * The character-select display: the scrolling face grid, its background, and the
 * portraits and name plates drawn over it.
 *
 * Split out of sel_pl.c, which was over the Lines of Code threshold.
 */

#include "sf33rd/Source/Game/screen/sel_pl.h"
#include "sf33rd/Source/Game/screen/sel_pl_exit.h"
#include "common.h"
#include "constants.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/com/com_data.h"
#include "sf33rd/Source/Game/debug/debug_config.h"
#include "sf33rd/Source/Game/demo/demo_dat.h"
#include "sf33rd/Source/Game/effect/eff38.h"
#include "sf33rd/Source/Game/effect/eff39.h"
#include "sf33rd/Source/Game/effect/eff42.h"
#include "sf33rd/Source/Game/effect/eff43.h"
#include "sf33rd/Source/Game/effect/eff50.h"
#include "sf33rd/Source/Game/effect/eff52.h"
#include "sf33rd/Source/Game/effect/eff58.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff69.h"
#include "sf33rd/Source/Game/effect/eff70.h"
#include "sf33rd/Source/Game/effect/eff75.h"
#include "sf33rd/Source/Game/effect/eff76.h"
#include "sf33rd/Source/Game/effect/eff79.h"
#include "sf33rd/Source/Game/effect/eff93.h"
#include "sf33rd/Source/Game/effect/eff99.h"
#include "sf33rd/Source/Game/effect/effa5.h"
#include "sf33rd/Source/Game/effect/effd8.h"
#include "sf33rd/Source/Game/effect/effk6.h"
#include "sf33rd/Source/Game/engine/grade.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/pls02.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/pulpul.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mmtmcnt.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/screen/next_cpu.h"
#include "sf33rd/Source/Game/screen/sel_data.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/sysdir.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "sf33rd/Source/Game/screen/sel_pl_faces.h"

void Setup_BG_General();
s16 Setup_Face_X();
s16 Setup_Face_Y();
void Face_1st();
void Face_2nd();
void Face_3rd();
void Face_4th();
void OBJ_1st();
void OBJ_2nd();
void OBJ_3rd();

void Initialize_BG() {
    Setup_BG_General();
    Setup_BG(2, 512, 0);
    Setup_BG(3, 704, 0);
    Setup_FACE_BG();
}

void Setup_BG_General() {
    Zoomf_Init();
    bg_w.bgw[0].old_pos_x = bg_w.bgw[0].xy[0].disp.pos;
    bg_pos_hosei2();
    Bg_Family_Set();
}

void Setup_FACE_BG() {
    s16 face_x;
    s16 face_y;

    Unsubstantial_BG[1] = 1;
    face_x = Setup_Face_X();
    face_y = Setup_Face_Y();
    bg_w.bgw[1].xy[0].disp.pos = face_x;
    bg_w.bgw[1].xy[1].disp.pos = face_y;
    bg_w.bgw[1].wxy[0].disp.pos = face_x;
    bg_w.bgw[1].wxy[1].disp.pos = face_y;
    bg_w.bgw[1].xy[0].disp.low = 0;
    bg_w.bgw[1].xy[1].disp.low = 0;
    bg_w.bgw[1].position_x = face_x;
    bg_w.bgw[1].position_y = face_y;
    bg_w.bgw[1].hos_xy[0].disp.pos = bg_w.bgw[1].wxy[0].disp.pos = bg_w.bgw[1].xy[0].disp.pos;
    Bg_Family_Set_Ex(1);
}

s16 Setup_Face_X() {
    if (Play_Type == 1) {
        return 604;
    }

    if (Aborigine == 0) {
        return 512;
    }

    return 696;
}

s16 Setup_Face_Y() {
    if (Play_Type == 1) {
        return 0;
    }

    if (Aborigine == 0) {
        return -24;
    }

    return 0;
}

void Face_Control() {
    void (*Face_Jmp_Tbl[4])() = { Face_1st, Face_2nd, Face_3rd, Face_4th };
    Face_Jmp_Tbl[Face_No[0]]();
    Move_Face_BG();
}

void Face_1st() {
    if (Select_Status[0] == 3) {
        Face_No[0] = 3;
    } else {
        Face_No[0] = 1;
    }
}

void Face_2nd() {
    if (Select_Status[0] == 3 && Face_MV_Request == 0) {
        Face_No[0] = 3;
        Face_MV_Time = 1;

        if (Aborigine == 1) {
            Face_MV_Request = 2;
            bg_mvxy.a[0].sp = -0x90000;
            bg_mvxy.d[0].sp = -0x8000;
            return;
        }

        Face_MV_Request = 1;
        bg_mvxy.a[0].sp = 0x90000;
        bg_mvxy.d[0].sp = 0x8000;
        return;
    }

    if (Sel_PL_Complete[Aborigine]) {
        Face_MV_Time = 5;
        Face_No[0]++;

        if (Aborigine == 0) {
            Face_MV_Request = 4;
            bg_mvxy.a[0].sp = -0xC0000;
            bg_mvxy.d[0].sp = -0x8000;
            return;
        }

        Face_MV_Request = 3;
        bg_mvxy.a[0].sp = 0xC0000;
        bg_mvxy.d[0].sp = 0x8000;
    }
}

void Face_3rd() {
    if (Select_Status[0] != 3) {
        return;
    }

    if (Face_MV_Request != 0) {
        return;
    }

    Face_No[0]++;
    Face_MV_Time = 1;

    if (Aborigine == 1) {
        Face_MV_Request = 2;
        bg_mvxy.a[0].sp = -0xC0000;
        bg_mvxy.d[0].sp = -0x8000;
        return;
    }

    Face_MV_Request = 1;
    bg_mvxy.a[0].sp = 0xC0000;
    bg_mvxy.d[0].sp = 0x8000;
}

void Face_4th() {}

void Move_Face_BG() {
    switch (Face_No[1]) {
    case 0:
        if (Face_MV_Request) {
            Face_No[1]++;
            Face_Move = Face_MV_Request;
            effect_93_init(Face_Move - 1, Face_MV_Time);
        }

        break;

    default:
        if (!(Face_MV_Request = Face_Move)) {
            Face_No[1] = 0;
        }

        break;
    }
}

void OBJ_Control() {
    void (*OBJ_Jmp_Tbl[3])() = { OBJ_1st, OBJ_2nd, OBJ_3rd };
    OBJ_Jmp_Tbl[SO_No[0]]();
}

void OBJ_1st() {
    Setup_EFF69();

    if (Select_Status[0] != 3) {
        SO_No[0] = 1;
        effect_38_init(Aborigine, Aborigine + 11, 127, 0, 2);
        Order[Aborigine + 11] = 1;
        Order_Timer[Aborigine + 11] = 35;
        effect_52_init(Aborigine, 37);
        Order[37] = 1;
        Order_Timer[37] = 30;
        Order_Dir[37] = 0;
        effect_K6_init(Aborigine, Aborigine + 31, 31, 2);
        Order[Aborigine + 31] = 1;
        Order_Timer[Aborigine + 31] = 35;
        Order_Dir[Aborigine + 31] = 0;
        effect_K6_init(Aborigine, Aborigine + 25, 25, 2);
        Order[Aborigine + 25] = 1;
        Order_Timer[Aborigine + 25] = 35;
        Order_Dir[Aborigine + 25] = 0;
        Order[0] = 1;
        Order_Timer[0] = 40;
        Order_Dir[0] = 4;
        Order[1] = 1;
        Order_Timer[1] = 45;
        Order_Dir[1] = 4;
        Order[3] = 1;
        Order_Timer[3] = 45;
        Order_Dir[3] = 4;
        effect_39_init(Aborigine, Aborigine + 13, 127, 2, 1);
        Order[Aborigine + 13] = 1;
        Order_Timer[Aborigine + 13] = 35;
        Order_Dir[Aborigine + 13] = 0;
        effect_42_init(5);
        Order[5] = 1;
        Order_Timer[5] = 45;
        Order_Dir[5] = 4;
        effect_42_init(6);
        Order[6] = 1;
        Order_Timer[6] = 45;
        Order_Dir[6] = 4;
        return;
    }

    SO_No[0] = 2;
    effect_75_init(42, 3, 2);
    Order[42] = 3;
    Order_Timer[42] = 1;
    Order_Dir[42] = 3;
    effect_38_init(0, 11, 127, 1, 2);
    Order[11] = 1;
    Order_Timer[11] = 86;
    effect_38_init(1, 12, 127, 1, 2);
    Order[12] = 1;
    Order_Timer[12] = 86;
    effect_K6_init(0, 33, 31, 2);
    Order[33] = 1;
    Order_Timer[33] = 86;
    Order_Dir[33] = 0;
    effect_52_init(0, 38);
    Order[38] = 3;
    Order_Timer[38] = 30;
    effect_K6_init(0, 27, 25, 2);
    Order[27] = 3;
    Order_Timer[27] = 86;
    effect_K6_init(1, 28, 25, 2);
    Order[28] = 3;
    Order_Timer[28] = 86;
    effect_K6_init(1, 34, 31, 2);
    Order[34] = 1;
    Order_Timer[34] = 86;
    Order_Dir[34] = 0;
    effect_52_init(1, 39);
    Order[39] = 3;
    Order_Timer[39] = 30;
    effect_39_init(0, 15, 127, 2, 0);
    Order[15] = 1;
    Order_Timer[15] = 86;
    Order_Dir[15] = 0;
    effect_39_init(1, 16, 127, 2, 0);
    Order[16] = 1;
    Order_Timer[16] = 86;
    Order_Dir[16] = 0;
    Order[4] = 3;
    Order_Timer[4] = 86;
    Order_Dir[4] = 255;
    effect_42_init(7);
    Order[7] = 0;
    Order_Timer[7] = 86;
    effect_42_init(8);
    Order[8] = 0;
    Order_Timer[8] = 86;
}

void OBJ_2nd() {
    if (Select_Status[0] != 3) {
        return;
    }

    SO_No[0]++;
    effect_75_init(42, 3, 2);
    Order[42] = 3;
    Order_Timer[42] = 1;
    Order_Dir[42] = 3;
    Order[Aborigine + 11] = 4;
    Order_Timer[Aborigine + 11] = 1;
    Select_Start[Aborigine] = 2;
    effect_38_init(New_Challenger, New_Challenger + 11, 127, 1, 2);
    Order[New_Challenger + 11] = 1;
    Order_Timer[New_Challenger + 11] = 1;
    Go_Away_Red_Lines();
    Order[Aborigine + 31] = 5;
    Order_Timer[Aborigine + 31] = 1;
    Order[Aborigine + 19] = 5;
    Order_Timer[Aborigine + 19] = 1;
    Order[Aborigine + 25] = 5;
    Order_Timer[Aborigine + 25] = 1;
    Order[Aborigine + 13] = 5;
    Order_Timer[Aborigine + 13] = 1;
    Order[37] = 4;
    Order_Timer[37] = 1;
    effect_K6_init(0, 33, 31, 2);
    Order[33] = 1;
    Order_Timer[33] = 1;
    Order_Dir[33] = 0;
    effect_K6_init(0, 27, 25, 2);
    Order[27] = 1;
    Order_Timer[27] = 1;
    Order_Dir[27] = 0;
    effect_39_init(0, 15, 127, 2, 0);
    Order[15] = 1;
    Order_Timer[15] = 1;
    Order_Dir[15] = 0;
    effect_K6_init(1, 34, 31, 2);
    Order[34] = 1;
    Order_Timer[34] = 1;
    Order_Dir[34] = 0;
    effect_K6_init(1, 28, 25, 2);
    Order[28] = 1;
    Order_Timer[28] = 1;
    Order_Dir[28] = 0;
    effect_39_init(1, 16, 127, 2, 0);
    Order[16] = 1;
    Order_Timer[16] = 1;
    Order_Dir[16] = 0;
    Order[4] = 3;
    Order_Timer[4] = 1;
    Order_Dir[4] = 255;
    effect_42_init(7);
    Order[7] = 0;
    Order_Timer[7] = 1;
    effect_42_init(8);
    Order[8] = 0;
    Order_Timer[8] = 1;
}

void OBJ_3rd() {}

void Setup_EFF69() {
    s16 xx;

    for (xx = 0; xx < 5; xx++) {
        Order[xx] = 0;
        effect_69_init(xx);
    }
}

void Go_Away_Red_Lines() {
    Order[0] = 2;
    Order_Timer[0] = 1;
    Order_Dir[0] = 8;
    Order[2] = 2;
    Order_Timer[2] = 1;
    Order_Dir[2] = 8;
    Order[1] = 2;
    Order_Timer[1] = 1;
    Order_Dir[1] = 8;
    Order[3] = 2;
    Order_Timer[3] = 1;
    Order_Dir[3] = 8;
    Order[5] = 2;
    Order[6] = 2;
    Order_Timer[5] = 1;
    Order_Timer[6] = 1;
    Order_Dir[5] = 8;
    Order_Dir[6] = 8;
}

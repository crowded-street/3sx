/**
 * @file demo00.c
 * Demo Sequence 0
 */

#include "sf33rd/Source/Game/demo/demo00.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Game/AcrUtil.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/GD3rd.h"
#include "sf33rd/Source/Game/MTRANS.h"
#include "sf33rd/Source/Game/OPENING.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/op_sub.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/texgroup.h"
#include "sf33rd/Source/Game/workuser.h"
#include "structs.h"

void CAPLOGO_Init();
s16 CAPLOGO_Move(u16 type);

static const f32 caplogo00[17] = { 0.25f, 0.25f, 1.0f,  0.5f,   0.0f, 0.0f,   192.0f, 64.0f, 0.0f,
                                   0.5f,  1.0f,  0.75f, 192.0f, 0.0f, 256.0f, 64.0f,  -1.0f };

static const f32 caplogo01[17] = { 0.0f,  0.0f,  1.0f, 0.25f,  0.0f, 0.0f,  256.0f, 64.0f, 0.0f,
                                   0.25f, 0.25f, 0.5f, 256.0f, 0.0f, 64.0f, 64.0f,  -1.0f };

static const f32* caplogo[2] = { caplogo00, caplogo01 };

s16 picon_no;
f32 picon_level;

s32 Warning() {
    setTexAdrsMode(0);
    setFilterMode(1);
    Next_Demo = 0;

    switch (D_No[1]) {
    case 0:
        D_No[1] = 5;
        D_No[1] = 9;
        D_Timer = 120;
        FadeInit();
        Next_Demo = 0;
        break;

    case 5:
        Put_Warning(1);
        Next_Demo = 0;

        if (FadeIn(1, 8, 8) != 0) {
            D_No[1] += 1;
            D_Timer = 120;
        }

        break;

    case 6:
        if ((Usage == 7) && (((p1sw_0 & 0x4FF0) | (p2sw_0 & 0x4FF0)) != 0)) {
            D_Timer = 2;
            D_No[1] = 7;
            FadeInit();
        }

        Put_Warning(1);
        Next_Demo = 0;

        if (!--D_Timer) {
            D_No[1] += 1;
            D_Timer = 180;
        }

        break;

    case 7:
        if (((p1sw_0 & 0x4FF0) | (p2sw_0 & 0x4FF0)) != 0) {
            D_Timer = 1;
        }

        if (!--D_Timer) {
            D_No[1] += 1;
            FadeInit();
        }

        Put_Warning(1);
        Next_Demo = 0;
        break;

    case 8:
        Put_Warning(1);
        Next_Demo = 0;

        if (FadeOut(1, 8, 8) != 0) {
            D_No[1] += 1;
        }

        break;

    default:
        D_No[1] = 0;
        TexRelease(590);
        Next_Demo = 1;
        break;
    }

    return Next_Demo;
}

s32 CAPCOM_Logo() {
    setTexAdrsMode(0);
    setFilterMode(0);
    ppgSetupCurrentDataList(&ppgCapLogoList);
    Next_Demo = 0;

    switch (D_No[1]) {
    case 0:
        D_No[1] += 1;
        checkAdxFileLoaded();
        checkSelObjFileLoaded();
        break;

    case 1:
        D_No[1] += 1;
        setup_pos_remake_key(1);
        Standby_BGM(67);
        CAPLOGO_Init();
        Push_LDREQ_Queue_Direct(0x16, 2);
        FadeInit();
        break;

    case 2:
        if (Check_LDREQ_Clear() != 0) {
            D_No[1] += 1;
            D_Timer = 10;
        }

        break;

    case 3:
        if (--D_Timer == 0) {
            D_No[1] += 1;
            op_timer0 = 0;
            Go_BGM();
        }

        break;

    case 4:
        if (!CAPLOGO_Move(0)) {
            D_No[1] += 1;
            Push_LDREQ_Queue_Direct(0x17, 2);
            FadeInit();
        }

        break;

    case 5:
        CAPLOGO_Move(1);

        if (FadeIn(1, 6, 8) != 0) {
            D_No[1] += 1;
            D_Timer = 256;
            Push_LDREQ_Queue_Direct(0x18, 2);
        }

        break;

    case 6:
        CAPLOGO_Move(1);

        if (--D_Timer == 0) {
            D_No[1] += 1;
            FadeInit();
        }

        break;

    case 7:
        CAPLOGO_Move(1);

        if (FadeOut(1, 6, 8) != 0) {
            D_No[1] += 1;
        }

        break;

    default:
        TexRelease(600);
        Next_Demo = 1;
        break;
    }

    return Next_Demo;
}

void CAPLOGO_Init() {
    void* loadAdrs;
    u32 loadSize;
    s16 key;

    mmDebWriteTag("\nCAPCOM LOGO\n\n");
    ppgCapLogoList.tex = &ppgCapLogoTex;
    ppgCapLogoList.pal = &ppgCapLogoPal;
    ppgSetupCurrentDataList(&ppgCapLogoList);
    loadSize = load_it_use_any_key2(75, &loadAdrs, &key, 2, 1); // CapLogo.ppg

    if (loadSize == 0) {
        flLogOut("カプロゴのテクスチャが読み込めませんでした。\n");
        while (1) {}
    }

    ppgSetupPalChunk(NULL, loadAdrs, loadSize, 0, 0, 1);
    ppgSetupTexChunk_1st(NULL, loadAdrs, loadSize, 600, 1, 0, 0);
    ppgSetupTexChunk_2nd(NULL, 600);
    ppgSetupTexChunk_3rd(NULL, 600, 1);
    Push_ramcnt_key(key);
    ppgSourceDataReleased(0);
}

s16 CAPLOGO_Move(u16 type) {
    s16 rnum = 0;

    switch (type) {
    case 0:
        if (!Game_pause && (op_timer0 != 61)) {
            njSetPaletteBankNumG(600, op_timer0 / 2);
            op_timer0 += 1;
            rnum = 1;
        }

        Put_char(caplogo[type], 600, 9, -16, 80, 1.0f, 1.0f);
        break;

    default:
        njSetPaletteBankNumG(600, 0x1F);
        Put_char(caplogo[type], 600, 9, 48, 88, 1.0f, 1.0f);
        break;
    }

    return rnum;
}

void Put_char(const f32* ptr, u32 indexG, u16 prio, s16 x, s16 y, f32 zx, f32 zy) {
    Polygon tex[4];
    s16 off_x;
    s16 off_y;

    if (No_Trans) {
        return;
    }

    tex[0].col = tex[1].col = tex[2].col = tex[3].col = 0xFFFFFFFF;
    tex[0].z = tex[1].z = tex[2].z = tex[3].z = PrioBase[prio];

    while (*ptr != -1.0f) {
        tex[0].u = tex[1].u = *ptr++;
        tex[0].v = tex[2].v = *ptr++;
        tex[2].u = tex[3].u = *ptr++;
        tex[1].v = tex[3].v = *ptr++;
        off_x = *ptr++;
        off_y = *ptr++;
        tex[0].x = tex[1].x = Frame_Zoom_X * (x + off_x * zx);
        tex[0].y = tex[2].y = Frame_Zoom_Y * (y + off_y * zy);
        tex[2].x = tex[3].x = Frame_Zoom_X * (x + (off_x * zx) + ((u32)*ptr++ * zx));
        tex[1].y = tex[3].y = Frame_Zoom_Y * (y + (off_y * zy) + ((u32)*ptr++ * zy));
        njDrawTexture(tex, 4, indexG, 1);
    }
}

void Warning_Init() {
    void* loadAdrs;
    u32 loadSize;
    s16 key;
    s16 i;

    mmDebWriteTag("\nWARNING\n\n");
    ppgWarList.tex = &ppgWarTex;
    ppgWarList.pal = &ppgWarPal;
    ppgAdxList.tex = &ppgWarTex;
    ppgAdxList.pal = &ppgAdxPal;
    ppgSetupCurrentDataList(&ppgWarList);
    loadSize = load_it_use_any_key2(12, &loadAdrs, &key, 2, 1); // Warning.ppg

    if (loadSize == 0) {
        flLogOut("警告文のテクスチャが読み込めませんでした。\n");
        while (1) {}
    }

    ppgSetupPalChunk(&ppgWarPal, loadAdrs, loadSize, 0, 0, 1);
    ppgSetupPalChunk(&ppgAdxPal, loadAdrs, loadSize, 0, 1, 1);
    ppgSetupTexChunk_1st(0, loadAdrs, loadSize, 590, 4, 0, 0);

    for (i = 0; i < ppgWarTex.textures; i++) {
        ppgSetupTexChunk_2nd(0, i + 590);
        ppgSetupTexChunk_3rd(0, i + 590, 1);
    }

    Push_ramcnt_key(key);
    ppgSourceDataReleased(0);
    picon_no = 0;
}

void Put_Warning(s16 type) {
    Polygon tex[4];
    f32 video_y = 16.0f;

    tex[0].col = tex[1].col = tex[2].col = tex[3].col = 0xFFFFFFFF;

    if (type == 2) {
        tex[0].z = tex[1].z = tex[2].z = tex[3].z = PrioBase[0x14];
    } else {
        tex[0].z = tex[1].z = tex[2].z = tex[3].z = PrioBase[0x1E];
    }

    switch (type) {
    case 0:
        ppgSetupCurrentDataList(&ppgWarList);
        ppgSetupCurrentPaletteNumber(0, 0);
        tex[0].u = tex[1].u = 0.0f;
        tex[0].v = tex[2].v = 0.0f;
        tex[2].u = tex[3].u = 1.0f;
        tex[1].v = tex[3].v = 1.0f;
        tex[0].x = tex[1].x = 0.0f;
        tex[0].y = tex[2].y = 0.0f;
        tex[2].x = tex[3].x = flPs2State.DispWidth;
        tex[1].y = tex[3].y = flPs2State.DispHeight;
        break;

    case 1:
        ppgSetupCurrentDataList(&ppgAdxList);
        ppgSetupCurrentPaletteNumber(0, 0);
        tex[0].u = tex[1].u = 0.0f;
        tex[0].v = tex[2].v = 0.0f;
        tex[2].u = tex[3].u = 1.0f;
        tex[1].v = tex[3].v = 0.875f;
        tex[0].x = tex[1].x = 0.0f;
        tex[0].y = tex[2].y = 0.0f;
        tex[2].x = tex[3].x = flPs2State.DispWidth;
        tex[1].y = tex[3].y = flPs2State.DispHeight;
        break;

    case 2:
    case 3:
    default:
        tex[0].x = tex[1].x = 64.0f;
        tex[0].y = tex[2].y = -32.0f;
        tex[2].x = tex[3].x = 576.0f;
        tex[1].y = tex[3].y = 480.0f;
        break;
    }

    if (type == 2) {
        njDrawTexture(tex, 4, type + 590, 1);
        return;
    }

    njDrawTexture(tex, 4, type + 590, 0);
}

void Pal_Cursor_Put(s16 type) {
    PAL_CURSOR_TBL pal_cursor_tbl[3] = {
        { { { 48.0f, 64.0f }, { 48.0f, 99.0f }, { 144.0f, 99.0f }, { 144.0f, 64.0f } },
          { { 0xA0FF0000 }, { 0xA0FF0000 }, { 0xA0FF0000 }, { 0xA0FF0000 } } },
        { { { 48.0f, 296.0f }, { 48.0f, 332.0f }, { 286.0f, 332.0f }, { 286.0f, 296.0f } },
          { { 0xA0FF0000 }, { 0xA0FF0000 }, { 0xA0FF0000 }, { 0xA0FF0000 } } },
        { { { 48.0f, 379.0f }, { 48.0f, 415.0f }, { 286.0f, 415.0f }, { 286.0f, 379.0f } },
          { { 0xA0FF0000 }, { 0xA0FF0000 }, { 0xA0FF0000 }, { 0xA0FF0000 } } }
    };

    f32 pal_alpha_tbl[4] = { 255.0f, 48.0f, 178.5f, 48.0f };
    PAL_CURSOR pal_cursor;
    f32 prio;
    PAL_CURSOR_TBL* pal_cursorwk;
    s16 i;

    switch (picon_no) {
    case 0:
        picon_no += 1;
        picon_level = pal_alpha_tbl[0];
        break;

    case 1:
        picon_level -= (pal_alpha_tbl[0] - pal_alpha_tbl[2]) / pal_alpha_tbl[1];

        if (picon_level <= pal_alpha_tbl[2]) {
            picon_level = pal_alpha_tbl[2];
            picon_no = 2;
        }

        break;

    case 2:
        picon_level += (pal_alpha_tbl[0] - pal_alpha_tbl[2]) / pal_alpha_tbl[3];

        if (!(picon_level < pal_alpha_tbl[0])) {
            picon_no = 1;
            picon_level = pal_alpha_tbl[0];
        }

        break;
    }

    if (No_Trans) {
        return;
    }

    pal_cursorwk = pal_cursor_tbl;
    prio = PrioBase[0x50];
    pal_cursor.p = pal_cursorwk[type].pal_cursor_p;
    pal_cursor.col = pal_cursorwk[type].pal_cursor_col;
    pal_cursor.num = 4;

    for (i = 0; i < 4; i++) {
        pal_cursor.col[i].argb.a = picon_level;
    }

    njDrawPolygon2D(&pal_cursor, 4, prio, 0x60);
}

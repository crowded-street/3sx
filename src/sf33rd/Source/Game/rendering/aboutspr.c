/**
 * @file aboutspr.c
 * Sprite, Shadow, and Debug rendering utilities
 */

#include "sf33rd/Source/Game/rendering/aboutspr.h"
#include "common.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/charid.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texgroup.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "sf33rd/Source/Game/system/work_sys.h"

// bss
MultiTexture mts[24];
MTS_OK mts_ok[24];
WORK dmwk_moji;
WORK dmwk_kage;

// first part of rodata
const u16 effk8k9_pattern[18] = { 0x9020, 0x9021, 0x9022, 0x9023, 0x9024, 0x9025, 0x9026, 0x9027, 0x9029,
                                  0x902A, 0x902B, 0x902C, 0x902D, 0x902E, 0x9030, 0x9032, 0x9034, 0x9036 };

const u32 judge_area_attr[17][2] = { { 0x7FFFDFFF, 0x60 }, { 0x7FFFCFFF, 0x60 }, { 0x7FFFAFFF, 0x60 },
                                     { 0x7FFF9FFF, 0x60 }, { 0x7FFF5FFF, 0x60 }, { 0x7FFF4FFF, 0x60 },
                                     { 0x7FFF3FFF, 0x60 }, { 0x7FFF2FFF, 0x60 }, { 0xAFFF3F00, 0x60 },
                                     { 0x7FFF7F00, 0x60 }, { 0xAF00FFFF, 0x60 }, { 0xAF00EFFF, 0x60 },
                                     { 0x7F00AFFF, 0x60 }, { 0x7F009FFF, 0x60 }, { 0x7FFFFFFF, 0x40 },
                                     { 0x7FFFFFFF, 0x20 }, { 0x7FFFFFFF, 0x60 } };

static const u32 HURTBOX_OUTLINE_COLOR = 0xFF0000FF;
static const u32 HITBOX_OUTLINE_COLOR = 0xFFFF0000;
static const u32 BOX_FILL_ALPHA = 0x33000000;

static u8 get_judge_highlight_alpha(s16 fade_cja) {
    if (fade_cja & 0x80) {
        return (u8)(0xC0 - (fade_cja & 0x7F));
    }

    return (u8)(fade_cja + 0x40);
}

static u32 set_color_alpha(u32 color, u8 alpha) {
    return (color & 0xFFFFFF) | ((u32)alpha << 24);
}

static u32 get_judge_outline_color(s16 index) {
    // 0..9 are hurt/catch/caught, 10..13 are attack, others use fallback.
    if (index >= 10 && index <= 13) {
        return HITBOX_OUTLINE_COLOR;
    }

    if (index >= 0 && index <= 9) {
        return HURTBOX_OUTLINE_COLOR;
    }

    return 0xFFFFFFFF;
}

static void draw_hit_judge_box(f32 px, f32 py, f32 sx, f32 sy, u32 fill_col, u32 outline_col, u32 attr) {
    f32 thickness_sx;
    f32 thickness_sy;

    if (sx < 0.0f) {
        px += sx;
        sx = -sx;
    }

    if (sy < 0.0f) {
        py += sy;
        sy = -sy;
    }

    if (sx <= 0.0f || sy <= 0.0f) {
        return;
    }

    thickness_sx = (sx < 1.0f) ? sx : 1.0f;
    thickness_sy = (sy < 1.0f) ? sy : 1.0f;

    draw_hit_judge_line(px, py, sx, sy, fill_col, attr);
    draw_hit_judge_line(px, py, sx, thickness_sy, outline_col, attr);
    draw_hit_judge_line(px, py + sy - thickness_sy, sx, thickness_sy, outline_col, attr);
    draw_hit_judge_line(px, py, thickness_sx, sy, outline_col, attr);
    draw_hit_judge_line(px + sx - thickness_sx, py, thickness_sx, sy, outline_col, attr);
}

void Init_load_on_memory_data() {
    copy_char_base_data();
    load_any_color(0x9C, 0x18);
    load_any_color(0x9D, 0x1F);
    load_any_color(0x14, 2);
    reservMemKeySelObj();
    load_any_texture_patnum(0x72A0, 0xF, 0);
    load_any_texture_patnum(0x7F30, 0xC, 0);
    dmwk_kage.my_mts = 0x11;
    dmwk_kage.current_colcd = 0x1FF;
    dmwk_kage.my_clear_level = 0x90;
    dmwk_kage.work_id = 0x10;
    dmwk_moji.work_id = 0x10;
}

s32 setup_GILL_exsa_obj() {
    WORK* ewk;
    s16 i;
    s16 ix;

    if ((ix = pull_effect_work(5)) == -1) {
        return -1;
    }
    ewk = (WORK*)frw[ix];
    ewk->disp_flag = 1;
    ewk->my_mts = 6;
    ewk->my_col_code = 2;

    for (i = 0; i < 8; i++) {
        ewk->cg_number = effk8k9_pattern[i];
        sort_push_request(ewk);
    }
    ewk->my_col_code = 10;

    for (i = 8; i < 18; i++) {
        ewk->cg_number = effk8k9_pattern[i];
        sort_push_request(ewk);
    }
    push_effect_work(ewk);
    return 1;
}

s32 setup_GILL_Opening_Ceremony() {
    return 1;
}

void setup_bonus_car_parts() {
    // Do nothing
}

void setup_dma_group(u16 num, u32 /* unused */) {
    load_any_texture_patnum(num, 2, 0);
}

void reset_dma_group(u16 num) {
    purge_texture_group_of_this(num);
}

void set_judge_area_sprite(WORK_Other_JUDGE* wk, s16 bsy) {
    s16 i;
    s16 mirror_factor;
    u8 highlight_alpha;
    bool is_selected;
    u32 outline_col;
    u32 fill_col;

    mlt_obj_matrix(&wk->wu, bsy);

    if (wk->wu.rl_flag) {
        mirror_factor = -1;
    } else {
        mirror_factor = 1;
    }

    for (i = 0; i < 15; i++) {
        if (wk->ja_disp_bit & (1 << i)) {
            outline_col = get_judge_outline_color(i);
            fill_col = (outline_col & 0xFFFFFF) | BOX_FILL_ALPHA;
            // Flash the currently selected judge slot using curr_ja/fade_cja.
            is_selected = wk->curr_ja > 0 && i == (wk->curr_ja - 1);

            if (is_selected) {
                highlight_alpha = get_judge_highlight_alpha(wk->fade_cja);
                fill_col = set_color_alpha(fill_col, highlight_alpha);
                outline_col = set_color_alpha(outline_col, highlight_alpha);
            }

            draw_hit_judge_box(wk->jx[i][0] * mirror_factor,
                               wk->jx[i][2],
                               wk->jx[i][1] * mirror_factor,
                               wk->jx[i][3],
                               fill_col,
                               outline_col,
                               judge_area_attr[i][1]);
        }
    }
    if (wk->ja_disp_bit & 0x8000) {
        draw_hit_judge_line(-1.0, -1.0, 2.0, 2.0, judge_area_attr[15][0], judge_area_attr[15][1]);
        draw_hit_judge_line(
            -2.0, (float)(-wk->wu.position_y - 2), 4.0, 4.0, judge_area_attr[16][0], judge_area_attr[16][1]);
    }
}

void draw_hit_judge_line(f32 px, f32 py, f32 sx, f32 sy, u32 col, u32 attr) {
    Vec3 point[2];
    PAL_CURSOR line;
    PAL_CURSOR_P xy[4];
    PAL_CURSOR_COL cc[4];

    point[0].x = px;
    point[0].y = py;
    point[0].z = 0.0f;
    point[1].x = px + sx;
    point[1].y = py + sy;
    point[1].z = 0.0f;
    njCalcPoints(NULL, point, point, 2);
    line.p = xy;
    line.col = cc;
    line.tex = NULL;
    line.num = 4;
    line.p[0].x = line.p[2].x = point[0].x;
    line.p[1].x = line.p[3].x = point[1].x;
    line.p[0].y = line.p[1].y = point[0].y;
    line.p[2].y = line.p[3].y = point[1].y;
    line.col[0].color = line.col[1].color = line.col[2].color = line.col[3].color = col;
    njDrawPolygon2D(&line, 4, PrioBase[1], attr);
}

s32 set_conn_sprite(WORK_Other_CONN* wk, s16 bsy) {
    s16 i;

    if (wk->num_of_conn == 0) {
        return 1;
    }

    wk->wu.current_colcd = wk->wu.my_col_code;
    dmwk_moji.my_family = wk->wu.my_family;
    dmwk_moji.my_bright_type = wk->wu.my_bright_type;
    dmwk_moji.my_bright_level = wk->wu.my_bright_level;
    dmwk_moji.my_clear_level = wk->wu.my_clear_level;
    dmwk_moji.my_mts = wk->wu.my_mts;
    dmwk_moji.my_mr_flag = wk->wu.my_mr_flag;
    dmwk_moji.my_mr = wk->wu.my_mr;
    dmwk_moji.rl_flag = wk->wu.rl_flag;
    dmwk_moji.cg_flip = 0;

    for (i = 0; i < wk->num_of_conn; i++) {
        dmwk_moji.position_x = (wk->wu.position_x + wk->conn[i].nx);
        dmwk_moji.position_y = (bsy + (wk->wu.position_y + wk->conn[i].ny));
        dmwk_moji.position_z = (wk->wu.position_z - wk->prio_reverse * i);
        dmwk_moji.current_colcd = (wk->wu.current_colcd + wk->conn[i].col);
        dmwk_moji.cg_number = wk->conn[i].chr;
        Mtrans_use_trans_mode(&dmwk_moji, 0);
    }

    return 2;
}

void all_cgps_put_back(WORK* wk) {
    // Do nothing
}

void Mtrans_use_trans_mode(WORK* wk, s16 bsy) {
    if (mts_ok[wk->my_mts].be == 0) {
        // A display request was received before MTS initialization. MTS number: %d\n
        // Original text: "ＭＴＳの初期化前に表示要求が入りました。ＭＴＳ番号：%d\n"
        // For some reason MWCC (or mwccgap) removes a single byte from the string, resulting in a mismatch.
        // single byte.
        flLogOut("\x82\x6c\x82\x73\x82\x72\x82\xcc\x8f\x89\x8a\xfa\x89\xbb\x91\x4f\x82\xc9\x95\x5c\x8e\xa6\x97\x76\x8b"
                 "\x81\x82\xaa\x93\xfc\x82\xe8\x82\xdc\x82\xb5\x82\xbd\x81\x42\x82\x6c\x82\x73\x82\x72\x94\xd4\x8d\x86"
                 "\x81\x46\x25\x64\x0a",
                 wk->my_mts);
        return;
    }

    if (No_Trans) {
        return;
    }

    wk->current_colcd &= 0x1FF;

    if (wk->my_col_mode & 0x400) {
        wk->my_clear_level = 0x90;
    }

    switch (mts[wk->my_mts].mode) {
    case 17:
        if ((Debug_w[0x24] != 1) || (Debug_w[0x27] != 1)) {
            wk->colcd = exchange_current_colcd(wk);
            mlt_obj_trans(&mts[wk->my_mts], wk, bsy);
        }

        break;

    case 18:
        if ((Debug_w[0x25] != 1) || (Debug_w[0x27] != 1)) {
            wk->colcd = wk->current_colcd;
            mlt_obj_trans_cp3(&mts[wk->my_mts], wk, bsy);
        }

        break;

    case 20:
        if ((Debug_w[0x26] != 1) || (Debug_w[0x27] != 1)) {
            wk->colcd = wk->current_colcd;
            mlt_obj_trans_rgb(&mts[wk->my_mts], wk, bsy);
        }

        break;

    case 33:
        if ((Debug_w[0x24] != 1) || (Debug_w[0x27] != 2)) {
            wk->colcd = wk->current_colcd;
            mlt_obj_disp(&mts[wk->my_mts], wk, (s32)bsy);
        }

        break;

    case 34:
        if (Debug_w[0x25] != 1) {
            return;
        }

        if ((u32)Debug_w[0x27] == 2) {
            // Do nothing
        }

        break;

    case 36:
        if ((Debug_w[0x26] != 1) || (Debug_w[0x27] != 2)) {
            wk->colcd = wk->current_colcd;
            mlt_obj_disp_rgb(&mts[wk->my_mts], wk, (s32)bsy);
        }

        break;
    }
}

s16 exchange_current_colcd(WORK* wk) {
    WORK* mwk;
    s16 col = ((WORK_Other*)wk)->wu.current_colcd;

    switch (((WORK_Other*)wk)->wu.work_id) {
    case 0x1:
        col = ((WORK_Other*)wk)->wu.id * 8;
        push_color_trans_req(((WORK_Other*)wk)->wu.current_colcd, col);
        break;

    case 0x8:
    case 0x10:
        mwk = (WORK*)((WORK_Other*)wk)->my_master;

        if ((((WORK_Other*)wk)->wu.id == 147) || (((WORK_Other*)wk)->wu.id == 148)) {
            col = mwk->id * 8 + 4;
        }

        break;

    case 0x20:
        mwk = (WORK*)((WORK_Other*)wk)->my_master;

        if ((((WORK_Other*)wk)->wu.my_col_code) == mwk->my_col_code) {
            col = mwk->id * 8;
        }

        break;

    case 0x40:
        mwk = (WORK*)((WORK_Other*)wk)->my_master;

        if (((WORK_Other*)wk)->wu.my_col_code == mwk->my_col_code) {
            col = ((mwk->id * 8) + 1);
        }

        break;
    }

    return col;
}

s32 sort_push_request(WORK* wk) {
    if (wk->my_mts == 0) {
        return 0;
    }

    wk->current_colcd = wk->my_col_code;

    if ((wk->work_id == 1) && ((wk->rl_flag + wk->cg_flip) & 1)) {
        wk->current_colcd |= 8;
    }

    if ((wk->work_id == 0x20) && (wk->my_col_code == ((WORK*)((WORK_Other*)wk)->my_master)->my_col_code) &&
        ((wk->rl_flag + wk->cg_flip) & 1)) {
        wk->current_colcd |= 8;
    }

    if (wk->extra_col_2) {
        wk->current_colcd = wk->extra_col_2;
    }

    if (wk->extra_col) {
        wk->current_colcd = wk->extra_col;
    }

    if (wk->disp_flag == 0 || wk->cg_number == 0) {
        return 1;
    }

    if ((wk->disp_flag == 2) && ((wk->blink_timing + Game_timer & 1))) {
        return 1;
    }

    Mtrans_use_trans_mode(wk, base_y_pos);

    if (wk->kage_flag) {
        shadow_setup(wk, base_y_pos);
    }

    return 2;
}

s32 sort_push_request2(WORK_Other* wk) {
    if (wk->wu.disp_flag == 0) {
        return 1;
    }

    set_judge_area_sprite((WORK_Other_JUDGE*)wk, base_y_pos);

    return 2;
}

s32 sort_push_request3(WORK* wk) {
    if (wk->my_mts == 0) {
        return 0;
    }

    wk->current_colcd = wk->my_col_code;

    if (wk->extra_col_2) {
        wk->current_colcd = wk->extra_col_2;
    }

    if (wk->extra_col) {
        wk->current_colcd = wk->extra_col;
    }

    if (wk->disp_flag == 0) {
        return 1;
    }

    if ((wk->disp_flag == 2) && (wk->blink_timing + Game_timer & 1)) {
        return 1;
    }

    if (set_conn_sprite((WORK_Other_CONN*)wk, base_y_pos) == 1) {
        return 1;
    }

    return 2;
}

s32 sort_push_request4(WORK* wk) {
    if (wk->my_mts == 0) {
        return 0;
    }

    if (wk->disp_flag == 0 || wk->cg_number == 0) {
        return 1;
    }

    if ((wk->disp_flag == 2) && ((wk->blink_timing + Game_timer) & 1)) {
        return 1;
    }

    wk->current_colcd = wk->my_col_code;

    if (wk->extra_col_2) {
        wk->current_colcd = wk->extra_col_2;
    }

    if (wk->extra_col) {
        wk->current_colcd = wk->extra_col;
    }

    if ((wk->id != 0x4C) && (wk->id != 0x46)) {
        if (judge_flag) {
            if (wk->position_z < 0x48) {
                wk->my_bright_type = 1;
                wk->my_bright_level = 7;
            }
        } else {
            wk->my_bright_type = 0;
            wk->my_bright_level = 0;
        }
    }

    Mtrans_use_trans_mode(wk, 0);

    if (wk->kage_flag) {
        shadow_setup(wk, 0);
    }

    return 2;
}

s32 sort_push_request8(WORK* wk) {
    if (wk->cg_number >= 0x748F) {
        wk->my_mts = 2;
    } else {
        wk->my_mts = 0xE;
    }

    if ((wk->my_col_code & 0x1FF) >= 0x20) {
        wk->my_mts = 0xE;
    }

    return sort_push_request(wk);
}

s32 sort_push_requestA(WORK* wk) {
    PAL_CURSOR_COL oricol;
    s16 i;
    s16 mf;

    if (No_Trans) {
        return 2;
    }

    if (wk->disp_flag == 0) {
        return 1;
    }

    oricol.color = box_color_attr[wk->my_col_code & 0x1FF][0];

    if (wk->my_clear_level) {
        oricol.argb.a = wk->my_clear_level;
    }

    if (wk->disp_flag == 2) {
        switch (wk->blink_timing) {
        case 1:
            if (Interrupt_Timer & 0x80) {
                oricol.argb.a = (wk->my_clear_level + (0x80 - (Interrupt_Timer & 0x7F)));
            } else {
                oricol.argb.a = (wk->my_clear_level + (Interrupt_Timer & 0x7F));
            }

            break;

        case 0:
            if (Interrupt_Timer & 0x40) {
                oricol.argb.a = (wk->my_clear_level + (0x40 - (Interrupt_Timer & 0x3F)));
            } else {
                oricol.argb.a = (wk->my_clear_level + (Interrupt_Timer & 0x3F));
            }

            break;

        case 2:
            if (Interrupt_Timer & 0x20) {
                oricol.argb.a = (wk->my_clear_level + (0x20 - (Interrupt_Timer & 0x1F)));
            } else {
                oricol.argb.a = (wk->my_clear_level + (Interrupt_Timer & 0x1F));
            }

            break;
        }
    }

    mlt_obj_matrix(wk, base_y_pos);

    if (wk->rl_flag) {
        mf = -1;
    } else {
        mf = 1;
    }

    draw_box((f32)(wk->shell_ix[0] * mf),
             (f32)(wk->shell_ix[2]),
             (f32)(wk->shell_ix[1] * mf),
             (f32)(wk->shell_ix[3]),
             oricol.color,
             box_color_attr[wk->my_col_code][1],
             wk->position_z);

    for (i = 0; i < wk->charset_id; i++) {
        draw_box((f32)((wk->shell_ix[0] - (i + 1) * 2) * mf),
                 (f32)(wk->shell_ix[2] + (1 << (i + 1))),
                 (f32)(mf * (wk->shell_ix[1] + (i + 1) * 2 * 2)),
                 (f32)(wk->shell_ix[3] - (1 << (i + 1)) * 2),
                 oricol.color,
                 box_color_attr[wk->my_col_code][1],
                 wk->position_z);

        draw_box((f32)(mf * (wk->shell_ix[0] + (1 << (i + 1)))),
                 (f32)(wk->shell_ix[2] - (i + 1) * 2),
                 (f32)(mf * (wk->shell_ix[1] - (1 << (i + 1)) * 2)),
                 (f32)(wk->shell_ix[3] + (i + 1) * 2 * 2),
                 oricol.color,
                 box_color_attr[wk->my_col_code][1],
                 wk->position_z);
    }

    return 2;
}

s32 sort_push_requestB(WORK* wk) {
    PAL_CURSOR_COL oricol;
    s16 i;
    s16 mf;

    if (No_Trans) {
        return 2;
    }

    if (wk->disp_flag == 0) {
        return 1;
    }

    oricol.color = box_color_attr[wk->my_col_code & 0x1FF][0];

    if (wk->my_clear_level) {
        oricol.argb.a = wk->my_clear_level;
    }

    if (wk->disp_flag == 2) {
        switch (wk->blink_timing) {
        case 1:
            if (Interrupt_Timer & 0x80) {
                oricol.argb.a = (wk->my_clear_level + (0x80 - (Interrupt_Timer & 0x7F)));
            } else {
                oricol.argb.a = (wk->my_clear_level + (Interrupt_Timer & 0x7F));
            }

            break;

        case 0:
            if (Interrupt_Timer & 0x40) {
                oricol.argb.a = (wk->my_clear_level + (0x40 - (Interrupt_Timer & 0x3F)));
            } else {
                oricol.argb.a = (wk->my_clear_level + (Interrupt_Timer & 0x3F));
            }

            break;

        case 2:
            if (Interrupt_Timer & 0x20) {
                oricol.argb.a = (wk->my_clear_level + (0x20 - (Interrupt_Timer & 0x1F)));
            } else {
                oricol.argb.a = (wk->my_clear_level + (Interrupt_Timer & 0x1F));
            }

            break;
        }
    }

    mlt_obj_matrix(wk, 0);

    if (wk->rl_flag) {
        mf = -1;
    } else {
        mf = 1;
    }

    draw_box((f32)(wk->shell_ix[0] * mf),
             (f32)(wk->shell_ix[2]),
             (f32)(wk->shell_ix[1] * mf),
             (f32)(wk->shell_ix[3]),
             oricol.color,
             box_color_attr[wk->my_col_code][1],
             wk->position_z);

    for (i = 0; i < wk->charset_id; i++) {
        draw_box((f32)(mf * (wk->shell_ix[0] - (i + 1) * 2)),
                 (f32)(wk->shell_ix[2] + (1 << (i + 1))),
                 (f32)(mf * (wk->shell_ix[1] + (i + 1) * 2 * 2)),
                 (f32)(wk->shell_ix[3] - (1 << (i + 1)) * 2),
                 oricol.color,
                 box_color_attr[wk->my_col_code][1],
                 wk->position_z);

        draw_box((f32)(mf * (wk->shell_ix[0] + (1 << (i + 1)))),
                 (f32)(wk->shell_ix[2] - (i + 1) * 2),
                 (f32)(mf * (wk->shell_ix[1] - (1 << (i + 1)) * 2)),
                 (f32)(wk->shell_ix[3] + (i + 1) * 2 * 2),
                 oricol.color,
                 box_color_attr[wk->my_col_code][1],
                 wk->position_z);
    }

    return 2;
}

void shadow_setup(WORK* wk, s16 bsy) {
    f32 base_y = (f32)bsy;

    if (No_Trans) {
        return;
    }

    njdp2d_sort(&base_y, (f32)PrioBase[wk->kage_prio], (uintptr_t)wk, 1);
}

void shadow_drawing(WORK* wk, s16 bsy) {
    s16 shadow;

    if (Debug_w[0x23]) {
        return;
    }

    dmwk_kage.position_x = (wk->position_x + wk->kage_hx * (1 - (wk->rl_flag != 0) * 2));
    dmwk_kage.position_y = wk->kage_hy;
    dmwk_kage.position_z = wk->kage_prio;
    dmwk_kage.my_family = wk->my_family;
    shadow = wk->kage_char - get_kage_width(wk->xyz[1].disp.pos - wk->kage_hy);

    if (shadow >= 0x1D) {
        shadow = 0x1C;
    } else if (shadow < 0) {
        shadow = 0;
    }

    dmwk_kage.cg_number = shadow + 0x7308;
    Mtrans_use_trans_mode(&dmwk_kage, bsy);
}

s8 get_kage_width(s16 cpy) {
    if (cpy <= 0) {
        return 0;
    }

    if ((s64)(cpy /= 4) >= 64) {
        return 0x10;
    }

    return gkw_table[cpy];
}

// latter part of rodata

const u32 box_color_attr[4][2] = { { 0xFF000000, 0x60 }, { 0x90000000, 0x60 }, { 0x555555, 0x60 }, { 0xFFFFFF, 0x60 } };

const char gkw_table[64] = { 0,  1,  2,  3,  4,  4,  5,  5,  5,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  8,  9,  9,
                             9,  9,  10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 13, 13,
                             13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15 };

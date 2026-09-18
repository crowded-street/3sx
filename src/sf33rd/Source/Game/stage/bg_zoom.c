/**
 * @file bg_zoom.c
 * Stage zoom requests, split from bg_sub.c.
 */

#include "sf33rd/Source/Game/stage/bg_sub.h"
#include "common.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/stage/bg_data.h"
#include "structs.h"

static void request_horizontal_zoom(s16 pos) {
    zoom_request_flag = 0x100;
    scr_req_x = pos;
}

static void select_horizontal_zoom_request_last(u16 p1zoom, u16 zoom_wk) {
    switch (p1zoom & 0xE200) {
    case 0x2200:
        switch (zoom_wk) {
        case 0x0:
        case 0x2200:
        case 0x4000:
            request_horizontal_zoom((plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1);
            break;
        case 0x2000:
            request_horizontal_zoom(plw[0].wu.xyz[0].disp.pos);
            break;
        case 0x200:
            request_horizontal_zoom(plw[1].wu.xyz[0].disp.pos);
            break;

            break;
        }
        break;
    }
}

static void select_horizontal_zoom_request_later(u16 p1zoom, u16 zoom_wk) {
    switch (p1zoom & 0xE200) {
    case 0x0:
        switch (zoom_wk) {
        case 0x2200:
            request_horizontal_zoom((plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1);
            break;

        case 0x2000:
            request_horizontal_zoom(plw[0].wu.xyz[0].disp.pos);
            break;

        case 0x200:
            request_horizontal_zoom(plw[1].wu.xyz[0].disp.pos);
            break;

        case 0x4000:
        case 0x0:
            break;
        }
        break;

    default:
        select_horizontal_zoom_request_last(p1zoom, zoom_wk);
        break;
    }
}

static void select_horizontal_zoom_request_middle(u16 p1zoom, u16 zoom_wk) {
    switch (p1zoom & 0xE200) {
    case 0x200:
        switch (zoom_wk) {
        case 0x2000:
        case 0x0:
        case 0x4000:
        case 0x2200:
            request_horizontal_zoom(plw[0].wu.xyz[0].disp.pos);
            break;

        case 0x200:
            request_horizontal_zoom((plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1);
            break;
        }
        break;

    default:
        select_horizontal_zoom_request_later(p1zoom, zoom_wk);
        break;
    }
}

static void select_horizontal_2000_zoom_request(u16 zoom_wk) {
    switch (zoom_wk) {
        case 0x4000:
            request_horizontal_zoom(plw[0].wu.xyz[0].disp.pos);
            break;

        case 0x2000:
            request_horizontal_zoom((plw[0].wu.xyz[0].disp.pos + plw[1].wu.xyz[0].disp.pos) >> 1);
            break;

        case 0x200:
        case 0x0:
        case 0x2200:
            request_horizontal_zoom(plw[1].wu.xyz[0].disp.pos);
            break;
    }
}

static void select_horizontal_zoom_request(u16 p1zoom, u16 p2zoom) {
    u16 zoom_wk;

    zoom_wk = p2zoom & 0xE200;

    switch (p1zoom & 0xE200) {
    case 0x4000:
        break;

    case 0x2000:
        select_horizontal_2000_zoom_request(zoom_wk);
        break;

    default:
        select_horizontal_zoom_request_middle(p1zoom, zoom_wk);
        break;
    }
}

static void request_vertical_zoom(s16 pos) {
    zoom_request_flag |= 0x1000;
    scr_req_y = pos;
}

static void select_vertical_zoom_request_last(u16 p1zoom, u16 zoom_wk) {
    switch (p1zoom & 0xD100) {
    case 0x1100:
        switch (zoom_wk) {
        case 0x1000:
            request_vertical_zoom(plw[0].wu.xyz[1].disp.pos);
            break;

        case 0x100:
            request_vertical_zoom(plw[1].wu.xyz[1].disp.pos);
            break;

        case 0x1100:
        case 0x0:
            request_vertical_zoom((plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1);
            break;

        case 0x4000:
            request_vertical_zoom(0);
            break;
        }
        break;
    }
}

static void select_vertical_zoom_request_later(u16 p1zoom, u16 zoom_wk) {
    switch (p1zoom & 0xD100) {
    case 0x0:
        switch (zoom_wk) {
        case 0x1000:
            request_vertical_zoom(plw[0].wu.xyz[1].disp.pos);
            break;

        case 0x100:
            request_vertical_zoom(plw[1].wu.xyz[1].disp.pos);
            break;

        case 0x1100:
            request_vertical_zoom((plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1);
            break;

        case 0x0:
            break;

        case 0x4000:
            request_vertical_zoom(0);
            break;
        }
        break;

    default:
        select_vertical_zoom_request_last(p1zoom, zoom_wk);
        break;
    }
}

static void select_vertical_zoom_request_middle(u16 p1zoom, u16 zoom_wk) {
    switch (p1zoom & 0xD100) {
    case 0x100:
        switch (zoom_wk) {
        case 0x1000:
        case 0x0:
        case 0x1100:
            request_vertical_zoom(plw[0].wu.xyz[1].disp.pos);
            break;

        case 0x100:
            request_vertical_zoom((plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1);
            break;

        case 0x4000:
            request_vertical_zoom(0);
            break;
        }
        break;

    default:
        select_vertical_zoom_request_later(p1zoom, zoom_wk);
        break;
    }
}

static void select_vertical_1000_zoom_request(u16 zoom_wk) {
    switch (zoom_wk) {
        case 0x1000:
            request_vertical_zoom((plw[0].wu.xyz[1].disp.pos + plw[1].wu.xyz[1].disp.pos) >> 1);
            break;

        case 0x100:
        case 0x0:
        case 0x1100:
            request_vertical_zoom(plw[1].wu.xyz[1].disp.pos);
            break;

        case 0x4000:
            request_vertical_zoom(0);
            break;
    }
}

static void select_vertical_zoom_request(u16 p1zoom, u16 p2zoom) {
    u16 zoom_wk;

    zoom_wk = p2zoom & 0xD100;

    switch (p1zoom & 0xD100) {
    case 0x4000:
        request_vertical_zoom(0);
        break;

    case 0x1000:
        select_vertical_1000_zoom_request(zoom_wk);
        break;

    default:
        select_vertical_zoom_request_middle(p1zoom, zoom_wk);
        break;
    }
}

static void update_fighter_screen_positions() {
    s16 i;

    for (i = 0; i < 2; i++) {
        if (plw[i].scr_pos_set_flag) {
            plw[i].wu.scr_mv_x = plw[i].wu.xyz[0].disp.pos;
            plw[i].wu.scr_mv_y = plw[i].wu.xyz[1].disp.pos;
        } else if (plw[i].tsukamare_f) {
            plw[i].wu.scr_mv_x = plw[(i + 1) & 1].wu.xyz[0].disp.pos;
            plw[i].wu.scr_mv_y = plw[(i + 1) & 1].wu.xyz[1].disp.pos;
        }
    }
}

static u16 higher_zoom_level(u16 p1zoom, u16 p2zoom) {
    u16 zmlv;

    zmlv = p1zoom & 0xFF;

    if (zmlv < (p2zoom & 0xFF)) {
        zmlv = p2zoom & 0xFF;
    }

    return zmlv;
}

void check_cg_zoom() {
    u16 p1zoom;
    u16 p2zoom;
    u16 zmlv;
    u16 lookp1;
    u16 lookp2;

    p1zoom = plw[0].wu.cg_zoom;
    p2zoom = plw[1].wu.cg_zoom;

    if (bg_stop != 0 && !((p1zoom | p2zoom) & 0x4000)) {
        zmlv = higher_zoom_level(p1zoom, p2zoom);

        lookp1 = p1zoom >> 8 & 3;
        lookp2 = p2zoom >> 8 & 3;
        p1zoom = zmlv | (lookp2 << 12 | lookp1 << 8);
        p2zoom = zmlv | (lookp1 << 12 | lookp2 << 8);
    }

    zoom_req_flag_old = zoom_request_flag;
    zoom_request_flag = 0;

    update_fighter_screen_positions();

    select_horizontal_zoom_request(p1zoom, p2zoom);
    select_vertical_zoom_request(p1zoom, p2zoom);

    zoom_request_level = higher_zoom_level(p1zoom, p2zoom);

    if (zoom_request_level) {
        zoom_request_flag |= 1;
    }
}

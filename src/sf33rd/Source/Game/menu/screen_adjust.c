#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

static s16 decrease_horizontal_position(void) {
    X_Adjust_Buff[2] -= 2;
    if (X_Adjust_Buff[2] < -10) {
        X_Adjust_Buff[2] = -10;
        return 0;
    }
    return 1;
}

static s16 decrease_vertical_position(void) {
    Y_Adjust_Buff[2] -= 2;
    if (Y_Adjust_Buff[2] < -10) {
        Y_Adjust_Buff[2] = -10;
        return 0;
    }
    return 1;
}

static s16 decrease_horizontal_size(void) {
    Disp_Size_H -= 2;
    if (Disp_Size_H < 94) {
        Disp_Size_H = 94;
        return 0;
    }
    return 1;
}

static s16 decrease_vertical_size(void) {
    Disp_Size_V -= 2;
    if (Disp_Size_V < 94) {
        Disp_Size_V = 94;
        return 0;
    }
    return 1;
}

static s16 increase_horizontal_position(void) {
    X_Adjust_Buff[2] += 2;
    if (X_Adjust_Buff[2] > 10) {
        X_Adjust_Buff[2] = 10;
        return 0;
    }
    return 1;
}

static s16 increase_vertical_position(void) {
    Y_Adjust_Buff[2] += 2;
    if (Y_Adjust_Buff[2] > 10) {
        Y_Adjust_Buff[2] = 10;
        return 0;
    }
    return 1;
}

static s16 increase_horizontal_size(void) {
    Disp_Size_H += 2;
    if (Disp_Size_H > 100) {
        Disp_Size_H = 100;
        return 0;
    }
    return 1;
}

static s16 increase_vertical_size(void) {
    Disp_Size_V += 2;
    if (Disp_Size_V > 100) {
        Disp_Size_V = 100;
        return 0;
    }
    return 1;
}

static s16 adjust_horizontal_position(u16 sw) {
    if (sw == 4) {
        return decrease_horizontal_position();
    }
    if (sw == 8) {
        return increase_horizontal_position();
    }
    return 0;
}

static s16 adjust_vertical_position(u16 sw) {
    if (sw == 4) {
        return decrease_vertical_position();
    }
    if (sw == 8) {
        return increase_vertical_position();
    }
    return 0;
}

static s16 adjust_horizontal_size(u16 sw) {
    if (sw == 4) {
        return decrease_horizontal_size();
    }
    if (sw == 8) {
        return increase_horizontal_size();
    }
    return 0;
}

static s16 adjust_vertical_size(u16 sw) {
    if (sw == 4) {
        return decrease_vertical_size();
    }
    if (sw == 8) {
        return increase_vertical_size();
    }
    return 0;
}

static s16 adjust_language(u16 sw) {
    switch (sw) {
    case 4:
    case 8:
        mpp_w.language = Language_Toggle(mpp_w.language);
        return 1;
    default:
        return 0;
    }
}

static s16 adjust_screen_size(u16 sw) {
    switch (Menu_Cursor_Y[0]) {
    case 0:
        return adjust_horizontal_position(sw);
    case 1:
        return adjust_vertical_position(sw);
    case 2:
        return adjust_horizontal_size(sw);
    case 3:
        return adjust_vertical_size(sw);
    case 4:
        return adjust_language(sw);
    default:
        return 0;
    }
}

void Screen_Move_Sub_LR(u16 sw) {
    s16 flag = adjust_screen_size(sw);

    if (flag) {
        SE_dir_cursor_move();
    }

    X_Adjust = X_Adjust_Buff[0] = X_Adjust_Buff[1] = X_Adjust_Buff[2];
    Y_Adjust = Y_Adjust_Buff[0] = Y_Adjust_Buff[1] = Y_Adjust_Buff[2];
}

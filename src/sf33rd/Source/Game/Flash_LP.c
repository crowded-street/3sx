#include "sf33rd/Source/Game/Flash_LP.h"
#include "common.h"
#include "sf33rd/Source/Game/SysDir.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/sc_data.h"
#include "sf33rd/Source/Game/sc_sub.h"
#include "sf33rd/Source/Game/workuser.h"

const u8 Lamp_Flash_Data[2][2] = { { 0x07, 0x6F }, { 0x1E, 0x03 } };

void Flash_Lamp() {
    u8 ix;
    u8 ix2p;
    u8 mark;
    u8 color;

    if (Mode_Type == MODE_NORMAL_TRAINING || Mode_Type == MODE_PARRY_TRAINING) {
        return;
    }

    if (omop_cockpit == 0) {
        return;
    }

    if (!Game_pause) {
        switch (Lamp_No) {
        case 0:
            Lamp_No = 1;
            Lamp_Index = 1;
            Lamp_Timer = 1;
            /* fallthrough */

        case 1:
            if (--Lamp_Timer == 0) {
                if (++Lamp_Index > 1) {
                    Lamp_Index = 0;
                }
                Lamp_Color = Lamp_Flash_Data[Lamp_Index][0];
                Lamp_Timer = Lamp_Flash_Data[Lamp_Index][1];
            }

            break;
        }
    }

    for (ix = 0; ix <= save_w[Present_Mode].Battle_Number[Play_Type]; ix++) {
        mark = flash_win_type[0][ix];

        if (flash_win_type[0][ix] == 0) {
            color = 7;
        } else {
            color = Lamp_Color;
        }

        if (flash_win_type[0][ix] == sync_win_type[0][ix]) {
            scfont_sqput(vmark_tbl[ix], 4, color, 0, mark * 2, 26, 2, 1, 2);
        }

        mark = flash_win_type[1][ix];
        ix2p = ix + 4;

        if (flash_win_type[1][ix] == 0) {
            color = 7;
        } else {
            color = Lamp_Color;
        }

        if (flash_win_type[1][ix] == sync_win_type[1][ix]) {
            scfont_sqput(vmark_tbl[ix2p], 4, color, 0, mark * 2, 26, 2, 1, 2);
        }
    }
}

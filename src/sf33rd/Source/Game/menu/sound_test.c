#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/effect/eff04.h"
#include "sf33rd/Source/Game/effect/eff64.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff61.h"
#include "sf33rd/Source/Game/effect/effa8.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/game.h"
#include "sf33rd/Source/Game/io/vm_sub.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/sound/se.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"
#include "structs.h"

u16 Sound_Cursor_Sub(s16 PL_id);
void Menu_Sub_case1(struct _TASK* task_ptr);
void Return_Option_Mode_Sub(struct _TASK* task_ptr);

static bool should_finish_sound_test(void) {
    return IO_Result == 0x200 || ((Menu_Cursor_Y[0] == 5) && (IO_Result == 0x100 || IO_Result == 0x4000));
}

void Sound_Test(struct _TASK* task_ptr) {
    s16 char_index;
    s16 ix;

    Clear_Flash_Sub();

    switch (task_ptr->r_no[2]) {
    case 0:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2] += 1;
        task_ptr->timer = 5;
        setupAlwaysSeamlessFlag(((plsw_00[0] | plsw_00[1]) & 0x4000) != 0);
        Clear_Flash_Init(4);
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Convert_Buff[3][1][4] = 0;

        if (sys_w.bgm_type == BGM_ARRANGED) {
            Convert_Buff[3][1][2] = 0;
        } else {
            Convert_Buff[3][1][2] = 1;
        }

        Convert_Buff[3][1][6] = 1;
        Order[0x4F] = 4;
        Order_Timer[0x4F] = 1;
        Order[0x4E] = 2;
        Order_Dir[0x4E] = 2;
        Order_Timer[0x4E] = 1;
        effect_57_init(0x72, MENU_HEADER_SOUND, 0, 0x3F, 2);
        Order[0x72] = 1;
        Order_Dir[0x72] = 8;
        Order_Timer[0x72] = 1;
        effect_04_init(2, 6, 2, 0x48);

        {
            s32 ixSoundMenuItem[3] = { 10, 10, 11 };

            for (ix = 0; ix < 3; ix++) {
                Order[ix + 0x57] = 1;
                Order_Dir[ix + 0x57] = 4;
                Order_Timer[ix + 0x57] = ix + 0x14;
                effect_64_init(ix + 0x57, 0, 2, ixSoundMenuItem[ix] + 1, ix, 0x7047, ix + 0xC, 3, 1);
            }
        }

        Order_Dir[0x78] = 0;
        effect_A8_init(0, 0x78, 0, 2, 4, 0x70A7, 0);
        Order_Dir[0x79] = 1;
        effect_A8_init(0, 0x79, 0, 2, 4, 0x70A7, 1);
        effect_A8_init(3, 0x7A, 0, 2, 4, 0x70A7, 3);
        Convert_Buff[3][1][4] = 0;
        Order_Dir[0x7B] = 0;
        effect_A8_init(2, 0x7B, 0, 2, 4, 0x70A7, 2);

        {
            s16 unused_s2;
            s16 unused_s3;

            for (ix = 0, unused_s3 = char_index = 0x3B; ix < 6; ix++, unused_s2 = char_index++) {
                effect_61_init(0, ix + 0x50, 0, 2, char_index, ix, 0x7047);
                Order[ix + 0x50] = 1;
                Order_Dir[ix + 0x50] = 4;
                Order_Timer[ix + 0x50] = ix + 0x14;
            }
        }

        Menu_Cursor_Move = 5;
        break;

    case 1:
        Menu_Sub_case1(task_ptr);
        break;

    case 2:
        if (FadeIn(1, 0x19, 8) != 0) {
            task_ptr->r_no[2] += 1;
            Suicide[3] = 0;
        }

        break;

    case 3:
        Sound_Cursor_Sub(0);

        if (IO_Result == 0) {
            Sound_Cursor_Sub(1);
        }

        if ((Menu_Cursor_Y[0] == 3) && (IO_Result == 0x100)) {
            SE_selected();
            Convert_Buff[3][1][0] = 0;
            Convert_Buff[3][1][1] = 0xF;
            Convert_Buff[3][1][2] = 0;
        }

        if (bgm_level != (s16)Convert_Buff[3][1][0]) {
            bgm_level = Convert_Buff[3][1][0];
            save_w[Present_Mode].BGM_Level = Convert_Buff[3][1][0];
            SsBgmHalfVolume(0);
        }

        if (se_level != (s16)Convert_Buff[3][1][1]) {
            se_level = Convert_Buff[3][1][1];
            setSeVolume(save_w[Present_Mode].SE_Level = Convert_Buff[3][1][1]);
        }

        save_w[Present_Mode].BgmType = Convert_Buff[3][1][2];

        if (sys_w.bgm_type != Convert_Buff[3][1][2]) {
            sys_w.bgm_type = Convert_Buff[3][1][2];
            Convert_Buff[3][1][4] = 0;
            BGM_Request_Code_Check(0x41);
        }

        Order_Dir[0x7B] = Convert_Buff[3][1][4];
        Save_Game_Data();

        if (Menu_Cursor_Y[0] == 4) {
            if (IO_Result == 0x100) {
                SsRequest((u16)Order_Dir[0x7B] + 1);
                Convert_Buff[3][1][6] = 1;
                return;
            }

            if ((IO_Result == 0x200) && Convert_Buff[3][1][6]) {
                Convert_Buff[3][1][6] = 0;
                BGM_Stop();
                return;
            }
        }

        if (should_finish_sound_test()) {
            SE_selected();
            Return_Option_Mode_Sub(task_ptr);
            setupAlwaysSeamlessFlag(0);
            Order[0x72] = 4;
            Order_Timer[0x72] = 4;
            BGM_Request_Code_Check(0x41);
        }

        break;
    }
}

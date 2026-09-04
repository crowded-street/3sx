#include "sf33rd/Source/Game/menu/menu.h"
#include "common.h"
#include "main.h"
#include "sf33rd/Source/Game/effect/effa3.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/menu/dir_data.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "structs.h"

void Dummy_Move_Sub(struct _TASK* task_ptr, s16 PL_id, s16 id, s16 type, s16 max);
void Default_Training_Option();
void Training_Disp_Sub(struct _TASK* task_ptr);

void Training_Option(struct _TASK* task_ptr) {
    s16 ix;
    s16 y;

    s16 s6;
    s16 s5;

    switch (task_ptr->r_no[2]) {
    case 0:
        task_ptr->r_no[2]++;
        Menu_Common_Init();
        Menu_Cursor_Y[0] = 0;
        Menu_Cursor_Y[1] = 0;
        Menu_Suicide[0] = 1;
        Training_Index = 3;

        for (ix = 0, s6 = y = 72; ix < 8; ix++, s5 = y += 16) {
            effect_A3_init(0, 6, ix, ix, 1, 48, y, 1);
        }

        effect_A3_init(0, 7, TRAINING_OPTION_SA_GAUGE, TRAINING_OPTION_SA_GAUGE, 1, 230, 72, 1);
        effect_A3_init(0, 15, TRAINING_OPTION_ATTACK_DATA, TRAINING_OPTION_ATTACK_DATA, 1, 230, 88, 1);
        effect_A3_init(0, 15, TRAINING_OPTION_HITBOXES, TRAINING_OPTION_HITBOXES, 1, 230, 104, 1);
        effect_A3_init(0, 15, TRAINING_OPTION_INPUT_HISTORY, TRAINING_OPTION_INPUT_HISTORY, 1, 230, 120, 1);
        effect_A3_init(0, 9, TRAINING_OPTION_DAMAGE, TRAINING_OPTION_DAMAGE, 1, 230, 136, 1);
        effect_A3_init(0, 10, TRAINING_OPTION_DIFFICULTY, TRAINING_OPTION_DIFFICULTY, 1, 230, 152, 1);

        break;

    case 1:
        Dummy_Move_Sub(task_ptr, Champion, 0, 1, 7);

        if (Menu_Cursor_Y[0] == 6 && IO_Result & 0x100) {
            Default_Training_Option();
            SE_selected();
            break;
        }

        save_w[Present_Mode].Damage_Level = Training[2].contents[0][1][TRAINING_OPTION_DAMAGE];
        save_w[Present_Mode].Difficulty = Training[2].contents[0][1][TRAINING_OPTION_DIFFICULTY];
        break;

    case 2:
        SE_selected();
        Menu_Suicide[0] = 0;
        Menu_Suicide[1] = 1;
        task_ptr->r_no[2] = 0;
        task_ptr->r_no[3] = 0;
        Training_Disp_Sub(task_ptr);
        Training[0] = Training[2];
        break;
    }
}

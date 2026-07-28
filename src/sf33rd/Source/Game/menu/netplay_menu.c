#include "sf33rd/Source/Game/menu/netplay_menu.h"
#include "main.h"
#include "platform/netplay/fistbump.h"
#include "platform/netplay/netplay.h"
#include "port/sdl/netplay_screen.h"
#include "sf33rd/AcrSDK/common/pad.h"
#include "sf33rd/Source/Game/effect/eff40.h"
#include "sf33rd/Source/Game/effect/eff57.h"
#include "sf33rd/Source/Game/effect/eff66.h"
#include "sf33rd/Source/Game/effect/eff67.h"
#include "sf33rd/Source/Game/effect/effa4.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/Game/ui/sc_sub.h"

#if NETPLAY_ENABLED
// Menu_Page picks the item set. The direct page replaces matchmaking when a
// remote was given on the command line.
#define NETPLAY_PAGE_LOGGED_OUT 0
#define NETPLAY_PAGE_LOGGED_IN 1
#define NETPLAY_PAGE_DIRECT 2

const u8 Netplay_Page_Data[3] = { 0, 2, 1 };

static bool is_logged_in = false;

// Returns true while matchmaking is pending, consuming input to cancel.
// Caller should skip normal menu logic when this returns true.
static bool check_netplay_cancelled() {
    if (!Netplay_IsMatchmakingPending()) {
        return false;
    }

    // I dont know if we want users to be able to cancel mm on their own?
    // s16 sw = (~plsw_01[0] & plsw_00[0]) | (~plsw_01[1] & plsw_00[1]);

    // if (sw & (SWK_SOUTH | SWK_EAST)) {
    //     Netplay_CancelMatchmaking();
    //     SE_selected();
    // }

    return true;
}

void Setup_Netplay_Menu(struct _TASK* task_ptr) {
    s16 ix;
    s16 char_index;

    Menu_Page_Buff = Menu_Page;
    effect_work_init();
    Menu_Common_Init();
    Menu_Cursor_Y[0] = 0;
    Order[0x4E] = 5;
    Order_Timer[0x4E] = 1;
    Menu_Max = Netplay_Page_Data[Menu_Page];
    Order_Dir[0x4E] = 1;
    effect_57_init(0x4E, MENU_HEADER_OPTION_MENU, 0, 0x45, 0);
    Order[0x73] = 3;
    Order_Dir[0x73] = 8;
    Order_Timer[0x73] = 1;
    effect_57_init(0x73, MENU_HEADER_NETWORK, 0, 0x3F, 2);

    switch (Menu_Page) {
    case NETPLAY_PAGE_LOGGED_OUT:
        effect_66_init(0x8A, 8, 2, 0, -1, -1, 0x800C);
        Order[0x8A] = 3;
        Order_Timer[0x8A] = 1;
        break;

    default:
        effect_66_init(0x8A, 8, 2, 0, -1, -1, 0x8006);
        Order[0x8A] = 3;
        Order_Timer[0x8A] = 1;

        for (ix = 0; ix < Menu_Max; ix++) {
            effect_A4_init(0, ix, ix, 2);

            // The direct page's items are actions, so they have no value column.
            if (Menu_Page != NETPLAY_PAGE_DIRECT) {
                effect_A4_init(1, ix, ix, 2);
            }
        }

        // EXIT button
        effect_40_init(2, 0, 0x48, 0, 2, 1);
        effect_40_init(2, 1, 0x49, 0, 2, 1);
        effect_40_init(2, 2, 0x4A, 0, 2, 0);
        effect_40_init(2, 3, 0x4B, 0, 2, 2);

        break;
    }
}

void Netplay_Menu(struct _TASK* task_ptr) {
    const FistbumpState fs = Fistbump_GetState();
    const bool login_state = Fistbump_IsLoggedIn();

    Menu_Cursor_Y[1] = Menu_Cursor_Y[0];

    switch (task_ptr->r_no[2]) {
    case 0:
        Netplay_BeginMatchmaking();

        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2]++;
        task_ptr->r_no[3] = 0;
        task_ptr->timer = 5;
        task_ptr->free[0] = 0;
        task_ptr->free[1] = 0;
        Menu_Suicide[1] = 1;
        Menu_Suicide[2] = 0;
        Page_Max = 1;
        Menu_Page_Buff = Menu_Page;
        break;

    case 1:
        FadeOut(1, 0xFF, 8);
        task_ptr->r_no[2]++;
        if (Netplay_IsDirectP2PConfigured()) {
            Menu_Page = NETPLAY_PAGE_DIRECT;
        } else {
            Menu_Page = is_logged_in ? NETPLAY_PAGE_LOGGED_IN : NETPLAY_PAGE_LOGGED_OUT;
        }

        Setup_Netplay_Menu(task_ptr);
        /* fallthrough */

    case 2:
        FadeOut(1, 0xFF, 8);

        if (fs == FISTBUMP_CONNECTING) {
            break;
        }

        if (--task_ptr->timer == 0) {
            display_netplay_text = true;
            task_ptr->r_no[2]++;
            task_ptr->r_no[3] = 1;
            FadeInit();
        }

        break;

    case 3:
        if (FadeIn(1, 25, 8)) {
            task_ptr->r_no[2]++;
            break;
        }

        break;

    case 4:
        Pause_ID = 0;
        Dir_Move_Sub(task_ptr, 0);

        if (IO_Result == 0) {
            Pause_ID = 1;
            Dir_Move_Sub(task_ptr, 1);
        }

        if (Menu_Cursor_Y[1] != Menu_Cursor_Y[0]) {
            SE_cursor_move();
        }

        if ((IO_Result == SWK_EAST ||
             (IO_Result == SWK_SOUTH && Menu_Cursor_Y[0] == Menu_Max && Menu_Page != NETPLAY_PAGE_LOGGED_OUT)) &&
            (fs == FISTBUMP_IDLE || fs == FISTBUMP_AWAITING_LOGIN)) {
            Menu_Suicide[0] = 0;
            Menu_Suicide[1] = 1;
            Menu_Suicide[2] = 1;
            task_ptr->r_no[1] = 1;
            task_ptr->r_no[2] = 0;
            task_ptr->r_no[3] = 0;
            task_ptr->free[0] = 0;
            Order[115] = 4;
            Order_Timer[115] = 4;

            Netplay_HandleMenuExit();
            SE_dir_selected();
            break;
        } else if (IO_Result == SWK_EAST && Fistbump_GetState() == FISTBUMP_AWAITING_MATCH) {
            Fistbump_CancelQueue();
            break;
        } else if (IO_Result == SWK_EAST && Fistbump_GetState() == FISTBUMP_MATCHED) {
            Fistbump_DeclineMatch();
            break;
        } else if (IO_Result == SWK_SOUTH) {
            if (Fistbump_GetState() == FISTBUMP_MATCHED) {
                Fistbump_AcceptMatch();
                break;
            }

            if (Menu_Page == NETPLAY_PAGE_DIRECT) {
                // DIRECT CONNECT is the only item; the EXIT row is handled above.
                Netplay_BeginDirectP2P();
                SE_selected();
                break;
            }

            switch (Menu_Cursor_Y[0]) {
            case 0:
                Netplay_FindMatch();
                break;

            case 1:
                Menu_Suicide[0] = 0;
                Menu_Suicide[1] = 1;
                Menu_Suicide[2] = 1;
                task_ptr->r_no[1] = 1;
                task_ptr->r_no[2] = 0;
                task_ptr->r_no[3] = 0;
                task_ptr->free[0] = 0;
                Order[115] = 4;
                Order_Timer[115] = 4;

                Fistbump_Logout();
                Netplay_HandleMenuExit();
                SE_dir_selected();
                break;
            }

            break;
        }

        break;
    }

    if (is_logged_in != login_state) {
        is_logged_in = login_state;
        task[TASK_MENU].r_no[2] = 1;
        task[TASK_MENU].timer = 5;
    }
}
#endif

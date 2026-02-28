#include "main.h"
#include "common.h"
#include "netplay/netplay.h"
#include "port/sdl/sdl_app.h"
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2debug.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Compress/zlibApp.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/effect/effect.h"
#include "sf33rd/Source/Game/engine/plcnt.h"
#include "sf33rd/Source/Game/engine/workuser.h"
#include "sf33rd/Source/Game/init3rd.h"
#include "sf33rd/Source/Game/io/gd3rd.h"
#include "sf33rd/Source/Game/io/ioconv.h"
#include "sf33rd/Source/Game/menu/menu.h"
#include "sf33rd/Source/Game/rendering/color3rd.h"
#include "sf33rd/Source/Game/rendering/dc_ghost.h"
#include "sf33rd/Source/Game/rendering/mtrans.h"
#include "sf33rd/Source/Game/rendering/texcash.h"
#include "sf33rd/Source/Game/sound/sound3rd.h"
#include "sf33rd/Source/Game/stage/bg.h"
#include "sf33rd/Source/Game/system/ramcnt.h"
#include "sf33rd/Source/Game/system/sys_sub.h"
#include "sf33rd/Source/Game/system/sys_sub2.h"
#include "sf33rd/Source/Game/system/work_sys.h"
#include "sf33rd/Source/PS2/mc/knjsub.h"
#include "sf33rd/Source/PS2/mc/mcsub.h"
#include "structs.h"

#if defined(DEBUG)
#include "sf33rd/Source/Game/debug/debug_config.h"
#endif

#include "port/io/afs.h"
#include "port/resources.h"

#include <SDL3/SDL.h>

#if defined(_WIN32)
#include <windef.h> // including windows.h causes conflicts with the Polygon struct, so I just included the header where AllocConsole is and the Windows-specific typedefs that it requires.

#include <ConsoleApi.h>
#include <stdio.h>
#endif

#include <memory.h>
#include <stdbool.h>

// sbss
s32 system_init_level;
MPP mpp_w;

static bool is_game_initialized = false;
static bool are_resources_checked = false;
static bool is_running_resource_flow = false;

// forward decls
static void game_init();
static void game_step_0();
static void game_step_1();
static void init_windows_console();

void distributeScratchPadAddress();
void appCopyKeyData();
u8* mppMalloc(u32 size);
void njUserInit();
void njUserMain();
void cpLoopTask();
void cpInitTask();

/// @brief Makes sure resources are present.
/// @return `true` if resources are present and execution can proceed, `false` otherwise.
static bool run_resource_flow() {
    if (are_resources_checked) {
        return true;
    }

    if (!is_running_resource_flow) {
        are_resources_checked = Resources_CheckIfPresent();

        if (are_resources_checked) {
            return true;
        }

        is_running_resource_flow = true;
    }

    are_resources_checked = Resources_RunResourceCopyingFlow();

    if (are_resources_checked) {
        // Cleanup
        is_running_resource_flow = false;
    }

    return are_resources_checked;
}

static void afs_init() {
    char* file_path = Resources_GetPath("SF33RD.AFS");
    AFS_Init(file_path);
    SDL_free(file_path);
}

static void step_0() {
    if (!run_resource_flow()) {
        return;
    }

    if (!is_game_initialized) {
        afs_init();
        game_init();
        is_game_initialized = true;
    }

    if (is_game_initialized) {
        AFS_RunServer();
        game_step_0();
    }
}

static void step_1() {
    if (!run_resource_flow() || !is_game_initialized) {
        return;
    }

    game_step_1();
}

int main(int argc, char* argv[]) {
    bool is_running = true;

    init_windows_console();
    SDLApp_Init();

    if (argc >= 3) {
        const int first_arg = SDL_atoi(argv[1]);
        if (first_arg == 1 || first_arg == 2) {
            // Direct P2P path: ./3sx <player> <ip>
            Netplay_SetParams(first_arg, argv[2]);
        } else {
            // Matchmaking path: ./3sx <server_ip> <server_port>
            const int port = SDL_atoi(argv[2]);
            Netplay_SetMatchmakingParams(argv[1], port);
        }
    }

    while (is_running) {
        is_running = SDLApp_PollEvents();
        SDLApp_BeginFrame();
        step_0();
        SDLApp_EndFrame();
        step_1();
    }

    AFS_Finish();
    SDLApp_Quit();
    return 0;
}

static void init_windows_console() {
#if defined(_WIN32)
    // attaches to an existing console for printouts. Works with windows CMD but not MSYS2
    if (AttachConsole(ATTACH_PARENT_PROCESS) == 0) {
        // if fails, then allocate a new console
        AllocConsole();
    }
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif
}

static void game_init() {
#if defined(DEBUG)
    DebugConfig_Init();
#endif

    flInitialize();
    flSetRenderState(FLRENDER_BACKCOLOR, 0);
    system_init_level = 0;
    ppgWorkInitializeApprication();
    distributeScratchPadAddress();
    njdp2d_init();
    njUserInit();
    palCreateGhost();
    ppgMakeConvTableTexDC();
    appSetupBasePriority();
    MemcardInit();
}

static void game_step_0() {
    flSetRenderState(FLRENDER_BACKCOLOR, 0xFF000000);

    if (Debug_w[0x43]) {
        flSetRenderState(FLRENDER_BACKCOLOR, 0xFF0000FF);
    }

    appSetupTempPriority();

    flPADGetALL();
    keyConvert();

#if defined(DEBUG)
    if (!test_flag) {
        if (mpp_w.sysStop) {
            sysSLOW = 1;

            switch (io_w.data[1].sw_new) {
            case SWK_LEFT_STICK:
                mpp_w.sysStop = false;
                // fallthrough

            case SWK_LEFT_SHOULDER:
                Slow_Timer = 1;
                break;

            default:
                switch (io_w.data[1].sw & (SWK_LEFT_SHOULDER | SWK_LEFT_TRIGGER)) {
                case SWK_LEFT_SHOULDER | SWK_LEFT_TRIGGER:
                    if ((sysFF = Debug_w[1]) == 0) {
                        sysFF = 1;
                    }

                    sysSLOW = 1;
                    Slow_Timer = 1;

                    break;

                case SWK_LEFT_TRIGGER:
                    if (Slow_Timer == 0) {
                        if ((Slow_Timer = Debug_w[0]) == 0) {
                            Slow_Timer = 1;
                        }

                        sysFF = 1;
                    }

                    break;

                default:
                    Slow_Timer = 2;
                    break;
                }

                break;
            }
        } else if (io_w.data[1].sw_new & SWK_LEFT_STICK) {
            mpp_w.sysStop = true;
        }
    }
#endif

    if ((Play_Mode != 3 && Play_Mode != 1) || (Game_pause != 0x81)) {
        p1sw_1 = p1sw_0;
        p2sw_1 = p2sw_0;
        p3sw_1 = p3sw_0;
        p4sw_1 = p4sw_0;
        p1sw_0 = p1sw_buff;
        p2sw_0 = p2sw_buff;
        p3sw_0 = p3sw_buff;
        p4sw_0 = p4sw_buff;

        if ((task[TASK_MENU].condition == 1) && (Mode_Type == MODE_PARRY_TRAINING) && (Play_Mode == 1)) {
            const u16 sw_buff = p2sw_0;
            p2sw_0 = p1sw_0;
            p1sw_0 = sw_buff;
        }
    }

    appCopyKeyData();

    mpp_w.inGame = false;

    if (Netplay_GetSessionState() != NETPLAY_SESSION_IDLE) {
        Netplay_Run();
        // Flush the 2D polygon buffer each frame when the game's normal render
        // loop isn't running, preventing the 100-item limit from overflowing.
        njdp2d_draw();
    } else {
        njUserMain();
        seqsBeforeProcess();
        njdp2d_draw();
        seqsAfterProcess();
        Netplay_TickMatchmaking();
        Netplay_TickDirectP2P();
    }

    KnjFlush();
    disp_effect_work();
    flFlip(0);
}

static void game_step_1() {
    Interrupt_Timer += 1;
    Record_Timer += 1;

    Scrn_Renew();
    Irl_Family();
    Irl_Scrn();
    BGM_Server();
}

u8 dctex_linear_mem[0x800];
u8 texcash_melt_buffer_mem[0x1000];
u8 tpu_free_mem[0x2000];

void distributeScratchPadAddress() {
    dctex_linear = (s16*)dctex_linear_mem;
    texcash_melt_buffer = (u8*)texcash_melt_buffer_mem;
    tpu_free = (TexturePoolUsed*)tpu_free_mem;
}

s32 mppGetFavoritePlayerNumber() {
    s32 i;
    s32 max = 1;
    s32 num = 0;

    if (Debug_w[0x2D]) {
        return Debug_w[0x2D] - 1;
    }

    for (i = 0; i < 0x14; i++) {
        if (max <= mpp_w.useChar[i]) {
            max = mpp_w.useChar[i];
            num = i + 1;
        }
    }

    return num;
}

void appCopyKeyData() {
    // FIXME: Should PLsw be saved/restored too?
    PLsw[0][1] = PLsw[0][0];
    PLsw[1][1] = PLsw[1][0];
    PLsw[0][0] = p1sw_buff;
    PLsw[1][0] = p2sw_buff;
}

u8* mppMalloc(u32 size) {
    return flAllocMemory(size);
}

void njUserInit() {
    s32 i;
    u32 size;

    sysFF = 1;
    mpp_w.sysStop = false;
    mpp_w.inGame = false;
    mpp_w.language = 0;
    mmSystemInitialize();
    flGetFrame(&mpp_w.fmsFrame);
    seqsInitialize(mppMalloc(seqsGetUseMemorySize()));
    ppg_Initialize(mppMalloc(0x60000), 0x60000);
    zlib_Initialize(mppMalloc(0x10000), 0x10000);
    size = flGetSpace();
    mpp_w.ramcntBuff = mppMalloc(size);
    Init_ram_control_work(mpp_w.ramcntBuff, size);

    for (i = 0; i < 0x14; i++) {
        mpp_w.useChar[i] = 0;
    }

    Interrupt_Timer = 0;
    Disp_Size_H = 100;
    Disp_Size_V = 100;
    Country = 4;

    if (Country == 0) {
        while (1) {}
    }

    Init_sound_system();
    Init_bgm_work();
    sndInitialLoad();
    cpInitTask();
    cpReadyTask(TASK_INIT, Init_Task);
}

void njUserMain() {
    CPU_Time_Lag[0] = 0;
    CPU_Time_Lag[1] = 0;
    CPU_Rec[0] = 0;
    CPU_Rec[1] = 0;

    Check_Replay_Status(0, Replay_Status[0]);
    Check_Replay_Status(1, Replay_Status[1]);

    cpLoopTask();

    if ((Game_pause != 0x81) && (Mode_Type == MODE_VERSUS) && (Play_Mode == 1)) {
        if ((plw[0].wu.operator == 0) && (CPU_Rec[0] == 0) && (Replay_Status[0] == 1)) {
            p1sw_0 = 0;

            Check_Replay_Status(0, 1);

            if (Debug_w[0x21]) {
                flPrintColor(0xFFFFFFFF);
                flPrintL(0x10, 0xA, "FAKE REC! PL1");
            }
        }

        if ((plw[1].wu.operator == 0) && (CPU_Rec[1] == 0) && (Replay_Status[1] == 1)) {
            p2sw_0 = 0;

            Check_Replay_Status(1, 1);

            if (Debug_w[0x21]) {
                flPrintColor(0xFFFFFFFF);
                flPrintL(0x10, 0xA, "FAKE REC!     PL2");
            }
        }
    }
}

void cpLoopTask() {
    disp_ramcnt_free_area();

#if defined(DEBUG)
    if (sysSLOW) {
        if (--Slow_Timer == 0) {
            sysSLOW = 0;
            Game_pause &= 0x7F;
        } else {
            Game_pause |= 0x80;
        }
    }
#endif

    for (int i = 0; i < 11; i++) {
        struct _TASK* task_ptr = &task[i];

        switch (task_ptr->condition) {
        case 1:
            task_ptr->func_adrs(task_ptr);
            break;

        case 2:
            task_ptr->condition = 1;
            break;

        case 3:
            break;
        }
    }
}

void cpInitTask() {
    memset(&task, 0, sizeof(task));
}

void cpReadyTask(TaskID num, void* func_adrs) {
    struct _TASK* task_ptr = &task[num];

    memset(task_ptr, 0, sizeof(struct _TASK));

    task_ptr->func_adrs = func_adrs;
    task_ptr->condition = 2;
}

void cpExitTask(TaskID num) {
    SDL_zero(task[num]);
}

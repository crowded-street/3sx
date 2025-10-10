#include "sf33rd/Source/Game/main.h"
#include "common.h"
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
#include "sf33rd/Source/Game/AcrUtil.h"
#include "sf33rd/Source/Game/DC_Ghost.h"
#include "sf33rd/Source/Game/EFFECT.h"
#include "sf33rd/Source/Game/GD3rd.h"
#include "sf33rd/Source/Game/IOConv.h"
#include "sf33rd/Source/Game/MTRANS.h"
#include "sf33rd/Source/Game/PLCNT.h"
#include "sf33rd/Source/Game/RAMCNT.h"
#include "sf33rd/Source/Game/SYS_sub.h"
#include "sf33rd/Source/Game/SYS_sub2.h"
#include "sf33rd/Source/Game/Sound3rd.h"
#include "sf33rd/Source/Game/WORK_SYS.h"
#include "sf33rd/Source/Game/bg.h"
#include "sf33rd/Source/Game/color3rd.h"
#include "sf33rd/Source/Game/debug/Debug.h"
#include "sf33rd/Source/Game/init3rd.h"
#include "sf33rd/Source/Game/texcash.h"
#include "sf33rd/Source/Game/workuser.h"
#include "sf33rd/Source/PS2/mc/knjsub.h"
#include "sf33rd/Source/PS2/mc/mcsub.h"
#include "sf33rd/Source/PS2/ps2Quad.h"
#include "structs.h"

#include "port/resources.h"

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
void MaskScreenEdge();
void appCopyKeyData();
u8* mppMalloc(u32 size);
void njUserInit();
s32 njUserMain();
void cpLoopTask();
void cpInitTask();
void cpReadyTask(u16 num, void* func_adrs);
void cpExitTask(u16 num);

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

static void step_0() {
    if (!run_resource_flow()) {
        return;
    }

    if (!is_game_initialized) {
        game_init();
        is_game_initialized = true;
    }

    if (is_game_initialized) {
        game_step_0();
    }
}

static void step_1() {
    if (!run_resource_flow() || !is_game_initialized) {
        return;
    }

    game_step_1();
}

int main() {
    bool is_running = true;

    init_windows_console();
    SDLApp_Init();

    while (is_running) {
        is_running = SDLApp_PollEvents();
        SDLApp_BeginFrame();
        step_0();
        SDLApp_EndFrame();
        step_1();
    }

    SDLApp_Quit();
    return 0;
}

bool get_game_initialized() {
    return is_game_initialized;
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
    flInitialize(flPs2State.DispWidth, flPs2State.DispHeight);
    flSetRenderState(FLRENDER_BACKCOLOR, 0);
    flSetDebugMode(0);
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
    initRenderState(0);
    mpp_w.ds_h[0] = mpp_w.ds_h[1];
    mpp_w.ds_v[0] = mpp_w.ds_v[1];
    mpp_w.ds_h[1] = 100;
    mpp_w.ds_v[1] = 100;
    mpp_w.vprm.x0 = 0.0f;
    mpp_w.vprm.y0 = 0.0f;
    mpp_w.vprm.x1 = (mpp_w.ds_h[0] * 384) / 100.0f;
    mpp_w.vprm.y1 = (mpp_w.ds_v[0] * 224) / 100.0f;
    mpp_w.vprm.ne = -1.0f;
    mpp_w.vprm.fa = 1.0f;

    appViewSetItems(&mpp_w.vprm);
    appViewMatrix();
    flAdjustScreen(X_Adjust + Correct_X[0], Y_Adjust + Correct_Y[0]);
    setBackGroundColor(0xFF000000);

    if (Debug_w[0x43]) {
        setBackGroundColor(0xFF0000FF);
    }

    appSetupTempPriority();

    flPADGetALL();
    keyConvert();

    if (((Usage == 7) || (Usage == 2)) && !test_flag) {
        if (mpp_w.sysStop) {
            if (mpp_w.sysStop == 1) {
                sysSLOW = 1;

                switch (io_w.data[1].sw_new) {
                case 0x2000:
                    mpp_w.sysStop = 0;
                    // fallthrough

                case 0x80:
                    Slow_Timer = 1;
                    break;

                default:
                    switch (io_w.data[1].sw & 0x880) {
                    case 0x880:
                        if ((sysFF = Debug_w[1]) == 0) {
                            sysFF = 1;
                        }

                        sysSLOW = 1;
                        Slow_Timer = 1;

                        break;

                    case 0x800:
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
            }
        } else if (io_w.data[1].sw_new & 0x2000) {
            mpp_w.sysStop = 1;
        }
    }

    Interrupt_Flag = 0;

    if ((Play_Mode != 3 && Play_Mode != 1) || (Game_pause != 0x81)) {
        p1sw_1 = p1sw_0;
        p2sw_1 = p2sw_0;
        p3sw_1 = p3sw_0;
        p4sw_1 = p4sw_0;
        p1sw_0 = p1sw_buff;
        p2sw_0 = p2sw_buff;
        p3sw_0 = p3sw_buff;
        p4sw_0 = p4sw_buff;

        if ((task[3].condition == 1) && (Mode_Type == 4) && (Play_Mode == 1)) {
            const u16 sw_buff = p2sw_0;
            p2sw_0 = p1sw_0;
            p1sw_0 = sw_buff;
        }
    }

    appCopyKeyData();
    render_start();

    mpp_w.inGame = 0;

    njUserMain();
    MaskScreenEdge();
    seqsBeforeProcess();
    njdp2d_draw();
    seqsAfterProcess();

    if (Debug_w[6] == 0) {
        CP3toPS2Draw();
    }

    KnjFlush();
    render_end();

    u32 sysinfodisp = 0;

    if (Debug_w[2] == 2) {
        sysinfodisp = 3;
    }

    if (Debug_w[2] == 1) {
        sysinfodisp = 2;
    }

    switch (mpp_w.sysStop) {
    case 2:
        sysinfodisp = 0;
        break;

    case 1:
        sysinfodisp &= ~2;
        break;
    }

    flSetDebugMode(sysinfodisp);
    disp_effect_work();
    flFlip(0);
}

static void game_step_1() {
    Interrupt_Flag = 1;
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

void MaskScreenEdge() {
    VPRM prm;
    f32 pos[8];

    appViewGetItems(&prm);

    if (prm.x1 < 384.0f) {
        pos[0] = pos[4] = mpp_w.vprm.x1;
        pos[2] = pos[6] = 384.0f;
        pos[1] = pos[3] = mpp_w.vprm.y0;
        pos[5] = pos[7] = 224.0f;

        njdp2d_sort(pos, PrioBase[0], (0xFF << 24), 0);
    }

    if (prm.y1 < 224.0f) {
        pos[0] = pos[4] = mpp_w.vprm.x0;
        pos[2] = pos[6] = 384.0f;
        pos[1] = pos[3] = mpp_w.vprm.y1;
        pos[5] = pos[7] = 224.0f;

        njdp2d_sort(pos, PrioBase[0], (0xFF << 24), 0);
    }
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
    mpp_w.sysStop = 0;
    mpp_w.inGame = 0;
    mpp_w.ctrDemo = 0;
    mpp_w.language = 0;
    mpp_w.langload = -1;
    mpp_w.pal50Hz = 0;
    mpp_w.vprm.x0 = 0.0f;
    mpp_w.vprm.y0 = 0.0f;
    mpp_w.vprm.x1 = 384.0f;
    mpp_w.vprm.y1 = 224.0f;
    mpp_w.vprm.ne = -1.0f;
    mpp_w.vprm.fa = 1.0f;
    appViewSetItems(&mpp_w.vprm);
    appViewMatrix();
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
    SA_Zoom_X = 0.0f;
    SA_Zoom_Y = 0.0f;
    Disp_Size_H = 100;
    Disp_Size_V = 100;
    mpp_w.ds_h[0] = mpp_w.ds_h[1] = Disp_Size_H;
    mpp_w.ds_v[0] = mpp_w.ds_v[1] = Disp_Size_V;
    Country = 0;
    Country = 4;
    Screen_PAL = 0;
    Turbo = 0;

    if (Country == 0) {
        while (1) {}
    }

    Turbo_Timer = 1;
    Screen_Zoom_X = 1.0f;
    Screen_Zoom_Y = 1.0f;
    Setup_Disp_Size(0);
    Correct_X[0] = 0;
    Correct_Y[0] = 0;
    Frame_Zoom_X = Screen_Zoom_X + SA_Zoom_X;
    Frame_Zoom_Y = Screen_Zoom_Y + SA_Zoom_Y;
    Zoom_Base_Position_X = 0;
    Zoom_Base_Position_Y = 0;
    Zoom_Base_Position_Z = 0;
    sys_w.disp.now = sys_w.disp.new = 1;
    sys_w.pause = 0;
    sys_w.reset = 0;

    Init_sound_system();
    Init_bgm_work();
    Setup_Directory_Record_Data();
    sndInitialLoad();
    cpInitTask();
    cpReadyTask(INIT_TASK_NUM, Init_Task);
}

s32 njUserMain() {
    CPU_Time_Lag[0] = 0;
    CPU_Time_Lag[1] = 0;
    CPU_Rec[0] = 0;
    CPU_Rec[1] = 0;

    Check_Replay_Status(0, Replay_Status[0]);
    Check_Replay_Status(1, Replay_Status[1]);

    Frame_Zoom_X = Screen_Zoom_X + SA_Zoom_X;
    Frame_Zoom_Y = Screen_Zoom_Y + SA_Zoom_Y;

    if (sys_w.disp.now == sys_w.disp.new) {
        cpLoopTask();

        if ((Game_pause != 0x81) && (Mode_Type == 1) && (Play_Mode == 1)) {
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
    } else {
        sys_w.disp.now = sys_w.disp.new;
    }

    return sys_w.gd_error;
}

void cpLoopTask() {
    struct _TASK* task_ptr = task;

    disp_ramcnt_free_area();

    if (sysSLOW) {
        if (--Slow_Timer == 0) {
            sysSLOW = 0;
            Game_pause &= 0x7F;
        } else {
            Game_pause |= 0x80;
        }
    }

    Process_Counter = 1;

    if (Turbo && (Game_pause != 0x81)) {
        if (--Turbo_Timer == 0) {
            Turbo_Timer = Turbo;
            Process_Counter = 2;
        }
    }

    for (current_task_num = 0; current_task_num < 11; current_task_num++) {
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

        task_ptr += 1;
    }
}

void cpInitTask() {
    memset(&task, 0, sizeof(task));
}

void cpReadyTask(u16 num, void* func_adrs) {
    struct _TASK* task_ptr = task + num;

    memset(task_ptr, 0, sizeof(struct _TASK));

    task_ptr->func_adrs = func_adrs;
    task_ptr->condition = 2;
}

void cpExitTask(u16 num) {
    struct _TASK* task_ptr = task + num;

    task_ptr->condition = 0;

    if (task_ptr->callback_adrs != NULL) {
        task_ptr->callback_adrs();
    }
}

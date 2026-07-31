#include "main.h"
#include "common.h"
#if NETPLAY_ENABLED
#include "platform/netplay/netplay.h"
#include "platform/netplay/netplay_stress.h"
#endif
#include "sf33rd/AcrSDK/common/mlPAD.h"
#include "sf33rd/AcrSDK/ps2/flps2etc.h"
#include "sf33rd/AcrSDK/ps2/flps2render.h"
#include "sf33rd/AcrSDK/ps2/foundaps2.h"
#include "sf33rd/Source/Common/MemMan.h"
#include "sf33rd/Source/Common/PPGFile.h"
#include "sf33rd/Source/Common/PPGWork.h"
#include "sf33rd/Source/Compress/zlibApp.h"
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
#include "structs.h"

#include <SDL3/SDL.h>

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>

s32 system_init_level;
MPP mpp_w;

static u8 dctex_linear_mem[0x800];
static u8 texcash_melt_buffer_mem[0x1000];
static u8 tpu_free_mem[0x2000];

static u8* mppMalloc(u32 size) {
    return flAllocMemory(size);
}

// Initialization

static void cpInitTask() {
    memset(&task, 0, sizeof(task));
}

static void njUserInit() {
    s32 i;
    u32 size;

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

static void distributeScratchPadAddress() {
    dctex_linear = (s16*)dctex_linear_mem;
    texcash_melt_buffer = (u8*)texcash_melt_buffer_mem;
    tpu_free = (TexturePoolUsed*)tpu_free_mem;
}

void Main_Init() {
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
}

// Iteration

static void cpLoopTask() {
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

static void appCopyKeyData() {
    // FIXME: Should PLsw be saved/restored too?
    PLsw[0][1] = PLsw[0][0];
    PLsw[1][1] = PLsw[1][0];
    PLsw[0][0] = p1sw_buff;
    PLsw[1][0] = p2sw_buff;
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
        }

        if ((plw[1].wu.operator == 0) && (CPU_Rec[1] == 0) && (Replay_Status[1] == 1)) {
            p2sw_0 = 0;

            Check_Replay_Status(1, 1);
        }
    }
}

void Main_StepFrame() {
    flSetRenderState(FLRENDER_BACKCOLOR, 0xFF000000);
    appSetupTempPriority();
    flPADGetALL();
    keyConvert();

#if NETPLAY_ENABLED
    Stress_InjectBootInput();
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

#if NETPLAY_ENABLED
    if (Netplay_GetSessionState() != NETPLAY_SESSION_IDLE) {
        Netplay_Run();
        // Flush the 2D polygon buffer each frame when the game's normal render
        // loop isn't running, preventing the 100-item limit from overflowing.
        njdp2d_draw();
    } else {
        njUserMain();
        njdp2d_draw();
        Netplay_TickMatchmaking();
        Netplay_TickDirectP2P();
        Stress_Tick();
    }
#else
    njUserMain();
    njdp2d_draw();
#endif

    flFlip(0);
}

void Main_FinishFrame() {
    Interrupt_Timer += 1;

    Scrn_Renew();
    Irl_Family();
    Irl_Scrn();
    BGM_Server();
}

// Tasks

void cpReadyTask(TaskID num, void* func_adrs) {
    struct _TASK* task_ptr = &task[num];

    memset(task_ptr, 0, sizeof(struct _TASK));

    task_ptr->func_adrs = func_adrs;
    task_ptr->condition = 2;
}

void cpExitTask(TaskID num) {
    SDL_zero(task[num]);
}

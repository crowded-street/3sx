#if CRS_APP_DRIVER_PSP

#include "main.h"
#include "platform/video/psp/psp_renderer.h"
#include "port/io/afs.h"
#include "port/resources.h"

#include <pspuser.h>

#include <stdbool.h>

PSP_MODULE_INFO("3SX", PSP_MODULE_USER, 0, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);
PSP_HEAP_THRESHOLD_SIZE_KB(1024);

static void init() {
    AFS_Init(Resources_GetAFSPath());
    PSPRenderer_Init();
    Main_Init();
}

static void begin_frame() {
    PSPRenderer_BeginFrame();
    AFS_RunServer();
}

static void end_frame() {
    PSPRenderer_RenderFrame();
    PSPRenderer_EndFrame();
}

int main() {
    init();

    while (true) {
        begin_frame();
        Main_StepFrame();

        end_frame();
        Main_FinishFrame();
    }

    return 0;
}

#endif

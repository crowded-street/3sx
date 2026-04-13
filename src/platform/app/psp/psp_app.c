#if CRS_APP_DRIVER_PSP

#include <pspdebug.h>
#include <pspuser.h>

PSP_MODULE_INFO("3SX", PSP_MODULE_USER, 0, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);
PSP_HEAP_THRESHOLD_SIZE_KB(1024);

int main() {
    pspDebugScreenInit();
    pspDebugScreenPrintf("Hello from 3SX!\n");
    sceKernelDelayThread(2 * 1000 * 1000);
    return 0;
}

#endif

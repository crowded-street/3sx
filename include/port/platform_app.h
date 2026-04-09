#ifndef PORT_PLATFORM_APP_H
#define PORT_PLATFORM_APP_H

#include <stdbool.h>

int PlatformApp_PreInit(void);
int PlatformApp_FullInit(void);
void PlatformApp_Quit(void);
bool PlatformApp_PollEvents(void);
void PlatformApp_BeginFrame(void);
void PlatformApp_EndFrame(void);
void PlatformApp_Exit(void);

#endif

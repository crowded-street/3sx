#ifndef PORT_APP_H
#define PORT_APP_H

#include <stdbool.h>

int App_PreInit();
int App_FullInit();
void App_Quit();
bool App_PollEvents();
void App_BeginFrame();
void App_EndFrame();
void App_Exit();

#endif

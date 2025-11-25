#ifndef NETPLAY_H
#define NETPLAY_H

#include <stdbool.h>

void Netplay_SetParams(int player, const char* ip);
void Netplay_Begin();
void Netplay_Run();

#endif

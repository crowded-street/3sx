#ifndef SDL_NET_ADAPTER_H
#define SDL_NET_ADAPTER_H

#include "gekkonet.h"

struct NET_DatagramSocket;

GekkoNetAdapter* SDLNetAdapter_Create(struct NET_DatagramSocket* sock);
void SDLNetAdapter_Destroy();

#endif

#include "netplay/sdl_net_adapter.h"

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

#define MAX_NETWORK_RESULTS 128

static NET_DatagramSocket* adapter_sock = NULL;
static GekkoNetAdapter adapter;
static GekkoNetResult* results[MAX_NETWORK_RESULTS];
static int result_count = 0;

static NET_Address* cached_remote = NULL;
static Uint16 cached_port = 0;

static void send_data(GekkoNetAddress* addr, const char* data, int length) {
    if (adapter_sock == NULL) {
        return;
    }

    if (cached_remote == NULL) {
        char ip[64];
        int port = 0;
        SDL_sscanf((const char*)addr->data, "%63[^:]:%d", ip, &port);
        cached_remote = NET_ResolveHostname(ip);
        cached_port = (Uint16)port;
    }

    switch (NET_GetAddressStatus(cached_remote)) {
    case NET_SUCCESS:
        NET_SendDatagram(adapter_sock, cached_remote, cached_port, data, length);
        break;
    case NET_FAILURE:
        NET_UnrefAddress(cached_remote);
        cached_remote = NULL;
        break;
    case NET_WAITING: // still resolving, skip — GekkoNet will retransmit
        break;
    }
}

static GekkoNetResult** receive_data(int* length) {
    result_count = 0;

    if (adapter_sock == NULL) {
        *length = 0;
        return results;
    }

    NET_Datagram* dgram = NULL;

    while (result_count < MAX_NETWORK_RESULTS && NET_ReceiveDatagram(adapter_sock, &dgram) && dgram) {
        const char* ip_str = NET_GetAddressString(dgram->addr);
        char addr_str[64];

        SDL_snprintf(addr_str, sizeof(addr_str), "%s:%d", ip_str, (int)dgram->port);

        GekkoNetResult* res = SDL_malloc(sizeof(GekkoNetResult));
        size_t addr_len = SDL_strlen(addr_str);

        res->addr.data = SDL_malloc(addr_len + 1);
        SDL_strlcpy((char*)res->addr.data, addr_str, addr_len + 1);
        res->addr.size = (unsigned int)addr_len;

        res->data = SDL_malloc(dgram->buflen);
        SDL_memcpy(res->data, dgram->buf, dgram->buflen);
        res->data_len = (unsigned int)dgram->buflen;

        results[result_count++] = res;
        NET_DestroyDatagram(dgram);
        dgram = NULL;
    }

    *length = result_count;
    return results;
}

static void free_data(void* ptr) {
    SDL_free(ptr);
}

GekkoNetAdapter* SDLNetAdapter_Create(struct NET_DatagramSocket* sock) {
    adapter_sock = sock;
    adapter.send_data = send_data;
    adapter.receive_data = receive_data;
    adapter.free_data = free_data;
    return &adapter;
}

void SDLNetAdapter_Destroy() {
    adapter_sock = NULL;
    if (cached_remote != NULL) {
        NET_UnrefAddress(cached_remote);
        cached_remote = NULL;
    }
    cached_port = 0;
}

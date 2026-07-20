#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

#if NETPLAY_ENABLED
typedef struct NetplayArgs {
    int p2p_local_player;
    const char* p2p_remote_ip;
    const char* matchmaking_ip;
    int matchmaking_port;
} NetplayArgs;
#endif

#if STATCHECK
typedef struct StatcheckArgs {
    const char* ram_archive_path;
    bool headless;
} StatcheckArgs;
#endif

typedef struct Args {
    bool no_sound;
#if NETPLAY_ENABLED
    NetplayArgs netplay;
#endif
#if STATCHECK
    StatcheckArgs statcheck;
#endif
} Args;

void init_args(int argc, const char* argv[]);
const Args* get_args();

#endif

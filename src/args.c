#include "args.h"
#include "main.h"

#include "argparse/argparse.h"

#include <stdio.h>
#include <stdlib.h>

static Args args = { 0 };

#if NETPLAY_ENABLED
static void error_out(const char* error) {
    fprintf(stderr, "%s Exiting.\n", error);
    exit(1);
}
#endif

static void verify_configuration(const Args* args) {
#if NETPLAY_ENABLED
    const NetplayArgs* netplay = &args->netplay;
    const bool p2p_specified = netplay->p2p_local_player > 0 || netplay->p2p_remote_ip != NULL;
    const bool matchmaking_specified = netplay->matchmaking_ip != NULL || netplay->matchmaking_port != 0;

    if (p2p_specified && matchmaking_specified) {
        error_out("Can't specify P2P and matchmaking at the same time.");
    }

    if (p2p_specified) {
        if (netplay->p2p_local_player != 1 && netplay->p2p_local_player != 2) {
            error_out("Local player must be 1 or 2.");
        }

        if (netplay->p2p_remote_ip == NULL) {
            error_out("You must specify --p2p-remote-ip.");
        }
    }

    if (matchmaking_specified) {
        if (netplay->matchmaking_ip == NULL) {
            error_out("You must specify --matchmaking-ip.");
        }

        if (netplay->matchmaking_port == 0) {
            error_out("You must specify --matchmaking-port.");
        }
    }
#endif
}

void init_args(int argc, const char* argv[]) {
    struct argparse_option options[] = {
        OPT_HELP(),

#if NETPLAY_ENABLED
        OPT_GROUP("Netplay"),
        OPT_INTEGER(
            0, "p2p-local-player", &args.netplay.p2p_local_player, "Number of the local player (1 or 2).", NULL, 0, 0
        ),
        OPT_STRING(0, "p2p-remote-ip", &args.netplay.p2p_remote_ip, "Remote player IP.", NULL, 0, 0),
        OPT_STRING(0, "matchmaking-ip", &args.netplay.matchmaking_ip, "Matchmaking server IP.", NULL, 0, 0),
        OPT_INTEGER(0, "matchmaking-port", &args.netplay.matchmaking_port, "Matchmaking server port.", NULL, 0, 0),
#endif

#if STATCHECK
        OPT_GROUP("Statcheck"),
        OPT_STRING(0, "states", &args.statcheck.states_path, "Path to states.", NULL, 0, 0),
#endif

        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, NULL, 0);
    argparse_parse(&argparse, argc, argv);

    verify_configuration(&args);
}

const Args* get_args() {
    return &args;
}

#include "args.h"
#include "main.h"

#include "argparse/argparse.h"

#include <stdio.h>
#include <stdlib.h>

static Args args = { 0 };

// Compiling conditionally to avoid "unused function" erros
#if NETPLAY_ENABLED || STATCHECK
static void error_out(const char* error) {
    fprintf(stderr, "%s Exiting.\n", error);
    exit(1);
}
#endif

#if NETPLAY_ENABLED
/* The compound tests verify_configuration makes, each copied character for
 * character from the condition it stood in. They sit inside this `#if` with
 * their only callers, so a build without netplay does not carry them. */
static bool p2p_is_specified(const NetplayArgs* netplay) {
    return netplay->p2p_local_player > 0 || netplay->p2p_remote_ip != NULL;
}

static bool matchmaking_is_specified(const NetplayArgs* netplay) {
    return netplay->matchmaking_ip != NULL || netplay->matchmaking_port != 0;
}

static bool stress_combined_with_network(const NetplayArgs* netplay, bool p2p_specified, bool matchmaking_specified) {
    return netplay->stress && (p2p_specified || matchmaking_specified);
}

static bool stress_options_without_stress(const NetplayArgs* netplay) {
    return !netplay->stress && (netplay->stress_seed != 0 || netplay->stress_check_distance != 0 ||
                                netplay->stress_frames != 0 || netplay->stress_out != NULL);
}

static bool negative_stress_counts(const NetplayArgs* netplay) {
    return netplay->stress_check_distance < 0 || netplay->stress_frames < 0;
}

static bool p2p_local_player_invalid(const NetplayArgs* netplay) {
    return netplay->p2p_local_player != 1 && netplay->p2p_local_player != 2;
}

/* The three things a stress session insists on. */
static void verify_stress_options(const NetplayArgs* netplay, bool p2p_specified, bool matchmaking_specified) {
    if (stress_combined_with_network(netplay, p2p_specified, matchmaking_specified)) {
        error_out("A stress session is local, so it can't be combined with P2P or matchmaking.");
    }

    if (stress_options_without_stress(netplay)) {
        error_out("Stress options require --stress.");
    }

    if (negative_stress_counts(netplay)) {
        error_out("Stress frame counts can't be negative.");
    }
}

/* What a P2P session needs beside being asked for. */
static void verify_p2p(const NetplayArgs* netplay) {
    if (p2p_local_player_invalid(netplay)) {
        error_out("Local player must be 1 or 2.");
    }

    if (netplay->p2p_remote_ip == NULL) {
        error_out("You must specify --p2p-remote-ip.");
    }
}

/* What a matchmaking session needs beside being asked for. */
static void verify_matchmaking(const NetplayArgs* netplay) {
    if (netplay->matchmaking_ip == NULL) {
        error_out("You must specify --matchmaking-ip.");
    }

    if (netplay->matchmaking_port == 0) {
        error_out("You must specify --matchmaking-port.");
    }
}
#endif

static void verify_configuration(const Args* args) {
#if NETPLAY_ENABLED
    const NetplayArgs* netplay = &args->netplay;
    const bool p2p_specified = p2p_is_specified(netplay);
    const bool matchmaking_specified = matchmaking_is_specified(netplay);

    if (p2p_specified && matchmaking_specified) {
        error_out("Can't specify P2P and matchmaking at the same time.");
    }

    verify_stress_options(netplay, p2p_specified, matchmaking_specified);

    if (p2p_specified) {
        verify_p2p(netplay);
    }

    if (matchmaking_specified) {
        verify_matchmaking(netplay);
    }
#endif

#if STATCHECK
    if (args->statcheck.ram_archive_path == NULL) {
        error_out("You must specify --ram-archive.");
    }
#endif
}

void init_args(int argc, const char* argv[]) {
    struct argparse_option options[] = {
        OPT_HELP(),

        OPT_BOOLEAN(0, "no-sound", &args.no_sound, "Run the game without sound.", NULL, 0, 0),

#if NETPLAY_ENABLED
        OPT_GROUP("Netplay"),
        OPT_INTEGER(
            0, "p2p-local-player", &args.netplay.p2p_local_player, "Number of the local player (1 or 2).", NULL, 0, 0
        ),
        OPT_STRING(0, "p2p-remote-ip", &args.netplay.p2p_remote_ip, "Remote player IP.", NULL, 0, 0),
        OPT_STRING(0, "matchmaking-ip", &args.netplay.matchmaking_ip, "Matchmaking server IP.", NULL, 0, 0),
        OPT_INTEGER(0, "matchmaking-port", &args.netplay.matchmaking_port, "Matchmaking server port.", NULL, 0, 0),
        OPT_BOOLEAN(
            0, "stress", &args.netplay.stress, "Run a local stress session that hunts for rollback desyncs.", NULL, 0, 0
        ),
        OPT_INTEGER(0, "stress-seed", &args.netplay.stress_seed, "Seed for the generated inputs.", NULL, 0, 0),
        OPT_INTEGER(
            0,
            "stress-check-distance",
            &args.netplay.stress_check_distance,
            "How many frames to roll back and re-simulate each update.",
            NULL,
            0,
            0
        ),
        OPT_INTEGER(
            0,
            "stress-frames",
            &args.netplay.stress_frames,
            "Exit after this many frames (0 runs until closed).",
            NULL,
            0,
            0
        ),
        OPT_STRING(
            0,
            "stress-out",
            &args.netplay.stress_out,
            "Directory for the trace and state dumps, so runs can go in parallel.",
            NULL,
            0,
            0
        ),
#endif

#if STATCHECK
        OPT_GROUP("Statcheck"),
        OPT_STRING(0, "ram-archive", &args.statcheck.ram_archive_path, "Path to RAM archive.", NULL, 0, 0),
        OPT_BOOLEAN(
            0, "headless", &args.statcheck.headless, "Run the game without a window. Implies --no-sound", NULL, 0, 0
        ),
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

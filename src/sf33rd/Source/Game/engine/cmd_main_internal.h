#ifndef CMD_MAIN_INTERNAL_H
#define CMD_MAIN_INTERNAL_H

#include "structs.h"
#include "types.h"

/* Shared between cmd_main.c and cmd_main_checks.c, which were one file.
 * Defined in cmd_main.c; check_init and check_next step through it to reach
 * the checker for the next step type. It already had external linkage. */
extern void (*chk_move_jp[28])();

#endif // CMD_MAIN_INTERNAL_H

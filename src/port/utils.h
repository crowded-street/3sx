#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#ifndef __dead2
#define __dead2 __attribute__((__noreturn__))
#endif

__dead2 void fatal_error(const char* fmt, ...);
__dead2 void not_implemented(const char* func);
void debug_print(const char* fmt, ...);
void stop_if(bool condition);

#endif

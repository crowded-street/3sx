#include "common.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
#define SYMBOL_NAME_MAX 256
#else
#include <execinfo.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define BACKTRACE_MAX 100

void fatal_error(const s8* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "Fatal error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    void* buffer[BACKTRACE_MAX];
#if !defined(_WIN32)
    int nptrs = backtrace(buffer, BACKTRACE_MAX);
    fprintf(stderr, "Stack trace:\n");
    backtrace_symbols_fd(buffer, nptrs, fileno(stderr));
#else
    fprintf(stderr, "Stack trace:\n");
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);
    int nptrs = CaptureStackBackTrace(0, BACKTRACE_MAX, buffer, NULL);
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(1, sizeof(SYMBOL_INFO) + SYMBOL_NAME_MAX);
    if (!symbol) {
        fprintf(stderr, "Calloc failed when allocating SYMBOL_INFO, bailing!\n\n");
        SymCleanup(process);
        abort();
    }
    symbol->MaxNameLen = SYMBOL_NAME_MAX;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for (int i = 0; i < nptrs; i++) {
        SymFromAddr(process, (DWORD64)buffer[i], 0, symbol);
        fprintf(stderr, "%i: %s - 0x%0llX\n", nptrs - i - 1, symbol->Name, symbol->Address);
    }
    free(symbol);
    SymCleanup(process);
#endif
    abort();
}

void not_implemented(const s8* func) {
    fatal_error("Function not implemented: %s\n", func);
}

void debug_print(const char* fmt, ...) {
#if defined(DEBUG)
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
#endif
}

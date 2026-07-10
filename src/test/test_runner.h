#if STATCHECK

#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <stdbool.h>

bool TestRunner_Init(const char* archive_path);
void TestRunner_Destroy();
void TestRunner_Prologue();
void TestRunner_Epilogue();

#endif

#endif

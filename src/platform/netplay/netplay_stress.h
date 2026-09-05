#ifndef NETPLAY_STRESS_H
#define NETPLAY_STRESS_H

#if NETPLAY_ENABLED

#include "types.h"

#include "gekkonet.h"

#include <stdbool.h>

/// A stress session re-simulates the last check_distance frames every update and
/// compares the resulting checksums against the first pass, so rollback desyncs
/// surface on one machine without a second peer. The run is unattended: it drives
/// the boot screens and character select itself, then hands the fight to the session.

void Stress_SetOutputDir(const char* directory);
void Stress_Begin(int seed, int check_distance, int frames);
void Stress_Tick();
void Stress_InjectBootInput();

bool Stress_IsRequested();
bool Stress_IsRunning();
bool Stress_IsFinished();

void Stress_Path(char* dst, size_t size, const char* relative);
void Stress_Trace(const char* fmt, ...);

void Stress_CreateSession(GekkoConfig* config, GekkoSession** session, int* player_handle);
u16 Stress_NextInput(int player);
void Stress_OnFrameAdvanced();
void Stress_RecordState(int frame, u32 checksum);
void Stress_OnDesync(int frame);
void Stress_OnResimulationDiverged(int frame, u32 first_checksum, u32 resim_checksum);

#endif // NETPLAY_ENABLED

#endif

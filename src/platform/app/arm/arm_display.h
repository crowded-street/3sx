#if CRS_APP_DRIVER_ARM

#ifndef ARM_DISPLAY_H
#define ARM_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

// ARM display frontend. Picks DRM first, then fbdev.

bool ArmDisplay_Init();
void ArmDisplay_Shutdown();
void ArmDisplay_GetResolution(int* out_width, int* out_height);
void ArmDisplay_ComputePresentRect(
    int out_width, int out_height, int* out_x, int* out_y, int* out_present_width, int* out_present_height
);

// Present one ARGB8888 frame.
void ArmDisplay_Present(const uint32_t* argb_pixels, int w, int h);

#endif

#endif // CRS_APP_DRIVER_ARM

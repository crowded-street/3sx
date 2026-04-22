#if CRS_APP_DRIVER_ARM

#include "platform/app/arm/arm_display.h"
#include "port/config/config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Backend entry points.

#if CRS_ARM_HAVE_DRM
bool arm_display_drm_init();
void arm_display_drm_shutdown();
void arm_display_drm_get_resolution(int* out_width, int* out_height);
void arm_display_drm_present(const uint32_t* argb_pixels, int w, int h);
#else
static inline bool arm_display_drm_init() {
    return false;
}

static inline void arm_display_drm_shutdown() {}

static inline void arm_display_drm_get_resolution(int* w, int* h) {
    (void)w;
    (void)h;
}

static inline void arm_display_drm_present(const uint32_t* px, int w, int h) {
    (void)px;
    (void)w;
    (void)h;
}
#endif

bool arm_display_fbdev_init();
void arm_display_fbdev_shutdown();
void arm_display_fbdev_get_resolution(int* out_width, int* out_height);
void arm_display_fbdev_present(const uint32_t* argb_pixels, int w, int h);

typedef enum DisplayBackend {
    DISPLAY_BACKEND_NONE = 0,
    DISPLAY_BACKEND_DRM,
    DISPLAY_BACKEND_FBDEV,
} DisplayBackend;

static DisplayBackend active_backend = DISPLAY_BACKEND_NONE;
static const float game_target_ratio = 4.0f / 3.0f;

static bool use_stretch_present() {
    const char* scale_mode = Config_GetString(CFG_KEY_SCALEMODE);
    return scale_mode != NULL && strcmp(scale_mode, "stretch") == 0;
}

bool ArmDisplay_Init() {
    if (arm_display_drm_init()) {
        active_backend = DISPLAY_BACKEND_DRM;
        fprintf(stderr, "[arm_display] using DRM/KMS backend\n");
        return true;
    }

    if (arm_display_fbdev_init()) {
        active_backend = DISPLAY_BACKEND_FBDEV;
        fprintf(stderr, "[arm_display] DRM unavailable, fell back to /dev/fb0\n");
        return true;
    }

    fprintf(stderr, "[arm_display] no display backend available\n");
    return false;
}

void ArmDisplay_Shutdown() {
    switch (active_backend) {
    case DISPLAY_BACKEND_DRM:
        arm_display_drm_shutdown();
        break;

    case DISPLAY_BACKEND_FBDEV:
        arm_display_fbdev_shutdown();
        break;

    case DISPLAY_BACKEND_NONE:
        break;
    }

    active_backend = DISPLAY_BACKEND_NONE;
}

void ArmDisplay_GetResolution(int* out_width, int* out_height) {
    switch (active_backend) {
    case DISPLAY_BACKEND_DRM:
        arm_display_drm_get_resolution(out_width, out_height);
        return;

    case DISPLAY_BACKEND_FBDEV:
        arm_display_fbdev_get_resolution(out_width, out_height);
        return;

    case DISPLAY_BACKEND_NONE:
        if (out_width != NULL) {
            *out_width = 0;
        }

        if (out_height != NULL) {
            *out_height = 0;
        }

        return;
    }
}

void ArmDisplay_ComputePresentRect(
    int out_width, int out_height, int* out_x, int* out_y, int* out_present_width, int* out_present_height
) {
    int present_x = 0;
    int present_y = 0;
    int present_w = out_width;
    int present_h = out_height;

    if (!use_stretch_present()) {
        present_h = (int)((float)out_width / game_target_ratio);

        if (present_h > out_height) {
            present_h = out_height;
            present_w = (int)((float)out_height * game_target_ratio);
        }

        present_x = (out_width - present_w) / 2;
        present_y = (out_height - present_h) / 2;
    }

    if (out_x != NULL) {
        *out_x = present_x;
    }

    if (out_y != NULL) {
        *out_y = present_y;
    }

    if (out_present_width != NULL) {
        *out_present_width = present_w;
    }

    if (out_present_height != NULL) {
        *out_present_height = present_h;
    }
}

void ArmDisplay_Present(const uint32_t* argb_pixels, int w, int h) {
    switch (active_backend) {
    case DISPLAY_BACKEND_DRM:
        arm_display_drm_present(argb_pixels, w, h);
        return;

    case DISPLAY_BACKEND_FBDEV:
        arm_display_fbdev_present(argb_pixels, w, h);
        return;

    case DISPLAY_BACKEND_NONE:
        return;
    }
}

#endif // CRS_APP_DRIVER_ARM

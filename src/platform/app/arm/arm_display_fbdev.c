#if CRS_APP_DRIVER_ARM

#include "platform/app/arm/arm_display.h"
#include "platform/video/software/sw_blit.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

// Simple /dev/fb0 fallback.

#define FBDEV_PATH "/dev/fb0"

static int fb_fd = -1;
static uint8_t* fb_map = NULL;
static size_t fb_size = 0;
static int fb_width = 0;
static int fb_height = 0;
static int fb_pitch_px = 0;
static bool fb_nearest_present = true;

// Letterbox rect.
static int lb_x = 0;
static int lb_y = 0;
static int lb_w = 0;
static int lb_h = 0;

static void compute_letterbox() {
    ArmDisplay_ComputePresentRect(fb_width, fb_height, &lb_x, &lb_y, &lb_w, &lb_h);
}

static void clear_fb_black() {
    if (fb_map == NULL) {
        return;
    }

    // Clear to opaque black.
    uint32_t* row = (uint32_t*)fb_map;

    for (int y = 0; y < fb_height; y++) {
        for (int x = 0; x < fb_width; x++) {
            row[x] = 0xFF000000u;
        }

        row += fb_pitch_px;
    }
}

bool arm_display_fbdev_init() {
    fb_fd = open(FBDEV_PATH, O_RDWR | O_CLOEXEC);

    if (fb_fd < 0) {
        fprintf(stderr, "[arm_display_fbdev] open(%s) failed: %s\n", FBDEV_PATH, strerror(errno));
        return false;
    }

    struct fb_var_screeninfo var = { 0 };
    struct fb_fix_screeninfo fix = { 0 };

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &var) != 0 || ioctl(fb_fd, FBIOGET_FSCREENINFO, &fix) != 0) {
        fprintf(stderr, "[arm_display_fbdev] ioctl failed: %s\n", strerror(errno));
        close(fb_fd);
        fb_fd = -1;
        return false;
    }

    if (var.bits_per_pixel != 32) {
        fprintf(stderr, "[arm_display_fbdev] require 32bpp framebuffer, got %u\n", var.bits_per_pixel);
        close(fb_fd);
        fb_fd = -1;
        return false;
    }

    fb_width = (int)var.xres;
    fb_height = (int)var.yres;
    fb_pitch_px = (int)(fix.line_length / sizeof(uint32_t));
    fb_size = fix.line_length * (size_t)var.yres;

    fb_map = mmap(NULL, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);

    if (fb_map == MAP_FAILED) {
        fprintf(stderr, "[arm_display_fbdev] mmap failed: %s\n", strerror(errno));
        fb_map = NULL;
        close(fb_fd);
        fb_fd = -1;
        return false;
    }

    compute_letterbox();
    clear_fb_black();
    return true;
}

void arm_display_fbdev_shutdown() {
    if (fb_map != NULL) {
        munmap(fb_map, fb_size);
        fb_map = NULL;
    }

    if (fb_fd >= 0) {
        close(fb_fd);
        fb_fd = -1;
    }
}

void arm_display_fbdev_get_resolution(int* out_width, int* out_height) {
    if (out_width != NULL) {
        *out_width = fb_width;
    }

    if (out_height != NULL) {
        *out_height = fb_height;
    }
}

void arm_display_fbdev_present(const uint32_t* argb_pixels, int src_w, int src_h) {
    if (fb_map == NULL) {
        return;
    }

    // Only draw into the letterboxed area.
    uint32_t* dst = (uint32_t*)fb_map + (size_t)lb_y * fb_pitch_px + lb_x;
    sw_present_scale_argb(dst, fb_pitch_px, lb_w, lb_h, argb_pixels, src_w, src_w, src_h, fb_nearest_present);
}

#endif // CRS_APP_DRIVER_ARM

#if CRS_APP_DRIVER_ARM && CRS_ARM_HAVE_DRM

#include "platform/app/arm/arm_display.h"
#include "platform/video/software/sw_blit.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <poll.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <drm.h>
#include <drm_fourcc.h>
#include <drm_mode.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

// DRM dumb-buffer presenter with legacy page flips.

#define DRM_DEVICE_PRIMARY "/dev/dri/card1"
#define DRM_DEVICE_FALLBACK "/dev/dri/card0"
#define DRM_BUFFER_COUNT 2

typedef struct DumbBuffer {
    uint32_t handle;
    uint32_t pitch;
    uint64_t size;
    uint8_t* map;
    uint32_t fb_id;
} DumbBuffer;

static int drm_fd = -1;
static uint32_t drm_crtc_id = 0;
static uint32_t drm_connector_id = 0;
static drmModeModeInfo drm_mode = { 0 };
static drmModeCrtc* drm_saved_crtc = NULL;
static DumbBuffer drm_buffers[DRM_BUFFER_COUNT] = { 0 };
static int drm_front_index = 0;
static bool drm_flip_pending = false;
static bool drm_nearest_present = true;
static int drm_tty_fd = -1;
static int drm_tty_saved_mode = -1;
static uint32_t drm_present_count = 0;
static int drm_lb_x = 0;
static int drm_lb_y = 0;
static int drm_lb_w = 0;
static int drm_lb_h = 0;

static void destroy_dumb_buffer(DumbBuffer* buf);

// Put the active TTY into graphics mode so fbcon stays out of the way.
static void tty_enter_graphics() {
    const char* candidates[] = { "/dev/tty0", "/dev/tty", "/dev/console", NULL };

    for (int i = 0; candidates[i] != NULL; i++) {
        int fd = open(candidates[i], O_RDWR | O_CLOEXEC);

        if (fd < 0) {
            continue;
        }

        int mode = -1;

        if (ioctl(fd, KDGETMODE, &mode) == 0) {
            drm_tty_fd = fd;
            drm_tty_saved_mode = mode;

            if (ioctl(fd, KDSETMODE, KD_GRAPHICS) != 0) {
                fprintf(
                    stderr, "[arm_display_drm] KDSETMODE KD_GRAPHICS on %s failed: %s\n", candidates[i], strerror(errno)
                );
            }

            return;
        }

        close(fd);
    }

    fprintf(stderr, "[arm_display_drm] could not put any TTY into graphics mode - fbcon may overlay\n");
}

static void tty_leave_graphics() {
    if (drm_tty_fd < 0) {
        return;
    }

    if (drm_tty_saved_mode >= 0) {
        ioctl(drm_tty_fd, KDSETMODE, drm_tty_saved_mode);
    }

    close(drm_tty_fd);
    drm_tty_fd = -1;
    drm_tty_saved_mode = -1;
}

static int open_drm_device() {
    int fd = open(DRM_DEVICE_PRIMARY, O_RDWR | O_CLOEXEC);

    if (fd >= 0) {
        return fd;
    }

    fd = open(DRM_DEVICE_FALLBACK, O_RDWR | O_CLOEXEC);
    return fd;
}

static bool pick_connector_and_mode(drmModeRes* resources, drmModeConnector** out_connector) {
    for (int i = 0; i < resources->count_connectors; i++) {
        drmModeConnector* connector = drmModeGetConnector(drm_fd, resources->connectors[i]);

        if (connector == NULL) {
            continue;
        }

        if (connector->connection == DRM_MODE_CONNECTED && connector->count_modes > 0) {
            *out_connector = connector;
            return true;
        }

        drmModeFreeConnector(connector);
    }

    return false;
}

static drmModeModeInfo choose_connector_mode(const drmModeConnector* connector) {
    int best_index = 0;

    for (int i = 0; i < connector->count_modes; i++) {
        const drmModeModeInfo* mode = &connector->modes[i];
        const drmModeModeInfo* best = &connector->modes[best_index];
        const bool mode_is_1080p = (mode->hdisplay == 1920 && mode->vdisplay == 1080);
        const bool best_is_1080p = (best->hdisplay == 1920 && best->vdisplay == 1080);
        const bool mode_is_preferred = (mode->type & DRM_MODE_TYPE_PREFERRED) != 0;
        const bool best_is_preferred = (best->type & DRM_MODE_TYPE_PREFERRED) != 0;
        const int mode_area = (int)mode->hdisplay * (int)mode->vdisplay;
        const int best_area = (int)best->hdisplay * (int)best->vdisplay;

        // Prefer 1080p so we can pillarbox inside a 16:9 mode.
        if (mode_is_1080p != best_is_1080p) {
            if (mode_is_1080p) {
                best_index = i;
            }
            continue;
        }

        if (mode_is_preferred != best_is_preferred) {
            if (mode_is_preferred) {
                best_index = i;
            }
            continue;
        }

        if (mode_area > best_area) {
            best_index = i;
        }
    }

    return connector->modes[best_index];
}

static bool pick_crtc(drmModeRes* resources, drmModeConnector* connector) {
    // Reuse the current CRTC when possible.
    if (connector->encoder_id != 0) {
        drmModeEncoder* encoder = drmModeGetEncoder(drm_fd, connector->encoder_id);

        if (encoder != NULL) {
            if (encoder->crtc_id != 0) {
                drm_crtc_id = encoder->crtc_id;
                drmModeFreeEncoder(encoder);
                return true;
            }

            drmModeFreeEncoder(encoder);
        }
    }

    // Fall back to any compatible CRTC.
    for (int i = 0; i < connector->count_encoders; i++) {
        drmModeEncoder* encoder = drmModeGetEncoder(drm_fd, connector->encoders[i]);

        if (encoder == NULL) {
            continue;
        }

        for (int j = 0; j < resources->count_crtcs; j++) {
            if ((encoder->possible_crtcs & (1 << j)) != 0) {
                drm_crtc_id = resources->crtcs[j];
                drmModeFreeEncoder(encoder);
                return true;
            }
        }

        drmModeFreeEncoder(encoder);
    }

    return false;
}

static bool create_dumb_buffer(DumbBuffer* buf, uint32_t width, uint32_t height) {
    *buf = (DumbBuffer) { 0 };

    struct drm_mode_create_dumb create = { 0 };
    create.width = width;
    create.height = height;
    create.bpp = 32;

    if (drmIoctl(drm_fd, DRM_IOCTL_MODE_CREATE_DUMB, &create) < 0) {
        fprintf(stderr, "[arm_display_drm] CREATE_DUMB failed: %s\n", strerror(errno));
        return false;
    }

    buf->handle = create.handle;
    buf->pitch = create.pitch;
    buf->size = create.size;

    uint32_t handles[4] = { buf->handle, 0, 0, 0 };
    uint32_t pitches[4] = { buf->pitch, 0, 0, 0 };
    uint32_t offsets[4] = { 0, 0, 0, 0 };

    if (drmModeAddFB2(drm_fd, width, height, DRM_FORMAT_XRGB8888, handles, pitches, offsets, &buf->fb_id, 0) != 0) {
        fprintf(stderr, "[arm_display_drm] AddFB2 failed: %s\n", strerror(errno));
        destroy_dumb_buffer(buf);
        return false;
    }

    struct drm_mode_map_dumb map = { 0 };
    map.handle = buf->handle;

    if (drmIoctl(drm_fd, DRM_IOCTL_MODE_MAP_DUMB, &map) < 0) {
        fprintf(stderr, "[arm_display_drm] MAP_DUMB failed: %s\n", strerror(errno));
        destroy_dumb_buffer(buf);
        return false;
    }

    buf->map = mmap(NULL, buf->size, PROT_READ | PROT_WRITE, MAP_SHARED, drm_fd, map.offset);

    if (buf->map == MAP_FAILED) {
        fprintf(stderr, "[arm_display_drm] mmap failed: %s\n", strerror(errno));
        buf->map = NULL;
        destroy_dumb_buffer(buf);
        return false;
    }

    memset(buf->map, 0, buf->size);
    return true;
}

static void destroy_dumb_buffer(DumbBuffer* buf) {
    if (buf->map != NULL) {
        munmap(buf->map, buf->size);
        buf->map = NULL;
    }

    if (buf->fb_id != 0) {
        drmModeRmFB(drm_fd, buf->fb_id);
        buf->fb_id = 0;
    }

    if (buf->handle != 0) {
        struct drm_mode_destroy_dumb destroy = { 0 };
        destroy.handle = buf->handle;
        drmIoctl(drm_fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
        buf->handle = 0;
    }
}

// Keep DPMS on while the game is running.
static void set_dpms_on() {
    drmModeObjectPropertiesPtr props = drmModeObjectGetProperties(drm_fd, drm_connector_id, DRM_MODE_OBJECT_CONNECTOR);
    if (props == NULL) {
        return;
    }

    for (uint32_t i = 0; i < props->count_props; i++) {
        drmModePropertyPtr prop = drmModeGetProperty(drm_fd, props->props[i]);
        if (prop == NULL) {
            continue;
        }

        if (strcmp(prop->name, "DPMS") == 0) {
            drmModeConnectorSetProperty(drm_fd, drm_connector_id, prop->prop_id, DRM_MODE_DPMS_ON);
        }

        drmModeFreeProperty(prop);
    }

    drmModeFreeObjectProperties(props);
}

static bool set_current_mode_fb(uint32_t fb_id) {
    return drmModeSetCrtc(drm_fd, drm_crtc_id, fb_id, 0, 0, &drm_connector_id, 1, &drm_mode) == 0;
}

static void flip_handler(int fd, unsigned int frame, unsigned int sec, unsigned int usec, void* data) {
    (void)fd;
    (void)frame;
    (void)sec;
    (void)usec;
    bool* pending = (bool*)data;
    *pending = false;
}

static bool wait_for_flip() {
    drmEventContext ev = {
        .version = DRM_EVENT_CONTEXT_VERSION,
        .page_flip_handler = flip_handler,
    };

    while (drm_flip_pending) {
        struct pollfd pfd = { .fd = drm_fd, .events = POLLIN };
        const int pr = poll(&pfd, 1, 1000);

        if (pr < 0) {
            if (errno == EINTR) {
                continue;
            }

            fprintf(stderr, "[arm_display_drm] poll failed: %s\n", strerror(errno));
            drm_flip_pending = false;
            return false;
        }

        if (pr == 0) {
            fprintf(stderr, "[arm_display_drm] timed out waiting for page flip\n");
            drm_flip_pending = false;
            return false;
        }

        if ((pfd.revents & POLLIN) != 0) {
            if (drmHandleEvent(drm_fd, &ev) != 0) {
                fprintf(stderr, "[arm_display_drm] drmHandleEvent failed: %s\n", strerror(errno));
                drm_flip_pending = false;
                return false;
            }
            continue;
        }

        fprintf(stderr, "[arm_display_drm] unexpected poll revents: 0x%x\n", pfd.revents);
        drm_flip_pending = false;
        return false;
    }

    return true;
}

bool arm_display_drm_init() {
    drm_fd = open_drm_device();

    if (drm_fd < 0) {
        fprintf(stderr, "[arm_display_drm] could not open DRM device: %s\n", strerror(errno));
        return false;
    }

    // If we cannot become DRM master, bail and let fbdev try.
    if (drmSetMaster(drm_fd) != 0 && drmIsMaster(drm_fd) != 1) {
        fprintf(stderr, "[arm_display_drm] drmSetMaster failed: %s\n", strerror(errno));
        close(drm_fd);
        drm_fd = -1;
        return false;
    }

    uint64_t has_dumb = 0;
    if (drmGetCap(drm_fd, DRM_CAP_DUMB_BUFFER, &has_dumb) != 0 || has_dumb == 0) {
        fprintf(stderr, "[arm_display_drm] DRM dumb buffers are not supported\n");
        drmDropMaster(drm_fd);
        close(drm_fd);
        drm_fd = -1;
        return false;
    }

    drmModeRes* resources = drmModeGetResources(drm_fd);

    if (resources == NULL) {
        fprintf(stderr, "[arm_display_drm] drmModeGetResources failed: %s\n", strerror(errno));
        drmDropMaster(drm_fd);
        close(drm_fd);
        drm_fd = -1;
        return false;
    }

    drmModeConnector* connector = NULL;

    if (!pick_connector_and_mode(resources, &connector)) {
        fprintf(stderr, "[arm_display_drm] no connected connector found\n");
        drmModeFreeResources(resources);
        drmDropMaster(drm_fd);
        close(drm_fd);
        drm_fd = -1;
        return false;
    }

    drm_connector_id = connector->connector_id;
    drm_mode = choose_connector_mode(connector);

    if (!pick_crtc(resources, connector)) {
        fprintf(stderr, "[arm_display_drm] no CRTC for connector\n");
        drmModeFreeConnector(connector);
        drmModeFreeResources(resources);
        drmDropMaster(drm_fd);
        close(drm_fd);
        drm_fd = -1;
        return false;
    }

    drm_saved_crtc = drmModeGetCrtc(drm_fd, drm_crtc_id);
    tty_enter_graphics();

    for (int i = 0; i < DRM_BUFFER_COUNT; i++) {
        if (!create_dumb_buffer(&drm_buffers[i], drm_mode.hdisplay, drm_mode.vdisplay)) {
            for (int j = 0; j < i; j++) {
                destroy_dumb_buffer(&drm_buffers[j]);
            }

            drmModeFreeConnector(connector);
            drmModeFreeResources(resources);
            tty_leave_graphics();
            drmDropMaster(drm_fd);
            close(drm_fd);
            drm_fd = -1;
            return false;
        }
    }

    // Show buffer 0 before the first flip.
    if (!set_current_mode_fb(drm_buffers[0].fb_id)) {
        fprintf(stderr, "[arm_display_drm] drmModeSetCrtc failed: %s\n", strerror(errno));

        for (int i = 0; i < DRM_BUFFER_COUNT; i++) {
            destroy_dumb_buffer(&drm_buffers[i]);
        }

        drmModeFreeConnector(connector);
        drmModeFreeResources(resources);
        tty_leave_graphics();
        drmDropMaster(drm_fd);
        close(drm_fd);
        drm_fd = -1;
        return false;
    }

    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
    drm_front_index = 0;
    drm_flip_pending = false;
    ArmDisplay_ComputePresentRect(
        (int)drm_mode.hdisplay, (int)drm_mode.vdisplay, &drm_lb_x, &drm_lb_y, &drm_lb_w, &drm_lb_h
    );

    set_dpms_on();
    return true;
}

void arm_display_drm_shutdown() {
    if (drm_fd < 0) {
        return;
    }

    wait_for_flip();

    if (drm_saved_crtc != NULL) {
        drmModeSetCrtc(
            drm_fd,
            drm_saved_crtc->crtc_id,
            drm_saved_crtc->buffer_id,
            drm_saved_crtc->x,
            drm_saved_crtc->y,
            &drm_connector_id,
            1,
            &drm_saved_crtc->mode
        );
        drmModeFreeCrtc(drm_saved_crtc);
        drm_saved_crtc = NULL;
    }

    for (int i = 0; i < DRM_BUFFER_COUNT; i++) {
        destroy_dumb_buffer(&drm_buffers[i]);
    }

    drmDropMaster(drm_fd);
    close(drm_fd);
    drm_fd = -1;

    tty_leave_graphics();
}

void arm_display_drm_get_resolution(int* out_width, int* out_height) {
    if (out_width != NULL) {
        *out_width = (int)drm_mode.hdisplay;
    }

    if (out_height != NULL) {
        *out_height = (int)drm_mode.vdisplay;
    }
}

void arm_display_drm_present(const uint32_t* argb_pixels, int src_w, int src_h) {
    if (drm_fd < 0) {
        return;
    }

    if (!wait_for_flip()) {
        if (!set_current_mode_fb(drm_buffers[drm_front_index].fb_id)) {
            fprintf(
                stderr,
                "[arm_display_drm] failed to restore current scanout after flip wait failure: %s\n",
                strerror(errno)
            );
        }
    }

    // Refresh DPMS now and then so vc4 does not blank the screen.
    drm_present_count++;
    if (drm_present_count % 300 == 0) {
        set_dpms_on();
    }

    const int back_index = (drm_front_index + 1) % DRM_BUFFER_COUNT;
    DumbBuffer* back = &drm_buffers[back_index];
    uint32_t* dst = (uint32_t*)back->map;
    const int dst_pitch_px = (int)(back->pitch / sizeof(uint32_t));

    dst += (size_t)drm_lb_y * dst_pitch_px + drm_lb_x;
    sw_present_scale_argb(dst, dst_pitch_px, drm_lb_w, drm_lb_h, argb_pixels, src_w, src_w, src_h, drm_nearest_present);

    if (drmModePageFlip(drm_fd, drm_crtc_id, back->fb_id, DRM_MODE_PAGE_FLIP_EVENT, &drm_flip_pending) != 0) {
        // Last resort if page flip fails.
        fprintf(stderr, "[arm_display_drm] page flip failed: %s\n", strerror(errno));
        if (set_current_mode_fb(back->fb_id)) {
            drm_front_index = back_index;
        } else {
            fprintf(stderr, "[arm_display_drm] fallback drmModeSetCrtc failed: %s\n", strerror(errno));
        }
        drm_flip_pending = false;
    } else {
        drm_flip_pending = true;
        drm_front_index = back_index;
    }
}

#endif // CRS_APP_DRIVER_ARM && CRS_ARM_HAVE_DRM

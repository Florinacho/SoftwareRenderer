#ifndef __WL_WINDOW_H__
#define __WL_WINDOW_H__

#include <stdbool.h>
#include <wayland-client.h>
#include "fdeq.h"

#if defined(WL_XDG)
#include "xdg-shell.h"
#endif

// #define WL_VERBOSE

#define WL_MAX_FRAME_BUFFER 2

#define BTN_LEFT         0x0110
#define BTN_RIGHT        0x0111
#define BTN_MIDDLE       0x0112

#define LATCH_SHIFT      0x0001
#define LATCH_CAPS_LOCK  0x0002
#define LATCH_CONTROL    0x0004
#define LATCH_ALT        0x0008
#define LATCH_NUM_LOCK   0x0010

#define WL_EVENT_UNKNOWN 0x0000
#define WL_EVENT_CLOSE   0x0001
#define WL_EVENT_KEY     0x0002
#define WL_EVENT_CURSOR  0x0003
#define WL_EVENT_BUTTON  0x0004

#define WL_EVENT_COUNT 32

typedef struct {
	uint8_t type;
	uint32_t a, b;
} wl_event;

typedef fdeq(wl_event, WL_EVENT_COUNT) EventQueue;

typedef struct {
	EventQueue events;
	
	struct wl_display *display;
	struct wl_registry *registry;
	struct wl_compositor *compositor;
	struct wl_shm *shm;
	struct wl_seat *seat;
	struct wl_pointer *pointer;
	struct wl_keyboard *keyboard;
#if defined(WL_XDG)
	struct zxdg_shell_v6 *xdg_shell;
#else
	struct wl_shell *shell;
#endif
} wl_context;

typedef struct {
	struct wl_buffer *buffer;
	void *data;
	bool busy;
} wl_frame_buffer;

typedef struct {
	wl_context *context;
	struct wl_surface *surface;
	struct wl_callback *callback;
#if defined(WL_XDG)
	struct zxdg_surface_v6 *xdg_surface;
	struct zxdg_toplevel_v6 *xdg_toplevel;
#else
	struct wl_shell_surface *shell_surface;
#endif
	wl_frame_buffer frameBuffer[WL_MAX_FRAME_BUFFER];
	
	void* shm_data;
	uint32_t shm_size;

	int x, y;
	uint32_t width, height;	
	uint8_t bufferCount, activeBuffer;
	bool ready;
} wl_window;

/* ==================== Display ==================== */
int wl_connect(wl_context* context);

void wl_disconnect(wl_context* context);

/* ==================== Window ==================== */
int wl_window_create(wl_context* context, wl_window* window, int x, int y, uint32_t width, uint32_t height);

bool wl_window_get_event(wl_window* window, wl_event* event);

void* wl_window_get_active_buffer(wl_window* window);

void wl_window_swap_buffers(wl_window* window, bool wait_for_sync);

void wl_window_destroy(wl_window* window);

#endif // __WL_WINDOW_H__

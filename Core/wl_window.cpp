// https://gitlab.freedesktop.org/wayland
// https://bugaevc.gitbooks.io/writing-wayland-clients/content/black-square/the-complete-code.html
// https://wayland-client-d.dpldocs.info/wayland.client.protocol.wl_registry_set_user_data.html
// https://wayland.freedesktop.org/docs/html/apa.html
// https://github.com/4X3L/example-wayland/blob/master/src/main.c
// https://github.com/kovidgoyal/kitty/blob/1c72a94b2f2c652bc388e06ca21e67fe881e7dc2/glfw/wl_window.c
// https://github.com/krh/weston/blob/master/clients/simple-shm.c

#include "wl_window.h"

#include <syscall.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>

#define UNUSED(X)        ((void)(X))

#if defined(WL_VERBOSE)
#define WL_PRINT(...) printf(__VA_ARGS__)
#else
#define WL_PRINT(...)
#endif

/* ==================== Pointer ==================== */
void pointer_enter_handler(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t x, wl_fixed_t y) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(serial);
	UNUSED(surface);
	UNUSED(x);
	UNUSED(y);

	WL_PRINT("Pointer enter %f, %f\n", wl_fixed_to_double(x), wl_fixed_to_double(y));
}

void pointer_leave_handler(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(serial);
	UNUSED(surface);

	WL_PRINT("Pointer leave\n");
}

void pointer_motion_handler(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(time);
	UNUSED(x);
	UNUSED(y);

	WL_PRINT("Pointer motion %d, %d\n", wl_fixed_to_int(x), wl_fixed_to_int(y));
	
	wl_context* context = (wl_context*)data;

	const wl_event event = {
		.type = WL_EVENT_CURSOR,
		.a = wl_fixed_to_int(x),
		.b = wl_fixed_to_int(y),
	};
	fdeq_push_back(&context->events, event);
}

void pointer_button_handler(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(serial);
	UNUSED(time);
	UNUSED(button);
	UNUSED(state);

	WL_PRINT("Button: %d, %d\n", button, state);
	
	wl_context* context = (wl_context*)data;

	const wl_event event = {
		.type = WL_EVENT_BUTTON,
		.a = button,
		.b = state,
	};
	fdeq_push_back(&context->events, event);
}

void pointer_axis_handler(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(time);
	UNUSED(axis);
	UNUSED(value);

	WL_PRINT("Axis: %d, %f\n", axis, wl_fixed_to_double(value));
}

void pointer_frame_handler(void* data, struct wl_pointer* pointer) {
	UNUSED(data);
	UNUSED(pointer);
}

void pointer_axis_source_handler(void* data, struct wl_pointer* pointer, uint32_t axis_source) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(axis_source);
}

void pointer_axis_stop_handler(void* data, struct wl_pointer* pointer, uint32_t time, uint32_t axis) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(time);
	UNUSED(axis);
}

void pointer_axis_discrete_handler(void* data, struct wl_pointer* pointer, uint32_t axis, int discrete) {
	UNUSED(data);
	UNUSED(pointer);
	UNUSED(axis);
	UNUSED(discrete);
}

const struct wl_pointer_listener pointer_listener = {
	.enter = pointer_enter_handler,
	.leave = pointer_leave_handler,
	.motion = pointer_motion_handler,
	.button = pointer_button_handler,
	.axis = pointer_axis_handler,
	.frame = pointer_frame_handler,
	.axis_source = pointer_axis_source_handler,
	.axis_stop = pointer_axis_stop_handler,
	.axis_discrete = pointer_axis_discrete_handler,
};

/* ==================== Keyboard ==================== */
void keymap_handler(void* data, struct wl_keyboard* keyboard, uint32_t format, int fd, uint32_t size) {
	UNUSED(data);
	UNUSED(keyboard);
	UNUSED(format);
	UNUSED(fd);
	UNUSED(size);

	WL_PRINT("Key map format: %u, fd: %d, size: %u\n", format, fd, size);
}

void key_enter_handler(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface, struct wl_array* keys) {
	UNUSED(data);
	UNUSED(keyboard);
	UNUSED(serial);
	UNUSED(surface);
	UNUSED(keys);
}

void key_leave_handler(void* data, struct wl_keyboard* keyboard, uint32_t serial, struct wl_surface* surface) {
	UNUSED(data);
	UNUSED(keyboard);
	UNUSED(serial);
	UNUSED(surface);
}

void key_handler(void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
	UNUSED(data);
	UNUSED(keyboard);
	UNUSED(serial);
	UNUSED(time);
	UNUSED(key);
	UNUSED(state);

	WL_PRINT("Key 0x%.2X is %s\n", key, (state ? "pressed" : "released"));

	wl_context* context = (wl_context*)data;

	const wl_event event = {
		.type = WL_EVENT_KEY,
		.a = key,
		.b = state,
	};
	fdeq_push_back(&context->events, event);
}

void key_modifiers_handler(void* data, struct wl_keyboard* keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
	UNUSED(data);
	UNUSED(keyboard);
	UNUSED(serial);
	UNUSED(mods_depressed);
	UNUSED(mods_latched);
	UNUSED(mods_locked);
	UNUSED(group);

	WL_PRINT("Key modifier depressed: 0x%.2X, latched: 0x%.2X, locked: 0x%.2X, group: %u\n", mods_depressed, mods_latched, mods_locked, group);
}

void key_repeat_info_handler(void* data, struct wl_keyboard* keyboard, int rate, int delay) {
	UNUSED(data);
	UNUSED(keyboard);
	UNUSED(rate);
	UNUSED(delay);

	WL_PRINT("Key repeat:\n\trate: %d\n\tdelay: %d\n\n", rate, delay);
}

const struct wl_keyboard_listener keyboard_listener = {
	.keymap = keymap_handler,
	.enter = key_enter_handler,
	.leave = key_leave_handler,
	.key = key_handler,
	.modifiers = key_modifiers_handler,
	.repeat_info = key_repeat_info_handler,
};

/* ==================== Seat ==================== */
void seat_capabilities_handler(void *data, struct wl_seat *seat, uint32_t capabilities) {
	wl_context *context = (wl_context *)data;

	if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
		context->pointer = wl_seat_get_pointer(seat);
		wl_pointer_add_listener(context->pointer, &pointer_listener, context);
	}
	if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
		context->keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(context->keyboard, &keyboard_listener, context);
	}
	if (capabilities & WL_SEAT_CAPABILITY_TOUCH) {
//		context->touch = wl_seat_get_touch(seat);
//		wl_touch_add_listener(context->touch, &touch_listener, NULL);
	}
}

void seat_name_handler(void* data, struct wl_seat* seat, const char* name) {
	UNUSED(data);
	UNUSED(seat);
	UNUSED(name);
}

const struct wl_seat_listener seat_listener = {
	.capabilities = seat_capabilities_handler,
	.name = seat_name_handler,
};

#if defined(WL_XDG)
/* ==================== XDG extension ==================== */
void xdg_toplevel_configure_handler(void *data, struct zxdg_toplevel_v6 *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
	UNUSED(data);
	UNUSED(xdg_toplevel);
	UNUSED(width);
	UNUSED(height);
	UNUSED(states);

	WL_PRINT("XDG configure: %dx%d\n", width, height);
}

void xdg_toplevel_close_handler(void *data, struct zxdg_toplevel_v6 *xdg_toplevel) {
	UNUSED(data);
	UNUSED(xdg_toplevel);

	WL_PRINT("XDG close\n");
}

const struct zxdg_toplevel_v6_listener xdg_toplevel_listener = {
	.configure = xdg_toplevel_configure_handler,
	.close = xdg_toplevel_close_handler
};

void xdg_surface_configure_handler(void *data, struct zxdg_surface_v6 *xdg_surface, uint32_t serial) {
	UNUSED(data);
	UNUSED(xdg_surface);
	UNUSED(serial);

	zxdg_surface_v6_ack_configure(xdg_surface, serial);
}

const struct zxdg_surface_v6_listener xdg_surface_listener = {
	.configure = xdg_surface_configure_handler
};

void xdg_shell_ping_handler(void *data, struct zxdg_shell_v6 *xdg_shell, uint32_t serial) {
	UNUSED(data);

	zxdg_shell_v6_pong(xdg_shell, serial);
	WL_PRINT("XDG ping-pong\n");
}

const struct zxdg_shell_v6_listener xdg_shell_listener = {
	.ping = xdg_shell_ping_handler
};
#endif


/* ==================== Shell surface ==================== */
void shell_surface_ping_handler(void* data, struct wl_shell_surface* shell_surface, uint32_t serial) {
	UNUSED(data);
	UNUSED(shell_surface);
	UNUSED(serial);

	WL_PRINT("Shell surface ping\n");
	wl_shell_surface_pong(shell_surface, serial);
}

void shell_surface_configure_handler(void* data, struct wl_shell_surface* shell_surface, uint32_t edges, int width, int height) {
	UNUSED(data);
	UNUSED(shell_surface);
	UNUSED(edges);
	UNUSED(width);
	UNUSED(height);

	WL_PRINT("Shell surface configure: %u, %d, %d\n", edges, width, height);
}

void shell_surface_popup_done_handler(void* data, struct wl_shell_surface* shell_surface) {
	UNUSED(data);
	UNUSED(shell_surface);

	WL_PRINT("Shell surface popup done\n");
}

const struct wl_shell_surface_listener shell_surface_listener = {
	.ping = shell_surface_ping_handler,
	.configure = shell_surface_configure_handler,
	.popup_done = shell_surface_popup_done_handler
};

/* ==================== Frame ==================== */
void buffer_release_handler(void *data, struct wl_buffer *buffer) {
	UNUSED(buffer);

	bool *busy = (bool*)data;
	*busy = false;
}

const struct wl_buffer_listener buffer_listener = {
	.release = buffer_release_handler,
};

void callback_set_ready(void *data, struct wl_callback *callback, uint32_t time) {
	UNUSED(callback);
	UNUSED(time);

	bool *ready = (bool*)data;
	*ready = true;
}

const struct wl_callback_listener frame_limiter_listener = {
	.done = callback_set_ready
};

/* ==================== Registry ==================== */
void registry_global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
	UNUSED(version);

	wl_context* context = (wl_context*)data;

	WL_PRINT("Register %u. %s v%u\n", name, interface, version);

	if (strcmp(interface, "wl_compositor") == 0) {
		context->compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 3);
	} else 
	if (strcmp(interface, "wl_shm") == 0) {
		context->shm = (struct wl_shm*)wl_registry_bind(registry, name, &wl_shm_interface, 1);
	} else 
	if (strcmp(interface, "wl_seat") == 0) {
		context->seat = (struct wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 1);
		wl_seat_add_listener(context->seat, &seat_listener, context);
	} else 
#if defined(WL_XDG)
	if (strcmp(interface, "zxdg_shell_v6") == 0) {
		context->xdg_shell = (struct zxdg_shell_v6*)wl_registry_bind(registry, name, &zxdg_shell_v6_interface, 1);
	} else
#else
	if (strcmp(interface, "wl_shell") == 0) {
		context->shell = (struct wl_shell*)wl_registry_bind(registry, name, &wl_shell_interface, 1);
	} else
#endif
	{
		// unknown
	}
}

void registry_global_remove_handler(void *data, struct wl_registry *registry, uint32_t name) {
	UNUSED(data);
	UNUSED(registry);
	UNUSED(name);

	WL_PRINT("Unregister %u\n", name);
}

const struct wl_registry_listener registry_listener = {
	.global = registry_global_handler,
	.global_remove = registry_global_remove_handler
};

/* ==================== Context ==================== */
int wl_connect(wl_context* context) {
	fdeq_init(&context->events);

	context->display = wl_display_connect(NULL);
	context->registry = wl_display_get_registry(context->display);
	
	wl_registry_add_listener(context->registry, &registry_listener, context);
	wl_display_roundtrip(context->display);
	
	return 0;
}

void wl_disconnect(wl_context* context) {
	wl_keyboard_destroy(context->keyboard);
	wl_pointer_destroy(context->pointer);
	wl_seat_destroy(context->seat);
	wl_shell_destroy(context->shell);
	wl_shm_destroy(context->shm);
	wl_compositor_destroy(context->compositor);
	wl_registry_destroy(context->registry);
	wl_display_disconnect(context->display);
}

/* ==================== Window ==================== */
int wl_window_create(wl_context* context, wl_window* window, int x, int y, uint32_t width, uint32_t height) {
	window->context = context;
	window->surface = NULL;
	window->callback = NULL;
	window->x = x;
	window->y = y;
	window->width = width;
	window->height = height;
	window->activeBuffer = 0;
	window->bufferCount = 2;
	window->ready = false;

	window->surface = wl_compositor_create_surface(context->compositor);
	if (window->surface == NULL) {
		return 1;
	}

#if defined(WL_XDG)
	window->xdg_surface = zxdg_shell_v6_get_xdg_surface(context->xdg_shell, window->surface);
	zxdg_surface_v6_add_listener(window->xdg_surface, &xdg_surface_listener, NULL);
	
	window->xdg_toplevel = zxdg_surface_v6_get_toplevel(window->xdg_surface);
	zxdg_toplevel_v6_add_listener(window->xdg_toplevel, &xdg_toplevel_listener, NULL);

	wl_display_roundtrip(display);
	zxdg_shell_v6_add_listener(context->xdg_shell, &xdg_shell_listener, NULL);
#else
	window->shell_surface = wl_shell_get_shell_surface(context->shell, window->surface);
	wl_shell_surface_set_toplevel(window->shell_surface);
	wl_shell_surface_add_listener(window->shell_surface, &shell_surface_listener, window);
#endif
	uint32_t stride = width * 4;
	uint32_t bufferSize = stride * height;
	uint32_t totalSize = bufferSize * window->bufferCount;

	// Open an anonymous file and write some zero bytes to it
	int fd = syscall(SYS_memfd_create, "frameBuffer", 0);
	if (ftruncate(fd, totalSize) != 0) {
		return 2;
	}

	// Map it to the memory
	window->shm_size = totalSize;
	window->shm_data = mmap(NULL, window->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	// turn it into a shared memory pool
	struct wl_shm_pool *pool = wl_shm_create_pool(context->shm, fd, window->shm_size);
	close(fd);

	// allocate the buffer in that pool
	for (uint32_t index = 0; index < window->bufferCount; ++index) {
		window->frameBuffer[index].buffer = wl_shm_pool_create_buffer(pool, index * bufferSize, width, height, stride, WL_SHM_FORMAT_XRGB8888);
		window->frameBuffer[index].data = (uint8_t*)window->shm_data + index * bufferSize;
		window->frameBuffer[index].busy = false;

		wl_buffer_add_listener(window->frameBuffer[index].buffer, &buffer_listener, &window->frameBuffer[index].busy);
	}

	wl_shm_pool_destroy(pool);
	
	window->ready = true;
	
	return 0;
}

bool wl_window_get_event(wl_window* window, wl_event* event) {
	UNUSED(window);
	UNUSED(event);
	
	wl_context* context = window->context;

	return fdeq_pop_front(&context->events, event);
}

void* wl_window_get_active_buffer(wl_window* window) {
	const uint32_t size = window->width * window->height * 4;
	return (void*)((uint8_t*)window->shm_data + window->activeBuffer * size);
}

void wl_window_swap_buffers(wl_window* window, bool wait_for_sync) {
	if (window->callback != NULL) {
		wl_callback_destroy(window->callback);
	}
	window->callback = wl_surface_frame(window->surface);
	wl_callback_add_listener(window->callback, &frame_limiter_listener, &window->ready);

	wl_surface_attach(window->surface, window->frameBuffer[window->activeBuffer].buffer, 0, 0);
	wl_surface_damage(window->surface, 0, 0, window->width, window->height);
	wl_surface_commit(window->surface);

	window->activeBuffer = (window->activeBuffer + 1) % window->bufferCount;

	do {
		wl_display_dispatch(window->context->display);
	} while (wait_for_sync && ((window->ready == false) || window->frameBuffer[window->activeBuffer].busy));
	
	window->ready = false;
	window->frameBuffer[window->activeBuffer].busy = true;
}

void wl_window_destroy(wl_window* window) {
	if (window->callback != NULL) {
		wl_callback_destroy(window->callback);
	}

	for (uint32_t index = 0; index < window->bufferCount; ++index) {
		wl_buffer_destroy(window->frameBuffer[index].buffer);
	}

	munmap(window->shm_data, window->shm_size);
#if defined(WL_XDG)
	zxdg_surface_v6_destroy(window->xdg_surface);
#else
	wl_shell_surface_destroy(window->shell_surface);
#endif
	wl_surface_destroy(window->surface);
}

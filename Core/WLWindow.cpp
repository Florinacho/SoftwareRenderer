#if defined(__linux__)
#include "WLWindow.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

bool WLWindow::initialize(const uvec2& newSize, int newPixelFormat, const char* newTitle) {
	size = newSize;
	pixelFormat = newPixelFormat;
	
	if (newTitle != NULL) {
		title = newTitle;
	}

	if (wl_connect(&context) != 0) {
		printf("Error: cannot connect to display server\n");
		return false;
	}
	
	if (wl_window_create(&context, &window, 0, 0, newSize.x, newSize.y) != 0) {
		printf("Error: cannot create window\n");
		return false;
	}
	
	return true;
}
void WLWindow::uninitialize() {
	wl_window_destroy(&window);
	wl_disconnect(&context);
	data = NULL;
}

WLWindow::WLWindow() {
	context = {0};
	window = {0};
	size.x = 1;
	size.y = 1;
	visible = false;
	decorations = true;
	resizable = true;
	fullscreen = false;
	title = "Untitled";
}

WLWindow::~WLWindow() {
	uninitialize();
}

void WLWindow::setVisible(bool value) {
	if (visible == value) {
		return;
	}

	// TODO

	visible = value;
}

bool WLWindow::getVisible() const {
	return visible;
}

void WLWindow::setDecorations(bool value) {
	if (decorations == decorations) {
		return;
	}

	// TODO

	decorations = value;
}

bool WLWindow::getDecorations() const {
	return decorations;
}

void WLWindow::setResizable(bool value) {
	if (resizable == value) {
		return;
	}

	// TODO

	resizable = value;
}

bool WLWindow::getResizable() const {
	return resizable;
}

void WLWindow::setFullscreen(bool value) {
	if (fullscreen == value) {
		return;
	}

	// TODO

	fullscreen = value;
}

bool WLWindow::getFullscreen() const {
	return fullscreen;
}

void WLWindow::setSize(const Vector2u& value) {
	if (size == value) {
		return;
	}
	
	// TODO

	size = value;
}

void WLWindow::setTitle(const char* value) {
	if (title == value) {
		return;
	}

	// TODO

	title = value;
}

const char* WLWindow::getTitle() const {
	return title.c_str();
}

void WLWindow::setMousePosition(const Vector2i& value) {
	if (mousePosition == value) {
		return;
	}

	// TODO

	mousePosition = value;
}

bool WLWindow::getEvent(Event* event) {
	wl_event wlEvent;

	if (wl_window_get_event(&window, &wlEvent)) {
		switch (wlEvent.type) {
		case WL_EVENT_CURSOR :
			event->type = Event::MOUSE_POSITION;
			event->x = wlEvent.a;
			event->y = wlEvent.b;
			break;
		case WL_EVENT_BUTTON :
			event->type = Event::MOUSE_BUTTON;
			event->button = wlEvent.a;
			event->state = wlEvent.b;
			break;
		case WL_EVENT_KEY :
			event->type = Event::KEYBOARD;
			event->key = wlEvent.a;
			event->state = wlEvent.b;
			break;
		}
		return true;
	}
	
	return false;
}

wl_context* WLWindow::getNativeDisplay() {
	return &context;
}

wl_window* WLWindow::getNativeHandle() {
	return &window;
}

void WLWindow::blit(const Image* image) {
	if (image->getSize() != getSize()) {
		return;
	}
	
	if (image->getPixelFormat() != getPixelFormat()) {
		return;
	}
		
//	memcpy(data, image->getData(), getDataLength());
//	BitBlt(_display, 0, 0, size.x, size.y, hDCMem, 0, 0, SRCCOPY);
	data = (uint8_t*)wl_window_get_active_buffer(&window);
	memcpy(data, image->getData(), getDataLength());
	wl_window_swap_buffers(&window, false);
}
#endif // __linux__

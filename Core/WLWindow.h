#ifndef __WAYLAND_WINDOW_H__
#define __WAYLAND_WINDOW_H__

#include "wl_window.h"

#include <string>

#include "Event.h"
#include "Image.h"

class WLWindow : public Image {
	bool visible;
	bool decorations;
	bool resizable;
	bool fullscreen;

	//Vector2u _size;
	Vector2i mousePosition;

	std::string title;
	
	wl_context context;
	wl_window window;

public:	
	WLWindow();

	~WLWindow();

	bool initialize(const uvec2& size, int pixelFormat, const char* newTitle);

	void uninitialize();

	void setVisible(bool visible);

	bool getVisible() const;

	void setDecorations(bool decorations);

	bool getDecorations() const;
	
	void setResizable(bool resizable);

	bool getResizable() const;

	void setFullscreen(bool fullscreen);

	bool getFullscreen() const;

	void setSize(const Vector2u& size);
	
	void setTitle(const char* title);

	const char* getTitle() const;
	
	bool getEvent(Event* event);
	
	void setMousePosition(const Vector2i& position);

	wl_context* getNativeDisplay();

	wl_window* getNativeHandle();
	
	void blit(const Image* image);
};

#endif // __WAYLAND_WINDOW_H__

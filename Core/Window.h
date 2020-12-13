#ifndef __WIN32_WINDOW_H__
#define __WIN32_WINDOW_H__

#define WIN32_LEAN_AND_MEAN
#if defined (_WIN32_WINNT) && _WIN32_WINNT < 0x501
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x501
#endif

#if not defined (MAPVK_VSC_TO_VK_EX)
	#define MAPVK_VSC_TO_VK_EX
#endif 

#include <windows.h>

#include <string>

#include "Event.h"
#include "Image.h"

class Win32Window : public Image {
	bool visible;
	bool decorations;
	bool resizable;
	bool fullscreen;

	//Vector2u _size;
	Vector2i mousePosition;

	std::string title;

	friend LRESULT CALLBACK Win32WindowProcess(HWND, UINT, WPARAM, LPARAM);
	HDC _display;
	HWND _window;
	Vector2u borderSize;
	Event* pEvent;
	
	HDC hDCMem;
	HBITMAP bitmap;
	HGDIOBJ oldBitmap;

public:	
	Win32Window();

	~Win32Window();

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

	HDC getNativeDisplay() const;

	HWND getNativeHandle() const;
	
	void blit(const Image* image);
};

#endif // __WIN32_WINDOW_H__
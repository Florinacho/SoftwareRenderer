#include "Window.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

Vector2u AdjustWindowVect2i(const Vector2u& vect, const long dwStyle) {
	RECT rectangle;
	rectangle.top = 0;
	rectangle.left = 0;
	rectangle.right = vect.x;
	rectangle.bottom = vect.y;
	AdjustWindowRect(&rectangle, dwStyle, FALSE);
	
	if (((dwStyle & WS_THICKFRAME) == false) && ((dwStyle & WS_CAPTION) == false)) {
		return vect;
	}

	return Vector2u(rectangle.right - rectangle.left, rectangle.bottom - rectangle.top);
}

WPARAM MapLeftRightKeys( WPARAM vk, LPARAM lParam) {
    WPARAM new_vk = vk;
    UINT scancode = (lParam & 0x00ff0000) >> 16;
    int extended  = (lParam & 0x01000000) != 0;
//	const unsigned int MAPVK_VSC_TO_VK_EX = 3;
    switch (vk) {
    case VK_SHIFT:
        new_vk = MapVirtualKey(scancode, 3);//MAPVK_VSC_TO_VK_EX
        break;
    case VK_CONTROL:
        new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
        break;
    case VK_MENU:
        new_vk = extended ? VK_RMENU : VK_LMENU;
        break;
    default:
        // not a key we map from generic to left/right specialized
        //  just return it.
        new_vk = vk;
        break;    
    }

    return new_vk;
}

LRESULT CALLBACK Win32WindowProcess(HWND hwindow, UINT message, WPARAM wParam, LPARAM lParam) {
	Win32Window* window = (Win32Window*)GetWindowLong(hwindow, GWL_USERDATA);
	
	if ((window == NULL) || (window->pEvent == NULL)) {
		return DefWindowProc(hwindow, message, wParam, lParam);
	}

	switch (message) {
	case WM_SYSKEYDOWN :
	case WM_KEYDOWN :
		window->pEvent->type = Event::KEY_DOWN;
		window->pEvent->key = MapLeftRightKeys(wParam, lParam);
		window->pEvent->value = 1;
		return 0;
		
	case WM_SYSKEYUP :
	case WM_KEYUP :
		window->pEvent->type = Event::KEY_UP;
		window->pEvent->key = MapLeftRightKeys(wParam, lParam);
		window->pEvent->value = 0;
		return 0;

	case WM_MOUSEMOVE :
		window->pEvent->type = Event::MOUSE_MOVE;
		window->mousePosition.x = LOWORD(lParam);
		window->mousePosition.y = HIWORD(lParam);
		window->pEvent->x = window->mousePosition.x;
		window->pEvent->y = window->mousePosition.y;
		return 0;

	case WM_LBUTTONDOWN :
		window->pEvent->type = Event::BUTTON_DOWN;
		window->pEvent->button = LEFT_BUTTON;
		return 0;
		
	case WM_LBUTTONUP :
		window->pEvent->type = Event::BUTTON_UP;
		window->pEvent->button = LEFT_BUTTON;
		return 0;
		
	case WM_MBUTTONDOWN :
		window->pEvent->type = Event::BUTTON_DOWN;
		window->pEvent->button = MIDDLE_BUTTON;
		return 0;
		
	case WM_MBUTTONUP :
		window->pEvent->type = Event::BUTTON_UP;
		window->pEvent->button = MIDDLE_BUTTON;
		return 0;
		
	case WM_RBUTTONDOWN : 
		window->pEvent->type = Event::BUTTON_DOWN;
		window->pEvent->button = RIGHT_BUTTON;
		return 0;
		
	case WM_RBUTTONUP :
		window->pEvent->type = Event::BUTTON_UP;
		window->pEvent->button = RIGHT_BUTTON;
		return 0;
		
	case WM_MOUSEWHEEL :
		window->pEvent->type = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? Event::WHEEL_UP : Event::WHEEL_DOWN;
		return 0;
		
	case WM_SIZE :
		window->pEvent->type = Event::WINDOW_SIZE;
		window->size.x = LOWORD(lParam);
		window->size.y = HIWORD(lParam);
		window->pEvent->width = window->size.x;
		window->pEvent->height = window->size.y;
		return 0;
		
	case WM_CLOSE :
		window->pEvent->type = Event::WINDOW_CLOSE;
		return 0;
	}
	
	return DefWindowProc(hwindow, message, wParam, lParam);
}

bool Win32Window::initialize(const uvec2& newSize, int newPixelFormat, const char* newTitle) {
	size = newSize;
	pixelFormat = newPixelFormat;
	
	if (newTitle != NULL) {
		title = newTitle;
	}

	HINSTANCE instance = GetModuleHandle(NULL);
	if (instance == NULL) {
		printf("Win32 ERROR: Unable to get the module handle.\n");
		return false;
	}
	
	static const char* className = "Win32WindowClass";
	
	WNDCLASS windowClass;
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = Win32WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;
	RegisterClass(&windowClass);
	
	const long dwStyle = WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX | WS_VISIBLE;
	Vector2u wsize = AdjustWindowVect2i(size, dwStyle);

	_window = CreateWindow(
		className, 
		title.c_str(), 
		dwStyle,
		0, 0, 
		wsize.x, wsize.y,
		NULL, 
		NULL, 
		instance, 
		(LPVOID)this);
	SetWindowLong(_window, GWL_USERDATA, (LONG_PTR)this);

	if (!_window) {
		printf("CWINWindow: Error! Unable to create window.\n");
		return false;
	}
	
	_display = GetDC(_window);
	if (_display == 0) {
		printf("Win32 ERROR: Unable to get display handle.\n");
		return false;
	}
	hDCMem = CreateCompatibleDC(_display);
	
	BITMAPINFO bitmapinfo; 
	ZeroMemory(&bitmapinfo, sizeof(BITMAPINFO));
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biWidth = size.x;
	bitmapinfo.bmiHeader.biHeight = -size.y;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biBitCount = getPixelSize() * 8;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biSizeImage = 0;
	bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
	bitmapinfo.bmiHeader.biClrUsed = 0;
	bitmapinfo.bmiHeader.biClrImportant = 0;
	
	bitmap = ::CreateDIBSection(hDCMem, &bitmapinfo, DIB_RGB_COLORS, (VOID**)&data, NULL, 0);
	oldBitmap = ::SelectObject(hDCMem, bitmap); 
	
	return true;
}
void Win32Window::uninitialize() {
	SelectObject(hDCMem, oldBitmap);  
	oldBitmap = 0;
	
    DeleteDC(hDCMem);  
	hDCMem = 0;
	
    DeleteObject(bitmap); // <-- very slow
	bitmap = 0;
	
	if (_window != 0) {
		DestroyWindow(_window);
		_window = 0;
	}
}

Win32Window::Win32Window() {
	_display = NULL;
	_window = 0;
	hDCMem = 0;
	oldBitmap = 0;
	bitmap = 0;

	size.x = 1;
	size.y = 1;
	visible = false;
	decorations = true;
	resizable = true;
	fullscreen = false;
	title = "Untitled";
	pEvent = NULL;
}

Win32Window::~Win32Window() {
	uninitialize();
}

void Win32Window::setVisible(bool value) {
	if (visible == value) {
		return;
	}

	if (value) {
		ShowWindow(_window, SW_SHOW);
	} else {
		ShowWindow(_window, SW_HIDE);
	}

	visible = value;
}

bool Win32Window::getVisible() const {
	return visible;
}

void Win32Window::setDecorations(bool value) {
	if (decorations == decorations) {
		return;
	}

	long dwStyle = GetWindowLong(_window, GWL_STYLE);
	if (value == true) {
		dwStyle |= WS_CAPTION;
	} else {
		dwStyle &= ~WS_CAPTION;
//		dwStyle &= WS_MINIMIZEBOX;
//		dwStyle &= WS_MAXIMIZEBOX;
	}
	SetWindowLong(_window, GWL_STYLE, dwStyle);

	decorations = value;
}

bool Win32Window::getDecorations() const {
	return decorations;
}

void Win32Window::setResizable(bool value) {
	if (resizable == value) {
		return;
	}

	long dwStyle = GetWindowLong(_window, GWL_STYLE);
	if (value == true) {
		dwStyle |= WS_SIZEBOX;
	} else {
		dwStyle &= ~WS_SIZEBOX;
	}
	SetWindowLong(_window, GWL_STYLE, dwStyle);

	resizable = value;
}

bool Win32Window::getResizable() const {
	return resizable;
}

void Win32Window::setFullscreen(bool value) {
	if (fullscreen == value) {
		return;
	}

	if (value) {
		DEVMODE fullscreenSettings;

		EnumDisplaySettings(NULL, 0, &fullscreenSettings);
		fullscreenSettings.dmPelsWidth = size.x;
		fullscreenSettings.dmPelsHeight = size.y;
		fullscreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
//		fullscreenSettings.dmBitsPerPel = colourBits;
//		fullscreenSettings.dmDisplayFrequency = refreshRate;
//		fullscreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;

		if (ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL) {
			SetWindowLongPtr(_window, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtr(_window, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(_window, HWND_TOPMOST, 0, 0, size.x, size.y, SWP_SHOWWINDOW);
			ShowWindow(_window, SW_MAXIMIZE);
		} else {
			return;
		}
	} else {
		const Vector2i position(0, 0);
		const Vector2i padding(10, 20);
		if (ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL) {
			SetWindowLongPtr(_window, GWL_EXSTYLE, WS_EX_LEFT);
			SetWindowLongPtr(_window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			SetWindowPos(_window, HWND_NOTOPMOST, position.x, position.y, size.x + padding.x, size.y + padding.y, SWP_SHOWWINDOW);
			ShowWindow(_window, SW_RESTORE);
		} else {
			return;
		}
	}

	fullscreen = value;
}

bool Win32Window::getFullscreen() const {
	return fullscreen;
}

void Win32Window::setSize(const Vector2u& value) {
	if (size == value) {
		return;
	}
	
	const long dwStyle = GetWindowLong(_window, GWL_STYLE);
	const Vector2u csize = AdjustWindowVect2i(value, dwStyle);
	SetWindowPos(_window, HWND_TOP, 0, 0, csize.x, csize.y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	size = value;
}

void Win32Window::setTitle(const char* value) {
	if (title == value) {
		return;
	}

	SetWindowText(_window, value);

	title = value;
}

const char* Win32Window::getTitle() const {
	return title.c_str();
}

void Win32Window::setMousePosition(const Vector2i& value) {
	if (mousePosition == value) {
		return;
	}

	POINT point;
	point.x = value.x;
	point.y = value.y;
	ClientToScreen(_window, &point);
	SetCursorPos(point.x, point.y);

	mousePosition = value;
}

bool Win32Window::getEvent(Event* output) {
	pEvent = output;
	if (pEvent) {
		pEvent->type = Event::UNKNOWN;
	}
	
	MSG message;
	if (PeekMessage(&message, _window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	pEvent = NULL;
	
	return ((output != NULL) && (output->type != Event::UNKNOWN));
}

HDC Win32Window::getNativeDisplay() const {
	return _display;
}

HWND Win32Window::getNativeHandle() const {
	return _window;
}

void Win32Window::blit(const Image* image) {
	if (image->getSize() != getSize()) {
		return;
	}
	
	if (image->getPixelFormat() != getPixelFormat()) {
		return;
	}
		
	memcpy(data, image->getData(), getDataLength());
	BitBlt(_display, 0, 0, size.x, size.y, hDCMem, 0, 0, SRCCOPY);
}
#ifndef __OS_H__
#define __OS_H__

#if defined( __linux__ )
	#define OS_LINUX
	#define OS_LINUX_X11
#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	#define OS_WINDOWS
	#define OS_WINDOWS_GDI
#else
	#error "Unknown platform"
#endif

#if defined (OS_LINUX_X11) 
	#define WM_HINTS_DECORATIONS		(1L << 1)

	#include <X11/Xlib.h>
	#include <X11/Xatom.h>
	#include <X11/extensions/Xrandr.h>
	#include <X11/keysym.h>

	struct MotifHints {
		unsigned long flags;
		unsigned long functions;
		unsigned long decorations;
		long input_mode;
		unsigned long status;
	};

	static const int _NET_WM_STATE_REMOVE = 0;
	static const int _NET_WM_STATE_ADD    = 1;
	static const int _NET_WM_STATE_TOGGLE = 2;
#elif defined (OS_WINDOWS_GDI)
	#define WIN32_LEAN_AND_MEAN
	#if defined (_WIN32_WINNT) && _WIN32_WINNT < 0x501
		#undef _WIN32_WINNT
		#define _WIN32_WINNT 0x501
	#endif
	
	#include <windows.h>
#endif

#endif //__OS_H__

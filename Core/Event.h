#ifndef __EVENT_H__
#define __EVENT_H__

#include "KeyCodes.h"

/*
#undef KEY_ESCAPE
#undef KEY_1
#undef KEY_2
#undef KEY_3
#undef KEY_4
#undef KEY_5
#undef KEY_6
#undef KEY_7
#undef KEY_8
#undef KEY_9
#undef KEY_0
#undef KEY_MINUS
#undef KEY_PLUS
#undef KEY_BACKSPACE
#undef KEY_TAB
#undef KEY_Q
#undef KEY_W
#undef KEY_E
#undef KEY_R
#undef KEY_T
#undef KEY_Y
#undef KEY_U
#undef KEY_I
#undef KEY_O
#undef KEY_P
#undef KEY_LEFT_BRACKET
#undef KEY_RIGHT_BRACKET
#undef KEY_RETURN
#undef KEY_LEFT_CONTROL
#undef KEY_A
#undef KEY_S
#undef KEY_D
#undef KEY_F
#undef KEY_G
#undef KEY_H
#undef KEY_J
#undef KEY_K
#undef KEY_L
#undef KEY_COLON
#undef KEY_QUOTE
#undef KEY_TILDE
#undef KEY_LEFT_SHIFT
#undef KEY_SLASH
#undef KEY_Z
#undef KEY_X
#undef KEY_C
#undef KEY_V
#undef KEY_B
#undef KEY_N
#undef KEY_M
#undef KEY_COMMA
#undef KEY_PERIOD
#undef KEY_BACKSLASH
#undef KEY_RIGHT_SHIFT
#undef KEY_MULTIPLY
#undef KEY_LEFT_ALT
#undef KEY_SPACE
#undef KEY_CAPS_LOCK
#undef KEY_F1
#undef KEY_F2
#undef KEY_F3
#undef KEY_F4
#undef KEY_F5
#undef KEY_F6
#undef KEY_F7
#undef KEY_F8
#undef KEY_F9
#undef KEY_F10
#undef KEY_NUM_LOCK
#undef KEY_SCROLL_LOCK
#undef KEY_NUM_7
#undef KEY_NUM_8
#undef KEY_NUM_9
#undef KEY_SUBTRACT
#undef KEY_NUM_4
#undef KEY_NUM_5
#undef KEY_NUM_6
#undef KEY_ADD
#undef KEY_NUM_1
#undef KEY_NUM_2
#undef KEY_NUM_3
#undef KEY_NUM_0
#undef KEY_NUM_DELETE
#undef KEY_F11 
#undef KEY_F12
#undef KEY_ENTER
#undef KEY_DIVIDE
#undef KEY_RIGHT_ALT
#undef KEY_HOME
#undef KEY_UP
#undef KEY_PAGE_UP
#undef KEY_LEFT
#undef KEY_RIGHT
#undef KEY_END
#undef KEY_DOWN
#undef KEY_PAGE_DOWN
#undef KEY_INSERT
#undef KEY_DELETE
#undef KEY_PAUSE
#undef KEY_LEFT_WINDOWS
#undef KEY_RIGHT_WINDOWS
#undef KEY_APPS
#undef KEY_RIGHT_CONTROL
*/
struct Event {
	enum Type {
		UNKNOWN,

		KEYBOARD,
		MOUSE_POSITION,
		MOUSE_BUTTON,
		MOUSE_WHEEL,
	
		WINDOW_SIZE,
		WINDOW_CLOSE
	};

	int type;
	union {
		int x;
		unsigned int width;
		KeyCode key;
		int button;
	};
	union {
		int y;
		unsigned int height;
		int value;
		int state;
	};

	Event(int eventType = 0, int lvalue = 0, int rvalue = 0)
		: type(eventType), x(lvalue), y(rvalue) { }
};

#endif //__EVENT_H__

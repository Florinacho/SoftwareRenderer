#ifndef __EVENT_H__
#define __EVENT_H__

#include "KeyCodes.h"

struct Event {
	enum Type {
		UNKNOWN,
		
		KEY_DOWN,
		KEY_UP,
		
		MOUSE_MOVE,
		BUTTON_DOWN,
		BUTTON_UP,
		WHEEL_DOWN,
		WHEEL_UP,
		
		WINDOW_SIZE,
		WINDOW_CLOSE,

		EVENT_COUNT
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

	Event(int eventType = UNKNOWN, int lvalue = 0, int rvalue = 0)
		: type(eventType), x(lvalue), y(rvalue) { }
};

#endif //__EVENT_H__

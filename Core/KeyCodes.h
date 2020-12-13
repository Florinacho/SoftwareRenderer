#ifndef __KEY_CODES_H__
#define __KEY_CODES_H__

typedef unsigned char KeyCode;
static const KeyCode KEY_UNDEFINED = 0x0;

#if defined (__linux__)
static const KeyCode KEY_ESCAPE = 0x09;
static const KeyCode KEY_1 = 0x0A;
static const KeyCode KEY_2 = 0x0B;
static const KeyCode KEY_3 = 0x0C;
static const KeyCode KEY_4 = 0x0D;
static const KeyCode KEY_5 = 0x0E;
static const KeyCode KEY_6 = 0x0F;
static const KeyCode KEY_7 = 0x10;
static const KeyCode KEY_8 = 0x11;
static const KeyCode KEY_9 = 0x12;
static const KeyCode KEY_0 = 0x13;
static const KeyCode KEY_MINUS = 0x14;
static const KeyCode KEY_PLUS = 0x15;
static const KeyCode KEY_BACKSPACE = 0x16;
static const KeyCode KEY_TAB = 0x17;
static const KeyCode KEY_Q = 0x18;
static const KeyCode KEY_W = 0x19;
static const KeyCode KEY_E = 0x1A;
static const KeyCode KEY_R = 0x1B;
static const KeyCode KEY_T = 0x1C;
static const KeyCode KEY_Y = 0x1D;
static const KeyCode KEY_U = 0x1E;
static const KeyCode KEY_I = 0x1F;
static const KeyCode KEY_O = 0x20;
static const KeyCode KEY_P = 0x21;
static const KeyCode KEY_LEFT_BRACKET = 0x22;
static const KeyCode KEY_RIGHT_BRACKET = 0x23;
static const KeyCode KEY_RETURN = 0x24;
static const KeyCode KEY_LEFT_CONTROL = 0x25;
static const KeyCode KEY_A = 0x26;
static const KeyCode KEY_S = 0x27;
static const KeyCode KEY_D = 0x28;
static const KeyCode KEY_F = 0x29;
static const KeyCode KEY_G = 0x2A;
static const KeyCode KEY_H = 0x2B;
static const KeyCode KEY_J = 0x2C;
static const KeyCode KEY_K = 0x2D;
static const KeyCode KEY_L = 0x2E;
static const KeyCode KEY_COLON = 0x2F;
static const KeyCode KEY_QUOTE = 0x30;
static const KeyCode KEY_TILDE = 0x31;
static const KeyCode KEY_LEFT_SHIFT = 0x32;
static const KeyCode KEY_SLASH = 0x33;
static const KeyCode KEY_Z = 0x34;
static const KeyCode KEY_X = 0x35;
static const KeyCode KEY_C = 0x36;
static const KeyCode KEY_V = 0x37;
static const KeyCode KEY_B = 0x38;
static const KeyCode KEY_N = 0x39;
static const KeyCode KEY_M = 0x3A;
static const KeyCode KEY_COMMA = 0x3B;
static const KeyCode KEY_PERIOD = 0x3C;
static const KeyCode KEY_BACKSLASH = 0x3D;
static const KeyCode KEY_RIGHT_SHIFT = 0x3E;
static const KeyCode KEY_MULTIPLY = 0x3F;
static const KeyCode KEY_LEFT_ALT = 0x40;
static const KeyCode KEY_SPACE = 0x41;
static const KeyCode KEY_CAPS_LOCK = 0x42;
static const KeyCode KEY_F1 = 0x43;
static const KeyCode KEY_F2 = 0x44;
static const KeyCode KEY_F3 = 0x45;
static const KeyCode KEY_F4 = 0x46;
static const KeyCode KEY_F5 = 0x47;
static const KeyCode KEY_F6 = 0x48;
static const KeyCode KEY_F7 = 0x49;
static const KeyCode KEY_F8 = 0x4A;
static const KeyCode KEY_F9 = 0x4B;
static const KeyCode KEY_F10 = 0x4C;
static const KeyCode KEY_NUM_LOCK = 0x4D;
static const KeyCode KEY_SCROLL_LOCK = 0x4E;
static const KeyCode KEY_NUM_7 = 0x4F;
static const KeyCode KEY_NUM_8 = 0x50;
static const KeyCode KEY_NUM_9 = 0x51;
static const KeyCode KEY_SUBTRACT = 0x52;
static const KeyCode KEY_NUM_4 = 0x53;
static const KeyCode KEY_NUM_5 = 0x54;
static const KeyCode KEY_NUM_6 = 0x55;
static const KeyCode KEY_ADD = 0x56;
static const KeyCode KEY_NUM_1 = 0x57;
static const KeyCode KEY_NUM_2 = 0x58;
static const KeyCode KEY_NUM_3 = 0x59;
static const KeyCode KEY_NUM_0 = 0x5A;
static const KeyCode KEY_NUM_DELETE = 0x5B;
// 0x5C - 0x5E
static const KeyCode KEY_F11 = 0x5F; // |??
static const KeyCode KEY_F12 = 0x60;
// 0x61 - 0x67
static const KeyCode KEY_ENTER = 0x68;
// 0x69
static const KeyCode KEY_DIVIDE = 0x6A;
// 0x6B;
static const KeyCode KEY_RIGHT_ALT = 0x6C;
// 0x6D
static const KeyCode KEY_HOME = 0x6E;
static const KeyCode KEY_UP = 0x6F;
static const KeyCode KEY_PAGE_UP = 0x70;
static const KeyCode KEY_LEFT = 0x71;
static const KeyCode KEY_RIGHT = 0x72;
static const KeyCode KEY_END = 0x73;
static const KeyCode KEY_DOWN = 0x74;
// 0x74 
static const KeyCode KEY_PAGE_DOWN = 0x75;
static const KeyCode KEY_INSERT = 0x76;
static const KeyCode KEY_DELETE = 0x77;
// 0x78-0x7E
static const KeyCode KEY_PAUSE = 0x7F;
// 0x80 - 0x84
static const KeyCode KEY_LEFT_WINDOWS = 0x85;
static const KeyCode KEY_RIGHT_WINDOWS = 0x86;
static const KeyCode KEY_APPS = 0x87;
static const KeyCode KEY_RIGHT_CONTROL = 0x105;
#elif defined(_WIN32)
static const KeyCode KEY_ESCAPE = 0x1B;//VK_ESCAPE;
static const KeyCode KEY_0 = 0x30;
static const KeyCode KEY_1 = 0x31;
static const KeyCode KEY_2 = 0x32;
static const KeyCode KEY_3 = 0x33;
static const KeyCode KEY_4 = 0x34;
static const KeyCode KEY_5 = 0x35;
static const KeyCode KEY_6 = 0x36;
static const KeyCode KEY_7 = 0x37;
static const KeyCode KEY_8 = 0x38;
static const KeyCode KEY_9 = 0x39;
static const KeyCode KEY_BACKSPACE = 0x08;//VK_BACK;
static const KeyCode KEY_TAB = 0x09;//VK_TAB;

static const KeyCode KEY_A = 0x41;
static const KeyCode KEY_B = 0x42;
static const KeyCode KEY_C = 0x43;
static const KeyCode KEY_D = 0x44;
static const KeyCode KEY_E = 0x45;
static const KeyCode KEY_F = 0x46;
static const KeyCode KEY_G = 0x47;
static const KeyCode KEY_H = 0x48;
static const KeyCode KEY_I = 0x49;
static const KeyCode KEY_J = 0x4A;
static const KeyCode KEY_K = 0x4B;
static const KeyCode KEY_L = 0x4C;
static const KeyCode KEY_M = 0x4D;
static const KeyCode KEY_N = 0x4E;
static const KeyCode KEY_O = 0x4F;
static const KeyCode KEY_P = 0x50;
static const KeyCode KEY_Q = 0x51;
static const KeyCode KEY_R = 0x52;
static const KeyCode KEY_S = 0x53;
static const KeyCode KEY_T = 0x54;
static const KeyCode KEY_U = 0x55;
static const KeyCode KEY_V = 0x56;
static const KeyCode KEY_W = 0x57;
static const KeyCode KEY_X = 0x58;
static const KeyCode KEY_Y = 0x59;
static const KeyCode KEY_Z = 0x5A;
static const KeyCode KEY_LEFT_BRACKET = 0xDB;//VK_OEM_4
static const KeyCode KEY_RIGHT_BRACKET = 0xDD;//VK_OEM_6
static const KeyCode KEY_LEFT_CONTROL = 0xA2;//VK_LCONTROL;
static const KeyCode KEY_COLON = 0xBA;//VK_OEM_1
static const KeyCode KEY_QUOTE = 0xDE;//VK_OEM_7
static const KeyCode KEY_TILDE = 0xC0;//VK_OEM_3
static const KeyCode KEY_LEFT_SHIFT = 0xA0;//VK_LSHIFT;
static const KeyCode KEY_SLASH = 0xDC;//VK_OEM_5
static const KeyCode KEY_COMMA = 0xBC;//VK_OEM_COMMA
static const KeyCode KEY_PERIOD = 0xBE;//VK_OEM_PERIOD
static const KeyCode KEY_BACKSLASH = 0xBF;//VK_OEM_2
static const KeyCode KEY_RIGHT_SHIFT = 0xA1;//VK_RSHIFT;
static const KeyCode KEY_MULTIPLY = 0x6A;//VK_MULTIPLY;
static const KeyCode KEY_LEFT_ALT = 0xA4;//VK_LMENU;
static const KeyCode KEY_SPACE = 0x20;//VK_SPACE;
static const KeyCode KEY_CAPS_LOCK = 0x14;//VK_CAPITAL;
static const KeyCode KEY_F1 = 0x70;//VK_F1;
static const KeyCode KEY_F2 = 0x71;//VK_F2;
static const KeyCode KEY_F3 = 0x72;//VK_F3;
static const KeyCode KEY_F4 = 0x73;//VK_F4;
static const KeyCode KEY_F5 = 0x74;//VK_F5;
static const KeyCode KEY_F6 = 0x75;//VK_F6;
static const KeyCode KEY_F7 = 0x76;//VK_F7;
static const KeyCode KEY_F8 = 0x77;//VK_F8;
static const KeyCode KEY_F9 = 0x78;//VK_F9;
static const KeyCode KEY_F10 = 0x79;//VK_F10;
static const KeyCode KEY_NUM_LOCK = 0x90;//VK_NUMLOCK;
static const KeyCode KEY_SCROLL_LOCK = 0x91;//VK_SCROLL;
static const KeyCode KEY_NUM_7 = 0x67;//VK_NUMPAD7;
static const KeyCode KEY_NUM_8 = 0x68;//VK_NUMPAD8;
static const KeyCode KEY_NUM_9 = 0x69;//VK_NUMPAD9;
static const KeyCode KEY_SUBTRACT = 0x6D;//VK_SUBTRACT;
static const KeyCode KEY_NUM_4 = 0x64;//VK_NUMPAD4;
static const KeyCode KEY_NUM_5 = 0x65;//VK_NUMPAD5;
static const KeyCode KEY_NUM_6 = 0x66;//VK_NUMPAD6;
static const KeyCode KEY_ADD = 0x6B;//VK_ADD;
static const KeyCode KEY_NUM_1 = 0x61;//VK_NUMPAD1;
static const KeyCode KEY_NUM_2 = 0x62;//VK_NUMPAD2;
static const KeyCode KEY_NUM_3 = 0x63;//VK_NUMPAD3;
static const KeyCode KEY_NUM_0 = 0x60;//VK_NUMPAD0;
static const KeyCode KEY_NUM_DELETE = 0x6E;//VK_DECIMAL; // ?
// 0x5C - 0x5E
static const KeyCode KEY_F11 = 0x7A;//VK_F11
static const KeyCode KEY_F12 = 0x7B;//VK_F12
// 0x61 - 0x67
static const KeyCode KEY_ENTER = 0x6C;//VK_SEPARATOR;
static const KeyCode KEY_RETURN = 0x0D;//VK_RETURN;
// 0x69
static const KeyCode KEY_DIVIDE = 0x6F;//VK_DIVIDE;
static const KeyCode KEY_PLUS = 0xBB;//VK_OEM_PLUS;
static const KeyCode KEY_MINUS = 0xBD;//VK_OEM_MINUS;
static const KeyCode KEY_RIGHT_ALT = 0xA5;//VK_RMENU;
// 0x6D
static const KeyCode KEY_HOME = 0x24;//VK_HOME;
static const KeyCode KEY_UP = 0x26;//VK_UP;
static const KeyCode KEY_PAGE_UP = 0x21;//VK_PRIOR;
static const KeyCode KEY_LEFT = 0x25;//VK_LEFT;
static const KeyCode KEY_RIGHT = 0x27;//VK_RIGHT;
static const KeyCode KEY_END = 0x23;//VK_END;
static const KeyCode KEY_DOWN = 0x28;//VK_DOWN;
// 0x74 
static const KeyCode KEY_PAGE_DOWN = 0x22;//VK_NEXT;
static const KeyCode KEY_INSERT = 0x2D;//VK_INSERT;
static const KeyCode KEY_DELETE = 0x2E;//VK_DELETE;
// 0x78-0x7E
static const KeyCode KEY_PAUSE = 0x13;//VK_PAUSE;
// 0x80 - 0x84
static const KeyCode KEY_LEFT_WINDOWS = 0x85;//VK_F22
static const KeyCode KEY_RIGHT_WINDOWS = 0x86;//VK_F23
static const KeyCode KEY_APPS = 0x87;//VK_F24
static const KeyCode KEY_RIGHT_CONTROL = 0xA3;//VK_RCONTROL;
#endif 

typedef unsigned char BUTTON_CODE;
const BUTTON_CODE LEFT_BUTTON = 0;
const BUTTON_CODE MIDDLE_BUTTON = 1;
const BUTTON_CODE RIGHT_BUTTON = 2;

#endif // __KEY_CODES_H__
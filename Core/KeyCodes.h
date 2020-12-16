#ifndef __KEY_CODES_H__
#define __KEY_CODES_H__

typedef unsigned short KeyCode;
static const KeyCode KEY_UNDEFINED = 0x0;

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

#if defined (__linux__)
static const KeyCode KEY_ESCAPE = 1;
static const KeyCode KEY_1 = 2;
static const KeyCode KEY_2 = 3;
static const KeyCode KEY_3 = 4;
static const KeyCode KEY_4 = 5;
static const KeyCode KEY_5 = 6;
static const KeyCode KEY_6 = 7;
static const KeyCode KEY_7 = 8;
static const KeyCode KEY_8 = 9;
static const KeyCode KEY_9 = 10;
static const KeyCode KEY_0 = 1;
static const KeyCode KEY_MINUS = 12;
static const KeyCode KEY_PLUS = 13;
static const KeyCode KEY_BACKSPACE = 14;
static const KeyCode KEY_TAB = 15;
static const KeyCode KEY_Q = 16;
static const KeyCode KEY_W = 17;
static const KeyCode KEY_E = 18;
static const KeyCode KEY_R = 19;
static const KeyCode KEY_T = 20;
static const KeyCode KEY_Y = 21;
static const KeyCode KEY_U = 22;
static const KeyCode KEY_I = 23;
static const KeyCode KEY_O = 24;
static const KeyCode KEY_P = 25;
static const KeyCode KEY_LEFT_BRACKET = 26;
static const KeyCode KEY_RIGHT_BRACKET = 27;
static const KeyCode KEY_ENTER = 28;
static const KeyCode KEY_LEFT_CONTROL = 29;
static const KeyCode KEY_A = 30;
static const KeyCode KEY_S = 31;
static const KeyCode KEY_D = 32;
static const KeyCode KEY_F = 33;
static const KeyCode KEY_G = 34;
static const KeyCode KEY_H = 35;
static const KeyCode KEY_J = 36;
static const KeyCode KEY_K = 37;
static const KeyCode KEY_L = 38;
static const KeyCode KEY_COLON = 39;
static const KeyCode KEY_QUOTE = 40;
static const KeyCode KEY_TILDE = 41;
static const KeyCode KEY_LEFT_SHIFT = 42;
static const KeyCode KEY_BACKSLASH = 43;
static const KeyCode KEY_Z = 44;
static const KeyCode KEY_X = 45;
static const KeyCode KEY_C = 46;
static const KeyCode KEY_V = 47;
static const KeyCode KEY_B = 48;
static const KeyCode KEY_N = 49;
static const KeyCode KEY_M = 50;
static const KeyCode KEY_COMMA = 51;
static const KeyCode KEY_PERIOD = 52;
static const KeyCode KEY_SLASH = 53;
static const KeyCode KEY_RIGHT_SHIFT = 54;
static const KeyCode KEY_MULTIPLY = 55;
static const KeyCode KEY_LEFT_ALT = 56;
static const KeyCode KEY_SPACE = 57;
static const KeyCode KEY_CAPS_LOCK = 58;
static const KeyCode KEY_F1 = 59;
static const KeyCode KEY_F2 = 60;
static const KeyCode KEY_F3 = 61;
static const KeyCode KEY_F4 = 62;
static const KeyCode KEY_F5 = 63;
static const KeyCode KEY_F6 = 64;
static const KeyCode KEY_F7 = 65;
static const KeyCode KEY_F8 = 66;
static const KeyCode KEY_F9 = 67;
static const KeyCode KEY_F10 = 68;
static const KeyCode KEY_NUM_LOCK = 69;
static const KeyCode KEY_SCROLL_LOCK = 70;
static const KeyCode KEY_NUM_7 = 71;
static const KeyCode KEY_NUM_8 = 72;
static const KeyCode KEY_NUM_9 = 73;
static const KeyCode KEY_NUM_MINUS = 74;
static const KeyCode KEY_NUM_4 = 75;
static const KeyCode KEY_NUM_5 = 76;
static const KeyCode KEY_NUM_6 = 77;
static const KeyCode KEY_NUM_PLUS = 78;
static const KeyCode KEY_NUM_1 = 79;
static const KeyCode KEY_NUM_2 = 80;
static const KeyCode KEY_NUM_3 = 81;
static const KeyCode KEY_NUM_0 = 82;
static const KeyCode KEY_NUM_DELETE = 83;
static const KeyCode KEY_F11 = 87;
static const KeyCode KEY_F12 = 88;
static const KeyCode KEY_NUM_ENTER = 96;
static const KeyCode KEY_NUM_SLASH = 98;
static const KeyCode KEY_RIGHT_ALT = 100;
static const KeyCode KEY_HOME = 102;
static const KeyCode KEY_UP = 103;
static const KeyCode KEY_PAGE_UP = 104;
static const KeyCode KEY_LEFT = 105;
static const KeyCode KEY_RIGHT = 106;
static const KeyCode KEY_END = 107;
static const KeyCode KEY_DOWN = 108;
static const KeyCode KEY_PAGE_DOWN = 109;
static const KeyCode KEY_INSERT = 110;
static const KeyCode KEY_DELETE = 111;
static const KeyCode KEY_PAUSE = 119;
static const KeyCode KEY_LEFT_WINDOWS = 0x85;
static const KeyCode KEY_RIGHT_WINDOWS = 0x86;
static const KeyCode KEY_APPS = 0x87;
static const KeyCode KEY_RIGHT_CONTROL = 97;
#elif defined(_WIN32)
static const KeyCode KEY_ESCAPE = 0x1B;
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
static const KeyCode KEY_BACKSPACE = 0x08;
static const KeyCode KEY_TAB = 0x09;
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
static const KeyCode KEY_LEFT_BRACKET = 0xDB;
static const KeyCode KEY_RIGHT_BRACKET = 0xDD;
static const KeyCode KEY_LEFT_CONTROL = 0xA2;
static const KeyCode KEY_COLON = 0xBA;
static const KeyCode KEY_QUOTE = 0xDE;
static const KeyCode KEY_TILDE = 0xC0;
static const KeyCode KEY_LEFT_SHIFT = 0xA0;
static const KeyCode KEY_SLASH = 0xDC;
static const KeyCode KEY_COMMA = 0xBC;
static const KeyCode KEY_PERIOD = 0xBE;
static const KeyCode KEY_BACKSLASH = 0xBF;
static const KeyCode KEY_RIGHT_SHIFT = 0xA1;
static const KeyCode KEY_MULTIPLY = 0x6A;
static const KeyCode KEY_LEFT_ALT = 0xA4;
static const KeyCode KEY_SPACE = 0x20;
static const KeyCode KEY_CAPS_LOCK = 0x14;
static const KeyCode KEY_F1 = 0x70;
static const KeyCode KEY_F2 = 0x71;
static const KeyCode KEY_F3 = 0x72;
static const KeyCode KEY_F4 = 0x73;
static const KeyCode KEY_F5 = 0x74;
static const KeyCode KEY_F6 = 0x75;
static const KeyCode KEY_F7 = 0x76;
static const KeyCode KEY_F8 = 0x77;
static const KeyCode KEY_F9 = 0x78;
static const KeyCode KEY_F10 = 0x79;
static const KeyCode KEY_NUM_LOCK = 0x90;
static const KeyCode KEY_SCROLL_LOCK = 0x91;
static const KeyCode KEY_NUM_7 = 0x67;
static const KeyCode KEY_NUM_8 = 0x68;
static const KeyCode KEY_NUM_9 = 0x69;
static const KeyCode KEY_SUBTRACT = 0x6D;
static const KeyCode KEY_NUM_4 = 0x64;
static const KeyCode KEY_NUM_5 = 0x65;
static const KeyCode KEY_NUM_6 = 0x66;
static const KeyCode KEY_ADD = 0x6B;
static const KeyCode KEY_NUM_1 = 0x61;
static const KeyCode KEY_NUM_2 = 0x62;
static const KeyCode KEY_NUM_3 = 0x63;
static const KeyCode KEY_NUM_0 = 0x60;
static const KeyCode KEY_NUM_DELETE = 0x6E;
static const KeyCode KEY_F11 = 0x7A;
static const KeyCode KEY_F12 = 0x7B;
static const KeyCode KEY_ENTER = 0x6C;
static const KeyCode KEY_RETURN = 0x0D;
static const KeyCode KEY_DIVIDE = 0x6F;
static const KeyCode KEY_PLUS = 0xBB;
static const KeyCode KEY_MINUS = 0xBD;
static const KeyCode KEY_RIGHT_ALT = 0xA5;
static const KeyCode KEY_HOME = 0x24;
static const KeyCode KEY_UP = 0x26;
static const KeyCode KEY_PAGE_UP = 0x21;
static const KeyCode KEY_LEFT = 0x25;
static const KeyCode KEY_RIGHT = 0x27;
static const KeyCode KEY_END = 0x23;
static const KeyCode KEY_DOWN = 0x28;
static const KeyCode KEY_PAGE_DOWN = 0x22;
static const KeyCode KEY_INSERT = 0x2D;
static const KeyCode KEY_DELETE = 0x2E;
static const KeyCode KEY_PAUSE = 0x13;
static const KeyCode KEY_LEFT_WINDOWS = 0x85;
static const KeyCode KEY_RIGHT_WINDOWS = 0x86;
static const KeyCode KEY_APPS = 0x87;
static const KeyCode KEY_RIGHT_CONTROL = 0xA3;
#endif 

typedef unsigned char BUTTON_CODE;
const BUTTON_CODE LEFT_BUTTON = 0;
const BUTTON_CODE MIDDLE_BUTTON = 1;
const BUTTON_CODE RIGHT_BUTTON = 2;

#endif // __KEY_CODES_H__

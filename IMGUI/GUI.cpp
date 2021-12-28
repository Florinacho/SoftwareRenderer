#include "GUI.hpp"
#include "Event.h"
#include "Image.h"
#include "SimpleFont.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>

const ubvec4 RawDefaultColor  (128, 128, 128, 255);
const ubvec4 RawFocusedColor  (165, 165, 165, 255);
const ubvec4 RawLightColor    (180, 180, 180, 255);
const ubvec4 RawDarkColor     ( 76,  76,  76, 255);
const ubvec4 TextColor        (  0,   0,   0, 255);
const ubvec4 TextColorDisabled(RawFocusedColor);
const ubvec4 RawSelectedColor (  0,   0, 130, 255);
const int Separator = 8;
const int TextSize = 2;
const int WindowTitlebarHeight = TextSize * 10;

void GetOverlap(ivec4& out, const ivec4& a, const ivec4& b) {
	ivec4 ans;
	out.x = std::max(a.x, b.x);
	out.y = std::max(a.y, b.y);
	out.z = std::min(a.z, b.z);
	out.w = std::min(a.w, b.w);
}

// Graphics wrapper
inline void GUIDrawLine(Image* output, const ivec2& begin, const ivec2& end, const ubvec4& color) {
	output->drawLine(begin, end, color);
}

inline void GUIDrawOutline(Image* output, const ivec4& bounds, bool invert = false) {
	if (((bounds.w - bounds.y) <= 0) || ((bounds.z - bounds.x) <= 0)) {
		return;
	}
	GUIDrawLine(output, ivec2(bounds.x, bounds.y), ivec2(bounds.x, bounds.w), invert ? RawDarkColor : RawLightColor);
	GUIDrawLine(output, ivec2(bounds.x, bounds.w), ivec2(bounds.z, bounds.w), invert ? RawLightColor : RawDarkColor);
	GUIDrawLine(output, ivec2(bounds.z, bounds.w), ivec2(bounds.z, bounds.y), invert ? RawLightColor : RawDarkColor);
	GUIDrawLine(output, ivec2(bounds.x, bounds.y), ivec2(bounds.z, bounds.y), invert ? RawDarkColor : RawLightColor);
}

inline void GUIDrawOutline2(Image* output, const ivec4& bounds, bool invert = false) {
	if (((bounds.w - bounds.y) <= 0) || ((bounds.z - bounds.x) <= 0)) {
		return;
	}
	// Left
	GUIDrawLine(output, ivec2(bounds.x + 1, bounds.y), ivec2(bounds.x + 1, bounds.w), RawLightColor);
	// Down
	GUIDrawLine(output, ivec2(bounds.x, bounds.w - 1), ivec2(bounds.z, bounds.w - 1), RawDarkColor);
	// Right
	GUIDrawLine(output, ivec2(bounds.z - 1, bounds.w), ivec2(bounds.z - 1, bounds.y), RawDarkColor);
	// Up
	GUIDrawLine(output, ivec2(bounds.x, bounds.y + 1), ivec2(bounds.z, bounds.y + 1), RawLightColor);
	
	// Left
	GUIDrawLine(output, ivec2(bounds.x + 0, bounds.y), ivec2(bounds.x + 0, bounds.w), RawDarkColor);
	// Down
	GUIDrawLine(output, ivec2(bounds.x, bounds.w - 0), ivec2(bounds.z, bounds.w - 0), RawLightColor);
	// Right 
	GUIDrawLine(output, ivec2(bounds.z - 0, bounds.w), ivec2(bounds.z - 0, bounds.y), RawLightColor);
	// Up
	GUIDrawLine(output, ivec2(bounds.x, bounds.y + 0), ivec2(bounds.z, bounds.y + 0), RawDarkColor);
}

inline void GUIDrawQuad(Image* output, const ivec4& bounds, const ubvec4& color) {
	if (((bounds.w - bounds.y) <= 0) || ((bounds.z - bounds.x) <= 0)) {
		return;
	}
	output->drawFilledRectangle(bounds, color);
}

inline bool IsInside(const ivec4& rect, const ivec2& point) {
	return ((point.x >= rect.x) && (point.x <= rect.z) && (point.y >= rect.y) && (point.y <= rect.w));
}

template <typename T>
T clamp(T value, T low, T high) {
	return std::min(std::max(value, low), high);
}

struct KeyEvent {
	KeyCode key;
	bool status;
};

static Image* frameBuffer = NULL;
static Layout currentLayout;
static ivec4 ParentWindowRectangle;

static int draggedID = -1;
static int windowID = -1;
static time_t TextBoxActivationTime = 0;

static const uint32_t MAX_KEY_EVENT_COUNT = 32;
static KeyEvent keyEvents[MAX_KEY_EVENT_COUNT];
static uint32_t keyEventCount;
static bool ShiftKey = false;

static ivec2 MousePosition, LastMousePosition;
static bool MouseButtons[3] = {false, false, false}, LastMouseButtons[3] = {false, false, false};


struct CharacterKeyBinding {
	KeyCode key;
	char value0;
	char value1;
};

static const CharacterKeyBinding CharacterKeyBindings[] = {
	{KEY_SPACE, ' ', ' '},
	{KEY_TILDE, '~', '`'},
	{KEY_1, '!', '1'},
	{KEY_2, '@', '2'},
	{KEY_3, '#', '3'},
	{KEY_4, '$', '4'},
	{KEY_5, '%', '5'},
	{KEY_6, '^', '6'},
	{KEY_7, '&', '7'},
	{KEY_8, '*', '8'},
	{KEY_9, '(', '9'},
	{KEY_0, ')', '0'},
	{KEY_MINUS, '_', '-'},
	{KEY_PLUS, '+', '='},
	{KEY_Q, 'Q', 'q'},
	{KEY_W, 'W', 'w'},
	{KEY_E, 'E', 'e'},
	{KEY_R, 'R', 'r'},
	{KEY_T, 'T', 't'},
	{KEY_Y, 'Y', 'y'},
	{KEY_U, 'U', 'u'},
	{KEY_I, 'I', 'i'},
	{KEY_O, 'O', 'o'},
	{KEY_P, 'P', 'p'},
	{KEY_LEFT_BRACKET, '{', '['},
	{KEY_RIGHT_BRACKET, '}', ']'},
	{KEY_SLASH, '|', '\\'},
	{KEY_A, 'A', 'a'},
	{KEY_S, 'S', 's'},
	{KEY_D, 'D', 'd'},
	{KEY_F, 'F', 'f'},
	{KEY_G, 'G', 'g'},
	{KEY_H, 'H', 'h'},
	{KEY_J, 'J', 'j'},
	{KEY_K, 'K', 'k'},
	{KEY_L, 'L', 'l'},
	{KEY_COLON, ':', ';'},
	{KEY_QUOTE, '"', '\''},
	{KEY_Z, 'Z', 'z'},
	{KEY_X, 'X', 'x'},
	{KEY_C, 'C', 'c'},
	{KEY_V, 'V', 'v'},
	{KEY_B, 'B', 'b'},
	{KEY_N, 'N', 'n'},
	{KEY_M, 'M', 'm'},
	{KEY_COMMA, '<', ','},
	{KEY_PERIOD, '>', '.'},
	{KEY_BACKSLASH, '?', '/'},
};
static const uint32_t CharacterKeyBindingCount = sizeof(CharacterKeyBindings) / sizeof(CharacterKeyBindings[0]);
static bool GetCharacter(char& ans, const KeyCode& key, bool shift) {
	for (unsigned int index = 0; index < CharacterKeyBindingCount; ++index) {
		if (CharacterKeyBindings[index].key == key) {
			ans = (shift ? CharacterKeyBindings[index].value0 : CharacterKeyBindings[index].value1);
			return true;
		}
	}
	return false;
}

Layout::Layout() {
	type = Layout::ABSOLUTE;
	parent = ivec4();
	elementIndex = 0;
	max = 0xFFFFFFFF;
	grid.count = ivec2(1, 1);
	grid.cellSize = ivec2(1, 1);
	grid.elementSize = 1;
	separator = Separator;
}

Layout::Layout(const Layout& other) {
	*this = other;
}
	
Layout& Layout::operator = (const Layout& other) {
	type = other.type;
	parent = other.parent;
	max = other.max;
	elementIndex = other.elementIndex;
	grid = other.grid;
	separator = other.separator;
	return *this;
}

ivec4 Layout::getCellBounds(const ivec4& bounds) {
	ivec4 ans;

	switch (currentLayout.type) {
	case Layout::ABSOLUTE :
		if ((bounds.w - bounds.y + bounds.z - bounds.x) != 0) {
			ans = bounds;
		} else {
			ans.x = 0;
			ans.y = 0;
			ans.z = ParentWindowRectangle.z - ParentWindowRectangle.x;
			ans.w = ParentWindowRectangle.w - ParentWindowRectangle.y;
		}
		break;
		
	case Layout::SPLIT :
		{
		const int width = ParentWindowRectangle.z - ParentWindowRectangle.x;
		const int height = ParentWindowRectangle.w - ParentWindowRectangle.y;
		const int cellIndex = currentLayout.elementIndex % 2;
		
		if (currentLayout.split.vertical) {
			const int topCellWidth = (width - currentLayout.separator) * currentLayout.split.weight;
			const int botCellWidth = width - currentLayout.separator - topCellWidth;
			ans = ivec4((topCellWidth + currentLayout.separator) * cellIndex, 0, topCellWidth + (botCellWidth + currentLayout.separator) * cellIndex, height);
		} else {
			const int topCellHeight = (height - currentLayout.separator) * currentLayout.split.weight;
			const int botCellHeight = height - currentLayout.separator - topCellHeight;
			ans = ivec4(0, (topCellHeight + currentLayout.separator + currentLayout.separator / 2) * cellIndex, width, topCellHeight + (currentLayout.separator + botCellHeight) * cellIndex);
		}
		currentLayout.elementIndex += 1;
		{ // TMP: clamp max Y size
			const int32_t mid = (ans.w + ans.y) / 2;
			int32_t height = ans.w - ans.y;
			if (height > currentLayout.max.y) {
				height = currentLayout.max.y;
			}
			ans.y = mid - height / 2;
			ans.w = mid + height / 2;
		}
		}
		break;
		
	case Layout::GRID:
		{
		const int indexX = (currentLayout.elementIndex % currentLayout.grid.count.x) % currentLayout.grid.count.x;
		const int indexY = (currentLayout.elementIndex / currentLayout.grid.count.x) % currentLayout.grid.count.y;
		const int gridCount = std::min(currentLayout.grid.elementSize, currentLayout.grid.count.x - indexX);
		ans = ivec4( indexX              * currentLayout.grid.cellSize.x +  indexX                  * currentLayout.separator,
					 indexY              * currentLayout.grid.cellSize.y +  indexY                  * currentLayout.separator,
					(indexX + gridCount) * currentLayout.grid.cellSize.x + (indexX + gridCount - 1) * currentLayout.separator, 
					(indexY + 1)         * currentLayout.grid.cellSize.y +  indexY                  * currentLayout.separator);
		currentLayout.elementIndex += gridCount;

		{ // TMP: clamp max Y size
			const int32_t mid = (ans.w + ans.y) / 2;
			int32_t height = ans.w - ans.y;
			if (height > currentLayout.max.y) {
				height = currentLayout.max.y;
			}
			ans.y = mid - height / 2;
			ans.w = mid + height / 2;
		}
		}
		break;
		
	default :
		assert(!"Fuck!");
	}

	return ans;
}

ivec4 Layout::getAbsoluteBounds(const ivec4& bounds) const {
	return (currentLayout.getCellBounds(bounds) + ivec4(ParentWindowRectangle.x, ParentWindowRectangle.y, ParentWindowRectangle.x, ParentWindowRectangle.y));
}

Layout AbsoluteLayout() {
	Layout ans;
	ans.type = Layout::ABSOLUTE;
	return ans;
}

Layout GridLayout(uint32_t x, uint32_t y, uint32_t elementSize) {
	Layout ans;
	ans.type = Layout::GRID;
	ans.elementIndex = 0;
	ans.grid.count.x = x;
	ans.grid.count.y = y;
	ans.grid.elementSize = elementSize;
	return ans;
}

Layout SplitLayout(bool vertical, float weight) {
	Layout ans;
	
	ans.type = Layout::SPLIT;
	ans.parent = ivec4();
	ans.elementIndex = 0;
	
	ans.split.vertical = vertical;
	ans.split.weight = weight;
	
	return ans;
}

void SetFrameBuffer(Image* value) {
	frameBuffer = value;
	
	if (frameBuffer) {
		const uvec2 frameBufferSize = frameBuffer->getSize();
		ParentWindowRectangle = ivec4(0, 0, frameBufferSize.x, frameBufferSize.y);
	}
	SetAbsoluteLayout();
	
	draggedID = 0;
	windowID = 1;
	
	for (uint32_t index = 0; index < 3; ++index) {
		MouseButtons[index] = false;
		LastMouseButtons[index] = false;
	}
}

Image* GetFrameBuffer() {
	return frameBuffer;
}

void SetGridElementWidth(int value) {
	if (currentLayout.type != Layout::GRID) {
		return;
	}
	currentLayout.grid.elementSize = value;
}

void SetAbsoluteLayout() {
	currentLayout.type = Layout::ABSOLUTE;
	currentLayout.elementIndex = 0;
}

void SetGridLayout(int x, int y, int padding) {
	currentLayout.type = Layout::GRID;
	currentLayout.elementIndex = 0;
	currentLayout.grid.count.x = x;
	currentLayout.grid.count.y = y;
	currentLayout.separator = padding;
	// Can be calculated at runtime and save 8 bytes / Layout
	currentLayout.grid.cellSize.x = (ParentWindowRectangle.z - ParentWindowRectangle.x - currentLayout.separator * (currentLayout.grid.count.x - 1)) / currentLayout.grid.count.x;
	currentLayout.grid.cellSize.y = (ParentWindowRectangle.w - ParentWindowRectangle.y - currentLayout.separator * (currentLayout.grid.count.y - 1)) / currentLayout.grid.count.y;
	currentLayout.grid.elementSize = 1;
}

void ProcessEvent(const Event* event) {
	if (event == NULL) {
		return;
	}
	
	switch (event->type) {
	case Event::KEYBOARD :
		switch (event->key) {
		case KEY_LEFT_SHIFT: 
		case KEY_RIGHT_SHIFT: 
			ShiftKey = event->state; 
			break;
		}
		if (keyEventCount < MAX_KEY_EVENT_COUNT) {
			keyEvents[keyEventCount++] = {event->key, event->state};
		}
		break;
	case Event::MOUSE_POSITION :
		MousePosition.x = event->x;
		MousePosition.y = event->y;
		break;
	case Event::MOUSE_BUTTON :
		MouseButtons[event->button] = event->state;
		break;
	}
	windowID = 1;
}

void PostEvent() {
	for (uint32_t index = 0; index < 3; ++index) {
		LastMouseButtons[index] = MouseButtons[index];
	}
	
	LastMousePosition = MousePosition;
	
	if (MouseButtons[0] == false) {
		draggedID = 0;
	}

	//keyEvents.clear();
	keyEventCount = 0;
}

void DummyElement(uint32_t count) {
	switch (currentLayout.type) {
	case Layout::GRID :
		currentLayout.elementIndex += currentLayout.grid.elementSize * count;
		break;
	case Layout::SPLIT :
		currentLayout.elementIndex += 1 * count;
		break;
	default :
		break;
	}
}

void Label(const char* text, uint32_t flags, const ivec4& bounds) {
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	
	if (flags & GUI_VISIBLE) {
		if (flags & GUI_BACKGROUND) {
			GUIDrawQuad(frameBuffer, absoluteBounds, RawFocusedColor);
		}
		
		if (flags & GUI_FOREGROUND) {
			if ((flags & GUI_ALIGN_CENTER) == GUI_ALIGN_CENTER) {
				DrawCenteredText(frameBuffer, absoluteBounds, text, TextColor, TextSize);
			} else {
				DrawText(frameBuffer, absoluteBounds, text, TextColor, TextSize);
			}
		}
		
		if (flags & GUI_OUTLINE) {
			GUIDrawOutline(frameBuffer, absoluteBounds, true);
		}
	}
}

bool Button(const char* text, uint32_t flags, const ivec4& bounds) {
	static const ivec4 TextPressedOffset( 2, 2, 2, 2);
	
	const bool enabled = flags & GUI_ENABLED;
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	const bool focused = (((draggedID == 0) && IsInside(absoluteBounds, MousePosition) && (enabled)) || (flags & GUI_FOCUSED));
	const bool clicked = ((focused && MouseButtons[0]) || (flags & GUI_CLICKED));
	
	if (flags & GUI_VISIBLE) {
		if (flags & GUI_BACKGROUND) {
			GUIDrawQuad(frameBuffer, absoluteBounds, focused ? RawFocusedColor : RawDefaultColor);
		}
		
		if (flags & GUI_FOREGROUND) {
			if ((flags & GUI_ALIGN_CENTER) == GUI_ALIGN_CENTER) {
				DrawCenteredText(frameBuffer, absoluteBounds + TextPressedOffset * clicked, text, enabled ? TextColor : TextColorDisabled, TextSize);
			} else {
				DrawText(frameBuffer, absoluteBounds + TextPressedOffset * clicked, text, enabled ? TextColor : TextColorDisabled, TextSize);
			}
		}
		
		if (flags & GUI_OUTLINE) {
			GUIDrawOutline(frameBuffer, absoluteBounds, clicked);
		}
	}
	
	return (focused && !MouseButtons[0] && LastMouseButtons[0]);
}

bool CheckBox(bool &checked, uint32_t flags, const ivec4& nbounds) {
	bool ans = false;
	
	ivec4 rect = currentLayout.getAbsoluteBounds(nbounds);
	
	const int centerX = (rect.z - rect.x) / 2 - 7;
	const int centerY = (rect.w - rect.y) / 2 - 7;
	
	// Keep square shape
	const int width  = rect.z - rect.x;
	const int height = rect.w - rect.y;
	int min = std::min(std::min(width, height), WindowTitlebarHeight);
	rect.z = rect.x + min;
	rect.w = rect.y + min;
	
	rect.x += centerX;
	rect.z += centerX;
	rect.y += centerY;
	rect.w += centerY;

	if (flags & GUI_VISIBLE) {
		if (flags & GUI_BACKGROUND) {
			GUIDrawQuad(frameBuffer, rect, RawFocusedColor);
		}
		if (flags & GUI_OUTLINE) {
			GUIDrawOutline(frameBuffer, rect, true);
		}
		if ((flags & GUI_FOREGROUND) && checked) {
			GUIDrawQuad(frameBuffer, rect + ivec4(3.0f, 3.0f,-3.0f,-3.0f), RawDarkColor);
		}
	}
	
	if ((draggedID == 0) && (flags & GUI_ENABLED) && IsInside(rect, MousePosition) && !MouseButtons[0] && LastMouseButtons[0]) {
		checked = !checked;
		ans = true;
	}
	
	return ans;
}

bool Palette(ubvec4& color, uint32_t flags, const ivec4& bounds) {
	static const uint32_t COLOR_COUNT_X = 8;
	static const uint32_t COLOR_COUNT_Y = 8;
	static const vec4 PRIME_COLORS[] = {
		vec4(1.0f, 0.0f, 0.0f, 1.0f), // RED
		vec4(1.0f, 1.0f, 0.0f, 1.0f), // YELLOW
		vec4(0.0f, 1.0f, 0.0f, 1.0f), // GREEN
		vec4(0.0f, 1.0f, 1.0f, 1.0f), // CYAN
		vec4(0.0f, 0.0f, 1.0f, 1.0f), // BLUE
		vec4(1.0f, 0.0f, 1.0f, 1.0f), // VIOLETE
		vec4(1.0f, 0.0f, 0.0f, 1.0f), // RED
	};
	static const uint32_t PrimeColorCount = (sizeof(PRIME_COLORS) / sizeof(PRIME_COLORS[0])) - 1;
	
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);

	const int width = absoluteBounds.z - absoluteBounds.x;
	const int height = absoluteBounds.w - absoluteBounds.y;
	
	const int colorTileSizeX = (width  / COLOR_COUNT_X / PrimeColorCount) * 1;
	const int colorTileSizeY = (height / COLOR_COUNT_Y / PrimeColorCount) * 3;
	const int offsetX = (width  - colorTileSizeX * COLOR_COUNT_X * PrimeColorCount) / 2;
	const int offsetY = (height - colorTileSizeY * COLOR_COUNT_Y * 2) / 2;
	
	if (flags & GUI_VISIBLE) {
		for (uint32_t y = 0; y < COLOR_COUNT_Y * 2; ++y) {
			const int colorIndexY = y / COLOR_COUNT_Y;
			const float procY = (float)(y % COLOR_COUNT_Y) / (float)(COLOR_COUNT_Y);
			const float iprocY = 1.0f - procY;
			for (uint32_t x = 0; x < COLOR_COUNT_X * PrimeColorCount; ++x) {
				const int colorIndexX = x / COLOR_COUNT_X;
				float procX = (float)(x % COLOR_COUNT_X)/ (float)(COLOR_COUNT_X);
				const float iprocX = 1.0f - procX;
				const vec4 tmpca[] = {
					vec4(1.0f, 1.0f, 1.0f, 1.0f),
					PRIME_COLORS[colorIndexX] * iprocX + PRIME_COLORS[colorIndexX + 1] * procX,
					vec4(0.0f, 0.0f, 0.0f, 1.0f)
				};
				vec4 k = tmpca[colorIndexY] * iprocY + tmpca[colorIndexY + 1] * procY;
				GUIDrawQuad(frameBuffer,
					ivec4(absoluteBounds.x + offsetX + x * colorTileSizeX, 
						  absoluteBounds.y + offsetY + y * colorTileSizeY, 
						  absoluteBounds.x + offsetX + (x + 1) * colorTileSizeX, 
						  absoluteBounds.y + offsetY + (y + 1) * colorTileSizeY),
					ubvec4(k.x * 255.0f, k.y * 255.0f, k.z * 255.0f, k.w * 255.0f));
			}
		}
	}
	
	const ivec4 paletteRect(absoluteBounds.x + offsetX, absoluteBounds.y + offsetY, absoluteBounds.x + offsetX + COLOR_COUNT_X * PrimeColorCount * colorTileSizeX, absoluteBounds.y + offsetY + COLOR_COUNT_Y * 2 * colorTileSizeY);
	const bool isInside = ((draggedID == 0) && IsInside(paletteRect, MousePosition));
	const ivec2 relativeMousePosition = MousePosition - ivec2(paletteRect.x, paletteRect.y);

	if ((flags & GUI_ENABLED) && isInside && MouseButtons[0] == false && LastMouseButtons[0] == true) {
		const int colorIndexX = relativeMousePosition.x / (COLOR_COUNT_X * colorTileSizeX);
		const int colorIndexY = relativeMousePosition.y / (COLOR_COUNT_Y * colorTileSizeY);
		const int x = (relativeMousePosition.x / colorTileSizeX) % COLOR_COUNT_X;
		const int y = (relativeMousePosition.y / colorTileSizeY) % COLOR_COUNT_Y;
		const float procX = (float)(x) / (float)(COLOR_COUNT_X - 1);
		const float procY = (float)(y) / (float)(COLOR_COUNT_Y - 1);
		const vec4 tmpca[] = {
			vec4(1.0f, 1.0f, 1.0f, 1.0f),
			PRIME_COLORS[colorIndexX] * (1.0f - procX) + PRIME_COLORS[colorIndexX + 1] * procX,
			vec4(0.0f, 0.0f, 0.0f, 1.0f)
		};
		const vec4 outputColor = tmpca[colorIndexY] * (1.0f - procY) + tmpca[colorIndexY + 1] * procY;
		color = ubvec4(outputColor.x * 255, outputColor.y * 255, outputColor.z * 255, outputColor.w * 255);
		return true;
	}
	return false;
}

void ProgressBar(float proc, uint32_t flags, const ivec4& bounds) {
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	const uint32_t width = absoluteBounds.z - absoluteBounds.x;
	
	if (flags & GUI_VISIBLE) {
		ivec4 barBounds;
		barBounds.x = absoluteBounds.x + 2;
		barBounds.y = absoluteBounds.y + 2;
		barBounds.z = absoluteBounds.x + (width - 2) * proc;
		barBounds.w = absoluteBounds.w - 2;
		
		if (flags & GUI_BACKGROUND) {
			GUIDrawQuad(frameBuffer, absoluteBounds, RawFocusedColor);
		}
		if (flags & GUI_OUTLINE) {
			GUIDrawOutline(frameBuffer, absoluteBounds, true);
		}
		if (flags & GUI_FOREGROUND) {
			GUIDrawQuad(frameBuffer, barBounds, RawSelectedColor);
			GUIDrawOutline(frameBuffer, barBounds, false);
		}
	}
}

bool Slider(float& proc, uint32_t flags, const ivec4& bounds) {
	static const int32_t BOX_WIDTH = WindowTitlebarHeight / 3;
	
	bool ans = false;
	
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	const int32_t width  = absoluteBounds.z - absoluteBounds.x;
	const int32_t height = absoluteBounds.w - absoluteBounds.y;
	const bool vertical = (height > width);
	
	ivec4 boxBounds;
	
	if (vertical) {
		const int mid = (absoluteBounds.x + absoluteBounds.z) / 2;
		
		if (flags & GUI_BACKGROUND) {
			GUIDrawLine(frameBuffer, ivec2(mid + 0, absoluteBounds.y), ivec2(mid + 0, absoluteBounds.w), RawDarkColor);
			GUIDrawLine(frameBuffer, ivec2(mid + 1, absoluteBounds.y), ivec2(mid + 1, absoluteBounds.w), RawLightColor);
		}
		
		int k = (float)(height - BOX_WIDTH) * proc;
		boxBounds = ivec4(absoluteBounds.x, absoluteBounds.w - k - BOX_WIDTH, absoluteBounds.z, absoluteBounds.w - k);
	} else {
		const int mid = (absoluteBounds.w + absoluteBounds.y) / 2;
		
		if (flags & GUI_BACKGROUND) {
			GUIDrawLine(frameBuffer, ivec2(absoluteBounds.x, mid + 0), ivec2(absoluteBounds.z, mid + 0), RawDarkColor);
			GUIDrawLine(frameBuffer, ivec2(absoluteBounds.x, mid + 1), ivec2(absoluteBounds.z, mid + 1), RawLightColor);
		}
		
		int k = (float)(width - BOX_WIDTH) * proc;
		boxBounds = ivec4(absoluteBounds.x + k, absoluteBounds.y, absoluteBounds.x + k + BOX_WIDTH, absoluteBounds.w);
	}
	const bool focused = (flags & GUI_ENABLED) && (IsInside(absoluteBounds, MousePosition) || IsInside(absoluteBounds, LastMousePosition));
	const bool clicked = (focused && MouseButtons[0]);
	
	if (clicked) {
		float newValue;
		
		if (vertical) {
			newValue = clamp<float>(1.0f - (float)(MousePosition.y - BOX_WIDTH / 2 - absoluteBounds.y) / (float)(height - BOX_WIDTH), 0.0f, 1.0f);
		} else {
			newValue = clamp<float>((float)(MousePosition.x - BOX_WIDTH / 2 - absoluteBounds.x) / (float)(width - BOX_WIDTH), 0.0f, 1.0f);
		}
		
		if (newValue != proc) {
			proc = newValue;
			ans = true;
		}
	}
	
	if (flags & GUI_FOREGROUND) {
		GUIDrawQuad(frameBuffer, boxBounds, focused ? RawFocusedColor : RawDefaultColor);
	}
	if (flags & GUI_OUTLINE) {
		GUIDrawOutline(frameBuffer, boxBounds, clicked);
	}
	
	return ans;
}

bool TextBox(std::string& value, int& carrot, uint32_t flags, const ivec4& bounds) {
	static const ubvec4 CARROT_COLOR(255, 255, 255, 255);
	static const uint32_t CARROT_WIDTH = 2;
	static const uint32_t FONT_GLYPH_SIZE = TextSize * 7;
	static const uint32_t FONT_GLYPH_HALF_SIZE = FONT_GLYPH_SIZE / 2;

	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	const ivec4 textBounds = absoluteBounds + ivec4(3, 2,-3,-2);
	const uint32_t width = textBounds.z - textBounds.x;
	const uint32_t height = textBounds.w - textBounds.y;
	const uint32_t visibleCount = width / FONT_GLYPH_SIZE;
	const uint32_t offset = ((carrot > visibleCount) ? carrot - visibleCount : 0);
	
	char c;
	bool ans = false;
	
	if (flags & GUI_BACKGROUND) {
		GUIDrawQuad(frameBuffer, absoluteBounds, RawFocusedColor);
	}
	if (flags & GUI_OUTLINE) {
		GUIDrawOutline(frameBuffer, absoluteBounds, true);
	}
	
	if (flags & GUI_ENABLED) {
		if (LastMouseButtons[0] && (MouseButtons[0] == false)) {//(flags & GUI_ENABLED) && 
			if (IsInside(absoluteBounds, MousePosition)) {
				carrot = clamp<int>((MousePosition.x - textBounds.x + FONT_GLYPH_HALF_SIZE) / FONT_GLYPH_SIZE, 0, value.size());
				TextBoxActivationTime = time(NULL);
			} else {
				carrot = -1;
			}
		}
	} else {
		carrot = -1;
	}
	
	if (carrot >= 0) {
		for (uint32_t index = 0; index < keyEventCount; ++index) {
			if (keyEvents[index].status) {
				switch (keyEvents[index].key) {
				case KEY_HOME :
					carrot = 0;
					TextBoxActivationTime = time(NULL);
					break;
				case KEY_END :
					carrot = value.size();
					TextBoxActivationTime = time(NULL);
					break;
				case KEY_LEFT : 
					carrot = std::max<int>(carrot - 1, 0);
					TextBoxActivationTime = time(NULL);
					break;
				case KEY_RIGHT :
					carrot = std::min<int>(carrot + 1, value.size());
					TextBoxActivationTime = time(NULL);
					break;
				case KEY_BACKSPACE : 
					if (carrot > 0) {
						value.erase(value.begin() + --carrot);
						ans = true;
						TextBoxActivationTime = time(NULL);
					}
					break;
				case KEY_DELETE :
					if (carrot < value.size()) {
						value.erase(value.begin() + carrot);
						carrot = std::min<int>(carrot, value.size());
						ans = true;
						TextBoxActivationTime = time(NULL);
					}
					break;
				default :
					if (GetCharacter(c, keyEvents[index].key, ShiftKey)) {
						value.insert(value.begin() + carrot++, c); 
						ans = true;
						TextBoxActivationTime = time(NULL);
					}
				}
			}
		}
		keyEventCount = 0;

		if ((flags & GUI_FOREGROUND) && ((time(NULL) - TextBoxActivationTime) % 2 == 0)) { // Carrot blinking every second
			const int32_t carrotPosition = (carrot - offset) * FONT_GLYPH_SIZE;
			const int32_t carrotHeight = std::min<int>(height, FONT_GLYPH_SIZE);
			GUIDrawQuad(frameBuffer, ivec4(textBounds.x + carrotPosition, textBounds.y, textBounds.x + carrotPosition + CARROT_WIDTH, textBounds.y + carrotHeight), CARROT_COLOR);
		}
	}
	
	if ((flags & GUI_FOREGROUND) && (height >= FONT_GLYPH_SIZE)) {
		uint32_t totalVisibleCount = std::min<int>(visibleCount, value.size());
		const char* textPtr = NULL;
		if (flags & GUI_HIDDEN) {
			DrawText(frameBuffer, uvec2(textBounds.x, textBounds.y), std::string(totalVisibleCount, '*').c_str(), TextColor, TextSize);
		} else {
			DrawText(frameBuffer, uvec2(textBounds.x, textBounds.y), value.c_str(), TextColor, TextSize);
		}
	}

	return ans;
}

bool Spinner(int& value, uint32_t flags, const ivec4& nbounds) {
	static const uint32_t LABEL_FLAG_MASK = GUI_VISIBLE | GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE | GUI_ALIGN_MASK;
	static const uint32_t BUTTON_FLAG_MASK = GUI_VISIBLE | GUI_ENABLED;
	static const uint32_t BUTTON_FLAGS = GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE | GUI_ALIGN_CENTER;
	
	static const uint32_t INT_MAX_CHARACTER_COUNT = 16;
	bool ans = false;
	char tmp[INT_MAX_CHARACTER_COUNT];
	
	Panel(AbsoluteLayout()){
		const ivec4 bounds = currentLayout.getCellBounds(nbounds);
		const int buttonWidth = std::min(bounds.z - bounds.x, WindowTitlebarHeight);

		snprintf(tmp, INT_MAX_CHARACTER_COUNT, "%d", value);
		
		Label(tmp, flags & LABEL_FLAG_MASK, ivec4(bounds.x + buttonWidth + 1, bounds.y, bounds.z - buttonWidth - 1, bounds.w));
		
		uint32_t button_flags = BUTTON_FLAGS | (flags & BUTTON_FLAG_MASK);
		if (Button("-", button_flags, ivec4(bounds.x, bounds.y, bounds.x + buttonWidth, bounds.w))) {
			value--;
			ans = true;
		}
		
		if (Button("+", button_flags, ivec4(bounds.z - buttonWidth, bounds.y, bounds.z, bounds.w))) {
			value++;
			ans = true;
		}
	}
	
	return ans;
}

bool ColorSelector(ivec4& r, ubvec4& color, uint32_t* flags) {
	static const char* LabelText[] = {"RED", "GREEN", "BLUE"};
	bool ans = false;
	
	Window(&r, "Palette", flags) {
		Panel(SplitLayout(false, 0.5f)) {
			Palette(color);
			Panel(GridLayout(4, 4)) {
				for (uint32_t index = 0; index < 3; ++index) {
					Label(LabelText[index]);
					
					SetGridElementWidth(3);
					int tmp = color[index];
					Spinner(tmp);
					color[index] = tmp;
					SetGridElementWidth(1);
				}
				
				SetGridElementWidth(4);
				currentLayout.elementIndex = 3 * currentLayout.grid.count.x;
				
				Panel(GridLayout(3, 1)) {
					DummyElement();
					if (Button("OK")) {
						if (flags) {
							*flags = *flags ^ GUI_VISIBLE;
						}
						ans = true;
					}
					if (Button("CANCEL")) {
						if (flags) {
							*flags = *flags ^ GUI_VISIBLE;
						}
					}
				}
			}
		}
	}
	
	return ans;
}

bool ListBox(int& selected, int& offset, const char** items, uint32_t count, const ivec4& bounds) {
	bool ans = false;
	
	Panel(AbsoluteLayout()) {
		const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
		const int width = absoluteBounds.z - absoluteBounds.x;
		const int height = absoluteBounds.w - absoluteBounds.y;
		const int itemHeight = 7 * TextSize;
		const int visibleItemCount = height / itemHeight;
		const int minCount = std::min((uint32_t)visibleItemCount, count);
		const int diff = count - visibleItemCount;
		offset = std::max(std::min(diff, offset), 0);

		GUIDrawQuad(frameBuffer, absoluteBounds, RawFocusedColor);
		GUIDrawOutline(frameBuffer, absoluteBounds, true);

		ivec4 listBounds = absoluteBounds;
		listBounds.z -= WindowTitlebarHeight;
		if (IsInside(listBounds, MousePosition) && MouseButtons[0] == false && LastMouseButtons[0] == true) {
			selected = clamp((MousePosition.y - absoluteBounds.y) / itemHeight + offset, offset, (int)count - 1);
			ans = true;
		}

		for (uint32_t index = offset; index < std::min(offset + minCount, (int)count); ++index) {
			int vOffset = (index - offset) * itemHeight;
			if (vOffset < 0) continue;
			const ivec4 itemBounds(absoluteBounds.x, absoluteBounds.y + vOffset, absoluteBounds.z, absoluteBounds.y + vOffset  + itemHeight);
			if (selected == index) {
				GUIDrawQuad(frameBuffer, itemBounds, ubvec4(  0,  0, 130, 255));
				//DrawCenteredText(frameBuffer, itemBounds, items[index], ubvec4(255) - TextColor, TextSize);
				DrawText(frameBuffer, itemBounds, items[index], ubvec4(255) - TextColor, TextSize);
			} else {
				//DrawCenteredText(frameBuffer, itemBounds, items[index], TextColor, TextSize);
				DrawText(frameBuffer, itemBounds, items[index], TextColor, TextSize);
			}
		}
		
		if (diff > 0) {
			ScrollBar(offset, diff, true, (float)visibleItemCount / (float)count, ivec4(width - WindowTitlebarHeight - 1, 0, width - 1, height));
		}
	}
	
	return ans;
}

bool ScrollBar(int& offset, const int max, bool vertical, float proc, const ivec4& relativeBounds) {
	bool ans = false;
	
	Panel(AbsoluteLayout()) {
		const ivec4 bounds = currentLayout.getCellBounds(relativeBounds);
		const ivec4 absoluteBounds = bounds + ivec4(ParentWindowRectangle.x, ParentWindowRectangle.y, ParentWindowRectangle.x, ParentWindowRectangle.y);

		const int buttonWidth  = std::min(bounds.z - bounds.x, WindowTitlebarHeight);
		const int buttonHeight = std::min(bounds.w - bounds.y, WindowTitlebarHeight);
		
		ivec4 barBounds;
		int barStep;
		int barLength;
		int maxBarHeight;
		ivec4 buttonBounds[2];
		ivec4 trackBounds;
		bool visible = true;
		
		if (vertical) {
			maxBarHeight = bounds.w - bounds.y - buttonHeight * 2;
			barLength = ((float)maxBarHeight) * proc - 1;
			barStep = ((float)maxBarHeight * (1.0f - proc)) / (float)max;
			
			barBounds = ivec4(absoluteBounds.x, absoluteBounds.y + buttonHeight + offset * barStep + 1, absoluteBounds.z, absoluteBounds.y + buttonHeight + offset * barStep + barLength);
			buttonBounds[0] = ivec4(bounds.x, bounds.y, bounds.z, bounds.y + buttonHeight);
			buttonBounds[1] = ivec4(bounds.x, bounds.w - buttonHeight, bounds.z, bounds.w - 1);
			visible = ((bounds.w - bounds.y) > 2 * WindowTitlebarHeight);
			
			trackBounds.x = absoluteBounds.x;
			trackBounds.y = absoluteBounds.y + buttonHeight;
			trackBounds.z = absoluteBounds.z;
			trackBounds.w = absoluteBounds.w - buttonWidth;
		} else {
			maxBarHeight = bounds.z - bounds.x - buttonWidth  * 2;
			barLength = ((float)maxBarHeight) * proc - 1;
			barStep = ((float)maxBarHeight * (1.0f - proc)) / (float)max;
			
			barBounds = ivec4(absoluteBounds.x + buttonWidth + offset * barStep + 1, absoluteBounds.y, absoluteBounds.x + buttonWidth + offset * barStep + barLength, absoluteBounds.w);
			buttonBounds[0] = ivec4(bounds.x, bounds.y, bounds.x + buttonWidth, bounds.w);
			buttonBounds[1] = ivec4(bounds.z - buttonWidth, bounds.y, bounds.z, bounds.w);
			visible = ((bounds.z - bounds.x) > 2 * WindowTitlebarHeight);
			
			trackBounds.x = absoluteBounds.x + buttonWidth;
			trackBounds.y = absoluteBounds.y;
			trackBounds.z = absoluteBounds.z - buttonWidth;
			trackBounds.w = absoluteBounds.w;
		}
		
		if (IsInside(trackBounds, MousePosition) && MouseButtons[0]) {
			if (vertical) {
				offset = (MousePosition.y - trackBounds.y + barLength / 2) / barLength;
			} else {
				offset = (MousePosition.x - trackBounds.x + barLength / 2) / barLength;
			}
		}
		
		if (visible) {
			// Draw background
			GUIDrawQuad(frameBuffer, absoluteBounds, RawFocusedColor);
			
			// Draw bar
			if (barLength > 2) {
				GUIDrawQuad(frameBuffer, barBounds, IsInside(barBounds, MousePosition) ? RawFocusedColor : RawDefaultColor);
				GUIDrawOutline(frameBuffer, barBounds, false);
			}

			// Draw buttons
			if (Button("-", GUI_DEFAULT_FLAGS_BUTTON, buttonBounds[0])) {
				offset = std::max(offset - 1, 0);
				ans = true;
			}
			if (Button("+", GUI_DEFAULT_FLAGS_BUTTON, buttonBounds[1])) {
				offset = std::min(offset + 1, max);
				ans = true;
			}
			
			// TODO: implement mouse drag
		}
	//	GUIDrawQuad(frameBuffer, trackBounds, ubvec4(255, 0, 0, 0));
	}
	
	return ans;
}

LayoutWrapper BeginPanel(Layout* bkp_layout, const Layout& layout) {
	LayoutWrapper ans = {currentLayout, 0};
	
	currentLayout.parent = ParentWindowRectangle;
	ParentWindowRectangle = currentLayout.getAbsoluteBounds(ivec4());
	*bkp_layout = currentLayout;

	currentLayout = layout;
	switch (currentLayout.type) {
	case Layout::GRID :
		SetGridLayout(currentLayout.grid.count.x, currentLayout.grid.count.y, Separator);
		SetGridElementWidth(currentLayout.grid.elementSize);
		break;
		
	}
	return ans;
}

void EndPanel(Layout* bkp_layout) {
	currentLayout = *bkp_layout;
	ParentWindowRectangle = currentLayout.parent;
}

LayoutWrapper BeginScrollPanel(Layout* bkp_layout, const uvec2& size, ivec2& offset) {
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(ivec4()); // ParentWindowRectangle;//
	const ivec2 extent = ivec2(absoluteBounds.z - absoluteBounds.x, absoluteBounds.w - absoluteBounds.y);
	const ivec2 diff = ivec2(size.x, size.y) - extent;
	ivec2 step;
	
	LayoutWrapper ans = BeginPanel(bkp_layout);
	
	SetAbsoluteLayout();
	if (diff.x > 0) {
		float w_proc = (float)extent.x / (float)size.x;
		const int32_t stepCount = (int32_t)size.x / extent.x;
		offset.x = std::min(offset.x, stepCount);
		step.x = diff.x / stepCount;
		currentLayout.separator = 0;
		ScrollBar(offset.x, stepCount, false, w_proc, ivec4((diff.y > 0) ? 28 : 0, 0, extent.x, WindowTitlebarHeight));
		offset.x = std::min(offset.x, stepCount);
	}
	if (diff.y > 0) {
		float h_proc = (float)extent.y / (float)size.y;
		const int32_t stepCount = (int32_t)size.y / extent.y;
		offset.y = std::min(offset.y, stepCount);
		step.y = diff.y / stepCount;
		currentLayout.separator = 0;
		ScrollBar(offset.y, stepCount, true, h_proc, ivec4(0, (diff.x > 0) ? 28 : 0, WindowTitlebarHeight, extent.y));
		offset.y = std::min(offset.y, stepCount);
	}
	
	if (diff.x > 0) {
		ParentWindowRectangle.x -= offset.x * step.x;
		ParentWindowRectangle.y += 28;
		ParentWindowRectangle.z = absoluteBounds.x + size.x - offset.x * step.x;
	}
	if (diff.y > 0) {
		ParentWindowRectangle.x += 28;
		ParentWindowRectangle.y -= offset.y * step.y;
		ParentWindowRectangle.w = absoluteBounds.y + size.y - offset.y * step.y;
	}
	
	return ans;
}

LayoutWrapper BeginGroup(Layout* bkp_layout, const char* text, const ivec4& bounds) {
	LayoutWrapper ans = BeginPanel(bkp_layout);
	
	const uint32_t k = TextSize * 7;
	const uint32_t l = k /2;
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	const ivec4 absoluteBounds2 = absoluteBounds  + ivec4(0, l,-0,-0);
	const ivec4 absoluteBounds3 = absoluteBounds2 + ivec4(l, l * 2,-l,-l);
	
	GUIDrawOutline2(frameBuffer, absoluteBounds2, true);
	ParentWindowRectangle = absoluteBounds3;
	
	const uint32_t width = absoluteBounds.z - absoluteBounds.x;
	const uint32_t height = absoluteBounds.w - absoluteBounds.y;
	if (height >= TextSize) {
	const uint32_t step = width / 4;
	const uint32_t textLength = std::min(step * 2, 7 * TextSize * (strlen(text) + 2));
	const ivec4 textBounds(absoluteBounds.x + step * 2 - textLength / 2, absoluteBounds.y + 1, absoluteBounds.x + step * 2 + textLength / 2, absoluteBounds.y + TextSize * 7 + 1);
	GUIDrawQuad(frameBuffer, textBounds, RawDefaultColor);
	DrawCenteredText(frameBuffer, 
					textBounds + 1, 
					text, 
					RawLightColor,
					TextSize);
	DrawCenteredText(frameBuffer, 
					textBounds, 
					text, 
					RawDarkColor / 2,
					TextSize);
	}
	return ans;
}

LayoutWrapper BeginSplitPanel(Layout* bkp_layout, bool vertical, float& weight, const ivec4& bounds) {
	LayoutWrapper ans = BeginPanel(bkp_layout, SplitLayout(vertical, weight));
	
	const ivec4 absoluteBounds = ParentWindowRectangle; // DAFUQ ?!
	const int width  = absoluteBounds.z - absoluteBounds.x;
	const int height = absoluteBounds.w - absoluteBounds.y;
	const int32_t halfPadding = Separator / 2;
	ivec4 separatorBounds;

	if (vertical) {
		const int k = (float)(width - Separator) * weight;
		separatorBounds = ivec4(absoluteBounds.x + k, absoluteBounds.y, absoluteBounds.x + k + Separator, absoluteBounds.w);

		const bool focused = IsInside(separatorBounds, MousePosition);
		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x + 2, separatorBounds.y), ivec2(separatorBounds.x + 2, separatorBounds.w), RawDarkColor);
		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x + 3, separatorBounds.y), ivec2(separatorBounds.x + 3, separatorBounds.w), focused ? RawSelectedColor : RawLightColor);

		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x + 5, separatorBounds.y), ivec2(separatorBounds.x + 5, separatorBounds.w), RawDarkColor);
		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x + 6, separatorBounds.y), ivec2(separatorBounds.x + 6, separatorBounds.w), focused ? RawSelectedColor : RawLightColor);
	} else {
		const int k = (float)height * weight;
		separatorBounds = ivec4(absoluteBounds.x, absoluteBounds.y + k, absoluteBounds.z, absoluteBounds.y + k + Separator);

		const bool focused = IsInside(separatorBounds, MousePosition);

		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x, separatorBounds.y + 2), ivec2(separatorBounds.z, separatorBounds.y + 2), RawDarkColor);
		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x, separatorBounds.y + 3), ivec2(separatorBounds.z, separatorBounds.y + 3), focused ? RawSelectedColor : RawLightColor);

		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x, separatorBounds.y + 5), ivec2(separatorBounds.z, separatorBounds.y + 5), RawDarkColor);
		GUIDrawLine(frameBuffer, ivec2(separatorBounds.x, separatorBounds.y + 6), ivec2(separatorBounds.z, separatorBounds.y + 6), focused ? RawSelectedColor : RawLightColor);
	}
	
	if ((draggedID == 0) && 
		(IsInside(separatorBounds, MousePosition) || IsInside(separatorBounds, LastMousePosition)) && 
		(MouseButtons[0] == true)) {
		if (LastMouseButtons[0]) {
			if (vertical) {
				weight = (float)(MousePosition.x - halfPadding - absoluteBounds.x) / (float)(width - Separator);
			} else {
				weight = (float)(MousePosition.y - halfPadding - absoluteBounds.y) / (float)height;
			}
			weight = std::min(std::max(weight, 0.0f), 1.0f);
		}
	}

	return ans;
}

LayoutWrapper BeginTabPanel(Layout* bkp_layout, const char** names, uint32_t count, int& selected, const ivec4& bounds) {
	//BeginPanel(AbsoluteLayout());
	LayoutWrapper ans = BeginPanel(bkp_layout, AbsoluteLayout());
	
	const ivec4 absoluteRectangle = currentLayout.getAbsoluteBounds(bounds);
	const int tabWidth = (absoluteRectangle.z - absoluteRectangle.x) / count;
	const int tabHeight = std::min(absoluteRectangle.w - absoluteRectangle.y, WindowTitlebarHeight);
	
	for (uint32_t index = 0; index < count; ++index) {
		if (Button(names[index], GUI_DEFAULT_FLAGS_BUTTON | ((index == selected) ? GUI_CLICKED | GUI_FOCUSED : 0), ivec4(index * tabWidth, 0, (index + 1) * tabWidth - 1, tabHeight))) {
			selected = index;
		}
	}
	
	ParentWindowRectangle = absoluteRectangle + ivec4(0, tabHeight + 1, 0, 0);
	GUIDrawOutline(frameBuffer, ParentWindowRectangle);
	
	return ans;
}

LayoutWrapper BeginWindow(Layout* bkp_layout, ivec4* rbounds, const char* text, uint32_t* flags) {
	LayoutWrapper ans = BeginPanel(bkp_layout);
	
	ivec4 bounds = currentLayout.getAbsoluteBounds(*rbounds);

	uint32_t defaultFlags = GUI_DEFAULT_FLAGS_WINDOW  & ~GUI_WINDOW_CLOSE;
	if (flags == NULL) {
		flags = & defaultFlags;
	}
	
	if ((*flags & GUI_VISIBLE) == 0) {
		++windowID;
		ans.index = 1;
		EndPanel(bkp_layout);
		return ans;
	}

	const int32_t width  = std::max(bounds.z - bounds.x, Separator * 2);
	const int32_t height = std::max(bounds.w - bounds.y, Separator * 2);

	if (bounds.z < bounds.x + Separator * 2) {
//		bounds.z = bounds.x + Separator * 2;
	}
	if (bounds.w < bounds.y + Separator * 2 + WindowTitlebarHeight) {
//		bounds.w = bounds.y + Separator * 2 + WindowTitlebarHeight;
	}

	static const uint32_t CloseButtonSize = WindowTitlebarHeight;
	const ivec4 titlebarRectangle(bounds.x, bounds.y, bounds.z - CloseButtonSize, bounds.y + WindowTitlebarHeight);
	if ((*flags & GUI_WINDOW_TITLEBAR) && (*flags & GUI_WINDOW_MOVE)) {
		if (MouseButtons[0] && LastMouseButtons[0] == false && IsInside(titlebarRectangle, MousePosition)) {
			draggedID = windowID;
		}
		if (((    MouseButtons[0] == true && IsInside(titlebarRectangle,     MousePosition)) ||
			 (LastMouseButtons[0] == true && IsInside(titlebarRectangle, LastMousePosition))) && 
			(draggedID == windowID) ) {
			ivec2 offset = MousePosition - LastMousePosition;
			bounds += ivec4(offset.x, offset.y, offset.x, offset.y);
		}	
	}
	
	// Draw titlebar
	ParentWindowRectangle = bounds;
	int offsetY = 0;
	if (*flags & GUI_WINDOW_TITLEBAR) {
		GUIDrawQuad(frameBuffer, ivec4(bounds.x, bounds.y, bounds.z, bounds.y + WindowTitlebarHeight), RawDarkColor);
		if (text) {
			DrawText(frameBuffer, uvec2(bounds.x, bounds.y) + 2, text, TextColor, TextSize);
		}
		const int width = bounds.z - bounds.x;
		if ((*flags & GUI_WINDOW_CLOSE) && Button("X", GUI_DEFAULT_FLAGS_BUTTON, ivec4(width - WindowTitlebarHeight + 1, 1, width - 1, WindowTitlebarHeight - 1))) {
			*flags &= ~GUI_VISIBLE;
		}
		offsetY += WindowTitlebarHeight;
	}

	ParentWindowRectangle = bounds + ivec4(currentLayout.separator, currentLayout.separator + offsetY, -currentLayout.separator, -currentLayout.separator);
	SetAbsoluteLayout();
	
	// Draw window body
	GUIDrawQuad(frameBuffer, bounds + ivec4(0, offsetY, 0, 0), RawDefaultColor);
	GUIDrawOutline(frameBuffer, bounds);
	
	if (*flags & GUI_WINDOW_SIZE) {
		for (uint32_t index = 1; index < 4; ++index) {
			GUIDrawLine(frameBuffer, ivec2(bounds.z - index * 5 + 0, bounds.w - 1), ivec2(bounds.z + 0, bounds.w - index * 5 + 0), RawLightColor);
			GUIDrawLine(frameBuffer, ivec2(bounds.z - index * 5 + 1, bounds.w + 0), ivec2(bounds.z + 0, bounds.w - index * 5 + 1), RawDarkColor);
		}
		const bool isInside  = ((draggedID == 0) && (    MousePosition.x >= bounds.z - 15) && (    MousePosition.x <= bounds.z) && (    MousePosition.y >= bounds.w - 15) && (    MousePosition.y <= bounds.w));
		const bool wasInside = ((draggedID == 0) && (LastMousePosition.x >= bounds.z - 15) && (LastMousePosition.x <= bounds.z) && (LastMousePosition.y >= bounds.w - 15) && (LastMousePosition.y <= bounds.w));
		if ((isInside || wasInside)&& MouseButtons[0]) {
			if (LastMouseButtons[0]) {
				ivec2 offset = MousePosition - LastMousePosition;
				bounds.z += offset.x;
				bounds.w += offset.y;
			}
		}
	}
	++windowID;
	if (rbounds) {
		*rbounds = bounds;
	}
	return ans;
}

Layout* GetLayout() {
	return &currentLayout;
}

LayoutWrapper BeginTable(Layout* bkp_layout, uint32_t collumnCount, uint32_t rowCount, const char** headers, int32_t& selected, uint32_t flags, const ivec4& bounds) {
	const int header_row = (int)(headers != NULL);
	const ivec4 absoluteBounds = currentLayout.getAbsoluteBounds(bounds);
	const ivec2 boundsSize(absoluteBounds.z - absoluteBounds.x, absoluteBounds.w - absoluteBounds.y);
	const ivec2 cellSize = boundsSize / ivec2(collumnCount, rowCount + header_row);

	const bool focused = ((draggedID == 0) && IsInside(absoluteBounds, MousePosition));
	const bool clicked = (focused && MouseButtons[0] && !LastMouseButtons[0]);
	
	LayoutWrapper ans = BeginPanel(bkp_layout);
	SetGridLayout(collumnCount, rowCount + header_row, 0);

	if (headers != NULL) {
		for (uint32_t index = 0; index < collumnCount; ++index) {
			Label(headers[index], GUI_ALIGN_CENTER | GUI_OUTLINE);// | GUI_BACKGROUND
		}
	}
	
	if (clicked) {
		int32_t new_selected = (MousePosition.y - absoluteBounds.y - header_row * cellSize.y) / cellSize.y;
		if (new_selected >= rowCount) {
			new_selected = -1;
		}
	}

	for (uint32_t index = 0; index < rowCount; ++index) {
		const ivec4 lineBounds(absoluteBounds.x, absoluteBounds.y + (index + header_row + 0) * cellSize.y, absoluteBounds.z, absoluteBounds.y + (index + header_row + 1) * cellSize.y);
		GUIDrawQuad(frameBuffer, lineBounds, (index == selected) ? RawSelectedColor : ((index % 2) ? RawDefaultColor : RawLightColor));
	}

	GUIDrawOutline(frameBuffer, absoluteBounds, true);
	
	return ans;
}

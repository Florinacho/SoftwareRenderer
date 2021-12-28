#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <stdint.h>
#include <string>

#include "Vector.h"

/* BILLBOARD BEGIN */
#warning BUG: Fix layout separator
#warning BUG: Fix window min size - crash of < X
#warning BUG: GUI_VISIBLE not implemented for Slider, rework implementation

#warning TODO: Implement container(panel) absolute bounds?
#warning TODO: Replace Layout::separator with Layout::padding[4]
#warning TODO: Replace getCellBoundsAdvance with explicit next element call
#warning TODO: Drop std::string from TextBox

#warning TEST: Table row selection broken?

#warning ENCH: Implement GUI_ALIGN for ProgressBar as a fill direction
/* BILLBOARD END   */

class Event;
class Image;

const uint32_t GUI_NONE        = 0;
const uint32_t GUI_VISIBLE     = 1 << 0;
const uint32_t GUI_ENABLED     = 1 << 1;
const uint32_t GUI_BACKGROUND  = 1 << 2;
const uint32_t GUI_FOREGROUND  = 1 << 3;
const uint32_t GUI_OUTLINE     = 1 << 4;
const uint32_t GUI_FOCUSED     = 1 << 5;
const uint32_t GUI_CLICKED     = 1 << 15;

const uint32_t GUI_WINDOW_TITLEBAR  = 1 << 6;
const uint32_t GUI_WINDOW_CLOSE     = 1 << 7;
const uint32_t GUI_WINDOW_MOVE      = 1 << 8;
const uint32_t GUI_WINDOW_SIZE      = 1 << 9;
const uint32_t GUI_WINDOW_DECORATION = GUI_WINDOW_TITLEBAR | GUI_WINDOW_CLOSE;// | GUI_WINDOW_SIZE;

const uint32_t GUI_HIDDEN           = 1 << 10;

const uint32_t GUI_ALIGN_LEFT   = 1 << 11;
const uint32_t GUI_ALIGN_BOTTOM = 1 << 12;
const uint32_t GUI_ALIGN_RIGHT  = 1 << 13;
const uint32_t GUI_ALIGN_TOP    = 1 << 14;

const uint32_t GUI_ALIGN_LEFT_TOP = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
const uint32_t GUI_ALIGN_LEFT_CENTER = GUI_ALIGN_TOP | GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
const uint32_t GUI_ALIGN_LEFT_BOTTOM = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
const uint32_t GUI_ALIGN_BOTTOM_CENTER = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT | GUI_ALIGN_RIGHT;
const uint32_t GUI_ALIGN_BOTTOM_RIGHT = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
const uint32_t GUI_ALIGN_CENTER_RIGHT = GUI_ALIGN_TOP | GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
const uint32_t GUI_ALIGN_TOP_RIGHT = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
const uint32_t GUI_ALIGN_TOP_CENTER = GUI_ALIGN_TOP | GUI_ALIGN_LEFT | GUI_ALIGN_RIGHT;
const uint32_t GUI_ALIGN_CENTER = GUI_ALIGN_TOP | GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT | GUI_ALIGN_RIGHT;

const uint32_t GUI_ALIGN_MASK = GUI_ALIGN_TOP | GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT | GUI_ALIGN_RIGHT;
const uint32_t GUI_ALIGN_OFFSET = 11;

const uint32_t GUI_DEFAULT_FLAGS_LABEL       = GUI_VISIBLE |               GUI_FOREGROUND |                                GUI_ALIGN_CENTER;
const uint32_t GUI_DEFAULT_FLAGS_BUTTON      = GUI_VISIBLE | GUI_ENABLED | GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE | GUI_ALIGN_CENTER;
const uint32_t GUI_DEFAULT_FLAGS_SPINNER     = GUI_VISIBLE | GUI_ENABLED | GUI_BACKGROUND | GUI_FOREGROUND |               GUI_ALIGN_CENTER;
const uint32_t GUI_DEFAULT_FLAGS_CHECKBOX    = GUI_VISIBLE | GUI_ENABLED | GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE;
const uint32_t GUI_DEFAULT_FLAGS_PALETTE     = GUI_VISIBLE | GUI_ENABLED;
const uint32_t GUI_DEFAULT_FLAGS_PROGRESSBAR = GUI_VISIBLE |               GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE;
const uint32_t GUI_DEFAULT_FLAGS_SLIDER      = GUI_VISIBLE | GUI_ENABLED | GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE;
const uint32_t GUI_DEFAULT_FLAGS_TEXTBOX     = GUI_VISIBLE | GUI_ENABLED | GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE | GUI_ALIGN_CENTER;
const uint32_t GUI_DEFAULT_FLAGS_WINDOW      = GUI_VISIBLE | GUI_ENABLED | GUI_BACKGROUND | GUI_FOREGROUND | GUI_OUTLINE | GUI_WINDOW_MOVE  | GUI_WINDOW_DECORATION;

enum Direction {
	LEFT,
	DOWN,
	RIGHT,
	UP,
	
	DIRECTION_COUNT
};

#if defined(ABSOLUTE)
	#undef ABSOLUTE
#endif // ABSOLUTE

struct Layout {
	enum Type : uint8_t {
		ABSOLUTE,
		GRID,
		SPLIT
	};
	union {
		struct {
			ivec2 count;
			ivec2 cellSize;
			int elementSize;
		} grid;
		struct {
			float weight;
			bool vertical;
		} split;
	};
	ivec4 parent;
	uvec2 max;
	int separator;
	// uint8_t padding[DIRECTION_COUNT]; // replace separator
	int elementIndex;
	Type type;

	Layout();
	Layout(const Layout& other);
	
	Layout& operator = (const Layout& other);
	
	ivec4 getCellBounds(const ivec4& bounds);
	ivec4 getCellBoundsAdvance(const ivec4& bounds);
	ivec4 getAbsoluteBounds(const ivec4& bounds) const;
};

typedef struct {
    Layout layout;
    int index;
} LayoutWrapper;

Layout AbsoluteLayout();

Layout SplitLayout(bool vertical, float weight = 0.5f);

Layout GridLayout(uint32_t x = 1, uint32_t y = 1, uint32_t elementSize = 1);
	
/*****************************************************************************/
/* Config functions.                                                         */
/*****************************************************************************/
void SetFrameBuffer(Image* value);

Image* GetFrameBuffer();

void ProcessEvent(const Event* event);

void PostEvent();

/*****************************************************************************/
/* Layout functions.                                                         */
/*****************************************************************************/
void SetAbsoluteLayout();

void SetSplitLayout(bool vertical, float weight);

void SetGridLayout(int x, int y, int padding = 8);
void SetGridElementWidth(int value);

Layout* GetLayout();

/*****************************************************************************/
/* GUI element                                                               */
/* Params: count - number of elements to skip                                */
/*****************************************************************************/
void DummyElement(uint32_t count = 1);

/*****************************************************************************/
/* LABEL                                                                     */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_BACKGROUND                                                     */
/*        GUI_FOREGROUND                                                     */
/*        GUI_OUTLINE                                                        */
/*        GUI_ALIGN_*                                                        */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_LABEL                                    */
/*                                                                           */
/*****************************************************************************/
void Label(const char* text, 
           uint32_t flags = GUI_DEFAULT_FLAGS_LABEL, 
           const ivec4& bounds = ivec4());

/*****************************************************************************/
/* BUTTON                                                                    */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_ENABLED                                                        */
/*        GUI_BACKGROUND                                                     */
/*        GUI_FOREGROUND                                                     */
/*        GUI_OUTLINE                                                        */
/*        GUI_ALIGN_*                                                        */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_BUTTON                                   */
/*                                                                           */
/*****************************************************************************/
bool Button(const char* text,
            uint32_t flags = GUI_DEFAULT_FLAGS_BUTTON,
            const ivec4& bounds = ivec4());

/*****************************************************************************/
/* CHECKBOX                                                                  */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_ENABLED                                                        */
/*        GUI_BACKGROUND                                                     */
/*        GUI_FOREGROUND                                                     */
/*        GUI_OUTLINE                                                        */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_CHECKBOX                                 */
/*                                                                           */
/*****************************************************************************/
bool CheckBox(bool& checked,
              uint32_t flags = GUI_DEFAULT_FLAGS_CHECKBOX,
              const ivec4& bounds = ivec4());

/*****************************************************************************/
/* SPINNER                                                                   */
/* Components: Label                                                         */
/*             Button                                                        */
/*                                                                           */
/* Flags:                                                                    */
/*   Label:  GUI_BACKGROUND                                                  */
/*           GUI_FOREGROUND                                                  */
/*           GUI_OUTLINE                                                     */
/*           GUI_ALIGN_*                                                     */
/*                                                                           */
/*   Button: GUI_ENABLED                                                     */
/*                                                                           */
/*   All:    GUI_VISIBLE                                                     */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_LABEL                                    */
/*                                                                           */
/*****************************************************************************/
bool Spinner(int& value,
             uint32_t flags = GUI_DEFAULT_FLAGS_SPINNER,
             const ivec4& bounds = ivec4());

/*****************************************************************************/
/* PALETTE                                                                   */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_ENABLED                                                        */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_PALETTE                                  */
/*                                                                           */
/*****************************************************************************/
bool Palette(ubvec4& color,
             uint32_t flags = GUI_DEFAULT_FLAGS_PALETTE,
             const ivec4& bounds = ivec4());

/*****************************************************************************/
/* PALETTE                                                                   */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_BACKGROUND                                                     */
/*        GUI_FOREGROUND                                                     */
/*        GUI_OUTLINE                                                        */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_PROGRESSBAR                              */
/*                                                                           */
/*****************************************************************************/
void ProgressBar(float progress,
                 uint32_t flags = GUI_DEFAULT_FLAGS_PROGRESSBAR,
                 const ivec4& bounds = ivec4());

/*****************************************************************************/
/* SLIDER                                                                    */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_ENABLED                                                        */
/*        GUI_BACKGROUND                                                     */
/*        GUI_FOREGROUND                                                     */
/*        GUI_OUTLINE                                                        */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_SLIDER                                   */
/*                                                                           */
/*****************************************************************************/
bool Slider(float& value,
            uint32_t flags = GUI_DEFAULT_FLAGS_SLIDER,
            const ivec4& bounds = ivec4());

/*****************************************************************************/
/* SLIDER                                                                    */
/*                                                                           */
/* Flags: GWF_VISIBLE                                                        */
/*        GUI_ENABLED                                                        */
/*        GUI_BACKGROUND                                                     */
/*        GUI_FOREGROUND                                                     */
/*        GUI_OUTLINE                                                        */
/*        GUI_ALIGN_*                                                        */
/*        GUI_HIDDEN                                                         */
/*                                                                           */
/* Default flags: GUI_DEFAULT_FLAGS_TEXTBOX                                  */
/*                                                                           */
/* Params: text   - input/output text to be displayed                        */
/*         carrot - cursor position                                          */
/*                - value '-1' indicates that the textbox is not active      */
/*         flags                                                             */
/*         bounds                                                            */
/*                                                                           */
/*****************************************************************************/
bool TextBox(std::string& text,
             int& carrot,
             uint32_t flags = GUI_DEFAULT_FLAGS_TEXTBOX,
             const ivec4& bounds = ivec4());

bool ColorSelector(ivec4& bounds,
                   ubvec4& color, 
                   uint32_t* flags = NULL);

bool ListBox(int&,
             int&,
             const char**,
             uint32_t,
             const ivec4& = ivec4());

bool ScrollBar(int&,
               int,
               bool,
               float = 1.0f,
               const ivec4& bounds = ivec4());

/*****************************************************************************/
/* GUI containers                                                            */
/*****************************************************************************/
LayoutWrapper BeginWindow(Layout* backup,
                          ivec4* bounds,
                          const char* title = NULL,
                          uint32_t* flags = NULL);

LayoutWrapper BeginPanel(Layout* backup,
                         const Layout& layout = Layout());

LayoutWrapper BeginSplitPanel(Layout* backup,
                              bool vertical,
                              float& proc,
                              const ivec4& bounds = ivec4());

LayoutWrapper BeginScrollPanel(Layout* backup,
                               const uvec2&,
                               ivec2&);

LayoutWrapper BeginTabPanel(Layout* backup,
                            const char** header_list,
                            uint32_t header_count,
                            int& selected_row,
                            const ivec4& bounds = ivec4());

LayoutWrapper BeginGroup(Layout* backup,
                         const char* title,
                         const ivec4& bounds = ivec4());

LayoutWrapper BeginTable(Layout* backup,
                         uint32_t,
                         uint32_t,
                         const char**,
                         int32_t&,
                         uint32_t,
                         const ivec4& bounds = ivec4());
	
void EndPanel(Layout*);

#define EndWindow EndPanel

#define EndSplitPanel EndPanel

#define EndScrollPanel EndPanel

#define EndTabPanel EndPanel

#define EndGroup EndPanel

#define EndTable EndPanel

#define Panel(new_layout)                                                                                             \
            for(LayoutWrapper ___tmp = BeginPanel(&___tmp.layout, new_layout);                                        \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndPanel(&___tmp.layout))
				
#define SplitPanel(vertical, weight)                                                                                  \
            for(LayoutWrapper ___tmp = BeginSplitPanel(&___tmp.layout, vertical, weight);                             \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndSplitPanel(&___tmp.layout))
				
#define TabPanel(names, count, selected)                                                                              \
            for(LayoutWrapper ___tmp = BeginTabPanel(&___tmp.layout, names, count, selected);                         \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndTabPanel(&___tmp.layout))
				
#define Group(text)                                                                                                   \
            for(LayoutWrapper ___tmp = BeginGroup(&___tmp.layout, text);                                              \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndGroup(&___tmp.layout))
				
#define Window(bounds, title, flags)                                                                                  \
            for(LayoutWrapper ___tmp = BeginWindow(&___tmp.layout, bounds, title, flags);                             \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndWindow(&___tmp.layout))
				
#define ScrollPanel(bounds, offset)                                                                                   \
            for(LayoutWrapper ___tmp = BeginScrollPanel(&___tmp.layout, bounds, offset);                              \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndScrollPanel(&___tmp.layout))
				
#define Table(collumnCount, rowCount, headers, selected, flags)                                                       \
            for(LayoutWrapper ___tmp = BeginTable(&___tmp.layout, collumnCount, rowCount, headers, selected, flags);  \
                !___tmp.index;                                                                                        \
                (___tmp.index += 1), EndTable(&___tmp.layout))

#endif // __GUI_HPP__
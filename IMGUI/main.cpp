#include <stdio.h>

#include "Window.h"
#include "Timer.h"
#include "GUI.hpp"

uint32_t resizeWindowFlags = GUI_VISIBLE | GUI_WINDOW_DECORATION | GUI_WINDOW_MOVE | GUI_WINDOW_SIZE;
uint32_t colorChooserFlags = GUI_VISIBLE | GUI_WINDOW_DECORATION | GUI_WINDOW_MOVE;
uint32_t fps = 0;

void ColorChooser() {
	static ivec4 colorChooserBounds(0, 0, 304, 236);
	static ubvec4 color(0, 0, 0, 255);
	
	if (ColorSelector(colorChooserBounds, color, &colorChooserFlags)) {
		printf("ColorSelector: %u, %u, %u, %u\n", color.x, color.y, color.z, color.w);
	}
}

void UtilWindow() {
	static ivec4 utilWindowBounds(0, 237, 304, 637);
	static uint32_t utilWindowFlags = GUI_DEFAULT_FLAGS_WINDOW &  (GUI_DEFAULT_FLAGS_WINDOW ^ GUI_WINDOW_CLOSE);
	
	Window(&utilWindowBounds, "UNTITLED", &utilWindowFlags) {
		SetAbsoluteLayout();
		if (Button("TITLEBAR", GUI_DEFAULT_FLAGS_BUTTON, ivec4(8, 8, 128, 28))) {
			resizeWindowFlags ^= GUI_WINDOW_TITLEBAR;
		}
		if (Button("VISIBLE", GUI_DEFAULT_FLAGS_BUTTON, ivec4(8, 36, 128, 56))) {
			resizeWindowFlags ^= GUI_VISIBLE;
		}
		if (Button("MOVABLE", GUI_DEFAULT_FLAGS_BUTTON, ivec4(8, 64, 128, 84))) {
			resizeWindowFlags ^= GUI_WINDOW_MOVE;
		}
		if (Button("SIZABLE", GUI_DEFAULT_FLAGS_BUTTON, ivec4(8, 92, 128, 112))) {
			resizeWindowFlags ^= GUI_WINDOW_SIZE;
		}
		static int scrollbarValue = 0;
		ScrollBar(scrollbarValue, 3, true,  0.33f, ivec4(136, 8, 160, 112));
		ScrollBar(scrollbarValue, 3, false, 0.33f, ivec4(8, 120, 128, 140));

		static bool checkboxValue = false;
		CheckBox(checkboxValue, GUI_DEFAULT_FLAGS_CHECKBOX, ivec4(8, 148, 28, 168));

		static int spinerValue = 123;
		Spinner(spinerValue, GUI_DEFAULT_FLAGS_SPINNER, ivec4(8, 176, 128, 196));

		static float sliderValue = 0.5f;
		Slider(sliderValue, GUI_DEFAULT_FLAGS_SLIDER, ivec4(168, 8, 188, 112));
		Slider(sliderValue, GUI_DEFAULT_FLAGS_SLIDER, ivec4(8, 206, 128, 226));
	}
}

void ResizeWindow() {
	static ivec4 resizeWindowBounds(305, 0, 800, 637);
	
	Window(&resizeWindowBounds, "Resize window", &resizeWindowFlags) {
		static float weight = 0.5f;
		SplitPanel(false, weight) {
			static const char* tabNames[] = {"Lists", "Split", "FPS", "Menu", "Con", "TABLE"};
			static const uint32_t tabCount = sizeof(tabNames) / sizeof(tabNames[0]);
			static int selectedTab = 0;

			TabPanel(tabNames, tabCount, selectedTab) {
				switch (selectedTab) {
				case 0 :
					static int listBox1Selected = 0;
					static int listBox2Selected = 0;
					static int listBox2Offset = 0;
					static const char* ListBoxItems[] = {
						"Line 0",
						"Line 1",
						"Line 2",
						"Line 3",
						"Line 4",
						"Line 5",
						"Line 6",
						"Line 7",
						"Line 8",
						"Line 9",
					};
					static float weight = 0.5f;
					SplitPanel(true, weight) {
						static int offset = 0;
						if (ListBox(listBox1Selected, offset, ListBoxItems, 10)) {
							printf("ListBox: %d\n", listBox1Selected);
						}
						static float tp = 0.5f;
						SplitPanel(false, tp) {
							Label("Hello there");
							if (ListBox(listBox2Selected, listBox2Offset, ListBoxItems, 10)) {
								printf("ListBox: %d\n", listBox2Selected);
							}
						}
					}
					break;
					
				case 1 :
					static float spp1 = 0.5f;
					static float spp2 = 0.5f;
					SplitPanel(false, spp1) {
						Label("Hello");
						SplitPanel(true, spp2) {
							Label("from the");
							Label("other side");
						}
					}
					break;
					
				case 2 :
					Panel(GridLayout(1, 1)) {
						Group("TEST") {
							Panel(SplitLayout(false, 0.85f)) {
								static char tmp[32];
								sprintf(tmp, "FPS: %d", fps);
								Label(tmp);
								
								static float progress = 0.0f;
								progress = fmod(progress + 0.001f, 1.0f);
								Layout* layout = GetLayout();
								layout->max.y = 20;
								ProgressBar(progress);
							}
						}
					}
					break;
				case 3 :
					Panel(GridLayout(4, 9)) {
						SetGridElementWidth(4);
						Label("Options");
						SetGridElementWidth(1);

						
						{
						static float value = 0.3f;
						Label("Volume", GUI_VISIBLE | GUI_FOREGROUND | GUI_ALIGN_LEFT_TOP);
						DummyElement();
						Slider(value);
						DummyElement();
						}
						
						{
						static int value = 1;
						Label("Slots", GUI_VISIBLE | GUI_FOREGROUND  | GUI_ALIGN_LEFT_TOP);
						DummyElement();
						Spinner(value);
						DummyElement();
						}
						
						{
						static bool value = true;
						Label("Subtitle", GUI_VISIBLE | GUI_FOREGROUND  | GUI_ALIGN_LEFT_TOP);
						DummyElement();
						CheckBox(value);
						DummyElement();
						}
						
						{
						Label("Account", GUI_VISIBLE | GUI_FOREGROUND  | GUI_ALIGN_LEFT_TOP);
						DummyElement();
						SetGridElementWidth(2);
						static std::string value;
						static int carrot = -1;
						if (TextBox(value, carrot)) {
							printf("TextBox1: \"%s\"\n", value.c_str());
						}
						SetGridElementWidth(1);
						}
						
						{
						Label("Password", GUI_VISIBLE | GUI_FOREGROUND  | GUI_ALIGN_LEFT_TOP);
						DummyElement();
						SetGridElementWidth(2);
						static std::string value;
						static int carrot = -1;
						if (TextBox(value, carrot, GUI_DEFAULT_FLAGS_TEXTBOX | GUI_HIDDEN)) {
							printf("TextBox2: \"%s\"\n", value.c_str());
						}
						SetGridElementWidth(1);
						}

						for (uint32_t index = 0; index < 2; ++index) {
							DummyElement(4);
						}
						
						DummyElement();
						Button("BACK", GUI_VISIBLE | GUI_ENABLED | GUI_FOREGROUND | GUI_OUTLINE);
						Button("APPLY");
					}
					break;
				case 4 :
					Panel(SplitLayout(false, 0.9f)) {
						static std::string labelText = "";
						static std::string textBoxText = "";
						static int carrot = -1;
						Label(labelText.c_str(), GUI_VISIBLE | GUI_FOREGROUND);
						Panel(SplitLayout(true, 0.9f)) {
							TextBox(textBoxText, carrot);
							if (Button("Send")) {
								labelText += "\n";
								labelText += textBoxText;
								textBoxText = "";
								carrot = 0;
							}
						}
					}
					break;
				case 5 :
					{
					static const char* TABLE_HEADERS[] = {
						"NAME",
						"AGE",
						"SOMETHING",
					};
					static const uint32_t COLLUMN_COUNT = sizeof(TABLE_HEADERS) / sizeof(TABLE_HEADERS[0]);
					static const uint32_t ROW_COUNT = 16;
					static int selected = -1;
					static int value = 20;
					Table(COLLUMN_COUNT, ROW_COUNT, TABLE_HEADERS, selected, 0) {
						Label("Test 1");
						static int age = 20;
						Spinner(age);
						static bool something = true;
						CheckBox(something);
					}
					}
					break;
				}
			}

			Panel(GridLayout(1, 3)) {
				for (uint32_t index = 0; index < 3; ++index) {
					static char tmp[32];
					snprintf(tmp, sizeof(tmp), "Group %d", index);
					Group(tmp) {
						SetGridLayout(5, 1);
						Label("Label");
						
						Panel(GridLayout(2, 2)) {
							static bool checked[] = {false, true, false};
							
							Label("AA");
							if (CheckBox(checked[index])) {
								printf("Checkbox1: %d\n", checked[index]);
							}
							
							Label("BB");
							if (CheckBox(checked[index])) {
								printf("Checkbox2: %d\n", checked[index]);
							}
						}
						
						Panel(GridLayout(1, 3)) {
							Layout* layout = GetLayout();
							layout->max.y = 20;
							static int values[] = {0, 0, 0};
							if (Spinner(values[index])) {
								printf("Spinner %d: %d\n", index, values[index]);
							}
							if (Spinner(values[index])) {
								printf("Spinner %d: %d\n", index, values[index]);
							}
							if (Spinner(values[index])) {
								printf("Spinner %d: %d\n", index, values[index]);
							}
						}

						SetGridElementWidth(2);
							if (Button("Button")) {
								printf("Button %d pressed\n", index);
							}
						SetGridElementWidth(1);
					}
				}
			}
		}
	}
}

void ScrollPanelTest() {
	static ivec4 bounds(0, 0, 400, 200);
	static uint32_t flags = GUI_DEFAULT_FLAGS_WINDOW;
	colorChooserFlags |= GUI_WINDOW_SIZE;
	static ivec2 offset = 0;
	Window(&bounds, "SCROLL", &colorChooserFlags) {
		ScrollPanel(uvec2(800, 350), offset) {
			SetGridLayout(5, 2);
//			Panel(GridLayout(5, 2)) {
				char tmp[8];
				for (uint32_t row = 0; row < 2; ++row) {
					for (uint32_t col = 0; col < 5; ++col) {
						snprintf(tmp, sizeof(tmp), "%d.%d", row + 1, col + 1);
/**
						if (Button(tmp)) {
							printf("Button %s pressed!\n", tmp);
						}
/**/
					}
				}
			}
//		}
	}
}

void TaskBar(bool& startMenuVisible) {
	static uint32_t flags = GUI_VISIBLE;
	static ivec4 bounds(0, 690, 1024, 720);

	Window(&bounds, "", &flags) {
		Panel(SplitLayout(true, 0.1f)) {
			if (Button("START")) {
				startMenuVisible = !startMenuVisible;
			}
			Panel(SplitLayout(true, 0.9f)) {
				static const uint32_t TaskCount = 8;
				Panel(GridLayout(TaskCount, 1)) {
					for(uint32_t index = 0; index < 3; ++index) {
						Button("test");
					}
				}
				Label("11:28");
			}
		}
	}
}

void StartMenu() {
	static uint32_t flags = GUI_VISIBLE;
	static ivec4 bounds(0, 350, 250, 690);
//	SetAbsoluteLayout();
	Window(&bounds, "", &flags) {
		static int selected = 0;
		static const char* ListBoxItems[] = {
			"PROGRAMS",
			"FAVORITES",
			"DOCUMENTS",
			"SETTINGS",
			"FIND",
			"HELP",
			"RUN",
			"LOG OFF",
			"SHUTDOWN",
		};
		static int offset = 0;
		if (ListBox(selected, offset, ListBoxItems, 9)) {
			printf("ListBox: %d\n", selected);
		}
	}
}

int main(int argc, char* argv[]) {
	Win32Window output;
	if (output.initialize(uvec2(1024, 720), Image::EPF_R8G8B8A8, "Immediate GUI") == false) {
		printf("Failed to initialize the win32 window.\n");
		return 1;
	}

	Image colorBuffer;
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping = Image::EWT_DISCARD;
	SetFrameBuffer(&colorBuffer);
	
	Event event;
	bool running = true;
	uint64_t lastSecondTime = Timer::GetMilliSeconds();
	uint32_t frameCount = 0;
	fps = 0;
	
	printf("sizeof(Layout) = %lu\n", sizeof(Layout));
	printf("sizeof(LayoutWrapper) = %lu\n", sizeof(LayoutWrapper));
	
	while (running) {
		const uint64_t currentTime = Timer::GetMilliSeconds();
		if (currentTime - lastSecondTime >= 1000ULL) {
			fps = frameCount;
			frameCount = 0;
			lastSecondTime += 1000ULL;
		}
		
		while (output.getEvent(&event)) {
			ProcessEvent(&event);
			
			switch (event.type) {
			case Event::WINDOW_CLOSE :
				running = false;
				break;
				
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_SPACE :
					if (event.state == 1) {
						colorChooserFlags ^= GUI_VISIBLE;
					}
					break;
					
				case KEY_ESCAPE :
					if (event.state == 0) {
						running = false;
					}
					break;
				}
				break;
			}
		}

		colorBuffer.clear();

		static bool startMenuVisible = false;
		TaskBar(startMenuVisible);
		ColorChooser();
		UtilWindow();
		ResizeWindow();
		
		if (startMenuVisible) {
			StartMenu();
		}
		output.blit(&colorBuffer);
		
		PostEvent();
		++frameCount;
	}
	
	return 0;
}
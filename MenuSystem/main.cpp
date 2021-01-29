#include <stdio.h>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include <linux/input.h>
#include "Input.h"
#include "FrameBuffer.h"
#else
#error Unsupported platform!
#endif

#include "GameMenu.h"

int main(int argc, char* argv[]) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(800, 600);
#if defined (_WIN32)
	Win32Window output;
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8A8, "Menu system") == false) {
		printf("Failed to initialize the win32 window.\n");
		return 1;
	}
#elif defined (__linux__)
	FrameBuffer output;
	if (output.initialize(NULL, ScreenSize, Image::EPF_R8G8B8A8) != 0) {
		printf("Failed to initialize the frame buffer.\n");
		return 1;
	}
	output.input.addAllInputs();
#endif
	Image colorBuffer;
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping = Image::EWT_DISCARD;

	/*************************************************************************/
	/* Menu system                                                           */
	/*************************************************************************/
	MenuManager menuManager(&MainMenu);

	/*************************************************************************/
	/* Misc.                                                                 */
	/*************************************************************************/
	Event event;

	while (running) {
		/*********************************************************************/
		/* Check keyboard events.                                            */
		/*********************************************************************/
		while (output.getEvent(&event)) {
			if (menuManager.onEvent(event)) {
				continue;
			}

			switch (event.type) {
			case Event::WINDOW_CLOSE :
				running = false;
				break;
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_ESCAPE :
					if (event.state == 0) {
						if (inGame) {
							menuManager.setActiveMenu(&InGameMenu);
						} else {
							running = false;
						}
					}
					break;
				}
				break;
			}
		}

		/*********************************************************************/
		/* GUI                                                               */
		/*********************************************************************/	
		colorBuffer.clear();
		menuManager.draw(&colorBuffer);

		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
	}

	return 0;
}
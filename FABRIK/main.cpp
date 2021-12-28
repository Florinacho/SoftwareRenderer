#include <stdio.h>
#include <vector>
#include <algorithm>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include <linux/input.h>
#include "Input.h"
#include "FrameBuffer.h"
#else
#error Unsupported platform!
#endif
#include "Timer.h"

static const int maxIterations = 100;
static const float minAcceptableDst = 0.0001f;

void ResolveKinematics(std::vector<vec2>& points, const vec2& target) {
	vec2 origin = points[0];
	std::vector<vec2> segmentLengths(points.size() - 1);
	for (int i = 0; i < points.size() - 1; ++i) {
		segmentLengths[i] = (points[i + 1] - points[i]).magnitude();	
	}

	for (int iteration = 0; iteration < maxIterations; ++iteration) {
		bool startingFromTarget = ((iteration % 2) == 0);

		std::reverse(points.begin(), points.end());
		std::reverse(segmentLengths.begin(), segmentLengths.end());
		points[0] = (startingFromTarget) ? target : origin;

		for (int i = 1; i < points.size(); ++i) {
			vec2 dir = (points[i] - points[i - 1]).normalize();
			points[i] = points[i - 1] + dir * segmentLengths[i - 1];
		}

		float dstToTarget = (points[points.size() - 1] - target).magnitude();
		if (!startingFromTarget && dstToTarget <= minAcceptableDst) {
			return;
		}
	}
}

int main(int argc, char* argv[]) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(800, 600);
#if defined (_WIN32)
	Win32Window output;
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8A8, "Cloth") == false) {
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
	/* Misc.                                                                 */
	/*************************************************************************/
	Event event;
	bool running = true;

	unsigned long lastSecondTime = Timer::GetMilliSeconds();
	unsigned int frameCount = 0;
	unsigned int lastFPS = 0;

	std::vector<vec2> points;
	vec2 mousePosition;
	vec2 target;

	for (uint32_t index = 0; index < 4; ++index) {
		points.push_back(vec2(200 + index * 100 + index * 50, 300));
	}

	while (running) {
		const unsigned long currentFrameTime = Timer::GetMilliSeconds();
		if (currentFrameTime - lastSecondTime >= 1000) {
			printf("FPS: %d\n", frameCount);
			frameCount = 0;
			lastSecondTime += 1000;
		}
		/*********************************************************************/
		/* Check keyboard events.                                            */
		/*********************************************************************/
		while (output.getEvent(&event)) {
			switch (event.type) {
			case Event::MOUSE_POSITION :
				mousePosition.x = event.x;
				mousePosition.y = event.y;
				break;
			case Event::MOUSE_BUTTON :
				switch (event.button) {
				case 0 :
					if (event.state == 1) {
						points[0] = mousePosition;
					}
					break;
				case 2 :
					if (event.state == 1) {
						target = mousePosition;
					}
					break;
				}
				break;
			case Event::WINDOW_CLOSE :
				running = false;
				break;
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_SPACE :
					ResolveKinematics(points, target);
					break;
				}
				break;
			}
		}

		colorBuffer.clear();

		colorBuffer.drawFilledRectangle(ivec4(target.x - 8, target.y - 8, target.x + 8, target.y + 8), ubvec4(0, 255, 255, 255));

		for (uint32_t index = 0; index < points.size() - 1; ++index) {
			colorBuffer.drawLine(ivec2(points[index].x, points[index].y), ivec2(points[index + 1].x, points[index + 1].y), ubvec4(255, 255, 255, 255));
		}

		for (uint32_t index = 0; index < points.size(); ++index) {
			static const ubvec4 colors[4] = {
				ubvec4(255,   0,   0, 255),
				ubvec4(  0, 255,   0, 255),
				ubvec4(  0,   0, 255, 255),
				ubvec4(255, 255,   0, 255),
			};
			colorBuffer.drawFilledCircle(ivec2(points[index].x, points[index].y), 16, colors[index]);
		}

		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
		++frameCount;
	}

	return 0;
}
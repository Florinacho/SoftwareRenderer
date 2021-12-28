#include <stdio.h>
#include <vector>

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

static const uint32_t MAX_CHAIN_ITERATION_COUNT = 10;
static const float    GRAVITY_ACCELERATION = 9.8f;

struct Point {
	vec2 position, prevPosition;
	bool locked;
	
	Point() {
		locked = false;
	}
};

struct Link {
	Point &A, &B;
	float length;

	Link(Point& a, Point& b) : A(a), B(b) {
		length = A.position.getDistanceFrom(B.position);
	}
	
	Link& operator = (const Link& other) {
		A = other.A;
		B = other.B;
		length = other.length;
		return *this;
	}
};

void SimulateChain(std::vector<Point>& points, 
                   const std::vector<Link>& links, 
			       const float deltaTime) {
	for (uint32_t index = 0; index < points.size(); ++index) {
		if (points[index].locked) {
			continue;
		}
		const vec2 positionBeforeUpdate = points[index].position;
		points[index].position += points[index].position - points[index].prevPosition;
		points[index].position += vec2(0.0f, GRAVITY_ACCELERATION) * deltaTime * deltaTime;
		points[index].prevPosition = positionBeforeUpdate;
	}

	for (uint32_t iteration = 0; iteration < MAX_CHAIN_ITERATION_COUNT; ++iteration) {
		for (uint32_t index = 0; index < links.size(); ++index) {
			const vec2 center = (links[index].A.position + links[index].B.position) / 2;
			const vec2 dir    = (links[index].A.position - links[index].B.position).normalize();
			if (links[index].A.locked == false) {
				links[index].A.position = center + dir * links[index].length / 2;
			}
			if (links[index].B.locked == false) {
				links[index].B.position = center - dir * links[index].length / 2;
			}
		}
	}
}

ivec2 v2ftoi(const vec2& value) {
	return ivec2(value.x, value.y);
}

int main(int argc, char* argv[]) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(800, 600);
#if defined (_WIN32)
	Win32Window output;
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8A8, "Chain Physics") == false) {
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
	
	std::vector<Point> points;
	points.reserve(32);
	std::vector<Link> links;
	vec2 mousePosition;
	
	int dragIndex = -1;
	bool runSimulation = false;
	
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
						dragIndex = -1;
						for (uint32_t index = 0; index < points.size(); ++index) {
							if (mousePosition.getDistanceFrom(points[index].position) < 8.0f) {
								dragIndex = index;
								break;
							}
						}
						if (dragIndex == -1) {
							Point point;
							point.position = mousePosition;
							point.prevPosition = mousePosition;
							points.push_back(point);
						}
					} else {
						if (dragIndex >= 0) {
							for (uint32_t index = 0; index < points.size(); ++index) {
								if ((dragIndex != index) && (mousePosition.getDistanceFrom(points[index].position) < 8.0f)) {
									links.push_back(Link(points[dragIndex], points[index]));
									break;
								}
							}
							dragIndex = -1;
						}
					}
					break;
				case 2 :
					if (event.state == 1) {
						for (uint32_t index = 0; index < points.size(); ++index) {
							if (mousePosition.getDistanceFrom(points[index].position) < 8.0f) {
								points[index].locked = !points[index].locked;
								break;
							}
						}
					}
					break;
				}
				break;
			case Event::WINDOW_CLOSE :
				running = false;
				break;
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_ESCAPE :
					running = false;
					break;
				case KEY_SPACE :
					if (event.state == 1)
						runSimulation = !runSimulation;
					break;
				}
				break;
			}
		}
		
		colorBuffer.clear();
		
		if (runSimulation) {
			SimulateChain(points, links, 0.01f);
		}

		for (int index = 0; index < (int)links.size(); ++index) {
			colorBuffer.drawLine(v2ftoi(links[index].A.position), v2ftoi(links[index].B.position), ubvec4(255));
		}
		
		for (uint32_t index = 0; index < points.size(); ++index) {
			colorBuffer.drawFilledCircle(v2ftoi(points[index].position), 
			                             8.0f, 
										 points[index].locked ? ubvec4(255, 0, 0, 255) : ubvec4(255, 255, 255, 255));
		}
		
		if (dragIndex >= 0) {
			colorBuffer.drawLine(v2ftoi(points[dragIndex].position), v2ftoi(mousePosition), ubvec4(255, 0.0f, 0.0f, 0.0f));
		}

		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
		++frameCount;
	}

	return 0;
}
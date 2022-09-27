#include <stdio.h>
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

vec2 GetRot(float angle) {
	const float rad = (angle + 45.0f) * (M_PI / 180.0f);
	const float cs = cosf(rad);
	const float sn = sinf(rad);
	return {cs - sn, sn + cs};
}

float lerp(float value, float target, float proc) {
	return (target - value) * proc;
}

struct Car {
	vec2 position;
	vec2 size;
	float rotation;     // car body rotation
	float velocity;
	float acceleration;
	float friction;
	float direction;    // wheel relative rotation
	float maxTurnSpeed;
	float maxTurnAngle;
	float maxForwardAcceleration;
	float maxBackwardAcceleration;
	
	void update(bool* keys, float deltaTime = 1.0f) {
		// Update direction
		if (keys[1]) {
			direction = std::max(direction - deltaTime * maxTurnSpeed, -maxTurnAngle);
		} else if (keys[3]) {
			direction = std::min(direction + deltaTime * maxTurnSpeed,  maxTurnAngle);
		} else {
			const float k = deltaTime * maxTurnSpeed;
			if (direction > 0.0f) {
				direction = std::max<float>(direction - k, 0.0f);
			} else if (direction < 0.0f) {
				direction = std::min<float>(direction + k, 0.0f);
			}
		}
		
		// Update rotation
		if (velocity != 0.0f) {
			rotation += lerp(rotation, rotation + direction, velocity / 80.0f * deltaTime);
		}
		const vec2 rotVel = GetRot(rotation + direction);
		
		// Update velocity
		if (keys[0]) {
			velocity = std::min<float>(velocity + deltaTime * acceleration, maxForwardAcceleration);
		} else if (keys[2]) {
			velocity = std::max<float>(velocity - deltaTime * acceleration, -maxBackwardAcceleration);
		} else {
			if (velocity > 0.0f) {
				velocity = std::max<float>(velocity - deltaTime * friction, 0.0f);
			} else if (velocity < 0.0f) {
				velocity = std::min<float>(velocity + deltaTime * friction, 0.0f);
			}
		}
		
		// Update position
		position += (rotVel * velocity * deltaTime);
	}
	
	void draw(Image* output) {
		vec2 points[] = {
			{-size.x / 2, -size.y / 2},
			{-size.x / 2,  size.y / 2},
			{ size.x / 2,  size.y / 2},
			{ size.x / 2, -size.y / 2},
		};

		const vec2 rot = GetRot(rotation + direction);
		const vec2 point = position + rot * velocity;
		
		// Transform car bounds
		const float rad = rotation * (M_PI / 180.0f);
		const float sn = sinf(rad);
		const float cs = cosf(rad);
		for (uint32_t index = 0; index < 4; ++index) {
			points[index] = {
				cs * points[index].x - sn * points[index].y, 
				sn * points[index].x + cs * points[index].y
			};
		}
		
		// Draw car bounds
		for (uint32_t index1 = 0, index2 = 1; index1 < 4; ++index1, index2 = (index1 + 1) % 4) {
			output->drawLine(ivec2(position.x + points[index1].x, position.y + points[index1].y), 
			                 ivec2(position.x + points[index2].x, position.y + points[index2].y), 
			                 ubvec4(255, 255, 255, 255));
		}
		// Draw the velocity vector
		output->drawLine(ivec2(position.x, position.y), ivec2(point.x, point.y), ubvec4(0, 0, 255, 255));
	}
};

int main(int argc, char* argv[]) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(800, 600);
#if defined (_WIN32)
	Win32Window output;
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8A8, "Car Physics") == false) {
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

	uint64_t lastSecondTime = Timer::GetMilliSeconds();
	uint64_t lastFrameTime = lastSecondTime;
	uint32_t frameCount = 0;
	uint32_t lastFPS = 0;
	
	Car car;
	car.position = {390.0f, 500.0f};
	car.size = {20.0f, 40.0f};
	car.acceleration = 2.0f;
	car.friction = 1.0f;
	car.rotation = 180.0f;
	car.maxTurnSpeed = 5.0f;
	car.maxTurnAngle = 35.0f;
	car.maxForwardAcceleration = 20.0f;
	car.maxBackwardAcceleration = 10.0f;
	
	bool keys[4] = {false, false, false, false};

	while (running) {
		const uint64_t currentFrameTime = Timer::GetMilliSeconds();
		if (currentFrameTime - lastSecondTime >= 1000) {
			printf("FPS: %d\n", frameCount);
			frameCount = 0;
			lastSecondTime += 1000;
		}
		const float deltaTime = (currentFrameTime - lastFrameTime) / 100.0f;
		lastFrameTime = currentFrameTime;
		/*********************************************************************/
		/* Check keyboard events.                                            */
		/*********************************************************************/
		while (output.getEvent(&event)) {
			switch (event.type) {
			case Event::WINDOW_CLOSE :
				running = false;
				break;
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_R :
					if (event.state == 0) {
						car.position = {390.0f, 500.0f};
						car.rotation = 180.0f;
						car.direction = 0.0f;
						car.velocity = 0.0f;
					}
					break;
				case KEY_UP :
					keys[0] = event.state;
					break;
				case KEY_LEFT :
					keys[1] = event.state;
					break;
				case KEY_DOWN :
					keys[2] = event.state;
					break;
				case KEY_RIGHT :
					keys[3] = event.state;
					break;
				case KEY_ESCAPE :
					running = event.state;
					break;
				}
				break;
			}
		}

		colorBuffer.clear();
		
		car.update(keys, deltaTime);
		car.draw(&colorBuffer);

		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
		++frameCount;
	}

	return 0;
}
// https://asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html

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

#define DEGTORAD(value) ((value) * (M_PI / 180.0f))

vec2 GetRotationVector(float angle) {
	const float rad = DEGTORAD(angle + 45.0f);
	const float cs = cosf(rad);
	const float sn = sinf(rad);
	return {cs - sn, sn + cs};
}

struct Car {
	vec2 position;
	vec2 size;
	float rotation;     // car body rotation

	float engineForce;
	float direction;    // wheel relative rotation
	float weight;
	float speed;

	float friction;
	float frontalArea;
	float airDensity;
	float rollingResistance;

	Car() {
		size = {1, 1};
		rotation = 0.0f;
		engineForce = 0.0f;
		direction = 0.0f;
		weight = 500.0f;
		speed = 0.0f;
		friction = 0.3f;
		frontalArea = 2.2f;
		airDensity = 1.29f;
		rollingResistance = 20.0f; // 0.02f;
	}

	void update(float deltaTime = 1.0f) {
		rotation += direction * std::min(speed / 80.0f * deltaTime, 1.0f);

		const float tractionForce = engineForce;
		const float dragForce = -(0.5f * friction * frontalArea * airDensity) * speed * fabs(speed);
		const float rollingResistanceForce = -rollingResistance * speed;
		const float longitudinalForce = tractionForce + dragForce + rollingResistanceForce;
		const float acceleration = longitudinalForce / weight;

		speed += acceleration * deltaTime;

		const vec2 velocity = GetRotationVector(rotation + direction) * speed;
		position += velocity * deltaTime;
	}

	void draw(Image* output) {
		vec2 points[] = {
			{-size.x / 2, -size.y / 2},
			{-size.x / 2,  size.y / 2},
			{ size.x / 2,  size.y / 2},
			{ size.x / 2, -size.y / 2},
		};

		// Transform car bounds
		const float rad = DEGTORAD(rotation);
		const float sn = sinf(rad);
		const float cs = cosf(rad);
		for (uint32_t index = 0; index < 4; ++index) {
			points[index] = {
				cs * points[index].x - sn * points[index].y,
				sn * points[index].x + cs * points[index].y,
			};
		}

		// Draw car bounds
		for (uint32_t index1 = 0, index2 = 1; index1 < 4; ++index1, index2 = (index1 + 1) % 4) {
			output->drawLine(ivec2(position.x + points[index1].x, position.y + points[index1].y),
			                 ivec2(position.x + points[index2].x, position.y + points[index2].y),
			                 ubvec4(255, 255, 255, 255));
		}

		// Draw the velocity vector
		const vec2 point = position + GetRotationVector(rotation + direction) * speed;
		output->drawLine(ivec2(position.x, position.y), ivec2(point.x, point.y), ubvec4(0, 0, 255, 255));
	}
};

void InitCar(Car& car) {
	car.position = {390.0f, 500.0f};
	car.size = {20.0f, 40.0f};
	car.rotation = 180.0f;
	car.engineForce = 0.0f;
	car.direction = 0.0f;
	car.speed = 0.0f;
	car.weight = 500.0f;
}

int main(int argc, char* argv[]) {
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

	Event event;
	bool running = true;
	bool keys[4] = {false, false, false, false};
	uint64_t lastFrameTime = Timer::GetMilliSeconds();

	Car car;
	InitCar(car);

	while (running) {
		const uint64_t currentFrameTime = Timer::GetMilliSeconds();
		const float deltaTime = (float)(currentFrameTime - lastFrameTime) / 100.0f;
		lastFrameTime = currentFrameTime;

		while (output.getEvent(&event)) {
			switch (event.type) {
			case Event::WINDOW_CLOSE : 
				running = false; 
				break;
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_R :
					if (event.state == 0) {
						InitCar(car);
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

		if (keys[0]) {
			car.engineForce = 2500.0f;
		} else if (keys[2]) {
			car.engineForce = -500.0f;
		} else {
			car.engineForce = 0.0f;
		}

		if (keys[1]) {
			car.direction = std::max(car.direction - 5.0f * deltaTime,-35.0f);
		} else if (keys[3]) {
			car.direction = std::min(car.direction + 5.0f * deltaTime, 35.0f);
		} else {
			car.direction += -car.direction * 0.5f * deltaTime;
		}

		car.update(deltaTime);
		car.draw(&colorBuffer);

		output.blit(&colorBuffer);
	}

	return 0;
}
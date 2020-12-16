/*****************************************************************************/
/* Sources: 
/* https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Pseudo3DPlanesMode7.cpp */
/* https://en.wikipedia.org/wiki/Mode_7                                      */
/*****************************************************************************/

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include "FrameBuffer.h"
//#include <linux/input.h>
#endif

#include "Image.h"
#include "Spline.h"
#include "Timer.h"
#include "Camera.h"

typedef Vector4<unsigned int> Vector4u;

const Vector2u WindowSize(640, 480);
const Vector2u HalfWindowSize = WindowSize / 2;
static const unsigned int RenderingThreads = 3;

static const float DEGTORAD = (M_PI / 180.0f);
static const float RADTODEG = (180.0f / M_PI);

const Vector4f BottomSky(0.0f, 1.0f, 1.0f, 1.0f);
const Vector4f TopSky(0.0f, 0.5f, 1.0f, 1.0f);

const int MaxLapCount = 1;

struct RenderContext {
	Frustum frustum;
	Image* output;
	Image* input;
};

void DrawPartial(RenderContext* context, int id, int start, int end) {
	const float oneOverWSizeX = 1.0f / (float)WindowSize.x;	

	for (unsigned int y = 0; y < HalfWindowSize.y; ++y) {
		const float fOneOverSampleDepth = (float)HalfWindowSize.y / (float)y;

		const Vector2f fStart((context->frustum.LeftFar - context->frustum.LeftNear) * fOneOverSampleDepth + context->frustum.LeftNear);
		const Vector2f fEnd((context->frustum.RightFar - context->frustum.RightNear) * fOneOverSampleDepth + context->frustum.RightNear);

		for (unsigned int x = start; x < end; ++x) {
			const float fSampleWidth = (float)x * oneOverWSizeX;
			const Vector2f fSample((fEnd - fStart) * fSampleWidth + fStart);

			Vector4f sample;
			if ((fSample.x >= 0.0f && fSample.x <= 1.0f) &&
				(fSample.y >= 0.0f && fSample.y <= 1.0f)) {
				sample = context->input->sample2D(fSample);
			} else {
				sample = BottomSky;
			}
			context->output->setPixel(x, y + HalfWindowSize.y, sample);
		}
	}

}

int main() {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
#if defined (_WIN32)
	Win32Window output;	
	if (output.initialize(WindowSize, Image::EPF_R8G8B8, "Mode 7") == false) {
		printf("Failed to initialize the win32 window.\n");
		return 1;
	}
#elif defined (__linux__)
	FrameBuffer output;
	if (output.initialize(NULL, WindowSize, Image::EPF_R8G8B8A8) != 0) {
		printf("Failed to initialize the frame buffer.\n");
		return 1;
	}
	output.input.addAllInputs();
#else
	#error Unsupported platform!
#endif

	Image colorBuffer;
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping.x = Image::EWT_DISCARD;
	colorBuffer.wrapping.y = Image::EWT_DISCARD;

	/*************************************************************************/
	/* Load map.                                                             */
	/*************************************************************************/
	Image mapImage;
	if (mapImage.load("map.tga") == false) {
		printf("Failed to load map.tga\n");
		return 2;
	}
	
	/************************************************************************/
	/* Render context.                                                      */
	/************************************************************************/
	RenderContext renderContext;
	renderContext.output = &colorBuffer;
	renderContext.input = &mapImage;

	/*************************************************************************/
	/* Setup camera.                                                         */
	/*************************************************************************/
	Camera camera;
	camera.position = Vector2f(924.0f / 1024.0f, 424.0f / 1024.0f);
	camera.rotation = -90.0f * DEGTORAD;
	camera.fov = M_PI / 4.0f;
	camera.depth = Vector2f(0.005f, 0.04f);

	/*************************************************************************/
	/* Populate follow spline.                                               */
	/*************************************************************************/
	Spline2f spline;
	spline.loop = true;
	spline.path.push_back(Vector2f(924.0f, 424.0f) / 1024.0f);
	spline.path.push_back(Vector2f(192.0f,  69.0f) / 1024.0f);
	spline.path.push_back(Vector2f( 81.0f, 190.0f) / 1024.0f);
	spline.path.push_back(Vector2f(105.0f, 735.0f) / 1024.0f);
	spline.path.push_back(Vector2f(520.0f, 582.0f) / 1024.0f);
	spline.path.push_back(Vector2f(817.0f, 917.0f) / 1024.0f);
	spline.path.push_back(Vector2f(933.0f, 783.0f) / 1024.0f);

	/*************************************************************************/
	/* Misc.                                                                 */
	/*************************************************************************/
	float deltaTime = 1.0f;
	unsigned long long lastTime = Timer::GetMilliSeconds();
	unsigned long long currentTime = lastTime;

	unsigned long long lastFPSTime = lastTime;
	unsigned int frameCount = 0;
	unsigned int totalFPS = 0;
	unsigned int totalSeconds = 0;

	bool playerControl = false;
	float progress = 0.0f;
	float lookAtProgress = 0.0f;
	int lapCount = 0;
	
	Event event;
	bool running = true;
	bool keys[4] = {false, false, false, false};
	
	while (running) {
		/*********************************************************************/
		/* Calculate FPS.                                                    */
		/*********************************************************************/
		currentTime = Timer::GetMilliSeconds();
		deltaTime = (currentTime - lastTime) * 0.001f;
		lastTime = currentTime;

		if (currentTime - lastFPSTime >= 1000) {
			totalFPS += frameCount;
			totalSeconds += 1;
			frameCount = 0;
			lastFPSTime += 1000;
		}

		/*********************************************************************/
		/* Check input events.                                               */
		/*********************************************************************/
		while (output.getEvent(&event)) {
			switch (event.type) {
			case Event::WINDOW_CLOSE :
				running = false;
				break;

			case Event::KEYBOARD:
				if (event.state == 1) {
					playerControl = true;
				}
				switch (event.key) {
				case KEY_ESCAPE :
					if (event.state == 0) {
						running = false;
					}
					break;
				case KEY_LEFT :
					keys[0] = event.state;
					break;
				case KEY_DOWN :
					keys[1] = event.state;
					break;
				case KEY_RIGHT :
					keys[2] = event.state;
					break;
				case KEY_UP :
					keys[3] = event.state;
					break;
				}
				break;
			}
		}
		
		/*********************************************************************/
		/* Update camera position and direction.                             */
		/*********************************************************************/
		if (playerControl) {
			if (keys[3]) {
				camera.position.x += cosf(camera.rotation) * 0.2f * deltaTime;
				camera.position.y += sinf(camera.rotation) * 0.2f  * deltaTime;
			} else if (keys[1]) {
				camera.position.x -= cosf(camera.rotation) * 0.2f  * deltaTime;
				camera.position.y -= sinf(camera.rotation) * 0.2f  * deltaTime;
			}
			if (keys[0] && !keys[2]) {
				camera.rotation -= 1.5f * deltaTime;
			}
			if (keys[2] && !keys[0]) {
				camera.rotation += 1.5f * deltaTime;
			}
		} else {
			camera.position = spline.getSplinePoint(progress);
			const Vector2f gradient = spline.getSplineGradient(lookAtProgress);
			camera.rotation = atan2(gradient.y, gradient.x);

			progress += deltaTime * 0.3f;
			if (progress >= spline.getMaxT()) {
				progress -= spline.getMaxT();
				++lapCount;
				if (lapCount >= MaxLapCount) {
					break;
				}
			}

			lookAtProgress = progress + 0.15f;
			if (lookAtProgress >= spline.getMaxT()) {
				lookAtProgress -= spline.getMaxT();
			}
		}

		/*********************************************************************/
		/* Calculate view frustum.                                           */
		/*********************************************************************/
		renderContext.frustum = camera.getFrustum();

		/*********************************************************************/
		/* Draw sky.                                                         */
		/*********************************************************************/
		for (unsigned int y = 0; y < HalfWindowSize.y; ++y) {
			const float fSampleDepth = (float)y / ((float)HalfWindowSize.y);
			const Vector4f skyColor = BottomSky * fSampleDepth + TopSky * 
								(1.0f - fSampleDepth);
			for (unsigned int x = 0; x < WindowSize.x; ++x) {
				colorBuffer.setPixel(x, y, skyColor);
			}
		}

		/*********************************************************************/
		/* Draw ground.                                                      */
		/*********************************************************************/		
		DrawPartial(&renderContext, 0, 0, WindowSize.x);

		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
	
		++frameCount;
	}

	/*************************************************************************/
	/* Clear screen and display frame information.                           */
	/*************************************************************************/
	//output.clearColor();
	printf("Average FPS: %.2f\n", (float)totalFPS / (float)totalSeconds);
	
	return 0;
}

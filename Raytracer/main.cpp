/*****************************************************************************/
/* Sources:                                                                  */
/* https://www.youtube.com/watch?v=gYRrGTC7GtA&t=736s                        */
/*****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <string.h>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include <linux/input.h>
#include "Input.h"
#include "FrameBuffer.h"
#endif 

#include "Spline.h"
#include "Timer.h"
#include "Camera.h"

const Vector2u WindowSize(640, 480);

static const float DEGTORAD = (M_PI / 180.0f);
static const float RADTODEG = (180.0f / M_PI);
const float M_2PI = M_PI * 2.0;
const float M_PI2 = M_PI / 2.0;
const float M_3PI2 = 3.0 * M_PI / 2.0;
const float DR = 0.0174533f;
const float CellSize = 1.0f;
const Vector2i MapSize(16, 8);
const int MaxDOF = std::max(MapSize.x, MapSize.y);
const int CellCount = MapSize.x * MapSize.y;
const char MapData[] = 
{
	1, 1, 1, 1, 1, 1, 1, 1,	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 0, 0, 0, 0, 1,	1, 0, 1, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

float zoom = 16.0f;

enum RenderState {
	ERS_2D,
	ERS_3D
};
int renderState = ERS_3D;

void DrawMap(Image* output, Camera& camera) {
	for (unsigned int xIndex = 0; xIndex < MapSize.x; ++xIndex) {
		for (unsigned int yIndex = 0; yIndex < MapSize.y; ++yIndex) {
			switch (MapData[yIndex * MapSize.x + xIndex]) {
			case 1 :
				output->drawFilledRectangle(
						  Vector4f(xIndex * zoom + 1, yIndex * zoom + 1, (xIndex + 1) * zoom - 1, (yIndex + 1) * zoom - 1), 
						  Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
		}
	}
}

void DrawPlayer(Image* output, Camera& camera) {
	const Vector2f half(2.0f, 2.0f);
	const Vector2f pd(cosf(camera.rotation), sinf(camera.rotation));
	output->drawFilledRectangle(
				  Vector4f(camera.position.x * zoom - half.x, camera.position.y * zoom - half.y, camera.position.x * zoom + half.x, camera.position.y * zoom + half.y), 
				  Vector4f(0.0f, 1.0f, 0.0f, 1.0f));
	output->drawLine(camera.position * zoom, (camera.position + pd) * zoom, Vector4f(1.0f, 1.0f, 0.0f, 1.0f));
}

void DrawRays(Image* output, Camera& camera, Image* textures) {
	float verticalDistance = 1000.0f;
	float horizontalDistance = 1000.0f;
	float distance = 0.0f;
	int mapIndex;
	
	Vector2f rayEnd;
	Vector2f rayOffset;
	float rayAngle;
	Vector2i mapIndex2;
	Vector2f verticalEnd = camera.position;
	Vector2f horizontalEnd = camera.position;
	int depthOfField = 0;
	int verticalDepthOfField = 0;
	
	rayAngle = camera.rotation - DR * camera.fov / 2.0f;
	if (rayAngle < 0.0f) {
		rayAngle += M_2PI;
	}

	for (int rayIndex = 0; rayIndex <= camera.fov; ++rayIndex) 
	{
		// HORIZONTAL CHECK
		depthOfField = 0;
		horizontalDistance = 1000.0f;
		const float aTan = -1.0f / tan(rayAngle);
		if (rayAngle > M_PI) {
			rayEnd.y = (float)(int)camera.position.y - 0.0001f;
			rayEnd.x = (camera.position.y - rayEnd.y) * aTan + camera.position.x;
			rayOffset.y = -CellSize;
			rayOffset.x = -rayOffset.y * aTan;
		} else if (rayAngle < M_PI) {
			rayEnd.y = (float)((int)camera.position.y + CellSize);
			rayEnd.x = (camera.position.y - rayEnd.y) * aTan + camera.position.x;
			rayOffset.y = CellSize;
			rayOffset.x = -rayOffset.y * aTan;
		} else {
			rayEnd = camera.position;
			depthOfField = MaxDOF;
		}
		while (depthOfField < MaxDOF) {
			mapIndex2.x = (int)(rayEnd.x);
			mapIndex2.y = (int)(rayEnd.y);
			mapIndex = mapIndex2.y * MapSize.x + mapIndex2.x;
			
			if ((mapIndex >= 0) && (mapIndex < CellCount) && (MapData[mapIndex] == 1)) {
				horizontalEnd = rayEnd;
				horizontalDistance = camera.position.getDistanceFrom(horizontalEnd);
				depthOfField = MaxDOF;
			} else {
				rayEnd += rayOffset;
				++depthOfField;
			}
		}

		// VERTICAL
		depthOfField = 0;
		verticalDistance = 1000.0f;
		const float nTan = -tan(rayAngle);
		if (rayAngle > M_PI2 && rayAngle < M_3PI2) {
			rayEnd.x = (float)(int)camera.position.x - 0.0001f;
			rayEnd.y = (camera.position.x - rayEnd.x) * nTan + camera.position.y;
			rayOffset.x = -CellSize;
			rayOffset.y = -rayOffset.x * nTan;
		} else if (rayAngle < M_PI2 || rayAngle > M_3PI2) {
			rayEnd.x = (float)((int)camera.position.x + CellSize);
			rayEnd.y = (camera.position.x - rayEnd.x) * nTan + camera.position.y;
			rayOffset.x = CellSize;
			rayOffset.y = -rayOffset.x * nTan;
		} else {
			rayEnd = camera.position;
			depthOfField = MaxDOF;
		}
		while (depthOfField < MaxDOF) {
			mapIndex2.x = (int)(rayEnd.x);
			mapIndex2.y = (int)(rayEnd.y);
			mapIndex = mapIndex2.y * MapSize.x + mapIndex2.x;
			
			if ((mapIndex >= 0) && (mapIndex < CellCount) && (MapData[mapIndex] == 1)) {
				verticalEnd = rayEnd;
				verticalDistance = camera.position.getDistanceFrom(verticalEnd);
				depthOfField = MaxDOF;
			} else {
				rayEnd += rayOffset;
				depthOfField += 1;
			}
		}
		
		Vector4f wallColor(1.0f, 1.0f, 1.0f, 1.0f);

		float uvX = 0;		
		if (horizontalDistance < verticalDistance) {
			rayEnd = horizontalEnd;
			distance = horizontalDistance;
			wallColor = Vector4f(0.7f, 0.7f, 0.7f, 1.0f);
			uvX = horizontalEnd.x - (int)horizontalEnd.x;
		} else {
			rayEnd = verticalEnd;
			distance = verticalDistance;
			wallColor = Vector4f(0.25f, 0.25f, 0.25f, 1.0f);
			uvX = verticalEnd.y - (int)verticalEnd.y;
		}
		uvX = fabs(uvX);

		switch (renderState) {
		case ERS_2D :
			output->drawLine(camera.position * zoom, rayEnd * zoom, Vector4f(1.0f, 0.0f, 0.0f, 1.0f));
			break;
			
		case ERS_3D :
			float correctionAngle = camera.rotation - rayAngle;
			if (correctionAngle < 0.0f) {
				correctionAngle += M_2PI;
			}
			distance *= cos(correctionAngle); // fix fisheye
			
			float lineHeight = WindowSize.y / distance;
			float lineOffset = WindowSize.y / 2 - lineHeight / 2.0f;

			const float LineWidth = WindowSize.x / camera.fov;
			int xx = rayIndex * LineWidth;
			int xz = xx + LineWidth;
			for (int x = std::max(xx, 0); x < std::min(xz, (int)output->getSize().x); ++x) {
				for (int y = 0; y < output->getSize().y; ++y) {
					if (y < lineOffset) {
						// Draw Ceilling
						//output->setPixelU(x, y, Vector4ub(204, 204, 204, 255));
					} else if (y >= lineOffset && y < lineOffset + lineHeight) {
						// Draw Walls
						const Vector4ub sample = textures[0].sampleRaw2D(Vector2f(uvX, ((float)y - lineOffset) / lineHeight));
						output->setRawPixel(x, y, sample);
					} else {
						// Draw floor
						//output->setPixelU(x, y, Vector4ub(102, 102, 102, 255));
					}
				}
			}
			break;
		}
		
		rayAngle += DR;
		if (rayAngle > M_2PI) {
			rayAngle -= M_2PI;
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
	if (output.initialize(NULL, WindowSize, Image::EPF_R8G8B8) != 0) {
		printf("Failed to initialize the frame buffer.\n");
		return 1;
	}
#else
	#error Unsupported platform!
#endif

	Image colorBuffer;
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping.x = Image::EWT_DISCARD;
	colorBuffer.wrapping.y = Image::EWT_DISCARD;

	/*************************************************************************/
	/* Load resources.                                                        */
	/*************************************************************************/
	Image textures[1];
	if (textures[0].load("brick.tga") == false) {
		printf("Error! Failed to load brick.tga\n");
		return 1;
	}
	textures[0].wrapping = Image::EWT_REPEAT;
	
	/*************************************************************************/
	/* Camera.                                                               */
	/*************************************************************************/
	Camera camera;
	camera.position.x = MapSize.x / 2;
	camera.position.y = MapSize.y / 2;
	camera.fov = 75.0f;

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
		/* Check keyboard events.                                            */
		/*********************************************************************/
		while (output.getEvent(&event)) {
	
			switch (event.key) {
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
			case KEY_2 :
				if (event.state == 0) {
					renderState = ERS_2D;
				}
				break;
			case KEY_3 :
				if (event.state == 0) {
					renderState = ERS_3D;
				}
				break;
			case KEY_4 :
				if (event.state == 0) {
					zoom *= 2.0f;
				}
				break;
			case KEY_5 :
				if (event.state == 0) {
					zoom /= 2.0f;
				}
				break;
			case KEY_ESCAPE :
				if (event.state == 0) {
					running = false;
				}
				break;
			}
		}
		
		Vector2f pd(cos(camera.rotation), sin(camera.rotation));
		if (keys[0]) {
			camera.position += pd * deltaTime;
		} else if (keys[2]) {
			camera.position -= pd * deltaTime;
		}
		if (keys[1] && !keys[3]) {
			camera.rotation -= 3.0f * deltaTime;
			if (camera.rotation < 0.0f) {
				camera.rotation += M_2PI;
			}
		}
		if (keys[3] && !keys[1]) {
			camera.rotation += 3.0f * deltaTime;
			if (camera.rotation > M_2PI) {
				camera.rotation -= M_2PI;
			}
		}

		/*********************************************************************/
		/* Render scene.                                                     */
		/*********************************************************************/
		switch (renderState) {
		case ERS_2D :
			colorBuffer.clear();
			DrawMap(&colorBuffer, camera);
			DrawPlayer(&colorBuffer, camera);
			DrawRays(&colorBuffer, camera, textures);
			break;
			
		case ERS_3D :
			colorBuffer.clear();
			unsigned char* rawPointer = (unsigned char*)colorBuffer.getData();
			const unsigned int halfSize = (colorBuffer.getSize().x * colorBuffer.getSize().y * colorBuffer.getPixelSize()) / 2;
			memset(rawPointer, 204, halfSize);
			memset(rawPointer + halfSize, 104, halfSize);
			DrawRays(&colorBuffer, camera, textures);
			break;
		}
		
		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
	
		++frameCount;
	}

	/*************************************************************************/
	/* Clear screen and display FPS.                                         */
	/*************************************************************************/
	// output.clearColor();
	printf("\nAverage FPS: %.2f\n", (float)totalFPS / (float)totalSeconds);
	
	return 0;
}

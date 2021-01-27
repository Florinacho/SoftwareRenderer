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
#include "Timer.h"

/*****************************************************************************/
/* Returns the minimum value from 2 inputs.                                  */
/*****************************************************************************/
template <typename T>
T min(T a, T b) {
	return (a < b) ? a : b;
}

/*****************************************************************************/
/* Returns the maximum value from 2 inputs.                                  */
/*****************************************************************************/
template <typename T>
T max(T a, T b) {
	return (a > b) ? a : b;
}

/*****************************************************************************/
/* Draws a scaled pixel.                                                     */
/*****************************************************************************/
inline void DrawPixel(Image* output, int x, int y, const ubvec4& color, int size = 1) {
	for (int yIndex = 0; yIndex < size; ++yIndex) {
		for (int xIndex = 0; xIndex < size; ++xIndex) {
			output->setRawPixel(x + xIndex, y + yIndex, color);
		}
	}
}

/*****************************************************************************/
/* Sprites                                                                   */
/*****************************************************************************/
unsigned char SpriteData[] = {
	// Brick
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	
	// Left falling water half
	0, 0, 2, 2, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1,
	0, 0, 2, 2, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1, 2, 3,
	0, 0, 3, 2, 2, 2, 1, 2, 3, 2, 1, 2, 1, 2, 2, 2,
	0, 0, 1, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2,
	0, 0, 0, 1, 1, 3, 2, 2, 2, 2, 2, 3, 2, 2, 1, 2,
	0, 0, 0, 1, 1, 1, 2, 3, 2, 3, 2, 1, 2, 3, 1, 1,
	0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1,
	
	// Right pipe half
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 1, 0, 0,
	
	// Coin
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 2, 2, 1, 1, 2, 2, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*****************************************************************************/
/* Tiles                                                                     */
/*****************************************************************************/
enum TileFlags {
	SF_FLIP_X = 1 << 0,
	SF_FLIP_Y = 1 << 1,
};
struct TileConfig {
	unsigned char spriteIndex;
	unsigned char paletteIndex;
	unsigned char flags;        // One of TileFlags
	unsigned char offsetX;
	unsigned char offsetY;
	unsigned char offsetXDelay;
	unsigned char offsetYDelay;
};

TileConfig TileTable[] = {
	{0, 0, 0, 0, 0, 0,  0}, // Empty space
	{0, 1, 0, 0, 0, 0,  0}, // Brick wall
	{1, 2, 0, 0, 0, 0, 50}, // Water left
	{1, 2, 1, 0, 0, 0, 50}, // Water right
	{2, 3, 1, 0, 0, 0,  0}, // Left tube half
	{2, 3, 0, 0, 0, 0,  0}, // Right rube half
};
static const unsigned int TileConfigCount = sizeof(TileTable) / sizeof(TileTable[0]);

/*****************************************************************************/
/* Renderer                                                                  */
/*****************************************************************************/
struct TileRenderer {
	static const unsigned int PaletteLength = 8;
	static const unsigned int PaletteCount = 8;
	static const unsigned int TileCountX = 16;
	static const unsigned int TileCountY = 15;
	static const unsigned int TileWidth = 16;
	static const unsigned int TileHeight = 16;
	static const unsigned int OutputWidth = TileCountX * TileWidth;
	static const unsigned int OutputHeight = TileCountY * TileHeight;
	static const unsigned int OutputScale = 3;
	
	ubvec4 paletteTable[PaletteCount][PaletteLength];
	ubvec4 transparent;
	const unsigned char *tileBuffer;
	uvec2 mapTileCount;
	ivec2 tileOffset;
	ivec2 pixelOffset;
	
	TileRenderer() {
		tileBuffer = NULL;
	}
	
	void setOffset(int offsetX, int offsetY) {
		tileOffset.x = offsetX / TileWidth;
		tileOffset.y = offsetY / TileHeight;
		pixelOffset.x = offsetX % TileWidth;
		pixelOffset.y = offsetY % TileHeight;
	}
	
	void loadScene() {
		static const unsigned char Scene[] = {
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2, 3, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		};
		tileBuffer = Scene;
		mapTileCount.x = 32;
		mapTileCount.y = 15;
		
		paletteTable[0][0] = ubvec4(   0,   0,   0,   0);
		paletteTable[0][1] = ubvec4(   0,   0, 255, 255);
		
		paletteTable[1][0] = ubvec4(  50, 150, 255, 255);
		paletteTable[1][1] = ubvec4(   0,   0,   0, 255);
		
		paletteTable[2][0] = transparent;
		paletteTable[2][1] = ubvec4(  50, 200, 200, 255);
		paletteTable[2][2] = ubvec4(   0, 100, 200, 255);
		paletteTable[2][3] = ubvec4( 100, 150, 200, 255);
		
		paletteTable[3][0] = transparent;
		paletteTable[3][1] = ubvec4(  50,  50,  50, 255);
		paletteTable[3][2] = ubvec4(   0, 200, 100, 255);
		paletteTable[3][3] = ubvec4(   0, 100,  50, 255);
	}
	
	void draw(Image *output) {
		// Update scrolling tiles
		const unsigned long currentTime = Timer::GetMilliSeconds();
		for (unsigned int index = 0; index < TileConfigCount; ++index) {
			if (TileTable[index].offsetXDelay > 0) {
				TileTable[index].offsetX = currentTime / TileTable[index].offsetXDelay;
			}
			if (TileTable[index].offsetYDelay > 0) {
				TileTable[index].offsetY = currentTime / TileTable[index].offsetYDelay;
			}
		}
		
		// Draw tiles
		for (int tileY = max<int>(tileOffset.y, 0); tileY < min<int>(mapTileCount.y, TileCountY + tileOffset.y); ++tileY) {
			for (int tileX = max<int>(tileOffset.x, 0); tileX < min<int>(mapTileCount.x, TileCountX + tileOffset.x); ++tileX) {
				const int tileIndex = tileY * mapTileCount.x + tileX;
				const unsigned char tileValue = tileBuffer[tileIndex];
				if (tileValue == 0) {
					continue;
				}
				TileConfig tileConfig = TileTable[tileValue];
				unsigned int spriteOffset = (tileConfig.spriteIndex) * TileHeight * TileWidth;
				if (spriteOffset >= sizeof(SpriteData)) {
					continue;
				}
				for (int pixelY = 0; pixelY < TileHeight; ++pixelY) {
					const int destPixelY = ((tileY - tileOffset.y) * TileHeight + pixelY - pixelOffset.y);
					if ((destPixelY < 0) || (destPixelY >= OutputHeight)) {
						continue;
					}
					for (int pixelX = 0; pixelX < TileWidth; ++pixelX) {
						const unsigned int sourcePixelOffsetX = (pixelX + tileConfig.offsetX) % TileWidth;
						const unsigned int sourcePixelOffsetY = (pixelY - tileConfig.offsetY) % TileHeight;
						const unsigned int sourcePixelX = ((tileConfig.flags & SF_FLIP_X) ? (TileWidth  - 1 - sourcePixelOffsetX) : sourcePixelOffsetX);
						const unsigned int sourcePixelY = ((tileConfig.flags & SF_FLIP_Y) ? (TileHeight - 1 - sourcePixelOffsetY) : sourcePixelOffsetY);
						const unsigned int sourcePixelIndex = spriteOffset + sourcePixelY * TileWidth + sourcePixelX;
						const unsigned int sourcePixel = SpriteData[sourcePixelIndex];
						const ubvec4 destPixel = paletteTable[tileConfig.paletteIndex][sourcePixel];
						if (destPixel == transparent) {
							continue;
						}
						const int destPixelX = ((tileX - tileOffset.x) * TileWidth  + pixelX - pixelOffset.x);
						if ((destPixelX < 0) || (destPixelX >= OutputWidth)) {
							continue;
						}
						DrawPixel(output, destPixelX * OutputScale, destPixelY * OutputScale, destPixel, OutputScale);
					}
				}
			}
		}
	}
};

int main(int argc, char* argv[]) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(TileRenderer::OutputWidth * TileRenderer::OutputScale, 
							TileRenderer::OutputHeight * TileRenderer::OutputScale);
#if defined (_WIN32)
	Win32Window output;
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8A8, "Tile renderer") == false) {
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
	/* Tile renderer                                                         */
	/*************************************************************************/	
	TileRenderer renderer;
	renderer.loadScene();

	/*************************************************************************/
	/* Misc.                                                                 */
	/*************************************************************************/
	Event event;
	bool running = true;
	bool keys[4] = {false, false, false, false};
	ivec2 offset;
	
	unsigned long lastSecondTime = Timer::GetMilliSeconds();
	unsigned int frameCount = 0;
	unsigned int lastFPS = 0;
	
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
			case Event::WINDOW_CLOSE :
				running = false;
				break;
			case Event::KEYBOARD :
				switch (event.key) {
				case KEY_LEFT :
					keys[0] = event.state;
					break;
				case KEY_RIGHT :
					keys[1] = event.state;
					break;
				case KEY_UP :
					keys[2] = event.state;
					break;
				case KEY_DOWN :
					keys[3] = event.state;
					break;
				}
				break;
			}
		}

		/*********************************************************************/
		/* Render scene.                                                     */
		/*********************************************************************/
		colorBuffer.clear();

		if (keys[0]) offset.x -= 1;
		if (keys[1]) offset.x += 1;
		if (keys[2]) offset.y -= 1;
		if (keys[3]) offset.y += 1;
		
		if (offset.x < 0) offset.x = 0;
		if (offset.y < 0) offset.y = 0;
		
		renderer.setOffset(offset.x, offset.y);
		renderer.draw(&colorBuffer);

		/*********************************************************************/
		/* Send color buffer to the screen.                                  */
		/*********************************************************************/
		output.blit(&colorBuffer);
		++frameCount;
	}

	return 0;
}
// Source: https://github.com/3DSage/OpenGL-Raycaster_v2/blob/main/3DSage_Raycaster_v2.c

#include <stdio.h>
#include <math.h>

#if defined (_WIN32)
#include "Window.h"
#elif defined (__linux__)
#include <linux/input.h>
#include "Input.h"
#include "FrameBuffer.h"
#endif

#include "Timer.h"
#include "../Experimental/Font.h"

const float DEGTORAD = M_PI / 180.0f;
int MaxDepth = 8;
float WrapAngle(float a) {
	if (a >= 360.0f) {
		a -= 360.0f;
	}
	if (a < 0.0f) {
		a += 360.0f;
	}
	return a;
}

/*****************************************************************************/
/* Textures                                                                  */
/*****************************************************************************/
const uvec2 TextureSize(32, 32);

unsigned char Textures[] = {
	//Checkerboard
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

	//Brick
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	
	// Window
	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,

	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,

	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
	
	// Door
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,

	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
	0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,1, 1,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,1,1,1,1,0,1, 1,0,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,0,1, 1,0,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,

	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,0,1, 1,0,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,0,1, 1,0,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1, 1,1,1,1,1,1,0,1, 1,0,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
};

/*****************************************************************************/
/* Map                                                                       */
/*****************************************************************************/
const ivec2 MapSize(8, 8);
const int MapSizeLength = MapSize.x * MapSize.y;

// Walls
int mapW[] = {
	1,1,1,1,1,3,1,1,
	1,0,0,1,0,0,0,1,
	1,0,0,4,0,2,0,1,
	1,1,4,1,0,0,0,1,
	2,0,0,0,0,0,0,1,
	2,0,0,0,0,1,0,1,
	2,0,0,0,0,0,0,1,
	1,1,3,1,3,1,3,1,
};

// Floors
int mapF[] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,
	0,0,0,0,2,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,2,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,
};

// Ceiling
int mapC[] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,0,
	0,1,3,1,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};

/*****************************************************************************/
/* Camera                                                                    */
/*****************************************************************************/
class Camera {
	vec2 position;
	vec2 direction;
	float rotation;
	float fieldOfView;
	bool keys[4];
	
public:
	Camera(const vec2& pos = vec2(), const float rot = 0.0f, const float fov = 60.0f) {
		setPosition(pos);
		setRotation(rot);
		setFieldOfView(fov);
		for (unsigned int index = 0; index < 4; ++index) {
			keys[index] = false;
		}
	}

	void setPosition(const vec2& value) {
		position = value;
	}

	const vec2& getPosition() const {
		return position;
	}

	void setRotation(const float value) {
		rotation = WrapAngle(value);
		direction.x =  cos(rotation * DEGTORAD);
		direction.y = -sin(rotation * DEGTORAD);
	}

	float getRotation() const {
		return rotation;
	}

	const vec2 getTarget(const float distance = 1.0f) const {
		return position + direction * distance;
	}

	void setFieldOfView(const float value) {
		fieldOfView = value;
	}

	float getFieldofView() const {
		return fieldOfView;
	}

	bool processEvent(const Event& event) {
		if (event.type != Event::KEYBOARD) {
			return false;
		}

		switch (event.key) {
		case KEY_UP :
		case KEY_W :
			keys[0] = event.state;
			return true;
		case KEY_LEFT :
		case KEY_A :
			keys[1] = event.state;
			return true;
		case KEY_DOWN :
		case KEY_S :
			keys[2] = event.state;
			return true;
		case KEY_RIGHT :
		case KEY_D :
			keys[3] = event.state;
			return true;
		case KEY_SPACE :
		case KEY_RETURN :
		case KEY_ENTER :
			{
				const vec2 target = getTarget(32.0f);
				const unsigned int index = (int)(target.y / 64.0f) * MapSize.x + (int)(target.x / 64.0f);
				switch (mapW[index]) {
				case 4: // Door
					mapW[index] = 0;
					break;
				}
			}
			return true;
		}

		return false;
	}

	void update(const float deltaTime = 1.0f) {
		if (keys[1]) {
			setRotation(getRotation() + deltaTime * 100.0f);
		}
		if (keys[3]) {
			setRotation(getRotation() - deltaTime * 100.0f);
		} 

		int xo = (direction.x < 0.0f) ? -20 : 20;
		int yo = (direction.y < 0.0f) ? -20 : 20;

		int ipx = position.x / 64.0f;		
		int ipy = position.y / 64.0f;
		
		// Move forward
		if (keys[0]) {
			const int ipx_add_xo = (position.x + xo) / 64.0f;
			const int ipy_add_yo = (position.y + yo) / 64.0f;
			
			if (mapW[ipy * MapSize.x + ipx_add_xo] == 0) {
				position.x += direction.x * deltaTime * 100.0f;
			}
			if (mapW[ipy_add_yo * MapSize.x + ipx] == 0) {
				position.y += direction.y * deltaTime * 100.0f;
			}
		}
		// Move backward
		if (keys[2]) {
			const int ipx_sub_xo = (position.x - xo) / 64.0f;
			const int ipy_sub_yo = (position.y - yo) / 64.0f;
			
			if (mapW[ipy * MapSize.x + ipx_sub_xo] == 0) {
				position.x -= direction.x * deltaTime * 100.0f;
			}
			if (mapW[ipy_sub_yo * MapSize.x + ipx] == 0) {
				position.y -= direction.y * deltaTime * 100.0f;
			}
		}
	}
};

void RenderScene(Image* output, const Camera* camera, uvec4 viewport = uvec4()) {
	vec2 rayEnd;
	vec2 verticalRayEnd;
	vec2 offset;
	vec2 axisDistance;
	ivec2 mapCoord;
	int depth; 
	int mapIndex;
	
	const vec2 playerPosition = camera->getPosition();
	const float playerAngle = camera->getRotation();
	const float fieldOfView = camera->getFieldofView();
	const uvec2 outputSize = output->getSize();

	const float DeltaRayAngle = (float)fieldOfView / (float)outputSize.x;
	const float WallScale = outputSize.y;
	const float FloorScale = (((int)WallScale) >> 1) - 1; // Rework - make map size dependent

	float rayAngle = WrapAngle(playerAngle + fieldOfView * 0.5f);
	
	for (int rayIndex = 0; rayIndex < outputSize.x; ++rayIndex) {
		int vmt = 0; // vertical map texture number 
		int hmt = 0; // horizontal map texture number 

		// Vertical intersection check
		depth = 0;
		axisDistance.y = 100000.0f;

		const float rayAngleRad = rayAngle * DEGTORAD;
		const float cosRayAngle = cosf(rayAngleRad);
		const float sinRayAngle = sinf(rayAngleRad);
		      float tanRayAngle = tanf(rayAngleRad);

		if (cosRayAngle > 0.001f) {
			// Looking left
			rayEnd.x = (((int)playerPosition.x >> 6) << 6) + 64.0f;
			rayEnd.y = (playerPosition.x - rayEnd.x) * tanRayAngle + playerPosition.y;
			offset.x = 64;
			offset.y = -offset.x * tanRayAngle;
		} else if (cosRayAngle < -0.001f) {
			// Looking right
			rayEnd.x = (((int)playerPosition.x >> 6) << 6) - 0.0001f;
			rayEnd.y = (playerPosition.x - rayEnd.x) * tanRayAngle + playerPosition.y;
			offset.x = -64.0f;
			offset.y = -offset.x * tanRayAngle;
		} else {
			// Looking up or down. no hit
			rayEnd = playerPosition;
			depth = MaxDepth;
		}

		while (depth < MaxDepth) { 
			mapCoord.x = (int)(rayEnd.x) >> 6;
			mapCoord.y = (int)(rayEnd.y) >> 6;
			mapIndex = mapCoord.y * MapSize.x + mapCoord.x;

			if ((mapIndex > 0) && (mapIndex < MapSize.x * MapSize.y) && (mapW[mapIndex] > 0)) {
				// Hit
				vmt = mapW[mapIndex] - 1;
				depth = MaxDepth;
				axisDistance.y = cosRayAngle * (rayEnd.x - playerPosition.x) - sinRayAngle * (rayEnd.y - playerPosition.y);
			} else {
				// Check next horizontal
				rayEnd += offset;
				++depth;
			}
		} 

		verticalRayEnd.x = rayEnd.x;
		verticalRayEnd.y = rayEnd.y;

		// Horizontal intersection check
		depth = 0; 
		axisDistance.x = 100000.0f;
		tanRayAngle = 1.0f / tanRayAngle; 

		if (sinRayAngle > 0.001f) {
			// Looking up 
			rayEnd.y = (((int)playerPosition.y >> 6) << 6) - 0.0001f;
			rayEnd.x = (playerPosition.y - rayEnd.y) * tanRayAngle + playerPosition.x;
			offset.y = -64.0f;
			offset.x = -offset.y * tanRayAngle;
		} else if (sinRayAngle < -0.001f) {
			// Looking down
			rayEnd.y = (((int)playerPosition.y >> 6) << 6) + 64.0f;
			rayEnd.x = (playerPosition.y - rayEnd.y) * tanRayAngle + playerPosition.x;
			offset.y = 64.0f;
			offset.x = -offset.y * tanRayAngle;
		} else {
			// Looking straight left or right
			rayEnd = playerPosition;
			depth = MaxDepth;
		}
 
		while (depth < MaxDepth) {
			mapCoord.x = (int)(rayEnd.x) >> 6;
			mapCoord.y = (int)(rayEnd.y) >> 6;
			mapIndex = mapCoord.y * MapSize.x + mapCoord.x;

			if ((mapIndex > 0) && (mapIndex < MapSize.x * MapSize.y) && (mapW[mapIndex] > 0)) {
				// Hit
				hmt = mapW[mapIndex] - 1;
				depth = MaxDepth;
				axisDistance.x = cosRayAngle * (rayEnd.x - playerPosition.x) - sinRayAngle * (rayEnd.y - playerPosition.y);
			} else {
				// Check next horizontal
				rayEnd += offset;
				++depth;
			}
		} 

		float shade = 1.0f;

		// Select the smallest wall distance 
		if (axisDistance.y < axisDistance.x) {
			hmt = vmt;
			shade = 0.5f;
			rayEnd.x = verticalRayEnd.x;
			rayEnd.y = verticalRayEnd.y;
			axisDistance.x = axisDistance.y;
		}

		// Fix fisheye
		axisDistance.x = axisDistance.x * cos(WrapAngle(rayAngle - playerAngle) * DEGTORAD);

		int lineHeight = (float)MapSizeLength * WallScale / axisDistance.x;
		const float ty_step = (float)TextureSize.y / (float)lineHeight;
		float ty_off = 0;

		if (lineHeight > outputSize.y) {
			// Line height and limit
			ty_off = ((float)lineHeight - (float)outputSize.y) * 0.5f;
			lineHeight = outputSize.y;
		}

		int lineOffset = (outputSize.y >> 1) - (lineHeight >> 1);

		// Draw walls
		int y;
		float ty = ty_off * ty_step + hmt * (float)TextureSize.y;
		float tx;

		// Fix texture orientation
		if (shade == 1) {
			tx = ((int)(rayEnd.x * 0.5f)) % TextureSize.x;
			if (rayAngle > 180.0f) {
				tx = TextureSize.x - 1 - tx;
			}
		} else {
			tx = ((int)(rayEnd.y * 0.5f)) % TextureSize.x;
			if ((rayAngle > 90.0f) && (rayAngle < 270.0f)) {
				tx = TextureSize.x - 1 - tx;
			}
		}

		for (y = 0; y < lineHeight; ++y) {
			float c = Textures[(int)ty * 32 + (int)tx] * shade;
			output->setRawPixel(rayIndex, y + lineOffset, Vector4ub(c * 255.0f, c * 255.0f, c * 255.0f, 255));
			ty += ty_step;
		}

		for (y = lineOffset + lineHeight; y < outputSize.y; ++y) {
			const float dy = y - (outputSize.y >> 1);
			const float raFix = cos(WrapAngle(playerAngle - rayAngle) * DEGTORAD);
			tx = playerPosition.x * 0.5f + cosRayAngle * FloorScale * 32 / dy / raFix;
			ty = playerPosition.y * 0.5f - sinRayAngle * FloorScale * 32 / dy / raFix;

			// Draw floors
			mapIndex = mapF[(int)(ty / 32.0f) * MapSize.x + (int)(tx / 32.0f)] * 32 * 32;
			float c = Textures[((int)(ty) & 31) * 32 + ((int)(tx) & 31) + mapIndex] * 0.7;
			output->setRawPixel(rayIndex, y, Vector4ub(c * 200.0f, c * 200.0f, c * 255.0f, 255));

			// Draw ceiling
			mapIndex = mapC[(int)(ty / 32.0f) * MapSize.x + (int)(tx / 32.0f)] * 32 * 32;
			c = Textures[((int)(ty) & 31) * 32 + ((int)(tx) & 31) + mapIndex] * 0.7f;
			output->setRawPixel(rayIndex, outputSize.y - y, Vector4ub(c * 128.0f, c * 255.0f, c * 128.0f, 255));
		}

		// Advance next ray angle
		rayAngle = WrapAngle(rayAngle - DeltaRayAngle);
	}
}

int main(int argc, char* argv[]) {
	/*************************************************************************/
	/* Output                                                                */
	/*************************************************************************/
	const uvec2 ScreenSize(640, 408);
#if defined (_WIN32)
	Win32Window output;
	if (output.initialize(ScreenSize, Image::EPF_R8G8B8, "Raycaster") == false) {
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
#else
	#error Unsupported platform!
#endif

	Image colorBuffer;
	colorBuffer.create(output.getSize(), output.getPixelFormat());
	colorBuffer.wrapping.x = Image::EWT_DISCARD;
	colorBuffer.wrapping.y = Image::EWT_DISCARD;

	/************************************************************************/
	/* Camera                                                               */
	/************************************************************************/
	Camera camera(vec2(2, 5) * 64.0f + 32.0f, 90.0f, 70.0f);

	/*************************************************************************/
	/* Misc.                                                                 */
	/*************************************************************************/
	unsigned long long lastTime = Timer::GetMilliSeconds();
	unsigned long long currentTime = lastTime;
	unsigned long long lastFPSTime = currentTime;
	unsigned int frameCount = 0;
	unsigned int totalFPS = 0;
	unsigned int totalSeconds = 0;
	unsigned int fps = 0;
	float deltaTime = 1.0f;
	
	const char* text = "OPEN DOOR";
	const int size = 4;
	const uvec2 textSize = GetTextSize(text, size);
	const uvec2 halfTextSize = textSize / 2;
	const uvec2 halfScreenSize = ScreenSize / 2;
	const uvec2 messagePosition(halfScreenSize.x - halfTextSize.x, ScreenSize.y - ScreenSize.y / 6 - textSize.y);
	
	Event event;
	bool running = true;

	while (running) {
		/*********************************************************************/
		/* Calculate FPS.                                                    */
		/*********************************************************************/
		currentTime = Timer::GetMilliSeconds();
		deltaTime = (currentTime - lastTime) * 0.001f;
		lastTime = currentTime;

		if (currentTime - lastFPSTime >= 1000) {
			totalFPS += frameCount;
			fps = frameCount;
			totalSeconds += 1;
			frameCount = 0;
			lastFPSTime += 1000;
		}

		/*********************************************************************/
		/* Check keyboard events.                                            */
		/*********************************************************************/
		while (output.getEvent(&event)) {
			if (camera.processEvent(event)) {
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
						running = false;
					}
					break;
				}
				break;
			}
		}

		/*********************************************************************/
		/* Render scene.                                                     */
		/*********************************************************************/
		camera.update(deltaTime);

		RenderScene(&colorBuffer, &camera);

		/*********************************************************************/
		/* GUI                                                               */
		/*********************************************************************/		
		DrawText(&colorBuffer, uvec2(0, 0), Vector4ub(255, 0, 0, 255), 4, "FPS:%d", fps);

		vec2 target = camera.getTarget(32.0f);
		unsigned int mapIndex = (int)(target.y / 64.0f) * MapSize.x + (int)(target.x / 64.0f);
		switch (mapW[mapIndex]) {
		case 4 :
			colorBuffer.drawFilledRectangle(vec4(messagePosition.x, messagePosition.y, messagePosition.x + textSize.x, messagePosition.y + textSize.y), vec4(0.0f, 1.0f, 0.0f, 1.0f));
			DrawText(&colorBuffer, messagePosition, text, Vector4ub(0, 0, 255, 255), size);
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
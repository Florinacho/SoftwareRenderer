#ifndef __FB_RENDERER_H__
#define __FB_RENDERER_H__

#include "FrameBuffer.h"
#include "RenderTarget.h"
#include "Renderer.h"

class FBRenderer : public Renderer {
	FrameBuffer frameBuffer;
	RenderTarget renderTarget;
	Image colorBuffer;
	Image depthBuffer;

public:
	int initialize(const char* fb_path, const Vector2u& size, const Image::PixelFormat& pixelFormat);

	void clear(bool color, bool depth);

	void swap();
};

#endif //__FB_RENDERER_H__

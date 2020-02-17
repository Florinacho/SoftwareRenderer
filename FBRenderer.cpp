#include <stdio.h>

#include "FBRenderer.h"

int FBRenderer::initialize(const char* fb_path, const Vector2u& size, const Image::PixelFormat& pixelFormat) {
	int status = 0;
	const vec4 viewport(0.0f, 0.0f, (float)size.x, (float)size.y);

	status = frameBuffer.initialize(fb_path, size, pixelFormat);
	if (status != FrameBuffer::ERR_NO_ERROR) {
		printf("Failed to initialize frame buffer.\nError: %s\n", 
			FrameBuffer::ErrorText[status]);
		return 1;
	}

	colorBuffer.create(size, frameBuffer.getPixelFormat());
	depthBuffer.create(size, Image::EPF_DEPTH);

	renderTarget.setBuffer(RenderTarget::ERT_COLOR_0, &colorBuffer);
	renderTarget.setBuffer(RenderTarget::ERT_DEPTH, &depthBuffer);

	setRenderTarget(&renderTarget);
	setViewport(viewport);

	return status;
}

void FBRenderer::clear(bool color, bool depth) {
	if (color) {
		colorBuffer.clearColor();
	}
	if (depth) {
		depthBuffer.clearColor();
	}
}

void FBRenderer::swap() {
	frameBuffer.draw(&colorBuffer);
}

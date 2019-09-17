#include <string.h>
#include "Graphics.h"
#include "Image.h"

#include "Matrix4.h"

//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
//https://github.com/joshb/linedrawing/blob/master/Rasterizer.cpp

void DefaultVertexShader2D(VertexShaderData& data, void* uniform) {
}
void DefaultPixelShader2D(PixelShaderData& data, void* uniform) {
	data.color = data.color * data.texture->getPixelByUV(data.uv);
}

void Graphics::drawLine(const Vector3f& begin, const Vector4f& beginColor, const Vector3f& end, const Vector4f& endColor) {
	float x1 = begin.x;
	float y1 = begin.y;
	float z1 = begin.z;

	float x2 = end.x;
	float y2 = end.y;
	float z2 = end.z;

	Vector4f color1 = beginColor;
	Vector4f color2 = endColor;
	
	float xdiff = (x2 - x1);
	float ydiff = (y2 - y1);
	float zdiff = (z2 - z1);

	if(xdiff == 0.0f && ydiff == 0.0f) {
		colorBuffer.setPixel(x1, y1, color1);
		return;
	}

	if(fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

		// set xmin to the lower x value given
		// and xmax to the higher value
		if(x1 < x2) {
			xmin = x1;
			xmax = x2;
		} else {
			xmin = x2;
			xmax = x1;
		}

		// draw line in terms of y slope
		float slope = ydiff / xdiff;
		for(float x = xmin; x <= xmax; x += 1.0f) {
			float y = y1 + ((x - x1) * slope);
			Vector4f color = color1 + ((color2 - color1) * ((x - x1) / xdiff));
			colorBuffer.setPixel(x, y, color);
		}
	} else {
		float ymin, ymax;

		// set ymin to the lower y value given
		// and ymax to the higher value
		if(y1 < y2) {
			ymin = y1;
			ymax = y2;
		} else {
			ymin = y2;
			ymax = y1;
		}

		// draw line in terms of x slope
		float slope = xdiff / ydiff;
		for(float y = ymin; y <= ymax; y += 1.0f) {
			float x = x1 + ((y - y1) * slope);
			Vector4f color = color1 + ((color2 - color1) * ((y - y1) / ydiff));
			colorBuffer.setPixel(x, y, color);
		}
	}
}


//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
inline float edgeFunction(const Vector4f& a, const Vector4f& b, const Vector4f& c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); 
}

inline int min(int a, int b, int c, int min) {
	int ans = a;
	if (b < a) {
		ans = b;
	}
	if (c < ans) {
		ans = c;
	}
	if (ans < min) {
		ans = min;
	}
	return ans;
}

inline int max(int a, int b, int c, int max) {
	int ans = a;
	if (b > a) {
		ans = b;
	}
	if (c > ans) {
		ans = c;
	}
	if (ans > max) {
		ans = max;
	}
	return ans;
}
#include <stdio.h>
//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
void Graphics::drawTriangle(
		const Vector3f& v0, const Vector2f& uv0, const Vector4f& c0, 
		const Vector3f& v1, const Vector2f& uv1, const Vector4f& c1, 
		const Vector3f& v2, const Vector2f& uv2, const Vector4f& c2) {

	VertexShaderData vertex[3];
	vertex[0].position = Vector4f(v0.x, v0.y, v0.z, 1.0f);
	vertex[0].uv = uv0;
	vertex[0].color = c0;

	vertex[1].position = Vector4f(v1.x, v1.y, v1.z, 1.0f);
	vertex[1].uv = uv1;
	vertex[1].color = c1;

	vertex[2].position = Vector4f(v2.x, v2.y, v2.z, 1.0f);
	vertex[2].uv = uv2;
	vertex[2].color = c2;

	if (vertexShaderCallback) {
		vertexShaderCallback(vertex[0], shaderUniform);
		vertexShaderCallback(vertex[1], shaderUniform);
		vertexShaderCallback(vertex[2], shaderUniform);

		for (unsigned int index = 0; index < 3; ++index) {
			if (vertex[index].position.w != 0.0f) {
				vertex[index].position.x /= vertex[index].position.w;
				vertex[index].position.y /= vertex[index].position.w;
				vertex[index].position.z /= vertex[index].position.w;
			}
//			Matrix4f viewportTransformation;
//			viewportTransformation.setViewport(viewport.x, viewport.y, viewport.z, viewport.w);
//			vertex[index].position = viewportTransformation * vertex[index].position;
		}
	} 

	const Vector2u size = colorBuffer.getSize();
	const int minX = min(vertex[0].position.x, vertex[1].position.x, vertex[2].position.x, 0);
	const int minY = min(vertex[0].position.y, vertex[1].position.y, vertex[2].position.y, 0);
	const int maxX = max(vertex[0].position.x, vertex[1].position.x, vertex[2].position.x, size.x);
	const int maxY = max(vertex[0].position.y, vertex[1].position.y, vertex[2].position.y, size.y);

	const float area = edgeFunction(vertex[0].position, vertex[1].position, vertex[2].position);

	if (area <= 0.0f) {
		return;
	}

	for (int j = minY; j <= maxY; ++j) {
		for (int i = minX; i <= maxX; ++i) {
			const Vector4f p((float)i + 0.5f, (float)j + 0.5f, 0.0f, 0.0f);
			Vector3f weight;
			if ((weight.x = edgeFunction(vertex[1].position, vertex[2].position, p)) < 0.0f) {
				continue;
			}
			if ((weight.y = edgeFunction(vertex[2].position, vertex[0].position, p)) < 0.0f) {
				continue;
			}
			if ((weight.z = edgeFunction(vertex[0].position, vertex[1].position, p)) < 0.0f) {
				continue;
			}
			weight /= area;

			const float depth = (vertex[0].position.z * weight.x + vertex[1].position.z * weight.y + vertex[2].position.z * weight.z);

//			if (fabs(1.0f - w) > 0.1f)printf("w = %f\n", w);

			// Clip relative z value
			if (depth < -6.0f || depth > 1.0f) {
//				continue;
			}

			if (depthTest && depth > depthBuffer.getPixel(i, j).x) {
//				continue;
			}

			PixelShaderData pixelShaderData;
			if (pixelShaderCallback) {
				pixelShaderData.texture = activeTexture;
				pixelShaderData.position = Vector3f(i, j, depth);
				pixelShaderData.uv = vertex[0].uv * weight.x + vertex[1].uv * weight.y + vertex[2].uv * weight.z;
				pixelShaderData.color = vertex[0].color * weight.x + vertex[1].color * weight.y + vertex[2].color * weight.z;

				pixelShaderCallback(pixelShaderData, shaderUniform);
			} 
		
			if (alphaBlend) {
				const Vector4f pixel = colorBuffer.getPixel(i, j);
				const float inv = 1.0f - pixelShaderData.color.w;
				pixelShaderData.color = pixelShaderData.color * pixelShaderData.color.w + pixel * inv;
			}
			colorBuffer.setPixel(i, j, pixelShaderData.color);

			if (depthMask) {
				depthBuffer.setPixel(i, j, Vector4f(depth, depth, depth, 1.0f));
			}
		}
	} 
}
	
Graphics::Graphics() {
	frameBuffer = NULL;
	activeTexture = NULL;

	vertexShaderCallback = NULL;
	pixelShaderCallback = NULL;
	shaderUniform = NULL;

	depthTest = true;
	depthMask = true;
	alphaBlend = false;
}

Graphics::~Graphics() {
	uninitialize();
}

bool Graphics::initialize(FrameBuffer* inframeBuffer, int depthBPP) {
	frameBuffer = inframeBuffer;
	if (frameBuffer == NULL) { 
		return false;
	}

	switch (frameBuffer->getBytesPerPixel()) {
	case 2 :
		colorBuffer.create(frameBuffer->getSize(), Image::EPF_R5G6B5);
		break;
	case 3 :
		colorBuffer.create(frameBuffer->getSize(), Image::EPF_R8G8B8);
		break;
	case 4 :
		colorBuffer.create(frameBuffer->getSize(), Image::EPF_R8G8B8A8);
		break;
	default :
		return false;
		break;
	}

	switch (depthBPP) {
	case 8 :
		depthBuffer.create(frameBuffer->getSize(), Image::EPF_L8);
		break;
	default :
		return false;
		break;
	}

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.z = frameBuffer->getSize().x;
	viewport.w = frameBuffer->getSize().y;

	return true;
}

void Graphics::uninitialize() {
	colorBuffer.removeData();
	depthBuffer.removeData();
}

void Graphics::setViewport(const Vector4f& value) {
	viewport = value;
}

const Vector4f& Graphics::getViewport() const {
	return viewport;
}

void Graphics::setDepthTest(bool value) {
	depthTest = value;
}

bool Graphics::getDepthTest() const {
	return depthTest;
}

void Graphics::setDepthMask(bool value) {
	depthMask = value;
}

bool Graphics::getDepthMask() const {
	return depthMask;
}

void Graphics::setAlphaBlend(bool value) {
	alphaBlend = value;
}

bool Graphics::getAlphaBlend() const {
	return alphaBlend;
}

Image* Graphics::getColorBuffer() {
	return &colorBuffer;
}

void Graphics::setVertexShaderCallback(VertexShaderCallback callback) {
	vertexShaderCallback = callback;
}

void Graphics::setPixelShaderCallback(PixelShaderCallback callback) {
	pixelShaderCallback = callback;
}

void Graphics::setShaderUniform(void* uniform) {
	shaderUniform = uniform;
}

void Graphics::setActiveTexture(const Image* texture) {
	activeTexture = texture;
}

const Image* Graphics::getActiveTexture() const {
	return activeTexture;
}

Image* Graphics::getDepthBuffer() {
	return &depthBuffer;
}
	
void Graphics::clear() {
	colorBuffer.clearColor();
	depthBuffer.clearColor();
}

void Graphics::drawPrimitive(int primitiveType, const Vertex* vertices, unsigned int vertexCount) {
	switch (primitiveType) {
	case EPT_LINES :
		if (vertexCount < 2) {
			return;
		}
		for (unsigned int index = 0; index < vertexCount - 1; index += 2) {
			drawLine(vertices[index + 0].position, vertices[index + 0].color, vertices[index + 1].position, vertices[index + 1].color);
		}
		break;
	case EPT_LINE_STRIP :
		for (unsigned int index = 1; index < vertexCount; index ++) {
			drawLine(vertices[index - 1].position, vertices[index - 1].color, vertices[index].position, vertices[index].color);
		}
		break;
	case EPT_TRIANGLES : 
		for (unsigned int index = 0; index < vertexCount - 2; index += 3) {
			drawTriangle(
				vertices[index + 0].position, vertices[index + 0].textureCoords, vertices[index + 0].color, 
				vertices[index + 1].position, vertices[index + 1].textureCoords, vertices[index + 1].color,
				vertices[index + 2].position, vertices[index + 2].textureCoords, vertices[index + 2].color);
		}
		break;
	case EPT_TRIANGLE_STRIP :
		for (unsigned int index = 2, k = 1; index < vertexCount; ++index, k = !k) {
			drawTriangle(
				vertices[index - 0].position, vertices[index - 0].textureCoords, vertices[index - 0].color,
				vertices[index - (k ? 1 : 2)].position, vertices[index - (k ? 1 : 2)].textureCoords, vertices[index - (k ? 1 : 2)].color,
				vertices[index - (k ? 2 : 1)].position, vertices[index - (k ? 2 : 1)].textureCoords, vertices[index - (k ? 2 : 1)].color);
		}
		break;
	}
}

void Graphics::drawLine(const Vector2f& begin, const Vector2f& end, const Vector4f& color) {
	const Vertex vertices[] = {
		Vertex(Vector3f(begin.x, begin.y, 0.0f), Vector2f(0.0f, 0.0f), color),
		Vertex(Vector3f(end.x, end.y, 0.0f), Vector2f(1.0f, 1.0f), color)
	};

	setDepthTest(false);
	setDepthMask(false);
	setAlphaBlend((color.w < 1.0f) || ((activeTexture == NULL) ? activeTexture->hasAlpha() : false));

	setVertexShaderCallback(DefaultVertexShader2D);
	setPixelShaderCallback(DefaultPixelShader2D);
	drawPrimitive(EPT_LINES, vertices, 2);
}

void Graphics::drawRectangle(const Vector4f& rectangle, const Vector4f& color) {
	const Vertex vertices[] = {
		Vertex(Vector3f(rectangle.x, rectangle.y, 0.0f), Vector2f(0.0f, 0.0f), color),
		Vertex(Vector3f(rectangle.z, rectangle.y, 0.0f), Vector2f(1.0f, 0.0f), color),
		Vertex(Vector3f(rectangle.x, rectangle.w, 0.0f), Vector2f(0.0f, 1.0f), color),
		Vertex(Vector3f(rectangle.z, rectangle.w, 0.0f), Vector2f(1.0f, 1.0f), color)
	};

	setDepthTest(false);
	setDepthMask(false);
	setAlphaBlend((color.w < 1.0f) || ((activeTexture == NULL) ? activeTexture->hasAlpha() : false));

	setVertexShaderCallback(DefaultVertexShader2D);
	setPixelShaderCallback(DefaultPixelShader2D);
	drawPrimitive(EPT_TRIANGLE_STRIP, vertices, 4);
}

void Graphics::swap() {
	if (frameBuffer == NULL) {
		return;
	}

	frameBuffer->draw(&colorBuffer);
}

#include <stdio.h>
#include <string.h>

#include "Renderer.h"

//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
//https://github.com/joshb/linedrawing/blob/master/Rasterizer.cpp

template<typename T>
inline T min(const T a, const T b) {
	return (a < b) ? a : b;
}

template<typename T>
inline T max(const T a, const T b) {
	return (a < b) ? b : a;
}

void Renderer::drawLine(const vec3& begin, const vec4& beginColor, const vec3& end, const vec4& endColor) {
	VertexShaderData vertex[2];
	vertex[0].position = vec4(begin, 1.0f);
	vertex[0].color = beginColor;
	vertex[1].position = vec4(end, 1.0f);
	vertex[1].color = endColor;

	if (activeShader) {
		for (unsigned int index = 0; index < 2; ++index) {
			activeShader->vertexShader(vertex[index]);
			vertex[index].position /= vertex[index].position.w;
			vertex[index].position = viewportTransformation * vertex[index].position;
		}
	}
	
	float xdiff = (vertex[1].position.x - vertex[0].position.x);
	float ydiff = (vertex[1].position.y - vertex[0].position.y);
	float zdiff = (vertex[1].position.z - vertex[0].position.z);

	const uvec2 size = colorBufferPtr->getSize();

	if(xdiff == 0.0f && ydiff == 0.0f) {
		colorBufferPtr->setPixel(vertex[0].position.x, vertex[0].position.y, beginColor);
		return;
	}

	if(fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

		if(vertex[0].position.x < vertex[1].position.x) {
			xmin = vertex[0].position.x;
			xmax = vertex[1].position.x;
		} else {
			xmin = vertex[1].position.x;
			xmax = vertex[0].position.x;
		}
		
		xmin = max(xmin, 0.0f);
		xmax = min(xmax, (float)(size.x - 1));

		// draw line in terms of y slope
		float slope = ydiff / xdiff;
		for(float x = xmin; x <= xmax; x += 1.0f) {
			const float y = vertex[0].position.y + ((x - vertex[0].position.x) * slope);
			const int invY = size.y - y - 1;
			const float k = (x - vertex[0].position.x) / xdiff;
			const float depth = 1.0f - (vertex[0].position.z + (vertex[1].position.z - vertex[0].position.z) * k);
			const vec4 color = beginColor + ((endColor - beginColor) * k);

			if (depth < 0.0f || depth > 1.0f) {
				continue;
			}

			if (renderFlags[ERF_DEPTH_TEST] && depth < depthBufferPtr->getPixel(x, invY).x) {
				continue;
			}

			colorBufferPtr->setPixel(x, invY, color);

			if (renderFlags[ERF_DEPTH_MASK]) {
				depthBufferPtr->setPixel(x, invY, vec4(depth, depth, depth, 1.0f));
			}
		}
	} else {
		float ymin, ymax;

		if(vertex[0].position.y < vertex[1].position.y) {
			ymin = vertex[0].position.y;
			ymax = vertex[1].position.y;
		} else {
			ymin = vertex[1].position.y;
			ymax = vertex[0].position.y;
		}

		// draw line in terms of x slope
		float slope = xdiff / ydiff;
		for(float y = ymin; y <= ymax; y += 1.0f) {
			const int invY = size.y - y - 1;
			const float x = vertex[0].position.x + ((y - vertex[0].position.y) * slope);
			const float k = (y - vertex[0].position.y) / ydiff;
			const float depth = 1.0f - (vertex[0].position.z + (vertex[1].position.z - vertex[0].position.z) * k);
			const vec4 color = beginColor + (endColor - beginColor) * k;

			if (depth < 0.0f || depth > 1.0f) {
				continue;
			}

			if (renderFlags[ERF_DEPTH_TEST] && depth < depthBufferPtr->getPixel(x, invY).x) {
				continue;
			}

			colorBufferPtr->setPixel(x, invY, color);

			if (renderFlags[ERF_DEPTH_MASK]) {
				depthBufferPtr->setPixel(x, invY, vec4(depth, depth, depth, 1.0f));
			}
		}
	}
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
inline float edgeFunction(const vec4& a, const vec4& b, const vec4& c) {
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); 
}

int min(int a, int b, int c, int d) {// printf("min(%d, %d, %d, %d)\n", a, b, c, d);
	int ans = a;
	if (b < ans) {
		ans = b;
	}
	if (c < ans) {
		ans = c;
	}
	if (ans < d) {
		ans = d;
	}
	return ans;
}

inline int max(int a, int b, int c, int d) {// printf("max(%d, %d, %d, %d)\n", a, b, c, d);
	int ans = a;
	if (b > ans) {
		ans = b;
	}
	if (c > ans) {
		ans = c;
	}
	if (ans > d) {
		ans = d;
	}
	return ans;
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
void Renderer::drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
	VertexShaderData vertex[3];
	vertex[0].position = vec4(v0.position, 1.0f);
	vertex[0].normal = vec4(v0.normal, 0.0f);
	vertex[0].uv = v0.textureCoords;
	vertex[0].color = v0.color;
	vertex[0].index = 0;

	vertex[1].position = vec4(v1.position, 1.0f);
	vertex[1].normal = vec4(v1.normal, 0.0f);
	vertex[1].uv = v1.textureCoords;
	vertex[1].color = v1.color;
	vertex[1].index = 1;

	vertex[2].position = vec4(v2.position, 1.0f);
	vertex[2].normal = vec4(v2.normal, 0.0f);
	vertex[2].uv = v2.textureCoords;
	vertex[2].color = v2.color;
	vertex[2].index = 2;

	float vz[3];

	if (activeShader) {
		for (unsigned int index = 0; index < 3; ++index) {
			if (activeShader->totalVaryingData != NULL) {
				activeShader->varying = activeShader->totalVaryingData + index * activeShader->varyingCount;
			}
			// printf("1. position[%d] = %f\n", index, vertex[index].position.y);
			activeShader->vertexShader(vertex[index]);

			if (renderFlags[GFX_PERSPECTIVE_CORRECT]) {
				vertex[index].uv /= vertex[index].position.z;
				vz[index] = 1.0f / vertex[index].position.z;
			}

			// Normalize the display coordinates
			// printf("2. position[%d] = %f\n", index, vertex[index].position.y);
			vertex[index].position /= vertex[index].position.w;

			// Scale the coordinates to the viewport size
			// printf("3. position[%d] = %f\n", index, vertex[index].position.y);
			vertex[index].position = viewportTransformation * vertex[index].position;
			// printf("3. position[%d] = %f\n", index, vertex[index].position.y);
		}
	} 
	
	const float area = edgeFunction(vertex[0].position, vertex[1].position, vertex[2].position);

	if (area <= 0.0f) {
		return;
	}	

	const uvec2 size = colorBufferPtr->getSize();
	// printf("4. position = %f, %f, %f\n", vertex[0].position.x, vertex[1].position.x, vertex[2].position.x);
	 int minX = min(vertex[0].position.x, vertex[1].position.x, vertex[2].position.x, 0);if (minX < 0)minX = 0;
	 int minY = min((int)vertex[0].position.y, (int)vertex[1].position.y, (int)vertex[2].position.y, 0);if (minY < 0)minY = 0;
	const int maxX = max(vertex[0].position.x, vertex[1].position.x, vertex[2].position.x, (int)size.x - 1);
	const int maxY = max(vertex[0].position.y, vertex[1].position.y, vertex[2].position.y, (int)size.y - 1);
	
	// printf("%d <= X <= %d\n", minX, maxX);
	// printf("%d <= Y <= %d\n", minY, maxY);

	for (int j = minY; j <= maxY; ++j) {
		for (int i = minX; i <= maxX; ++i) {
			int invJ = size.y - 1 - j;
			const vec4 p((float)i + 0.5f, (float)j + 0.5f, 0.0f, 0.0f);
			vec3 weight;
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

			const float kdepth = (
					vertex[0].position.z * weight.x + 
					vertex[1].position.z * weight.y + 
					vertex[2].position.z * weight.z);
			const float depth = 1.0f - kdepth;
			
			float nz = 1.0f;
			if (renderFlags[GFX_PERSPECTIVE_CORRECT]) {
				nz = 1.0f / (weight.x * vz[0] + weight.y * vz[1] + weight.z * vz[2]);
			}

			if (depth < 0.0f || depth > 1.0f) {
				continue;
			}

			if (renderFlags[ERF_DEPTH_TEST] && depth < depthBufferPtr->getPixel(i, invJ).x) {
				continue;
			}
			
			// Prepare for pixel shader
			PixelShaderData pixelShaderData;

			for (unsigned int index = 0; index < MaxTextureCount; ++index) {
				pixelShaderData.texture[index] = activeTexture[index];
			}

			pixelShaderData.normal = 
					vertex[0].normal * weight.x + 
					vertex[1].normal * weight.y + 
					vertex[2].normal * weight.z;				

			pixelShaderData.uv = 
					vertex[0].uv * weight.x + 
					vertex[1].uv * weight.y + 
					vertex[2].uv * weight.z;
					
			if (renderFlags[GFX_PERSPECTIVE_CORRECT]) {
				pixelShaderData.uv *= nz;
			}
			
			pixelShaderData.color = 
					vertex[0].color * weight.x + 
					vertex[1].color * weight.y + 
					vertex[2].color * weight.z;

			if (activeShader->totalVaryingData != NULL) {
				const unsigned int vxc = 3;
				const unsigned int vrc = activeShader->varyingCount;
				
				for (unsigned int index = 0; index < vrc; ++index) {
					activeShader->totalVaryingData[vxc * vrc + index] = 
						activeShader->totalVaryingData[0 * vrc + index] * weight.x + 
						activeShader->totalVaryingData[1 * vrc + index] * weight.y + 
						activeShader->totalVaryingData[2 * vrc + index] * weight.z;
				}
				activeShader->varying = activeShader->totalVaryingData + 3 * activeShader->varyingCount;
			}

			activeShader->pixelShader(pixelShaderData);

			if (renderFlags[ERF_ALPHA_BLEND]) {
				const vec4 pixel = colorBufferPtr->getPixel(i, invJ);
				const float inv = 1.0f - pixelShaderData.color.w;
				pixelShaderData.color = pixelShaderData.color * pixelShaderData.color.w + pixel * inv;
			}
			colorBufferPtr->setPixel(i, invJ, pixelShaderData.color);

			if (renderFlags[ERF_DEPTH_MASK]) {
				depthBufferPtr->setPixel(i, invJ, vec4(depth, depth, depth, 1.0f));
			}
		}
	} 
}
	
Renderer::Renderer() {
	renderTarget = NULL;
	for (unsigned int index = 0; index < MaxTextureCount; ++index) {
		activeTexture[index] = NULL;
	}

	activeShader = NULL;

	for (unsigned int index = 0; index < ERF_COUNT; ++index) {
		renderFlags[index] = false;
	}
	renderFlags[GFX_PERSPECTIVE_CORRECT] = true;
}

Renderer::~Renderer() {
}

void Renderer::setRenderTarget(RenderTarget* newRenderTarget) {
	renderTarget = newRenderTarget;
	if (renderTarget != NULL) {
		colorBufferPtr = renderTarget->getBuffer(RenderTarget::ERT_COLOR_0);
		depthBufferPtr = renderTarget->getBuffer(RenderTarget::ERT_DEPTH);
	}
}

RenderTarget* Renderer::getRenderTarget() const {
	return renderTarget;
}

void Renderer::setFlag(const RenderFlag renderFlag, bool value) {
	renderFlags[renderFlag] = value;
}

bool Renderer::getFlag(const RenderFlag renderFlag) const {
	return renderFlags[renderFlag];
}

bool Renderer::toggleFlag(const RenderFlag renderFlag) {
	renderFlags[renderFlag] = !renderFlags[renderFlag];
	return renderFlags[renderFlag];
}

void Renderer::setViewport(const vec4& value) {
	if (viewport == value) {
		return;
	}

	viewport = value;

	viewportTransformation.setViewport(viewport.x, viewport.y, viewport.z, viewport.w);
	orthogonalProjection.setOrthogonal(0.0f, viewport.getWidth(), 0.0f, viewport.getHeight(), 0.0f, 1.0f);
}

const vec4& Renderer::getViewport() const {
	return viewport;
}

void Renderer::setActiveTexture(const unsigned int index, const Image* texture) {
	if (index >= MaxTextureCount) {
		return;
	}

	activeTexture[index] = texture;
}

const Image* Renderer::getActiveTexture(const unsigned int index) const {
	if (index >= MaxTextureCount) {
		return NULL;
	}

	return activeTexture[index];
}

void Renderer::setShader(Shader* shader) {
	activeShader = shader;
}

Shader* Renderer::getShader() const {
	return activeShader;
}

void Renderer::render(const PrimitiveType primitiveType, const Vertex* vertices, const unsigned int vertexCount) {
	if (renderTarget == NULL) {
		return;
	}

	switch (primitiveType) {
	case EPT_LINES :
		if (vertexCount < 2) {
			return;
		}
		for (unsigned int index = 0; index < vertexCount - 1; index += 2) {
			drawLine(
				vertices[index + 0].position, vertices[index + 0].color, 
				vertices[index + 1].position, vertices[index + 1].color);
		}
		break;
		
	case EPT_LINE_STRIP :
		for (unsigned int index = 1; index < vertexCount; index ++) {
			drawLine(
				vertices[index - 1].position, vertices[index - 1].color, 
				vertices[index + 0].position, vertices[index + 0].color);
		}
		break;
		
	case EPT_TRIANGLES :
		if (vertexCount < 3) {
			return;
		}
		for (unsigned int index = 0; index < vertexCount - 2; index += 3) {
			if (renderFlags[GFX_WIREFRAME]) {
				drawLine(vertices[index + 0].position, vertices[index + 0].color, vertices[index + 1]. position, vertices[index + 1].color);
				drawLine(vertices[index + 1].position, vertices[index + 1].color, vertices[index + 2]. position, vertices[index + 2].color);
				drawLine(vertices[index + 2].position, vertices[index + 2].color, vertices[index + 0]. position, vertices[index + 0].color);
			} else {
				drawTriangle(vertices[index + 0], vertices[index + 1], vertices[index + 2]);
			}
		}
		break;
		
	case EPT_TRIANGLE_STRIP :
		for (unsigned int index = 2, k = 1; index < vertexCount; ++index, k = !k) {
			if (renderFlags[GFX_WIREFRAME]) {
//				drawLine(vertices[index + 0].position, vertices[index + 0].color, vertices[index + 1]. position, vertices[index + 1].color);
//				drawLine(vertices[index + 1].position, vertices[index + 1].color, vertices[index + 2]. position, vertices[index + 2].color);
//				drawLine(vertices[index + 2].position, vertices[index + 2].color, vertices[index + 0]. position, vertices[index + 0].color);
			} else {
				drawTriangle(vertices[index - 0], vertices[index - (k ? 2 : 1)], vertices[index - (k ? 1 : 2)]);
			}
		}
		break;
	}
}

void Renderer::render(const PrimitiveType primitiveType, const Vertex* vertices, const unsigned int vertexCount, const unsigned int* indices, const unsigned int indexCount) {
	if (renderTarget == NULL) {
		return;
	}

	switch (primitiveType) {
	case EPT_LINES :
		if (indexCount < 2) {
			return;
		}
		for (unsigned int index = 0; index < indexCount - 1; index += 2) {
			drawLine(
				vertices[indices[index + 0]].position, vertices[indices[index + 0]].color, 
				vertices[indices[index + 1]].position, vertices[indices[index + 1]].color);
		}
		break;
		
	case EPT_LINE_STRIP :
		for (unsigned int index = 1; index < indexCount; index ++) {
			drawLine(
				vertices[indices[index - 1]].position, vertices[indices[index - 1]].color, 
				vertices[indices[index + 0]].position, vertices[indices[index + 0]].color);
		}
		break;
		
	case EPT_TRIANGLES :
		if (indexCount < 3) {
			return;
		}
		for (unsigned int index = 0; index < indexCount - 2; index += 3) {
			
			drawTriangle(
				vertices[indices[index + 0]],
				vertices[indices[index + 1]],
				vertices[indices[index + 2]]);
		}
		break;
		
	case EPT_TRIANGLE_STRIP :
		for (unsigned int index = 2, k = 1; index < indexCount; ++index, k = !k) {
			drawTriangle(
				vertices[indices[index - 0]],
				vertices[indices[index - (k ? 2 : 1)]],
				vertices[indices[index - (k ? 1 : 2)]]);
		}
		break;
	}
}
/*
void Renderer::draw2DLine(const vec2& begin, const vec2& end, const vec4& color) {
	const Vertex vertices[] = {
		Vertex(vec3(begin.x, begin.y, 0.0f), vec2(0.0f, 0.0f), color),
		Vertex(vec3(end.x,   end.y,   0.0f), vec2(1.0f, 1.0f), color)
	};

	setFlag(ERF_DEPTH_TEST, false);
	setFlag(ERF_DEPTH_MASK, false);
	setFlag(ERF_ALPHA_BLEND, 
		(color.w < 1.0f) || 
		((activeTexture[0] == NULL) ? 
			false : activeTexture[0]->hasAlpha()));

	setVertexShaderCallback(DefaultVertexShader);
	setPixelShaderCallback(DefaultPixelShader);

	setShaderUniform(&orthogonalProjection);

	render(EPT_LINES, vertices, 2);
}

void Renderer::draw2DRectangle(const vec4& rectangle, const vec4& color) {
	const Vertex vertices[] = {
		Vertex(vec3(rectangle.x, rectangle.y, 0.0f), vec2(0.0f, 0.0f), color),
		Vertex(vec3(rectangle.z, rectangle.y, 0.0f), vec2(1.0f, 0.0f), color),
		Vertex(vec3(rectangle.x, rectangle.w, 0.0f), vec2(0.0f, 1.0f), color),
		Vertex(vec3(rectangle.z, rectangle.w, 0.0f), vec2(1.0f, 1.0f), color)
	};

	setFlag(ERF_DEPTH_TEST, false);
	setFlag(ERF_DEPTH_MASK, false);
	setFlag(ERF_ALPHA_BLEND, 
		(color.w < 1.0f) || 
		((activeTexture[0] == NULL) ? 
			false : activeTexture[0]->hasAlpha()));

	setVertexShaderCallback(DefaultVertexShader);
	setPixelShaderCallback(DefaultPixelShader);

	setShaderUniform(&orthogonalProjection);

	render(EPT_TRIANGLE_STRIP, vertices, 4);
}

void Renderer::draw2DImage(Image* image, const vec4& rectangle, const vec4 color) {
	const Vertex vertices[] = {
		Vertex(vec3(rectangle.x, rectangle.y, 0.0f), vec2(0.0f, 0.0f), color),
		Vertex(vec3(rectangle.z, rectangle.y, 0.0f), vec2(1.0f, 0.0f), color),
		Vertex(vec3(rectangle.x, rectangle.w, 0.0f), vec2(0.0f, 1.0f), color),
		Vertex(vec3(rectangle.z, rectangle.w, 0.0f), vec2(1.0f, 1.0f), color)
	};

	setActiveTexture(0, image);

	setFlag(ERF_DEPTH_TEST, false);
	setFlag(ERF_DEPTH_MASK, false);
	setFlag(ERF_ALPHA_BLEND, 
		(color.w < 1.0f) || 
		((activeTexture[0] == NULL) ? 
			false : activeTexture[0]->hasAlpha()));

	setVertexShaderCallback(DefaultVertexShader);
	setPixelShaderCallback(DefaultPixelShader);

	setShaderUniform(&orthogonalProjection);

	render(EPT_TRIANGLE_STRIP, vertices, 4);
}
*/
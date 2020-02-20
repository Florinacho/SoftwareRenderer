#include <stdio.h>
#include <string.h>

#include "Renderer.h"
#include "Image.h"

#include "Matrix4.h"

//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
//https://github.com/joshb/linedrawing/blob/master/Rasterizer.cpp

void DefaultVertexShader(VertexShaderData& vertex) {
	const mat4 projection = *reinterpret_cast<const mat4*>(vertex.uniform);
	vertex.position = projection * vertex.position;
}

void DefaultPixelShader(PixelShaderData& data) {
	if (data.texture[0] == NULL) {
		return;
	}
	data.color *= data.texture[0]->getPixelByUV(data.uv);
}

template<typename T>
inline T min(const T a, const T b) {
	return (a < b) ? a : b;
}

template<typename T>
inline T max(const T a, const T b) {
	return (a < b) ? b : a;
}

void Renderer::drawLine(const Vector3f& begin, const Vector4f& beginColor, 
			const Vector3f& end, const Vector4f& endColor) {
	VertexShaderData vertex[2];
	vertex[0].position = Vector4f(begin, 1.0f);
	vertex[0].color = beginColor;
	vertex[0].uniform = shaderUniform;

	vertex[1].position = Vector4f(end, 1.0f);
	vertex[1].color = endColor;
	vertex[1].uniform = shaderUniform;

	if (vertexShaderCallback) {
		for (unsigned int index = 0; index < 2; ++index) {
			vertexShaderCallback(vertex[index]);
			vertex[index].position /= vertex[index].position.w;
			vertex[index].position = viewportTransformation * vertex[index].position;
		}
	}

	float x1 = vertex[0].position.x;
	float y1 = vertex[0].position.y;
	float z1 = vertex[0].position.z;

	float x2 = vertex[1].position.x;
	float y2 = vertex[1].position.y;
	float z2 = vertex[1].position.z;

	Vector4f color1 = beginColor;
	Vector4f color2 = endColor;
	
	float xdiff = (x2 - x1);
	float ydiff = (y2 - y1);
	float zdiff = (z2 - z1);

	const Vector2u size = colorBufferPtr->getSize();

	if(xdiff == 0.0f && ydiff == 0.0f) {
		colorBufferPtr->setPixel(x1, y1, color1);
		return;
	}

	if(fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

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
			const float y = y1 + ((x - x1) * slope);
			const int invY = size.y - y;
			const float k = (x - x1) / xdiff;
			const float depth = 1.0f - (z1 + (z2 - z1) * k);
			const Vector4f color = color1 + ((color2 - color1) * k);

			if (depth < 0.0f || depth > 1.0f) {
				continue;
			}

			if (renderFlags[ERF_DEPTH_TEST] && depth < depthBufferPtr->getPixel(x, invY).x) {
				continue;
			}

			colorBufferPtr->setPixel(x, invY, color);

			if (renderFlags[ERF_DEPTH_MASK]) {
				depthBufferPtr->setPixel(x, invY, Vector4f(depth, depth, depth, 1.0f));
			}
		}
	} else {
		float ymin, ymax;

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
			const int invY = size.y - y;
			const float x = x1 + ((y - y1) * slope);
			const float k = (y - y1) / ydiff;
			const float depth = 1.0f - (z1 + (z2 - z1) * k);
			const Vector4f color = color1 + (color2 - color1) * k;

			if (depth < 0.0f || depth > 1.0f) {
				continue;
			}

			if (renderFlags[ERF_DEPTH_TEST] && depth < depthBufferPtr->getPixel(x, invY).x) {
				continue;
			}

			colorBufferPtr->setPixel(x, invY, color);

			if (renderFlags[ERF_DEPTH_MASK]) {
				depthBufferPtr->setPixel(x, invY, Vector4f(depth, depth, depth, 1.0f));
			}
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

//https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
void Renderer::drawTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
	VertexShaderData vertex[3];
	vertex[0].position = Vector4f(v0.position, 1.0f);
	vertex[0].normal = Vector4f(v0.normal, 0.0f);
	vertex[0].uv = v0.textureCoords;
	vertex[0].color = v0.color;
	vertex[0].uniform = shaderUniform;
	vertex[0].index = 0;

	vertex[1].position = Vector4f(v1.position, 1.0f);
	vertex[1].normal = Vector4f(v1.normal, 0.0f);
	vertex[1].uv = v1.textureCoords;
	vertex[1].color = v1.color;
	vertex[1].uniform = shaderUniform;
	vertex[1].index = 1;

	vertex[2].position = Vector4f(v2.position, 1.0f);
	vertex[2].normal = Vector4f(v2.normal, 0.0f);
	vertex[2].uv = v2.textureCoords;
	vertex[2].color = v2.color;
	vertex[2].uniform = shaderUniform;
	vertex[2].index = 2;

	UniformVariable tmp[3];

	if (vertexShaderCallback) {
		for (unsigned int index = 0; index < 3; ++index) {
			if (shader) {
				shader->VertexShader(vertex[index]);
				tmp[index] = shader->uniform[0].get<vec4>();
			} else {
				vertexShaderCallback(vertex[index]);
			}


			// Normalize the display coordinates
			vertex[index].position /= vertex[index].position.w;
//			vertex[index].normal /= vertex[index].normal.w;

			// Scale the coordinates to the viewport size
			vertex[index].position = viewportTransformation * vertex[index].position;
		}
	} 

	const Vector2u size = colorBufferPtr->getSize();
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
			int invJ = size.y - j;
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

			const float depth = 1.0f - (
					vertex[0].position.z * weight.x + 
					vertex[1].position.z * weight.y + 
					vertex[2].position.z * weight.z);

			if (depth < 0.0f || depth > 1.0f) {
				continue;
			}

			if (renderFlags[ERF_DEPTH_TEST] && depth < depthBufferPtr->getPixel(i, invJ).x) {
				continue;
			}

			PixelShaderData pixelShaderData;
			if (pixelShaderCallback) {
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

				pixelShaderData.color = 
						vertex[0].color * weight.x + 
						vertex[1].color * weight.y + 
						vertex[2].color * weight.z;
				pixelShaderData.uniform = shaderUniform;

				pixelShaderData.eye = 
						vertex[0].eye * weight.x + 
						vertex[1].eye * weight.y + 
						vertex[2].eye * weight.z;	

				pixelShaderData.lightDirection = 
						vertex[0].lightDirection * weight.x + 
						vertex[1].lightDirection * weight.y + 
						vertex[2].lightDirection * weight.z;	

				if (shader) {
					for (unsigned int index = 0; index < shader->uniform.size(); ++index) {
						switch (shader->uniform[index].dataType) {
						case ShaderVariable::EDT_VEC4 :
							shader->uniform[index] = tmp[0].get<vec4>() * weight.x + tmp[1].get<vec4>() * weight.y + tmp[2].get<vec4>() * weight.z;
							break;
						}
					}
					shader->FragmentShader(pixelShaderData);
				} else {
					pixelShaderCallback(pixelShaderData);
				}
			} 

			if (renderFlags[ERF_ALPHA_BLEND]) {
				const Vector4f pixel = colorBufferPtr->getPixel(i, invJ);
				const float inv = 1.0f - pixelShaderData.color.w;
				pixelShaderData.color = pixelShaderData.color * pixelShaderData.color.w + pixel * inv;
			}
			colorBufferPtr->setPixel(i, invJ, pixelShaderData.color);

			if (renderFlags[ERF_DEPTH_MASK]) {
				depthBufferPtr->setPixel(i, invJ, Vector4f(depth, depth, depth, 1.0f));
			}
		}
	} 
}
	
Renderer::Renderer() {
	renderTarget = NULL;
	for (unsigned int index = 0; index < MaxTextureCount; ++index) {
		activeTexture[index] = NULL;
	}

	vertexShaderCallback = NULL;
	pixelShaderCallback = NULL;
	shader = NULL;
	shaderUniform = NULL;

	for (unsigned int index = 0; index < ERF_COUNT; ++index) {
		renderFlags[index] = false;
	}
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

void Renderer::setViewport(const Vector4f& value) {
	if (viewport == value) {
		return;
	}

	viewport = value;

	viewportTransformation.setViewport(viewport.x, viewport.y, viewport.z, viewport.w);
	orthogonalProjection.setOrthogonal(0.0f, viewport.getWidth(), 0.0f, viewport.getHeight(), 0.0f, 1.0f);
}

const Vector4f& Renderer::getViewport() const {
	return viewport;
}

void Renderer::setVertexShaderCallback(VertexShaderCallback callback) {
	vertexShaderCallback = callback;
}

void Renderer::setPixelShaderCallback(PixelShaderCallback callback) {
	pixelShaderCallback = callback;
}

void Renderer::setShader(Shader* value) {
	shader = value;
}

Shader* Renderer::getShader() const {
	return shader;
}

void Renderer::setShaderUniform(const void* uniform) {
	shaderUniform = uniform;
}

const void* Renderer::getShaderUniform() const {
	return shaderUniform;
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
			drawTriangle(vertices[index + 0], vertices[index + 1], vertices[index + 2]);
		}
		break;
	case EPT_TRIANGLE_STRIP :
		for (unsigned int index = 2, k = 1; index < vertexCount; ++index, k = !k) {
			drawTriangle(vertices[index - 0], vertices[index - (k ? 2 : 1)], vertices[index - (k ? 1 : 2)]);
		}
		break;
	}
}

void Renderer::render(const PrimitiveType primitiveType, const Vertex* vertices, const unsigned int vertexCount,
			const unsigned int* indices, const unsigned int indexCount) {
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

void Renderer::draw2DLine(const Vector2f& begin, const Vector2f& end, const Vector4f& color) {
	const Vertex vertices[] = {
		Vertex(Vector3f(begin.x, begin.y, 0.0f), Vector2f(0.0f, 0.0f), color),
		Vertex(Vector3f(end.x,   end.y,   0.0f), Vector2f(1.0f, 1.0f), color)
	};

	setFlag(ERF_DEPTH_TEST, false);
	setFlag(ERF_DEPTH_MASK, false);
	setFlag(ERF_ALPHA_BLEND, 
		(color.w < 1.0f) || 
		((activeTexture[0] == NULL) ? 
			false : activeTexture[0]->hasAlpha()));

	setVertexShaderCallback(DefaultVertexShader);
	setPixelShaderCallback(DefaultPixelShader);
	setShader(NULL);

	setShaderUniform(&orthogonalProjection);

	render(EPT_LINES, vertices, 2);
}

void Renderer::draw2DRectangle(const Vector4f& rectangle, const Vector4f& color) {
	const Vertex vertices[] = {
		Vertex(Vector3f(rectangle.x, rectangle.y, 0.0f), Vector2f(0.0f, 0.0f), color),
		Vertex(Vector3f(rectangle.z, rectangle.y, 0.0f), Vector2f(1.0f, 0.0f), color),
		Vertex(Vector3f(rectangle.x, rectangle.w, 0.0f), Vector2f(0.0f, 1.0f), color),
		Vertex(Vector3f(rectangle.z, rectangle.w, 0.0f), Vector2f(1.0f, 1.0f), color)
	};

	setFlag(ERF_DEPTH_TEST, false);
	setFlag(ERF_DEPTH_MASK, false);
	setFlag(ERF_ALPHA_BLEND, 
		(color.w < 1.0f) || 
		((activeTexture[0] == NULL) ? 
			false : activeTexture[0]->hasAlpha()));

	setVertexShaderCallback(DefaultVertexShader);
	setPixelShaderCallback(DefaultPixelShader);
	setShader(NULL);

	setShaderUniform(&orthogonalProjection);

	render(EPT_TRIANGLE_STRIP, vertices, 4);
}

void Renderer::draw2DImage(Image* image, const Vector4f& rectangle, const Vector4f color) {
	const Vertex vertices[] = {
		Vertex(Vector3f(rectangle.x, rectangle.y, 0.0f), Vector2f(0.0f, 0.0f), color),
		Vertex(Vector3f(rectangle.z, rectangle.y, 0.0f), Vector2f(1.0f, 0.0f), color),
		Vertex(Vector3f(rectangle.x, rectangle.w, 0.0f), Vector2f(0.0f, 1.0f), color),
		Vertex(Vector3f(rectangle.z, rectangle.w, 0.0f), Vector2f(1.0f, 1.0f), color)
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
	setShader(NULL);

	setShaderUniform(&orthogonalProjection);

	render(EPT_TRIANGLE_STRIP, vertices, 4);
//	const unsigned int indices[] = {1, 0, 2, 2, 3, 1};
//	render(EPT_TRIANGLES, vertices, 4, indices, 6);
}

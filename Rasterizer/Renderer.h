#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Vertex.h"
#include "Matrix4.h"
#include "Image.h"
#include "RenderTarget.h"

#include "Shader.h"

typedef void (*VertexShaderCallback)(VertexShaderData&);
typedef void (*PixelShaderCallback)(PixelShaderData&);

class Renderer {
public:
	enum RenderFlag {
		ERF_DEPTH_TEST,
		ERF_DEPTH_MASK,
		ERF_ALPHA_BLEND,
		GFX_PERSPECTIVE_CORRECT,
		GFX_WIREFRAME,

		ERF_COUNT
	};

private:
	vec4 viewport;
	mat4 viewportTransformation;
	mat4 orthogonalProjection;
	bool renderFlags[ERF_COUNT];
	const Image* activeTexture[MaxTextureCount];
	RenderTarget* renderTarget;
	Image* depthBufferPtr;
	Image* colorBufferPtr;

	Shader* activeShader;

	void drawLine(const vec3&, const vec4&, const vec3&, const vec4&);
	void drawTriangle(const Vertex&, const Vertex&, const Vertex&);

public:
	enum PrimitiveType {
		EPT_LINES,
		EPT_LINE_STRIP,
		EPT_TRIANGLES,
		EPT_TRIANGLE_STRIP
	};
	
	Renderer();
	
	~Renderer();

	void setRenderTarget(RenderTarget* newRenderTarget);

	RenderTarget* getRenderTarget() const;

	void setViewport(const vec4& value);

	const vec4& getViewport() const;

	void setFlag(const RenderFlag renderFlag, bool value);

	bool getFlag(const RenderFlag renderFlag) const;
	
	bool toggleFlag(const RenderFlag renderFlag);

	void setActiveTexture(unsigned int index, const Image* image);

	const Image* getActiveTexture(unsigned int index) const;
	
	void setShader(Shader* shader);
	
	Shader* getShader() const;

	void render(const PrimitiveType primitiveType, const Vertex* vertices, unsigned int vertexCount);

	void render(const PrimitiveType primitiveType, const Vertex* vertices, const unsigned int vertexCount, const unsigned int* indices, const unsigned int indexCount);
/*
	void draw2DLine(const vec2& begin, const vec2& end, const vec4& color = vec4(1.0f, 1.0f, 1.0f, 1.0f));

	void draw2DRectangle(const vec4& rectangle, const vec4& color = vec4(1.0f, 1.0f, 1.0f, 1.0f));

	void draw2DImage(Image* image, const vec4& rectangle, const vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	*/
};
	
#endif // __RENDERER_H__
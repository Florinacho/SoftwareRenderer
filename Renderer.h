#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "FrameBuffer.h"
#include "Vertex.h"
#include "Image.h"
#include "Matrix4.h"
#include "RenderTarget.h"

static const unsigned int MaxTextureCount = 4;
static const unsigned int MaxRenderTargetCount = 4;

struct VertexShaderData {
	Vector4f position;
	Vector2f uv;
	Vector4f color;
	int index;
	const void* uniform;
};

struct PixelShaderData {
	const Image* texture[MaxTextureCount];
	Vector2f uv;
	Vector4f color;
	const void* uniform;
};

typedef void (*VertexShaderCallback)(VertexShaderData&);
typedef void (*PixelShaderCallback)(PixelShaderData&);

void DefaultVertexShader(VertexShaderData&);
void DefaultPixelShader(PixelShaderData&);

class Renderer {
public:
	enum RenderFlag {
		ERF_DEPTH_TEST,
		ERF_DEPTH_MASK,
		ERF_ALPHA_BLEND,

		ERF_COUNT
	};

private:
	Vector4f viewport;
	Matrix4f viewportTransformation;
	Matrix4f orthogonalProjection;
	bool renderFlags[ERF_COUNT];
	const Image* activeTexture[MaxTextureCount];
	const void* shaderUniform;
	RenderTarget* renderTarget;
	Image* depthBufferPtr;
	Image* colorBufferPtr;

	VertexShaderCallback vertexShaderCallback;
	PixelShaderCallback pixelShaderCallback;

	void drawLine(
		const Vector3f&, const Vector4f&, 
		const Vector3f&, const Vector4f&);

	void drawTriangle(
		const Vector3f&, const Vector2f&, const Vector4f&, 
		const Vector3f&, const Vector2f&, const Vector4f&, 
		const Vector3f&, const Vector2f&, const Vector4f&);

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

	void setViewport(const Vector4f& value);

	const Vector4f& getViewport() const;

	void setFlag(const RenderFlag renderFlag, bool value);

	bool getFlag(const RenderFlag renderFlag) const;

	void setVertexShaderCallback(VertexShaderCallback callback);

	void setPixelShaderCallback(PixelShaderCallback callback);

	void setShaderUniform(const void* data);

	const void* getShaderUniform() const;

	void setActiveTexture(unsigned int index, const Image* image);

	const Image* getActiveTexture(unsigned int index) const;
	
	void clear(bool color = true, bool depth = true);

	void swap();

	void drawPrimitive(const PrimitiveType primitiveType, 
			const Vertex* vertices, unsigned int vertexCount);

	void draw2DLine(const Vector2f& begin, const Vector2f& end, 
		const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	void draw2DRectangle(const Vector4f& rectangle, 
		const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

	void draw2DImage(Image* image, const Vector4f& rectangle, 
		const Vector4f color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
};
	
#endif //__RENDERER_H__

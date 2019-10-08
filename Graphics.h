#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "FrameBuffer.h"
#include "Vertex.h"
#include "Image.h"
#include "Matrix4.h"

static const unsigned int MaxTextureCount = 4;
static const unsigned int MaxRenderTargetCount = 4;

struct VertexShaderData {
	Vector4f position;
	Vector2f uv;
	Vector4f color;
	int index;
};

struct PixelShaderData {
	const Image* texture[MaxTextureCount];
	Vector2f uv;
	Vector4f color;
};

typedef void (*VertexShaderCallback)(VertexShaderData&, void*);
typedef void (*PixelShaderCallback)(PixelShaderData&, void*);

void DefaultVertexShader(VertexShaderData&, void*);
void DefaultPixelShader(PixelShaderData&, void*);

class Graphics {
public:
	enum RenderFlag {
		ERT_DEPTH_TEST,
		ERT_DEPTH_MASK,
		ERT_ALPHA_BLEND,

		ERT_COUNT
	};
private:
	FrameBuffer* frameBuffer;
	Vector4f viewport;
	Matrix4f viewportTransformation;
	Matrix4f orthogonalProjection;
	bool renderFlags[ERT_COUNT];

	const Image* activeTexture[MaxTextureCount];

	VertexShaderCallback vertexShaderCallback;
	PixelShaderCallback pixelShaderCallback;
	void* shaderUniform;

	Image depthBuffer;
	Image colorBuffer;

	void drawLine(const Vector3f& begin, const Vector4f& beginColor, const Vector3f& end, const Vector4f& endColor);
	void drawTriangle(
		const Vector3f& a, const Vector2f& auv, const Vector4f& aColor, 
		const Vector3f& b, const Vector2f& buv, const Vector4f& bColor, 
		const Vector3f& c, const Vector2f& cuv, const Vector4f& cColor);

public:
	enum {
		EPT_LINES,
		EPT_LINE_STRIP,
		EPT_TRIANGLES,
		EPT_TRIANGLE_STRIP
	};
	
	Graphics();
	
	~Graphics();

	bool initialize(FrameBuffer* frameBuffer, int depthBPP = 8);

	void uninitialize();

	void setViewport(const Vector4f& value);

	const Vector4f& getViewport() const;

	void setFlag(const RenderFlag renderFlag, bool value);

	bool getFlag(const RenderFlag renderFlag) const;

	void setVertexShaderCallback(VertexShaderCallback callback);

	void setPixelShaderCallback(PixelShaderCallback callback);

	void setShaderUniform(void* data);

	void* getShaderUniform() const;

	void setActiveTexture(unsigned int index, const Image* image = NULL);

	const Image* getActiveTexture(unsigned int index) const;

	Image* getColorBuffer();

	Image* getDepthBuffer();
	
	void clear();

	void swap();

	void drawPrimitive(int primitiveType, const Vertex* vertices, unsigned int vertexCount);

	void draw2DLine(const Vector2f& begin, const Vector2f& end, const Vector4f& color);

	void draw2DRectangle(const Vector4f& rectangle, const Vector4f& color);

	void draw2DImage(Image* image, const Vector4f& rectangle, const Vector4f color);
};
	
#endif //__GRAPHICS_H__

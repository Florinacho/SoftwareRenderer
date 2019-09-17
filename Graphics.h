#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "FrameBuffer.h"
#include "Vertex.h"
#include "Image.h"

struct VertexShaderData {
	Vector4f position;
	Vector2f uv;
	Vector4f color;
	int index;
};

struct PixelShaderData {
	const Image* texture;
	Vector3f position;
	Vector2f uv;
	Vector4f color;
	PixelShaderData() {
		color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
};

typedef void (*VertexShaderCallback)(VertexShaderData&, void*);
typedef void (*PixelShaderCallback)(PixelShaderData&, void*);
	
class Graphics {
	static const unsigned int MAX_BUFFER_COUNT = 1;		// Unused

	FrameBuffer* frameBuffer;
	const Image* activeTexture;

	Image depthBuffer;
	Image colorBuffer;

	bool depthTest;
	bool depthMask;
	bool alphaBlend;
	
	void drawLine(const Vector3f& begin, const Vector4f& beginColor, const Vector3f& end, const Vector4f& endColor);
	void drawTriangle(
		const Vector3f& a, const Vector2f& auv, const Vector4f& aColor, 
		const Vector3f& b, const Vector2f& buv, const Vector4f& bColor, 
		const Vector3f& c, const Vector2f& cuv, const Vector4f& cColor);

	VertexShaderCallback vertexShaderCallback;
	PixelShaderCallback pixelShaderCallback;
	void* shaderUniform;
	Vector4f viewport;
	
public:
	enum {
		EPT_LINES,
		EPT_LINE_STRIP,
		EPT_TRIANGLES,
		EPT_TRIANGLE_STRIP
	};
	
	Graphics();
	
	~Graphics();

	// Supported color pixel depths: 16, 24, 32 (This also have to be 
	// supported by the hardware.
	bool initialize(FrameBuffer* frameBuffer, int depthBPP = 8);

	void uninitialize();

	void setViewport(const Vector4f& value);

	const Vector4f& getViewport() const;

	// Take the depth buffer into account
	void setDepthTest(bool value);

	bool getDepthTest() const;

	// Write depth in depth buffer
	void setDepthMask(bool value);

	bool getDepthMask() const;

	// Blend new pixel with old pixel based on the Vector4::w component
	void setAlphaBlend(bool value);

	bool getAlphaBlend() const;

	void setVertexShaderCallback(VertexShaderCallback callback);

	void setPixelShaderCallback(PixelShaderCallback callback);

	void setShaderUniform(void* data);

	void* getShaderUniform() const;

	void setActiveTexture(const Image* image = NULL);

	const Image* getActiveTexture() const;

	Image* getColorBuffer();

	Image* getDepthBuffer();
	
	void clear();

	void swap();

	void drawPrimitive(int primitiveType, const Vertex* vertices, unsigned int vertexCount);

	void drawLine(const Vector2f& begin, const Vector2f& end, const Vector4f& color);

	void drawRectangle(const Vector4f& rectangle, const Vector4f& color);
};
	
#endif //__GRAPHICS_H__

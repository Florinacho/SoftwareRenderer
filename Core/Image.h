#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Vector.h"

class Image {
protected:
	union {
		unsigned char* data;
		float* fdata;
	};
	Vector2u size;
	int pixelFormat;


	// Make the copy operation illegal
	Image(const Image& other){}
	Image& operator = (const Image& other) {return *this;}
	
public:
	/*************************************************************************/
	/* Updates the coordinates based on the wrapping member.                 */
	/* Return true if the coordinates should be discarded(outside bounds).   */
	/*************************************************************************/
	bool updateCoordinates(int& x, int& y) const;
	
	Vector2ub wrapping;
	
	enum PixelFormat {
		EPF_L8,		// 8,  Luminance
		EPF_R5G6B5,	// 16, 
		EPF_R8G8B8,	// 24, Red + Green + Blue
		EPF_R8G8B8A8,	// 32, Red + Green + Blue + Alpha

		EPF_DEPTH,
	};
	
	enum WrappingType {
		EWT_REPEAT,	// Default
		EWT_MIRROR,
		EWT_CLAMP,
		EWT_DISCARD
	};

	Image();

	virtual ~Image();
	
	void create(const Vector2u& size, int pixelFormat);
	
	const unsigned char* getData() const;

	virtual unsigned int getDataLength() const;

	Vector2u getSize() const;
	
	virtual unsigned int getLineStride() const;

	Image::PixelFormat getPixelFormat() const;

	unsigned int getPixelSize() const;

	bool hasAlpha() const;

	bool load(const char* filename);

	void flipVertical();

	void flipHorizontal();
	
	void clear();

	void destroy();
	
	virtual void setPixel(const int x, const int y, const Vector4f& value);
	
	virtual Vector4f getPixel(int x, int y) const;
	
	virtual Vector4f getPixel(const Vector2u& uv) const;

	Vector4f sample2D(const Vector2f& uv) const;
	
	virtual void setRawPixel(const int x, const int y, const Vector4ub& value);
	
	virtual Vector4ub getRawPixel(int x, int y) const;
	
	Vector4ub sampleRaw2D(const Vector2f& uv) const;

	void generateTestPattern(const Vector2u& size, int pixelFormat);
	
	void drawLine(const Vector2f& begin, const Vector4f& beginColor, const Vector2f& end, const Vector4f& endColor);
	
	void drawLine(const Vector2f& begin, const Vector2f& end, const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	
	void drawRectangle(const Vector4f& rectangle, const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	
	void drawFilledRectangle(const Vector4f& rectangle, const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	
	void drawCircle(const Vector2f& center, float radius, const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	
	void drawFilledCircle(const Vector2f& center, float radius, const Vector4f& color = Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
	
	void blit(const Image* image, const uvec2& position);
};

#endif // __IMAGE_H__

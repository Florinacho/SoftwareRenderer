#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Vector.h"

class Image {
	unsigned char* data;
	Vector2u size;
	int pixelFormat;

public:
	enum PixelFormat {
		EPF_L8,		// Luminance
		EPF_R5G6B5,
		EPF_R8G8B8,	// Red + Green + Blue
		EPF_R8G8B8A8,	// Red + Green + Blue + Alpha
	};

	Image();

	~Image();
	
	void create(const Vector2u& size, int pixelFormat);
	
	const unsigned char* getData() const;

	unsigned int getDataSize() const;

	Vector2u getSize() const;

	Image::PixelFormat getPixelFormat() const;

	unsigned int getPixelSize() const;

	bool hasAlpha() const;

	unsigned int getPixelCount() const;

	bool load(const char* filename);

	void set(unsigned char* data, const Vector2u& size, int pixelFormat);

	void fill(const Vector2u size, PixelFormat pixelFomar, const Vector4f& color);

	void flipVertical();

	void flipHorizontal();
	
	void clearColor();
	
	void clear();

	void removeData();
	
	void setPixel(const unsigned int x, const unsigned int y, const Vector4f& value);
	
	Vector4f getPixel(unsigned int x, unsigned int y) const;

	Vector4f getPixelByUV(const Vector2f& uv) const;

	Vector4f operator()(const Vector2f& uv) const;

	void blur(int value);

	void generateNoise();
	
	void removeAlpha();
};

#endif // __IMAGE_H__

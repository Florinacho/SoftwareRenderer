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
	enum PixelFormat {
		EPF_L8,		// 8,  Luminance
		EPF_R5G6B5,	// 16, 
		EPF_R8G8B8,	// 24, Red + Green + Blue
		EPF_R8G8B8A8,	// 32, Red + Green + Blue + Alpha

		EPF_DEPTH,
	};

	Image();

	virtual ~Image();
	
	void create(const Vector2u& size, int pixelFormat);
	
	const unsigned char* getData() const;

	virtual unsigned int getDataLength() const;

	Vector2u getSize() const;

	Image::PixelFormat getPixelFormat() const;

	unsigned int getPixelSize() const;

	bool hasAlpha() const;

	bool load(const char* filename);

	void flipVertical();

	void flipHorizontal();
	
	void clearColor();
	
	void clear();

	void removeData();
	
	virtual void setPixel(const unsigned int x, const unsigned int y, const Vector4f& value);
	
	virtual Vector4f getPixel(unsigned int x, unsigned int y) const;

	Vector4f getPixelByUV(const Vector2f& uv) const;
};

#endif // __IMAGE_H__

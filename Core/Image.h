#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "Vector.h"

class Image {
protected:
	uint8_t* colorMapData;
	uint32_t colorMapLength;
	union {
		uint8_t* data;
		float* fdata;
	};
	uint8_t pixelFormat;
	Vector2u size;

	// Make the copy operation illegal
	Image(const Image& other){}
	Image& operator = (const Image& other) {return *this;}
	
	bool readColorMapData(uint8_t* colorMap, uint32_t depth, uint32_t length, FILE* file);
	bool readUncompressedPixelData(uint8_t* pixels, uint32_t depth, uint32_t width, uint32_t height, FILE* file);
	bool readCompressedPixelData(uint8_t* pixels, uint32_t depth, uint32_t width, uint32_t height, FILE* file);
	
public:
	/*************************************************************************/
	/* Updates the coordinates based on the wrapping member.                 */
	/* Return true if the coordinates should be discarded(outside bounds).   */
	/*************************************************************************/
	bool updateCoordinates(int& x, int& y) const;
	
	Vector2ub wrapping;
	
	enum PIXEL_FORMAT {
		EPF_NONE,
		
		EPF_INDEX_RGB,
		EPF_INDEX_RGBA,
		// EPF_A1R5G5B5,
		EPF_R8G8B8,
		EPF_R8G8B8A8,
		EPF_GRAYSCALE,
		EPF_GRAYSCALE_ALPHA,
		
		EPF_DEPTH,
	};
	enum COLORMAP_TYPE {
		ECT_NONE,
		ECT_R8G8B8,
		ECT_R8G8B8A8
	};	
	enum WRAPPING_TYPE {
		EWT_REPEAT,	  // Default
		EWT_MIRROR,
		EWT_CLAMP,
		EWT_DISCARD
	};

	Image();

	virtual ~Image();
	
	void create(const Vector2u& size, PIXEL_FORMAT pixelFormat);
	
	const uint8_t* getData() const;

	virtual uint32_t getDataLength() const;

	Vector2u getSize() const;
	
	virtual uint32_t getLineStride() const;

	Image::PIXEL_FORMAT getPixelFormat() const;

	uint32_t getPixelSize() const;

	bool hasAlpha() const;

	bool load(const char* filename, bool convertToTruecolor = true);

	void flipVertical();

	void flipHorizontal();
	
	void clear();

	void destroy();

	virtual void setPixel(int x, int y, const ubvec4& color);
	
	virtual const ubvec4 getPixel(int x, int y) const;
	
	virtual void setPixelf(int x, int y, const vec4& color);
	
	virtual vec4 getPixelf(int x, int y) const;
	
	virtual vec4 sample2D(const vec2& uv) const;
	
	virtual void drawLine(const ivec2& begin, const ubvec4& beginColor, const ivec2& end, const ubvec4& endColor);
	
	virtual void drawLine(const ivec2& begin, const ivec2& end, const ubvec4& color);
	
	virtual void drawRectangle(const ivec4& bounds, const ubvec4& color);
	
	virtual void drawFilledRectangle(const ivec4& bounds, const ubvec4& color);
	
	virtual void drawCircle(const ivec2& center, int radius, const ubvec4& color);
	
	virtual void drawFilledCircle(const ivec2& center, int radius, const ubvec4& color);
	
	void blit(const Image* image, const uvec2& position);
};

#endif // __IMAGE_H__
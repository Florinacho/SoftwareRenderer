#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#include <linux/fb.h>
#include "Image.h"

class Image;

class FrameBuffer : public Image {
	int fileDescriptor;
	fb_var_screeninfo varInfo;
	fb_fix_screeninfo fixInfo;

public:
	enum ErrorCode {
		ERR_NO_ERROR = 0,
		ERR_CANNOT_OPEN_FILE,
		ERR_CANNOT_READ_FIX_INFO,
		ERR_CANNOT_READ_VAR_INFO,
		ERR_CANNOT_WRITE_VAR_INFO,
		ERR_CANNOT_MAP_MEMORY,
		ERR_COUNT
	};

	static const char* const ErrorText[];

	FrameBuffer();
	
	~FrameBuffer();
	
	int initialize(const char* filename, const Vector2u& size, int pixelFormat);
	
	void uninitialize();
	
	void draw(const Image* image);

	void setPixel(unsigned int x, unsigned int y, const Vector4f& value);
	
	Vector4f getPixel(unsigned int x, unsigned int y);
};

#endif // __RBP_FRAME_BUFFER_H__

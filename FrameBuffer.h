#ifndef __RBP_FRAME_BUFFER_H__
#define __RBP_FRAME_BUFFER_H__

#include <linux/fb.h>
#include "Vector.h"

class Image;

class FrameBuffer {
	Vector2u size;
	unsigned int bpp;

	int fileDescriptor;
	fb_var_screeninfo varInfo;
	fb_fix_screeninfo fixInfo;

	unsigned char* buffer;	
	unsigned int screenSize;

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
	
	int initialize(int width, int height, int bitsPerPixel);
	
	void uninitialize();
	
	Vector2u getSize() const;

	unsigned int getBitsPerPixel() const;

	unsigned int getBytesPerPixel() const;

	void draw(const Image* image);
};

#endif // __RBP_FRAME_BUFFER_H__

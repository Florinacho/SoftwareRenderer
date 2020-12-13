#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

#ifdef __linux__

#include <linux/fb.h>
#include "Image.h"

class Image;

class FrameBuffer : public Image {
	int fileDescriptor;
	int tty0;
	fb_var_screeninfo varInfo;
	fb_fix_screeninfo fixInfo;
	
public:
	enum ErrorCode {
		ERR_NO_ERROR = 0,
		ERR_CANNOT_OPEN_TTY0,
		ERR_CANNOT_SET_KDMODE,
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

	unsigned int getLineStride() const;

	unsigned int getDataLength() const;
	
	void draw(const Image* image);
};

#endif // __linux__

#endif // __RBP_FRAME_BUFFER_H__

#ifndef __RBP_FRAME_BUFFER_H__
#define __RBP_FRAME_BUFFER_H__

#include <linux/fb.h>
#include "Vector.h"

class FrameBuffer {
	Vector2u size;
	unsigned int bpp;

	int fileDescriptor;
	fb_var_screeninfo varInfo;
	fb_fix_screeninfo fixInfo;
	
public:
	unsigned char* buffer;
	
	FrameBuffer();
	
	~FrameBuffer();
	
	bool initialize(int width, int height, int bpp);
	
	void uninitialize();
	
	Vector2u getSize() const;

	unsigned int getBPP() const;
	
	unsigned int getBufferLength() const;
};

#endif // __RBP_FRAME_BUFFER_H__

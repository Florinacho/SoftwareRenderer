#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/ioctl.h> // for ioctl

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	bpp = 0;
	
	buffer = NULL;
	
	fileDescriptor = -1;
}
	
FrameBuffer::~FrameBuffer() {
	uninitialize();
}
	
bool FrameBuffer::initialize(int width, int height, int inbpp) {
//	fb_var_screeninfo varInfo;
//	fb_fix_screeninfo fixInfo;

	uninitialize();
	
	fileDescriptor = open("/dev/fb0", O_RDWR);
	if (fileDescriptor == -1) {
		printf("Error! Cannot open /dev/fb0\n");
		return false;
	}
    
	if (ioctl(fileDescriptor, FBIOGET_VSCREENINFO, &varInfo)) {
		printf("Error! Cannot get the framebuffer variable infomation\n");
		return false;
	}
	if (ioctl(fileDescriptor, FBIOGET_FSCREENINFO, &fixInfo)) {
		printf("Error! Cannot get the framebuffer fixed information.\n");
		return false;
	}
/**/
	varInfo.xres = width;
	varInfo.yres = height;
	varInfo.xres_virtual = varInfo.xres;
	varInfo.yres_virtual = varInfo.yres;

	varInfo.bits_per_pixel = inbpp;
	
	if (ioctl(fileDescriptor, FBIOPUT_VSCREENINFO, &varInfo) == -1) {
		printf("Warning! Cannot change the pixel format.\n");
	}

	size.x = width;
	size.y = height;
	bpp = inbpp;
/**/
	printf("FrameBuffer: %dx%d (%dx%d virtual) %d bpp (%d smen_len)\n", 
		varInfo.xres, varInfo.yres, 
		varInfo.xres_virtual, varInfo.yres_virtual,
		varInfo.bits_per_pixel,
		fixInfo.smem_len);

	buffer = (unsigned char*)mmap(0, fixInfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

	printf("FB: %d | %d\n", getBufferLength(), fixInfo.smem_len);

	return true;
}

void FrameBuffer::uninitialize() {
	if (fileDescriptor == -1) {
		return;
	}
	
	munmap(buffer, fixInfo.smem_len);
	close(fileDescriptor);
	
	fileDescriptor = -1;
	size.x = 0;
	size.y = 0;
	bpp = 0;
}

Vector2u FrameBuffer::getSize() const {
	return size;
}

unsigned int FrameBuffer::getBPP() const {
	return bpp;
}
	
unsigned int FrameBuffer::getBufferLength() const {
	return size.x * size.y * bpp / 8;
}

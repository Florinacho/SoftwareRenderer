#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <sys/ioctl.h> // for ioctl

#include "FrameBuffer.h"
#include "Image.h"

const char* const FrameBuffer::ErrorText[] = {
	"No error",
	"Cannot open file /dev/fb",
	"Cannot read fix screen information",
	"Cannot read variable screen information",
	"Cannot write variable screen information",
	"Cannot map memory"
};

FrameBuffer::FrameBuffer() 
	: bpp(0), buffer(NULL), fileDescriptor(-1) {
}
	
FrameBuffer::~FrameBuffer() {
	uninitialize();
}
	
int FrameBuffer::initialize(int width, int height, int bitsPerPixel) {
	uninitialize();
	
	fileDescriptor = open("/dev/fb0", O_RDWR, 777);
	if (fileDescriptor == -1) {
		return ERR_CANNOT_OPEN_FILE;
	}
    
	if (ioctl(fileDescriptor, FBIOGET_FSCREENINFO, &fixInfo) != 0) {
		return ERR_CANNOT_READ_FIX_INFO;
	}

	if (ioctl(fileDescriptor, FBIOGET_VSCREENINFO, &varInfo) != 0) {
		return ERR_CANNOT_READ_VAR_INFO;
	}

	varInfo.xres = width;
	varInfo.yres = height;
	varInfo.xres_virtual = varInfo.xres;
	varInfo.yres_virtual = varInfo.yres;

	varInfo.bits_per_pixel = bitsPerPixel;

	printf("fixInfo.mmio_len = %d\n", fixInfo.mmio_len);
	if (fixInfo.mmio_len != 0) {
		printf("Internal error! mmio_len = %d\n", fixInfo.mmio_len);
	}
	
	if (ioctl(fileDescriptor, FBIOPUT_VSCREENINFO, &varInfo) != 0) {
		return ERR_CANNOT_WRITE_VAR_INFO;
	}

	size.x = width;
	size.y = height;
	bpp = bitsPerPixel;

//	screenSize = varInfo.xres * varInfo.yres * (varInfo.bits_per_pixel >> 3);
	screenSize = fixInfo.smem_len;

	buffer = (unsigned char*)mmap(NULL, fixInfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (buffer == (void*)-1) {
		return ERR_CANNOT_MAP_MEMORY;
	}

	return ERR_NO_ERROR;
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

unsigned int FrameBuffer::getBitsPerPixel() const {
	return bpp;
}

unsigned int FrameBuffer::getBytesPerPixel() const {
	return getBitsPerPixel() / 8;
}

void FrameBuffer::draw(const Image* image) {
	if (image == NULL) {
		return;
	}

	if (size.x != image->getSize().x) {
		return;
	}
	if (size.y != image->getSize().y) {
		return;
	}
	if ((bpp / 8) != image->getPixelSize()) {
		return;
	}
	if (bpp / 8 != 4) {
		return;
	}

	const unsigned int visualLineLength = size.x * (bpp / 8);

	unsigned char lineData[visualLineLength];
	unsigned char* src = lineData;

	for (unsigned int index = 0; index < size.y; ++index) {
#if 1
		memcpy(
			buffer + index * fixInfo.line_length, 
			image->getData() + index * visualLineLength, 
			visualLineLength);
#elif 1
		const float proc = (float)index / (float)size.y;
		unsigned char data = (unsigned char)(255.0f * proc);
		memset(lineData, data, visualLineLength);
		memcpy(
			buffer + index * fixInfo.line_length, 
			src, 
			visualLineLength);
#else
		memset(buffer + index * fixInfo.line_length, index % 600, visualLineLength);
#endif
	}
}

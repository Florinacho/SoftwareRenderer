#ifdef __linux__

#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <errno.h>

#include "FrameBuffer.h"

const char* const FrameBuffer::ErrorText[] = {
	"No error.",
	"Cannot open \"/dev/tty0\".",
	"Cannot set KDMODE."
	"Cannot open file \"/dev/fb\".",
	"Cannot read fix screen information.",
	"Cannot read variable screen information.",
	"Cannot write variable screen information.",
	"Cannot map memory."
};

unsigned int FrameBuffer::getLineStride() const {
	return fixInfo.line_length;
}

FrameBuffer::FrameBuffer() 
	: fileDescriptor(-1), tty0(-1) {
}
	
FrameBuffer::~FrameBuffer() {
	uninitialize();
}

//https://unix.stackexchange.com/questions/173712/best-practice-for-hiding-virtual-console-while-rendering-video-to-framebuffer	
int FrameBuffer::initialize(const char* filename, const Vector2u& nsize, int npixelFormat) {
	uninitialize();
	
	tty0 = open("/dev/tty0", O_RDWR, 777);
	if (tty0 == -1) {
		printf("Error! Cannot open /dev/tty0.\n");
		return ERR_CANNOT_OPEN_TTY0;
	}
	if (ioctl(tty0, KDSETMODE, KD_GRAPHICS) != 0) {
		printf("Error! Cannot set KD_GRAPHICS on tty0.\n");
		return ERR_CANNOT_SET_KDMODE;
	}
	
	fileDescriptor = open(((filename == NULL) ? "/dev/fb0" : filename), O_RDWR);
	if (fileDescriptor == -1) {
		printf("Cannot open %s\n", filename);
		return ERR_CANNOT_OPEN_FILE;
	}
    
	if (ioctl(fileDescriptor, FBIOGET_FSCREENINFO, &fixInfo) != 0) {
		return ERR_CANNOT_READ_FIX_INFO;
	}

	if (ioctl(fileDescriptor, FBIOGET_VSCREENINFO, &varInfo) != 0) {
		return ERR_CANNOT_READ_VAR_INFO;
	}

	varInfo.xres = nsize.x;
	varInfo.yres = nsize.y;
	varInfo.xres_virtual = varInfo.xres;
	varInfo.yres_virtual = varInfo.yres;

	switch (npixelFormat) {
	// case EPF_R5G6B5 :
	// 	varInfo.bits_per_pixel = 16;
	// 	break;
	case EPF_R8G8B8 :
		varInfo.bits_per_pixel = 24;
		break;
	case EPF_R8G8B8A8 :
		varInfo.bits_per_pixel = 32;
		break;
	default :
		printf("%s::%u: Internal error!\n", __FILE__, __LINE__);
		return 1;
	}
	
	if (ioctl(fileDescriptor, FBIOPUT_VSCREENINFO, &varInfo) != 0) {
		return ERR_CANNOT_WRITE_VAR_INFO;
	}

	size = nsize;
	pixelFormat = npixelFormat;

	data = (unsigned char*)mmap(NULL, fixInfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (data == (void*)-1) {
		return ERR_CANNOT_MAP_MEMORY;
	}

	return ERR_NO_ERROR;
}

void FrameBuffer::uninitialize() {
	if (fileDescriptor != -1) {
		munmap(data, fixInfo.smem_len);
		data = NULL;
		close(fileDescriptor);

		fileDescriptor = -1;
		size.x = 0;
		size.y = 0;
	}
	
	if (tty0 != -1) {
		if (ioctl(tty0, KDSETMODE, KD_TEXT) != 0) {
			printf("Error! Cannot set KD_TEXT on tty0.\n");
		}
		close(tty0);
		tty0 = -1;
	}
}

unsigned int FrameBuffer::getDataLength() const {
	return size.y * fixInfo.line_length;
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

	if (pixelFormat != image->getPixelFormat()) {
		return;
	}

	const unsigned int visualLineLength = size.x * getPixelSize();

	for (unsigned int index = 0; index < size.y; ++index) {
		memcpy(data + index * fixInfo.line_length, 
			image->getData() + index * visualLineLength, 
			visualLineLength);
	}
}

void FrameBuffer::blit(const Image* image) {
	draw(image);
}

bool FrameBuffer::getEvent(Event* event) {
	return input.getEvent(*event);
}

#endif // __linux__

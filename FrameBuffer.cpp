#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

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
	: fileDescriptor(-1) {
}
	
FrameBuffer::~FrameBuffer() {
	uninitialize();
}
	
int FrameBuffer::initialize(const char* filename, const Vector2u& nsize, int npixelFormat) {
	uninitialize();
	
	fileDescriptor = open(((filename == NULL) ? "/dev/fb0" : filename), O_RDWR, 777);
	if (fileDescriptor == -1) {
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
	case EPF_R8G8B8 :
		varInfo.bits_per_pixel = 24;
		break;
	case EPF_R8G8B8A8 :
		varInfo.bits_per_pixel = 32;
		break;
	default :
		return 1;
	}

	if (fixInfo.mmio_len != 0) {
		printf("%s::%d: Internal error! mmio_len = %d\n", __FILE__, __LINE__, fixInfo.mmio_len);
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
	if (fileDescriptor == -1) {
		return;
	}
	
	munmap(data, fixInfo.smem_len);
	data = NULL;
	close(fileDescriptor);
	
	fileDescriptor = -1;
	size.x = 0;
	size.y = 0;
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

void FrameBuffer::setPixel(unsigned int x, unsigned int y, const Vector4f& value) {
	if (x > size.x) {
		return;
	}
	if (y > size.y) {
		return;
	}

	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
	unsigned int offset;

	switch (pixelFormat) {
	case EPF_R8G8B8A8 :
		offset = (y * fixInfo.line_length + x * getPixelSize());

		red = (unsigned char)(255.0f * value.x);
		green = (unsigned char)(255.0f * value.y);
		blue = (unsigned char)(255.0f * value.z);
		alpha = (unsigned char)(255.0f * value.w);

		*(data + offset + 0) = blue;
		*(data + offset + 1) = green;
		*(data + offset + 2) = red;
		*(data + offset + 3) = alpha;		
		break;

	case EPF_R8G8B8 :
		offset = (y * fixInfo.line_length + x * getPixelSize());

		red = (unsigned char)(255.0f * value.x);
		green = (unsigned char)(255.0f * value.y);
		blue = (unsigned char)(255.0f * value.z);

		*(data + offset + 0) = blue;
		*(data + offset + 1) = green;
		*(data + offset + 2) = red;
		break;
	}
}

Vector4f FrameBuffer::getPixel(unsigned int x, unsigned int y) const {
	Vector4f ans;

	if (x > size.x) {
		return ans;
	}
	if (y > size.y) {
		return ans;
	}

	unsigned int offset;

	switch (pixelFormat) {
	case EPF_R8G8B8A8 :
		offset = (y * fixInfo.line_length + x * getPixelSize()); 

		ans.x = (float)(255.0f * *(data + offset + 2));
		ans.y = (float)(255.0f * *(data + offset + 1));
		ans.z = (float)(255.0f * *(data + offset + 0));
		ans.w = (float)(255.0f * *(data + offset + 3));

		break;

	case EPF_R8G8B8 :
		offset = (y * fixInfo.line_length + x * getPixelSize()); 

		ans.x = (float)(255.0f * *(data + offset + 2));
		ans.y = (float)(255.0f * *(data + offset + 1));
		ans.z = (float)(255.0f * *(data + offset + 0));

		break;
	}

	return ans;
}

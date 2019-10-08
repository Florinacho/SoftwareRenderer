#include <stdio.h>
#include <string.h>

#include "Image.h"
#include "TGA.h"

struct __attribute__((packed)) Pixel8 {
	unsigned int red : 3;
	unsigned int green : 3;
	unsigned int blue : 2;
};

struct __attribute__((packed)) Pixel16 {
	unsigned int red : 5;
	unsigned int green : 6;
	unsigned int blue : 5;
};

Image::Image() {
	data = NULL;
	size = Vector2u(0, 0);
	pixelFormat = Image::EPF_L8;
}

Image::~Image() {
	clear();
}

void Image::create(const Vector2u& insize, int inpixelFormat) {
	clear();
	
	size = insize;
	pixelFormat = inpixelFormat;
	
	const unsigned int pixelCount = size.x * size.y;
	const unsigned int pixelSize = getPixelSize();
	const unsigned int totalPixelSize = pixelCount * pixelSize;
	switch (inpixelFormat) {
	case Image::EPF_DEPTH :
		fdata = new float[pixelCount];
		memset(fdata, 0, pixelCount);
		break;
	default :
		data = new unsigned char[totalPixelSize];
		memset(data, 0, totalPixelSize);
		break;
	}
}

const unsigned char* Image::getData() const {
	return data;
}

unsigned int Image::getDataSize() const {
	return size.x * size.y * getPixelSize();
}

Vector2u Image::getSize() const {
	return size;
}

Image::PixelFormat Image::getPixelFormat() const {
	return (Image::PixelFormat)pixelFormat;
}

unsigned int Image::getPixelSize() const {
	switch (pixelFormat) {
	case Image::EPF_L8 :
		return 1;
	case Image::EPF_R5G6B5 :
		return 2;
	case Image::EPF_R8G8B8 :
		return 3;
	case Image::EPF_R8G8B8A8 :
		return 4;
	case Image::EPF_DEPTH :
		return sizeof(float);
	}
	return 0;
}

bool Image::hasAlpha() const {
	switch (pixelFormat) {
	case Image::EPF_L8 :
	case Image::EPF_R8G8B8 :
	case Image::EPF_R5G6B5 :
	case Image::EPF_DEPTH :
		return false;
	case Image::EPF_R8G8B8A8 :
		return true;
	}
	return false;
}

unsigned int Image::getPixelCount() const {
	return (size.x * size.y);
}

bool Image::load(const char* filename) {
	clear();

	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Image::load(%s) error! Cannot open file.\n", filename);
		return false;
	}

	tga::Header header;
	if (fread(&header, tga::HeaderSize, 1, file) != 1) {
		printf("Image::load(%s) error! Cannot read TGA header.\n", filename);
		fclose(file);
		return false;
	}

	const unsigned int pixelCount = header.width * header.height;
	const unsigned int pixelSize = header.depth / 8;
	const unsigned int totalPixelsSize = pixelCount * pixelSize;
	unsigned char* newdata = new unsigned char[totalPixelsSize];

	switch (header.compression) {
	case 0: //no image data is present
		printf("Image::load(%s) warning! No image data is present.\n", filename);
		break;
	case tga::COLOR_MAP : //uncompressed color-mapped image
		printf("Image::load(%s) error! TGA_COLOR_MAP not supported yet.\n", filename);
		break;
	case tga::TRUE_COLOR : //uncompressed true-color image
	case tga::GRAYSCALE : //uncompressed black-and-white (grayscale) image
		if (fread(newdata, sizeof(unsigned char), totalPixelsSize, file) != totalPixelsSize) {
			printf("%s: Error! Unable to read image data.\n", filename);
			fclose(file);
			delete [] newdata;
			return false;
		}
		break;
	case tga::RLE_COLOR_MAP : //run-length encoded color-mapped image
		printf("Image::load(%s) error! TGA_RLE_COLOR_MAP not supported yet.\n", filename);
		break;
	case tga::RLE_TRUE_COLOR : //run-length encoded true-color image
	case tga::RLE_GRAYSCALE : //run-length encoded black-and-white (grayscale) image
		{
		unsigned char* ptr = newdata;
		for (unsigned int y = 0; y < header.height; y++) {
			static int repeat = 0;
			static int direct = 0;
			static unsigned char sample[4];
			int head;

			for (int x = 0; x < header.width; x++) {
				if (repeat == 0 && direct == 0) {
					head = getc(file);
					if (head == EOF) {
						printf("fuck \n");
						fclose(file);
						return false;
					} else if (head >= 128) {
						repeat = head - 127;
						if (fread (sample, pixelSize, 1, file) < 1) {
							printf("Image::load(%s) error! Cannot read pixel data 1\n", filename);
							fclose(file);
							return false;
						}
					} else {
						direct = head + 1;
					}
				}

				if (repeat > 0) {
					for (unsigned int k = 0; k < pixelSize; ++k) {
						ptr[k] = sample[k];
					}
					repeat--;
				} else { /* direct > 0 */
					if (fread(ptr, pixelSize, 1, file) < 1) {
						printf("Image::load(%s) error! Cannot read pixel data 2\n", filename);
						fclose(file);
						return false;
					}
					direct--;
				}
				ptr += pixelSize;
			}
		}
		}
		break;
	default:
		printf("Image::load(%s) error! Unknown compression type %d.\n", filename, header.compression);
		fclose(file);
		return false;
	}

	fclose(file);

	switch (header.depth) {
	case 8 :
		pixelFormat = Image::EPF_L8;
		break;
	case 16 :
		pixelFormat = Image::EPF_R5G6B5;
		break;
	case 24 :
		pixelFormat = Image::EPF_R8G8B8;
		break;
	case 32 :
		pixelFormat = Image::EPF_R8G8B8A8;
		break;
	default :
		printf("Image::load(%s) error! Unsupported pixel depth %d.\n", filename, header.depth);
		return false;
	}

	data = newdata;
	size.x = header.width;
	size.y = header.height;

	return true;
}

void Image::set(unsigned char* indata, const Vector2u& insize, int pixelFormat) {
	clear();
	data = indata;
	size = insize;
	pixelFormat = pixelFormat;
}

void Image::fill(const Vector2u insize, PixelFormat pixelFormat, const Vector4f& color) {
	clear();
	size = insize;
	pixelFormat = pixelFormat;

	const unsigned int pixelCount = getPixelCount();
	const unsigned int pixelSize = getPixelSize();
	const unsigned int totalPixelSize = pixelCount * pixelSize;

	unsigned char pixelData[4];
	pixelData[0] = color.x * 255;
	pixelData[1] = color.y * 255;
	pixelData[2] = color.z * 255;
	pixelData[3] = color.w * 255;	


	printf("%s::%d: Warning! not implemented yet.\n", __FILE__, __LINE__);
	return;

	data = new unsigned char[totalPixelSize];
	for (unsigned int index = 0; index < pixelCount; ++index) {
		unsigned char* pointer = data + index * pixelSize;
		switch (pixelFormat) {
		case Image::EPF_L8 :
			memcpy(pointer, pixelData, pixelSize);
			break;
		default :
			;
		}
	}
}

void Image::flipVertical() {
	const unsigned int pixelSize = getPixelSize();
	unsigned char tmp[8];

	for (unsigned int yIndex = 0; yIndex < size.y / 2; ++yIndex) {
		for (unsigned int xIndex = 0; xIndex < size.x; ++xIndex) {
			const unsigned int index0 = yIndex * size.x + xIndex;
			const unsigned int index1 = (size.y - 1 - yIndex) * size.x + xIndex;

			unsigned char* pointer0 = data + index0 * pixelSize;
			unsigned char* pointer1 = data + index1 * pixelSize;
			
			memcpy(tmp, pointer0, pixelSize);
			memcpy(pointer0, pointer1, pixelSize);
			memcpy(pointer1, tmp, pixelSize);
		}
	}
}

void Image::flipHorizontal() {
	const unsigned int pixelSize = getPixelSize();
	unsigned char tmp[8];

	for (unsigned int yIndex = 0; yIndex < size.y; ++yIndex) {
		for (unsigned int xIndex = 0; xIndex < size.x / 2; ++xIndex) {
			const unsigned int index0 = yIndex * size.x + xIndex;
			const unsigned int index1 = yIndex * size.x + size.x - xIndex;

			unsigned char* pointer0 = data + index0 * pixelSize;
			unsigned char* pointer1 = data + index1 * pixelSize;
			
			memcpy(tmp, pointer0, pixelSize);
			memcpy(pointer0, pointer1, pixelSize);
			memcpy(pointer1, tmp, pixelSize);
		}
	}
}

void Image::clearColor() {
	memset(data, 0, getPixelCount() * getPixelSize());
}

void Image::clear() {
	removeData();
	size.x = 0;
	size.y = 0;
}

void Image::removeData() {
	if (data == NULL) {
		return;
	}
	delete [] data;
	data = NULL;
}

void Image::setPixel(unsigned int x, unsigned int y, const Vector4f& value) {
	if (x >= size.x) {
		return;
	}
	if (y >= size.y) {
		return;
	}

	const unsigned int pixelIndex = (y * size.x + x) * getPixelSize();

	unsigned char red = (unsigned char)(value.x * 255.0f);
	unsigned char green = (unsigned char)(value.y * 255.0f);
	unsigned char blue = (unsigned char)(value.z * 255.0f);
	unsigned char alpha = (unsigned char)(value.w * 255.0f);
	
	switch (pixelFormat) {
	case Image::EPF_L8 :
		data[pixelIndex + 0] = red;
		break;
	case Image::EPF_R5G6B5 :	// EPF_R5G6B5
		{
		union {
			Pixel16 pixel;
			struct {
				unsigned char ub0;
				unsigned char ub1;
			} raw;
		};
		pixel.red = (unsigned char)(value.z * 31.0f);
		pixel.green = (unsigned char)(value.y * 63.0f);
		pixel.blue = (unsigned char)(value.x * 31.0f);

		data[pixelIndex + 0] = raw.ub0;
		data[pixelIndex + 1] = raw.ub1;
		}
		break;
	case Image::EPF_R8G8B8 :
		data[pixelIndex + 0] = blue;
		data[pixelIndex + 1] = green;
		data[pixelIndex + 2] = red;
		break;
	case Image::EPF_R8G8B8A8 :
		data[pixelIndex + 0] = blue;
		data[pixelIndex + 1] = green;
		data[pixelIndex + 2] = red;
		data[pixelIndex + 3] = alpha;
		break;	
	case Image::EPF_DEPTH :
		fdata[y * size.x + x] = value.x;
		break;
	}
}
	
Vector4f Image::getPixel(unsigned int x, unsigned int y) const {
	Vector4f ans;

	x %= size.x;
	y %= size.y;
	
	const unsigned int pixelIndex = (y * size.x + x) * getPixelSize();
	
	switch (pixelFormat) {
	case Image::EPF_L8 :
		ans.x = (float)data[pixelIndex + 0] / 255.0f;
		ans.y = ans.x;		// drawing depth buffer
		ans.z = ans.x;
		ans.w = 1.0f;
		break;
	case Image::EPF_R5G6B5 :
		{
		union {
			Pixel16 pixel;
			struct {
				unsigned char ub0;
				unsigned char ub1;
			} raw;
		};

		raw.ub0 = data[pixelIndex + 0];
		raw.ub1 = data[pixelIndex + 1];
		ans.x = (float)pixel.red / 63.0f;
		ans.y = (float)pixel.green / 127.0f;
		ans.z = (float)pixel.blue / 63.0f;
		ans.w = 1.0f; //(float)pixel.alpha / 63.0f;
		}
		break;
	case Image::EPF_R8G8B8 :
		ans.x = (float)data[pixelIndex + 0] / 255.0f;
		ans.y = (float)data[pixelIndex + 1] / 255.0f;
		ans.z = (float)data[pixelIndex + 2] / 255.0f;
		ans.w = 1.0f;
		break;
	case Image::EPF_R8G8B8A8 :
		ans.z = (float)data[pixelIndex + 0] / 255.0f;
		ans.y = (float)data[pixelIndex + 1] / 255.0f;
		ans.x = (float)data[pixelIndex + 2] / 255.0f;
		ans.w = (float)data[pixelIndex + 3] / 255.0f;
		break;
	case Image::EPF_DEPTH :
		ans.x = fdata[y * size.x + x];
//		ans.x = 300.0f * (0.1f + ans.x) / ((300.0f - 0.1f) * ans.x);
		ans.y = ans.x;
		ans.z = ans.x;
		ans.w = 1.0f;
		break;
	}
	
	return ans;
}

Vector4f Image::getPixelByUV(const Vector2f& uv) const {
	float width = (float)size.x;
	float height = (float)size.y;

	return getPixel((unsigned int)width * uv.x, (unsigned int)height * uv.y);
}

Vector4f Image::operator()(const Vector2f& uv) const {
	return getPixelByUV(uv);
}

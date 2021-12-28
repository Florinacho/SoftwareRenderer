#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stdint.h>

#include "Image.h"

namespace tga {

// https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf

//	8	4	2	1
//	0	0	0	1
//	0	0	1	0
//	0	0	1	1
//	1	0	0	0
// least significant byte first, most significant byte last
enum TGA_IMAGE_TYPE : uint8_t {
	NO_IMAGE_DATA  =  0,  // No image data included.
	COLOR_MAP      =  1,  // Uncompressed, color-mapped images.
	TRUE_COLOR     =  2,  // Uncompressed, RGB images.
	GRAYSCALE      =  3,  // Uncompressed, black and white images.
	RLE_COLOR_MAP  =  9,  // Runlength encoded color-mapped images.
	RLE_TRUE_COLOR = 10,  // Runlength encoded RGB images.
	RLE_GRAYSCALE  = 11,  // Compressed, black and white images.
	
};

#pragma pack(push, 1)
struct Header {
	// This field identifies the number of bytes contained in Field 6, the Image ID Field. The maximum number
	// of characters is 255. A value of zero indicates that no Image ID field is included with the imag
	uint8_t  idLength;
	
	// 0 - indicates that no color-map data is included with this image.
	// 1 - indicates that a color-map is included with this image.
	uint8_t  colorMapType;
	
	uint8_t  imageType;
	uint16_t colorMapOffset;
	uint16_t colorMapLength;
	uint8_t  colorMapDepth;
	uint16_t xOrigin;
	uint16_t yOrigin;
	uint16_t width;
	uint16_t height;
	uint8_t  depth;
	uint8_t  descriptor;
};

struct Footer {
	uint32_t extensionOffset;
	uint32_t developerAreaOffset;
	char     signature[18];
};
#pragma pack(pop)

static const uint32_t HeaderSize = sizeof(Header);
static const uint32_t FooterSize = sizeof(Footer);
static const char FooterSignature[] = "TRUEVISION-XFILE.";
static const uint32_t FooterSignatureLength = 18;

} // namespace tga
/*
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
*/

void convertGrayscaleToR8G8B8(uint8_t* dest, uint8_t* src, uint32_t length) {
	static const uint32_t DEST_PIXEL_SIZE = 3;
	for (uint32_t index = 0; index < length; ++index) {
		dest[index * DEST_PIXEL_SIZE + 0] = src[index];
		dest[index * DEST_PIXEL_SIZE + 1] = src[index];
		dest[index * DEST_PIXEL_SIZE + 2] = src[index];
	}
}

void convertGrayscaleToA8R8G8B8(uint8_t* dest, uint8_t* src, uint32_t length) {
	static const uint32_t DEST_PIXEL_SIZE = 4;
	for (uint32_t index = 0; index < length; ++index) {
		dest[index * DEST_PIXEL_SIZE + 0] = src[index];
		dest[index * DEST_PIXEL_SIZE + 1] = src[index];
		dest[index * DEST_PIXEL_SIZE + 2] = src[index];
		dest[index * DEST_PIXEL_SIZE + 3] = 255; // max opacity
	}
}

void convertIndexedToR8G8B8(uint8_t* dest, uint8_t* src, uint32_t length, uint8_t* palete) {
	static const uint32_t DEST_PIXEL_SIZE = 3;
	for (uint32_t index = 0; index < length; ++index) {
		memcpy(dest + index * DEST_PIXEL_SIZE,
		       palete + src[index] * DEST_PIXEL_SIZE,
			   DEST_PIXEL_SIZE);
	}
}

void convertIndexedToA8R8G8B8(uint8_t* dest, uint8_t* src, uint32_t length, uint8_t* palete) {
	static const uint32_t DEST_PIXEL_SIZE = 4;
	for (uint32_t index = 0; index < length; ++index) {
		memcpy(dest + index * DEST_PIXEL_SIZE,
		       palete + src[index] * DEST_PIXEL_SIZE,
			   DEST_PIXEL_SIZE);
	}
}

bool Image::updateCoordinates(int& x, int& y) const {
	switch (wrapping.x) {
	case Image::EWT_REPEAT :
		if (x <            0) x += size.x;
		if (x >= (int)size.x) x -= size.x;
		break;
	case Image::EWT_MIRROR : return true; // TODO test
		if (x <            0) x = -x;
		if (x >= (int)size.x) x = size.x - 1 - (x - size.x);
		break;
	case Image::EWT_CLAMP :
		if (x <            0) x = 0;
		if (x >= (int)size.x) x = size.x - 1;
		break;
	case Image::EWT_DISCARD :
		if ((x < 0) || (x >= (int)size.x)) return true;
		break;
	}
			
	switch (wrapping.y) {
	case Image::EWT_REPEAT :
		if (y <            0) y += size.y;
		if (y >= (int)size.y) y -= size.y;
		break;
	case Image::EWT_MIRROR : return true; // TODO test
		if (y <            0) y = -y;
		if (y >= (int)size.y) y = size.y - 1 - (y - size.y);
		break;
	case Image::EWT_CLAMP :
		if (y <            0) y = 0;
		if (y >= (int)size.y) y = size.y - 1;
		break;
	case Image::EWT_DISCARD :
		if ((y < 0) || (y >= (int)size.y)) return true;
		break;
	}
			
	return false;
}

uint32_t Image::getLineStride() const {
	return size.x * getPixelSize();
}

Image::Image() {
	colorMapData = NULL;
	colorMapLength = 0;
	
	data = NULL;

	size.x = 0;
	size.y = 0;

	pixelFormat = Image::EPF_NONE;
	wrapping = Image::EWT_DISCARD;
}

Image::~Image() {
	destroy();
}

void Image::create(const Vector2u& insize, Image::PIXEL_FORMAT inpixelFormat) {
	destroy();
	
	size = insize;
	pixelFormat = inpixelFormat;
	
	const uint32_t totalPixelSize = size.x * size.y * getPixelSize();

	data = new uint8_t[totalPixelSize];

	memset(data, 0, totalPixelSize);
}

const uint8_t* Image::getData() const {
	return data;
}

uint32_t Image::getDataLength() const {
	return size.x * size.y * getPixelSize();
}

Vector2u Image::getSize() const {
	return size;
}

Image::PIXEL_FORMAT Image::getPixelFormat() const {
	return (Image::PIXEL_FORMAT)pixelFormat;
}

uint32_t Image::getPixelSize() const {
	switch (pixelFormat) {
	case EPF_NONE :
		return 0;
	case EPF_INDEX_RGB :
		return sizeof(uint8_t);
	case EPF_INDEX_RGBA :
		return sizeof(uint8_t);
	case EPF_GRAYSCALE :
		return sizeof(uint8_t);
	case EPF_GRAYSCALE_ALPHA :
		return sizeof(uint8_t) * 2;
	case EPF_R8G8B8 :
		return sizeof(uint8_t) * 3;
	case EPF_R8G8B8A8 :
		return sizeof(uint8_t) * 4;
	case EPF_DEPTH :
		return sizeof(float);
	}
	return 0;
}

bool Image::hasAlpha() const {
	switch (pixelFormat) {
	case EPF_NONE :
		return false;
	case EPF_INDEX_RGB :
		return false;
	case EPF_INDEX_RGBA :
		return true;
	case EPF_GRAYSCALE :
		return false;
	case EPF_GRAYSCALE_ALPHA :
		return true;
	case EPF_R8G8B8 :
		return false;
	case EPF_R8G8B8A8 :
		return true;
	case EPF_DEPTH :
		return false;
	}
	return false;
}

bool Image::readUncompressedPixelData(uint8_t* pixels, uint32_t depth, uint32_t width, uint32_t height, FILE* file) {
	const uint32_t pixelCount = width * height;
	uint32_t totalByteCount = pixelCount * depth;
	return (fread(pixels, sizeof(uint8_t), totalByteCount, file) == totalByteCount);
}

bool ReadRLEData(uint8_t* data, uint32_t size, uint32_t count, FILE* file) {
	const uint32_t totalSize = size * count;
	uint32_t offset = 0;
	uint8_t chunkHeader = 0;
	
	while (offset < totalSize) {
		if (fread(&chunkHeader, sizeof(chunkHeader), 1, file) != 1) {
			return false;
		}
		if (chunkHeader < 128) {
			chunkHeader += 1;
			if (fread(data + offset, size, chunkHeader, file) != chunkHeader) {
				return false;
			}
			offset += chunkHeader * size;
		} else {
			chunkHeader -= 127;
			const uint32_t dataOffset = offset;
			if (fread(data + dataOffset, size, 1, file) != 1) {
				return false;
			}
			for (uint32_t counter = 0; counter < chunkHeader; ++counter) {
				memcpy(data + offset, data + dataOffset, size);
				offset += size;
			}
		}
	}
	return (offset == totalSize);
}

bool Image::readCompressedPixelData(uint8_t* pixels, uint32_t depth, uint32_t width, uint32_t height, FILE* file) {
#if 1
	return ReadRLEData(pixels, depth, width * height, file);
#else
	uint8_t* ptr = pixels;
	for (uint32_t y = 0; y < height; ++y) {
		static int repeat = 0;
		static int direct = 0;
		static uint8_t sample[4];
		int head;

		for (int x = 0; x < width; ++x) {
			if (repeat == 0 && direct == 0) {
				head = getc(file);
				if (head == EOF) {
					return false;
				} else if (head >= 128) {
					repeat = head - 127;
					if (fread(sample, depth, 1, file) != 1) {
						return false;
					}
				} else {
					direct = head + 1;
				}
			}

			if (repeat > 0) {
				for (uint32_t index = 0; index < depth; ++index) {
					ptr[index] = sample[index];
				}
				--repeat;
			} else { /* direct > 0 */
				if (fread(ptr, depth, 1, file) < 1) {
					return false;
				}
				--direct;
			}
			ptr += depth;
		}
	}
	return true;
#endif
}

// https://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
bool Image::load(const char* filename, bool convertToTruecolor) {
	tga::Header header;
	uint8_t* colorMapData = NULL;
	uint8_t* pixelData = NULL;
	
	destroy();

	FILE* file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Image::load(%s) error! Cannot open file.\n", filename);
		return false;
	}

	// Read TGA header
	if (fread(&header, tga::HeaderSize, 1, file) != 1) {
		printf("Image::load(%s) error! Cannot read TGA header.\n", filename);
		fclose(file);
		return false;
	}

	// Skip file identification field	
	if (header.idLength > 0) {
		fseek(file, header.idLength, SEEK_CUR);
	}
	
	// Read color map data
	if (header.colorMapType == 1) {
		const uint32_t colorMapSize = (header.colorMapDepth / 8) * header.colorMapLength;
		colorMapData = new uint8_t[colorMapSize];
		if (fread(colorMapData, header.colorMapDepth / 8, header.colorMapLength, file) != header.colorMapLength) {
			delete [] colorMapData;
			fclose(file);
			return false;
		}
		printf("\n");
		for (uint32_t index = 0; index < header.colorMapLength; ++index) {
			if (header.colorMapDepth == 24) {
				printf("\tubvec4(%3.1hhu, %3.1hhu, %3.1hhu, %3.1hhu),\n",
						colorMapData[index * 3 + 2],
						colorMapData[index * 3 + 1],
						colorMapData[index * 3 + 0],
						255);
			} else {
				printf("\tubvec4(%3.1hhu, %3.1hhu, %3.1hhu, %3.1hhu),\n",
						colorMapData[index * 4 + 2],
						colorMapData[index * 4 + 1],
						colorMapData[index * 4 + 0],
						colorMapData[index * 4 + 3]);
			}
		}
	}

	const uint32_t pixelCount = header.width * header.height;
	const uint32_t pixelSize = header.depth / 8;
	const uint32_t totalPixelsSize = pixelCount * pixelSize;

	// Read image data
	if (header.imageType == tga::NO_IMAGE_DATA) {
		printf("Image::load(%s) No image data present.\n", filename);
		if (colorMapData != NULL) {
			delete [] colorMapData;
		}
		fclose(file);
		return false;
	} else {
		pixelData = new uint8_t[totalPixelsSize];
		
		if (header.imageType & 0x8) {
			if (readCompressedPixelData(pixelData, pixelSize, header.width, header.height, file) == false) {
				if (colorMapData != NULL) {
					delete [] colorMapData;
				}
				delete [] pixelData;
				fclose(file);
				return false;
			}
		} else {
			if (readUncompressedPixelData(pixelData, pixelSize, header.width, header.height, file) == false) {
				if (colorMapData != NULL) {
					delete [] colorMapData;
				}
				delete [] pixelData;
				fclose(file);
				return false;
			}
		}
	}

	fclose(file);

	// Convert TGA pixel data to Image pixel data
	switch (header.depth) {
	case 8 :
		switch (header.imageType & 0x3) {
		case tga::GRAYSCALE :
			pixelFormat = EPF_GRAYSCALE;
			data = pixelData;
			break;
		case tga::COLOR_MAP :
			switch (header.colorMapDepth) {
			case 24 :
				if (convertToTruecolor) {
					pixelFormat = EPF_R8G8B8;
					data = new uint8_t[pixelCount * 3];
					convertIndexedToR8G8B8(data, pixelData, pixelCount, colorMapData);
					delete [] pixelData;
				} else {
					pixelFormat = EPF_INDEX_RGB;
					data = pixelData;
					this->colorMapData = colorMapData;
					this->colorMapLength = header.colorMapLength;
				}
				break;
			case 32 :
				if (convertToTruecolor) {
					pixelFormat = EPF_R8G8B8A8;
					data = new uint8_t[pixelCount * 4];
					convertIndexedToA8R8G8B8(data, pixelData, pixelCount, colorMapData);
					delete [] pixelData;
				} else {
					pixelFormat = EPF_INDEX_RGBA;
					data = pixelData;
					this->colorMapData = colorMapData;
					this->colorMapLength = header.colorMapLength;
				}
				break;
			default :
				printf("Image::load(%s): Unsupported color map depth %d\n", filename, header.colorMapDepth / 8);
				pixelFormat = EPF_NONE;
				delete [] pixelData;
				if (colorMapData != NULL) {
					delete [] colorMapData;
				}
				break;
			}
			break;
		default :
			printf("TODO: error!\n");
			break;
		}
		break;
	case 16 :
		// 16bit grayscale
		// A1R5G5B5
		{
			for (uint32_t index = 0; index < 64; index += 2) {
				printf("(%hhu, %hhu) ", pixelData[index + 0], pixelData[index + 1]);
			}
		}
		switch (header.imageType & 0x3) {
		case tga::GRAYSCALE :
			pixelFormat = EPF_GRAYSCALE_ALPHA;
			data = pixelData;
			break;
		default :
			printf("imageType: %d\n", header.imageType);
			printf("Image::load(%s) Unsupported pixel depth %d.\n", filename, header.depth);
			if (colorMapData != NULL) {
				delete [] colorMapData;
			}
			if (pixelData != NULL) {
				delete [] pixelData;
			}
			return false;
			break;
		}
		break;
	case 24 :
		pixelFormat = Image::EPF_R8G8B8;
		data = pixelData;
		break;
	case 32 :
		pixelFormat = Image::EPF_R8G8B8A8;
		data = pixelData;
		break;
	default :
		printf("Image::load(%s) error! Unsupported pixel depth %d.\n", filename, header.depth);
		return false;
	}

	size.x = header.width;
	size.y = header.height;

	if (header.yOrigin == 0) {
		//flipVertical();
	}

	return true;
}

void Image::flipVertical() {
	const uint32_t pixelSize = getPixelSize();
	uint8_t tmp[8];

	for (uint32_t yIndex = 0; yIndex < size.y / 2; ++yIndex) {
		for (uint32_t xIndex = 0; xIndex < size.x; ++xIndex) {
			const uint32_t index0 = yIndex * size.x + xIndex;
			const uint32_t index1 = (size.y - 1 - yIndex) * size.x + xIndex;

			uint8_t* pointer0 = data + index0 * pixelSize;
			uint8_t* pointer1 = data + index1 * pixelSize;
			
			memcpy(tmp, pointer0, pixelSize);
			memcpy(pointer0, pointer1, pixelSize);
			memcpy(pointer1, tmp, pixelSize);
		}
	}
}

void Image::flipHorizontal() {
	const uint32_t pixelSize = getPixelSize();
	uint8_t tmp[8];

	for (uint32_t yIndex = 0; yIndex < size.y; ++yIndex) {
		for (uint32_t xIndex = 0; xIndex < size.x / 2; ++xIndex) {
			const uint32_t index0 = yIndex * size.x + xIndex;
			const uint32_t index1 = yIndex * size.x + size.x - xIndex;

			uint8_t* pointer0 = data + index0 * pixelSize;
			uint8_t* pointer1 = data + index1 * pixelSize;
			
			memcpy(tmp, pointer0, pixelSize);
			memcpy(pointer0, pointer1, pixelSize);
			memcpy(pointer1, tmp, pixelSize);
		}
	}
}

void Image::clear() {
	memset(data, 0, getDataLength());
}

void Image::destroy() {
	if (colorMapData != NULL) {
		delete [] colorMapData;
		colorMapData = NULL;
		colorMapLength = 0;
	}
	
	if (data != NULL) {
		delete [] data;
		data = NULL;
		size.x = 0;
		size.y = 0;
	}
	
	pixelFormat = EPF_NONE;
}

void Image::setPixel(int x, int y, const ubvec4& color) {
	if (updateCoordinates(x, y)) {
		return;
	}

	const uint32_t pixelIndex = y * getLineStride() + x * getPixelSize();
	
	union {
		uint8_t * data_u8;
		float * data_f32;
	};
	
	switch (pixelFormat) {
	case Image::EPF_INDEX_RGB :
		return;
	case Image::EPF_INDEX_RGBA :
		return;
	case Image::EPF_GRAYSCALE :
		data[pixelIndex] = (color.x + color.y + color.z) / 3;
		break;
	case Image::EPF_GRAYSCALE_ALPHA :
		data[pixelIndex + 0] = (color.x + color.y + color.z) / 3;
		data[pixelIndex + 1] = color.w;
		break;
	case Image::EPF_R8G8B8 :
		data[pixelIndex + 0] = color.z;
		data[pixelIndex + 1] = color.y;
		data[pixelIndex + 2] = color.x;
		break;
	case Image::EPF_R8G8B8A8 :
#if 0
		*(uint32_t*)&data[pixelIndex] = *((uint32_t*)&color);
#else
		data[pixelIndex + 0] = color.z;
		data[pixelIndex + 1] = color.y;
		data[pixelIndex + 2] = color.x;	
		data[pixelIndex + 3] = color.w;
#endif
		break;
	case EPF_DEPTH :
//		printf("%s::%d Not implemented!\n", __FILE__, __LINE__);
//		data_u8 = data;
		*((float*)(&data[pixelIndex])) = ((float)color.x) / 255.0f;
		//data_f32[y * size.x + x] = (float)color.x / 255.0f;
		break;
	}
}
	
const ubvec4 Image::getPixel(int x, int y) const {
	ubvec4 ans;

	if (updateCoordinates(x, y)) {
		return ans;
	}

	const uint32_t pixelIndex = y * getLineStride() + x * getPixelSize();
	
	switch (pixelFormat) {
	case Image::EPF_INDEX_RGB :
		ans.x = colorMapData[data[pixelIndex] * 3 + 0]; // 2?
		ans.y = colorMapData[data[pixelIndex] * 3 + 1]; 
		ans.z = colorMapData[data[pixelIndex] * 3 + 2]; // 0 ?
		ans.w = 255;
		break;
	case Image::EPF_INDEX_RGBA :
		ans.x = colorMapData[data[pixelIndex] * 4 + 0]; // 2?
		ans.y = colorMapData[data[pixelIndex] * 4 + 1]; 
		ans.z = colorMapData[data[pixelIndex] * 4 + 2]; // 0 ?
		ans.w = colorMapData[data[pixelIndex] * 4 + 3]; // 0 ?;
		break;
	case Image::EPF_GRAYSCALE :
		ans = data[pixelIndex];
		break;
	case Image::EPF_GRAYSCALE_ALPHA :
		ans.x = data[pixelIndex + 0];
		ans.y = data[pixelIndex + 0];
		ans.z = data[pixelIndex + 0];
		ans.w = data[pixelIndex + 1];
		break;
	case Image::EPF_R8G8B8 :
		ans.z = data[pixelIndex + 0];
		ans.y = data[pixelIndex + 1];
		ans.x = data[pixelIndex + 2];
		ans.w = 255.0f;
		break;
	case Image::EPF_R8G8B8A8 :
#if 0
		*((uint32_t*)&ans) = *(uint32_t*)&data[pixelIndex];
#else
		ans.z = data[pixelIndex + 0];
		ans.y = data[pixelIndex + 1];
		ans.x = data[pixelIndex + 2];
		ans.w = data[pixelIndex + 3];
#endif
		break;
	case EPF_DEPTH :
		ans.x = *((float*)(&data[pixelIndex])) * 255.0f;
		ans.y = ans.x;
		ans.z = ans.x;
		ans.w = 255;
		break;
	}
	return ans;
}

void Image::setPixelf(int x, int y, const vec4& color) {
	if (updateCoordinates(x, y)) {
		return;
	}

	uint32_t pixelIndex = y * size.x + x;
	
	switch (pixelFormat) {
	case Image::EPF_INDEX_RGB :
		return;
	case Image::EPF_INDEX_RGBA :
		return;
	case Image::EPF_GRAYSCALE :
		data[pixelIndex] = (color.x + color.y + color.z) * 85.0f; // * 255.0f / 3.0f; // TODO: test this
		break;
	case Image::EPF_GRAYSCALE_ALPHA :
		pixelIndex *= 2;
		data[pixelIndex + 0] = (color.x + color.y + color.z) * 85.0f; // * 255.0f / 3.0f; // TODO: test this
		data[pixelIndex + 1] = color.w * 255.0f;
		break;
	case Image::EPF_R8G8B8 :
		pixelIndex *= 3;
		data[pixelIndex + 0] = color.z * 255.0f;
		data[pixelIndex + 1] = color.y * 255.0f;
		data[pixelIndex + 2] = color.x * 255.0f;
		break;
	case Image::EPF_R8G8B8A8 :
		pixelIndex *= 4;
		data[pixelIndex + 0] = color.z * 255.0f;
		data[pixelIndex + 1] = color.y * 255.0f;
		data[pixelIndex + 2] = color.x * 255.0f;	
		data[pixelIndex + 3] = color.w * 255.0f;
		break;
	case EPF_DEPTH :
		fdata[pixelIndex] = color.x;
		break;
	}
}

vec4 Image::getPixelf(int x, int y) const {
	vec4 ans;

	if (updateCoordinates(x, y)) {
		return ans;
	}

	uint32_t pixelIndex = y * size.x + x;
	
	switch (pixelFormat) {
	case Image::EPF_INDEX_RGB :
		ans.x = (float)colorMapData[data[pixelIndex] * 3 + 0] / 255.0f; // 2?
		ans.y = (float)colorMapData[data[pixelIndex] * 3 + 1] / 255.0f; 
		ans.z = (float)colorMapData[data[pixelIndex] * 3 + 2] / 255.0f; // 0 ?
		ans.w = 1.0f;
		break;
	case Image::EPF_INDEX_RGBA :
		ans.x = (float)colorMapData[data[pixelIndex] * 4 + 0] / 255.0f; // 2?
		ans.y = (float)colorMapData[data[pixelIndex] * 4 + 1] / 255.0f; 
		ans.z = (float)colorMapData[data[pixelIndex] * 4 + 2] / 255.0f; // 0 ?
		ans.w = (float)colorMapData[data[pixelIndex] * 4 + 3] / 255.0f; // 0 ?;
		break;
	case Image::EPF_GRAYSCALE :
		ans = (float)data[pixelIndex] / 255.0f;
		break;
	case Image::EPF_GRAYSCALE_ALPHA :
		pixelIndex *= 2;
		ans.x = (float)data[pixelIndex + 0] / 255.0f;
		ans.y = ans.x;
		ans.z = ans.x;
		ans.w = (float)data[pixelIndex + 1] / 255.0f;
		break;
	case Image::EPF_R8G8B8 :
		pixelIndex *= 3;
		ans.z = (float)data[pixelIndex + 0] / 255.0f;
		ans.y = (float)data[pixelIndex + 1] / 255.0f;
		ans.x = (float)data[pixelIndex + 2] / 255.0f;
		ans.w = 1.0f;
		break;
	case Image::EPF_R8G8B8A8 :
		pixelIndex *= 4;
		ans.z = (float)data[pixelIndex + 0] / 255.0f;
		ans.y = (float)data[pixelIndex + 1] / 255.0f;
		ans.x = (float)data[pixelIndex + 2] / 255.0f;
		ans.w = (float)data[pixelIndex + 3] / 255.0f;
		break;
	case EPF_DEPTH :
		ans.x = fdata[pixelIndex];
		ans.y = ans.x;
		ans.z = ans.x;
		ans.w = 1.0f;
		break;
	}
	return ans;
}

vec4 Image::sample2D(const vec2& uv) const {
	return getPixelf(uv.x * size.x, uv.y * size.y);
}
	
void Image::drawLine(const ivec2& begin, const ubvec4& beginColor, const ivec2& end, const ubvec4& endColor) {
	float xdiff = (end.x - begin.x);
	float ydiff = (end.y - begin.y);
	ubvec4 colorDiff;
	float slope;

	if((xdiff == 0) && (ydiff == 0)) {
		setPixel(begin.x, begin.y, (beginColor + endColor) / 2);
		return;
	}

//	colorDiff = endColor - beginColor;

	if(fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

		if(begin.x < end.x) {
			xmin = begin.x;
			xmax = end.x;
		} else {
			xmin = end.x;
			xmax = begin.x;
		}

		// draw line in terms of y slope
		slope = ydiff / xdiff;
		for(float x = xmin; x <= xmax; ++x) {
			const float y = begin.y + ((x - begin.x) * slope);
			const float proc = (x - begin.x) / xdiff;
			setPixel(x, y, beginColor + colorDiff * proc);
		}
	} else {
		float ymin, ymax;

		if(begin.y < end.y) {
			ymin = begin.y;
			ymax = end.y;
		} else {
			ymin = end.y;
			ymax = begin.y;
		}

		slope = xdiff / ydiff;
		for(float y = ymin; y <= ymax; ++y) {
			const float x = begin.x + ((y - begin.y) * slope);
			const float proc = (y - begin.y) / ydiff;
			setPixel(x, y, beginColor + colorDiff * proc);
		}
	}
}
	
void Image::drawLine(const ivec2& begin, const ivec2& end, const ubvec4& color) {
	float xdiff = (end.x - begin.x);
	float ydiff = (end.y - begin.y);
	float slope;

	if((xdiff == 0.0f) && (ydiff == 0.0f)) {
		setPixel(begin.x, begin.y, color);
		return;
	}

	if(fabs(xdiff) > fabs(ydiff)) {
		float xmin, xmax;

		if(begin.x < end.x) {
			xmin = begin.x;
			xmax = end.x;
		} else {
			xmin = end.x;
			xmax = begin.x;
		}

		// draw line in terms of y slope
		slope = ydiff / xdiff;
		for(float x = xmin; x <= xmax; ++x) {
			const float y = begin.y + ((x - begin.x) * slope);
			const float proc = (x - begin.x) / xdiff;
			setPixel(x, y, color);
		}
	} else {
		float ymin, ymax;

		if(begin.y < end.y) {
			ymin = begin.y;
			ymax = end.y;
		} else {
			ymin = end.y;
			ymax = begin.y;
		}

		slope = xdiff / ydiff;
		for(float y = ymin; y <= ymax; ++y) {
			const float x = begin.x + ((y - begin.y) * slope);
			const float proc = (y - begin.y) / ydiff;
			setPixel(x, y, color);
		}
	}
}
	
void Image::drawRectangle(const ivec4& bounds, const ubvec4& color) {
}
	
void Image::drawFilledRectangle(const ivec4& bounds, const ubvec4& color) {
	for (int x = std::max<int>(bounds.x, 0); x < std::min<int>(bounds.z, size.x - 1); ++x) {
		for (int y = std::max<int>(bounds.y, 0); y < std::min<int>(bounds.w, size.y - 1); ++y) {
			setPixel(x, y, color);
		}
	}
}
	
void Image::drawCircle(const ivec2& center, int radius, const ubvec4& color) {
	const int iradius = (int)floor(radius + 0.5f);
	const int iradius_sqr = iradius * iradius;
	for (int y = -iradius; y < iradius; ++y) {
		const int width = (int)std::sqrt(iradius_sqr - y * y);
		setPixel(center.x - width, center.y + y, color);
		setPixel(center.x + width, center.y + y, color);
	}
}

void Image::drawFilledCircle(const ivec2& center, int radius, const ubvec4& color) {
	const int iradius = (int)floor(radius + 0.5f);
	const int iradius_sqr = iradius * iradius;
	for (int y = -iradius; y < iradius; ++y) {
		const int width = (int)std::sqrt(iradius_sqr - y * y);
		//int offset = (center.y + y) * size.x + center.x;
		for (int x = -width; x < width; ++x) {
			setPixel(center.x + x, center.y + y, color);
		}
	}
}

void Image::blit(const Image* image, const uvec2& position) {
	if (image == NULL) {
		return;
	}
	
	if (pixelFormat != image->getPixelFormat()) {
		return;
	}
	
	const uint32_t dstWidth  = size.x - position.x;
	const uint32_t dstHeight = size.y - position.y;
	
	const uint32_t srcWidth  = (dstWidth  < image->getSize().x) ? dstWidth  : image->getSize().x;
	const uint32_t srcHeight = (dstHeight < image->getSize().y) ? dstHeight : image->getSize().y;

	for (uint32_t y = 0; y < srcHeight; ++y) {
		memcpy(data + ((position.y + y) * size.x + position.x) * getPixelSize(),
				image->data + (y * image->getSize().x) * getPixelSize(),
				srcWidth * getPixelSize());
	}
}
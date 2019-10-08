#ifndef __TGA_H__
#define __TGA_H__

namespace tga {

enum ImageType {
	NO_IMAGE_DATA = 0,
	COLOR_MAP = 1,			//UNCOMPRESSED
	TRUE_COLOR = 2,			//UNCOMPRESSED
	GRAYSCALE = 3,			//UNCOMPRESSED
	RLE_COLOR_MAP = 9,		//COMPRESSED
	RLE_TRUE_COLOR = 10,		//COMPRESSED
	RLE_GRAYSCALE = 11		//COMPRESSED
};

#pragma pack(push, 1)
struct Header {
	unsigned char length;
	unsigned char colorMapType;
	unsigned char compression;
	unsigned short colorMapOffset;
	unsigned short colorMapLength;
	unsigned char colorMapDepth;
	unsigned short xOrigin;
	unsigned short yOrigin;
	unsigned short width;
	unsigned short height;
	unsigned char depth;
	unsigned char descriptor;
};

struct Footer {
	unsigned int extensionOffset;
	unsigned int developerAreaOffset;
	char signature[18];
};

#pragma pack(pop)
static const unsigned int HeaderSize = sizeof(Header);
static const unsigned int FooterSize = sizeof(Footer);
static const char* FooterSignature = "TRUEVISION-XFILE.";
static const unsigned int FooterSignatureLength = 18; // including '\0'

} // namespace tga

#endif // __TGA_H__

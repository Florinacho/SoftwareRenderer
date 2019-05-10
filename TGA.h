#ifndef __TGA_H__
#define __TGA_H__

namespace tga {

//enum ColorMapType {
//	NO_COLOR_MAP = 0,
//	COLOR_MAP = 1,
//	//2–127 reserved by Truevision
//	//128–255 available for developer use
//};

enum ImageType {
	NO_IMAGE_DATA = 0,
	COLOR_MAP = 1,			//UNCOMPRESSED
	TRUE_COLOR = 2,		//UNCOMPRESSED
	GRAYSCALE = 3,			//UNCOMPRESSED
	RLE_COLOR_MAP = 9,		//COMPRESSED
	RLE_TRUE_COLOR = 10,	//COMPRESSED
	RLE_GRAYSCALE = 11		//COMPRESSED
};
/**
TYPE			8	4	2	1
---------------------------------
NO_DATA		0	0	0	0
COLOR_MAP		0	0	0	1
TRUE_COLOR	0	0	1	0
GRAYSCALE		0	0	1	1
---------------------------------
RLE_COLOR_MAP	1	0	0	1
RLE_TRUE_COLOR	1	0	1	0
RLE_GRAYSCALE	1	0	1	1

/**/
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

struct FooterExtension {
	unsigned short length;			// Extension area length; Max value: 65535
	char signature[25];
	unsigned short spriteWidth;		// Max value: 65535
	unsigned short spriteHeight;		// Max value: 65535
	unsigned char animationCount;		// Max value: 255
	// AnimationData
	//	name
	//		unsigned char length;	// Max 255
	//		char* data;
	//	short begin;
	//	short end;
	//	short delay;
	//	bool loop;
	//	bool pingPong;
};

struct TestAnimationData {
	char name[256];
	short begin;
	short end;
	short delay;
	bool loop;
	bool pingPong;
};

static const char* ExtensionSignature = "TGA-EXT-BOMBERMAN-SPRITE";
static const unsigned int ExtenSionSignatureLength = 25;

#pragma pack(pop)
static const unsigned int HeaderSize = sizeof(Header);
static const unsigned int FooterSize = sizeof(Footer);
static const char* FooterSignature = "TRUEVISION-XFILE.";
static const unsigned int FooterSignatureLength = 18; // including '\0'

} // namespace tga

#endif // __TGA_H__

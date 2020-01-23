#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

class Image;

struct RenderTarget {
	enum BufferType {
		ERT_COLOR_0,
		ERT_COLOR_1,
		ERT_COLOR_2,
		ERT_COLOR_3,
		ERT_DEPTH,
		ERT_COUNT
	};
	Image* buffers[ERT_COUNT];

	RenderTarget();

	void setBuffer(const BufferType type, Image* buffer);

	Image* getBuffer(const BufferType type) const;
};

#endif //__RENDER_TARGET_H__

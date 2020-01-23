#include <stdlib.h>

#include "RenderTarget.h"

RenderTarget::RenderTarget() {
	for (unsigned int index = 0; index < RenderTarget::ERT_COUNT; ++index) {
		buffers[index] = NULL;
	}
}

void RenderTarget::setBuffer(const BufferType type, Image* buffer) {
	buffers[type] = buffer;
}

Image* RenderTarget::getBuffer(const BufferType type) const {
	return buffers[type];
}

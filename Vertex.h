#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Vector.h"

struct Vertex {
	Vector3f position;
	Vector2f textureCoords;
	Vector4f color;

	Vertex() {
	}
	
	Vertex(const Vector3f& nposition, const Vector2f& ntextureCoords, const Vector4f& ncolor)
		: position(nposition), textureCoords(ntextureCoords), color(ncolor) {
	}
};

#endif //__VERTEX_H__

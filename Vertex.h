#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Vector.h"

struct Vertex {
	Vector3f position;
	Vector3f normal;
	Vector2f textureCoords;
	Vector4f color;

	Vertex() {
	}

	Vertex(const Vector3f& nposition)
		: position(nposition) {
	}

	Vertex(const Vector3f& nposition, const Vector2f& ntextureCoords)
		: position(nposition), textureCoords(ntextureCoords) {
	}

	Vertex(const Vector3f& nposition, const Vector4f& ncolor)
		: position(nposition), color(ncolor) {
	}
	
	Vertex(const Vector3f& nposition, const Vector2f& ntextureCoords, const Vector4f& ncolor)
		: position(nposition), textureCoords(ntextureCoords), color(ncolor) {
	}

	Vertex(const Vector3f& nposition, const Vector3f nnormal, const Vector2f& ntextureCoords, const Vector4f& ncolor)
		: position(nposition), normal(nnormal), textureCoords(ntextureCoords), color(ncolor) {
	}
};

#endif //__VERTEX_H__

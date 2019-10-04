#ifndef __CUBE_MESH_H__
#define __CUBE_MESH_H__

#include "Vertex.h"

static const float CubeSize = 10.0f;
static const float CubeHalfSize = CubeSize / 2.0f;

static const Vertex CubeVertices[] = {
	// Back
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(1.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f)),

	// Front
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(1.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 0.0f, 0.5f)),

	// Left
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(1.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 0.5f)),

	// Right
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(0.0f, 1.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(1.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 1.0f, 0.5f)),

	// Up
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.5f)),

	// Down
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 0.5f)),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec2(1.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 0.5f)),

};
static const unsigned int CubeVerticesSize = sizeof(CubeVertices);
static const unsigned int CubeVerticesCount = CubeVerticesSize / sizeof(Vertex);

#endif // __CUBE_MESH_H__

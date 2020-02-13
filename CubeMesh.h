#ifndef __CUBE_MESH_H__
#define __CUBE_MESH_H__

#include "Vertex.h"

static const float CubeSize = 10.0f;
static const float CubeHalfSize = CubeSize / 2.0f;
static const vec4 TopColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
static const vec4 BottomColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
static const vec4 FrontColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
static const vec4 RightColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
static const vec4 BackColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
static const vec4 LeftColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);

static const Vertex CubeVertices[] = {
	// Front
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), FrontColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), FrontColor),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), FrontColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), FrontColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), FrontColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), FrontColor),
	// Back
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 0.0f,-1.0f), vec2(0.0f, 1.0f), BackColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 0.0f,-1.0f), vec2(1.0f, 0.0f), BackColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 0.0f,-1.0f), vec2(1.0f, 1.0f), BackColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 0.0f,-1.0f), vec2(0.0f, 1.0f), BackColor),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 0.0f,-1.0f), vec2(0.0f, 0.0f), BackColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 0.0f,-1.0f), vec2(1.0f, 0.0f), BackColor),
	// Left
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), LeftColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), LeftColor),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), LeftColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), LeftColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3(-1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), LeftColor),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3(-1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), LeftColor),
	// Right
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), RightColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), RightColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), RightColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), RightColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), RightColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), RightColor),
	// Up
	Vertex(vec3(-CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), TopColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), TopColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), TopColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize,-CubeHalfSize), vec3( 0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), TopColor),
	Vertex(vec3( CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), TopColor),
	Vertex(vec3(-CubeHalfSize, CubeHalfSize, CubeHalfSize), vec3( 0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), TopColor),
	// Down
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 0.0f,-1.0f, 0.0f), vec2(0.0f, 1.0f), BottomColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 0.0f,-1.0f, 0.0f), vec2(1.0f, 0.0f), BottomColor),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 0.0f,-1.0f, 0.0f), vec2(0.0f, 0.0f), BottomColor),
	Vertex(vec3(-CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 0.0f,-1.0f, 0.0f), vec2(0.0f, 1.0f), BottomColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize, CubeHalfSize), vec3( 0.0f,-1.0f, 0.0f), vec2(1.0f, 1.0f), BottomColor),
	Vertex(vec3( CubeHalfSize,-CubeHalfSize,-CubeHalfSize), vec3( 0.0f,-1.0f, 0.0f), vec2(1.0f, 0.0f), BottomColor),

};
static const unsigned int CubeVerticesSize = sizeof(CubeVertices);
static const unsigned int CubeVerticesCount = CubeVerticesSize / sizeof(Vertex);

#endif // __CUBE_MESH_H__

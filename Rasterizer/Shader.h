#ifndef __SHADER_H__
#define __SHADER_H__

#include "Vector.h"
#include "Image.h"

static const unsigned int MaxTextureCount = 4;

struct VertexShaderData {
	vec4 position;
	vec4 normal;
	vec4 color;
	vec2 uv;
	int index;
};

struct PixelShaderData {
	const Image* texture[MaxTextureCount];
	vec4 normal;
	vec4 color;
	vec2 uv;
};

struct Shader {
	void *uniform;
	vec4 *totalVaryingData;
	vec4 *varying;
	unsigned int varyingCount;
	
	Shader();
	
	virtual ~Shader();
	
	void allocVarying(const unsigned int count);
	
	virtual void vertexShader(VertexShaderData& vertexSahderData);
	
	virtual bool pixelShader(PixelShaderData& pixelShaderData);
};

#endif // __SHADER_H__
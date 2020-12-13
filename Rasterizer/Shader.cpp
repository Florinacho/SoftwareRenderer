#include <stdio.h>

#include "Shader.h"

Shader::Shader() {
	uniform = NULL;
	totalVaryingData = NULL;
	varying = NULL;
	varyingCount = 0;
}

Shader::~Shader() {
	if (varying != NULL) {
		delete [] varying;
	}
}
	
void Shader::allocVarying(const unsigned int count) {
	totalVaryingData = new vec4[count * 4];
	varyingCount = count;
}
	
void Shader::vertexShader(VertexShaderData& vertexSahderData) {
}
	
bool Shader::pixelShader(PixelShaderData& pixelShaderData) {
	return true;
}
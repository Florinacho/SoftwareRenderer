#include <stdio.h>
#include <string.h>

#include "Shader.h"

ShaderVariable::ShaderVariable(Type type, const char* nidentifier, DataType ndataType)
	: type(type), dataType(ndataType) {
	strcpy(identifier, nidentifier);
}

const int ShaderVariable::DataTypeSize[] = {
		sizeof(bool),
		sizeof(int),
		sizeof(unsigned int),
		sizeof(float),
		sizeof(double),

		sizeof(int) * 2,
		sizeof(int) * 3,
		sizeof(int) * 4,

		sizeof(unsigned int) * 2,
		sizeof(unsigned int) * 3,
		sizeof(unsigned int) * 4,

		sizeof(float) * 2,
		sizeof(float) * 3,
		sizeof(float) * 4,

		sizeof(double) * 2,
		sizeof(double) * 3,
		sizeof(double) * 4,

		-1, // EDT_MAT2
		-1, // EDT_MAT3
		-1, // EDT_MAT4

		-1, // EDT_OPAQUE

		sizeof(unsigned int),

		-1, // EDT_IMAGE

		-1 //EDT_STRUCT
};

UniformVariable::UniformVariable(const char* identifier, const ShaderVariable::DataType dataType)
	: ShaderVariable(ShaderVariable::ESVT_UNIFORM, identifier, dataType) {
}

int Shader::addUniform(const char* identifier, ShaderVariable::DataType dataType) {
	int location = -1;
	for (unsigned int index = 0; index < uniform.size(); ++index) {
		if (strcmp(uniform[index].identifier, identifier) == 0) {
			if (uniform[index].dataType != dataType) {
				return location;
			}
			return index;
		}
	}
	location = uniform.size();
	uniform.push_back(UniformVariable(identifier, dataType));
	return location;
}

int Shader::getUniformLocation(const char* identifier) const {
	for (unsigned int index = 0; index < uniform.size(); ++index) {
		if (strcmp(uniform[index].identifier, identifier) == 0) {
			return index;
		}
	}
	return -1;
}

int Shader::addVarying(const char* identifier, ShaderVariable::DataType dataType) {
	return -1;
}

int Shader::getVaryingLocation(const char* identifier) const {
	return -1;
}

#ifndef __SHADER_H__
#define __SHADER_H__

#include <vector>

#include "Vector.h"
#include "Image.h"
#include "Matrix4.h"

static const unsigned int MaxTextureCount = 4;

struct VertexShaderData {
	Vector4f position;
	Vector4f normal;
	Vector2f uv;
	Vector4f color;
	Vector4f eye;
	Vector3f lightDirection;
	int index;
	const void* uniform;
};

struct PixelShaderData {
	const Image* texture[MaxTextureCount];
	Vector4f normal;
	Vector2f uv;
	Vector4f color;
//	Vector4f color[RenderTarget::ERT_COUNT];
	Vector4f eye;
	Vector3f lightDirection;
	const void* uniform;
};

struct ShaderVariable {
	enum Type {
		// The declaration is of a compile time constant.
		ESVT_CONST,

		// Global variables that may change per vertex, that are passed from 
		// the OpenGL application to vertex shaders. This qualifier can only be 
		// used in vertex shaders. For the shader this is a read-only variable.
		ESVT_ATTRIBUTE,

		// Global variables that may change per primitive [...], that are 
		// passed from the OpenGL application to the shaders. This qualifier 
		// can be used in both vertex and fragment shaders. For the shaders 
		// this is a read-only variable.
		ESVT_UNIFORM,

		// Used for interpolated data between a vertex shader and a fragment 
		// shader. Available for writing in the vertex shader, and read-only 
		// in a fragment shader.
		ESVT_VARYING
	};

	enum DataType {
		EDT_BOOL,
		EDT_INT,
		EDT_UINT,
		EDT_FLOAT,
		EDT_DOUBLE,

		EDT_IVEC2,
		EDT_IVEC3,
		EDT_IVEC4,

		EDT_UVEC2,
		EDT_UVEC3,
		EDT_UVEC4,

		EDT_VEC2,
		EDT_VEC3,
		EDT_VEC4,

		EDT_DVEC2,
		EDT_DVEC3,
		EDT_DVEC4,

		EDT_MAT2,
		EDT_MAT3,
		EDT_MAT4,

		// Opaque types represent some external object which the shader 
		// references in some fashion. Opaque variables do not have "values" 
		// in the same way as regular types; they are markers that reference 
		// the real data. As such, they can only be used as parameters to 
		// functions. These functions return/modify the actual referenced data. 
		EDT_OPAQUE,

		EDT_SAMPLE,

		EDT_IMAGE,

		EDT_STRUCT
	};

	static const int DataTypeSize[];

	const Type type;
	char identifier[32];
	const DataType dataType;
	
	ShaderVariable(Type type, const char* nidentifier, const DataType ndataType);
};
#include <assert.h>
#include <string.h>
struct UniformVariable : public ShaderVariable {
	unsigned char data[sizeof(float) * 16];

	UniformVariable() 
		: ShaderVariable(ShaderVariable::ESVT_UNIFORM, "no_name", ShaderVariable::EDT_MAT4){
	};

	UniformVariable(const char* identifier, const ShaderVariable::DataType dataType);

	template<typename T>
	UniformVariable& operator = (const T& value) {
		const unsigned int dataTypeSize = ShaderVariable::DataTypeSize[dataType];
		assert(ShaderVariable::DataTypeSize[dataType] >= sizeof(value));
		memcpy(data, &value, sizeof(value));
		return *this;
	}
/*
	template<typename T>
	void set(const T& value) const {
		assert(sizeof(value) <= sizeof(data));

		memcpy(data, &value, sizeof(value));
	}
*/
	template<typename T>
	T get() const {
		assert(sizeof(T) <= sizeof(data));

		T ans;
		memcpy(&ans, data, sizeof(ans));
		return ans;
	}
};

struct VaryingVariable : public ShaderVariable {
};

struct Shader {
	std::vector<UniformVariable> uniform;
	std::vector<VaryingVariable> varying;

	Shader() {
	}

	~Shader() {
	}

	virtual void VertexShader(VertexShaderData& vertex) {
	}

	virtual void FragmentShader(PixelShaderData& pixel) {
	}

	int addUniform(const char* identifier, ShaderVariable::DataType dataType);

	int getUniformLocation(const char* identifier) const;

	int addVarying(const char* identifier, ShaderVariable::DataType dataType);

	int getVaryingLocation(const char* identifier) const;
};

#endif // __SHADER_H__

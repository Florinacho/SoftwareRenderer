#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Vector.h"

struct Light {
	enum Type {
		ELT_POINT,
		ELT_SPOT,
		ELT_DIRECTIONAL
	};

	Type type;
	vec3 position;
	vec3 direction;
	vec4 diffuse;

	Light(Light::Type ntype, const vec4& color)
		: type(ntype), diffuse(color) {
	}
};

#endif //__LIGHT_H__

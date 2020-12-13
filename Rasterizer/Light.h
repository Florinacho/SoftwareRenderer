#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Vector.h"

struct Light {
	enum Type {
		ELT_DIRECTIONAL,
		ELT_SPOT,
		ELT_POINT
	};
	
	int type;
	
	vec4 ambientColor;
	vec4 difuseColor;
	vec4 specularColor;
	vec3 position;
	vec3 direction;
};

#endif // __LIGHT_H__
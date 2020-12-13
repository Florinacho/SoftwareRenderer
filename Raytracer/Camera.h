#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector.h"

struct Camera {
	vec2 position;
	float rotation;
	int fov;
	
	Camera() 
		: rotation(0.0f), fov(75) {
	}
	
	Camera(const vec2& nposition, const float& nrotation)
		: position(nposition), rotation(nrotation) {
	}
};

#endif // __CAMERA_H__
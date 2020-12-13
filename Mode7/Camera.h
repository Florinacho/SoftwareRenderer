#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Frustum.h"

struct Camera {
	Vector2f position;
	float rotation;
	float fov;
	Vector2f depth;
	
	Camera() : rotation(0.0f), fov(0.0f) {
	}
	
	Camera(const Vector2f& nposition, const float& nrotation, const float& nfov, const Vector2f& ndepth)
		: position(nposition), rotation(nrotation), fov(nfov), depth(ndepth) {
	}
	
	Frustum getFrustum() const {
		Frustum ans;
		const float crmf = cosf(rotation - fov);
		const float srmf = sinf(rotation - fov);
		const float crpf = cosf(rotation + fov);
		const float srpf = sinf(rotation + fov);
		ans.LeftFar.x = position.x + crmf * depth.y;
		ans.LeftFar.y = position.y + srmf * depth.y;
		ans.LeftNear.x = position.x + crmf * depth.x;
		ans.LeftNear.y = position.y + srmf * depth.x;
		ans.RightFar.x = position.x + crpf * depth.y;
		ans.RightFar.y = position.y + srpf * depth.y;
		ans.RightNear.x = position.x + crpf * depth.x;
		ans.RightNear.y = position.y + srpf * depth.x;
		return ans;
	}
};

#endif // __CAMERA_H__
#ifndef __CAMERA_H__
#define __CAMERA_H__

struct Camera {
	vec3 position;
	vec3 rotation;
	vec3 scale;

	vec3 target;
	vec3 up;

	float fieldOfView;
	float aspectRatio;
	float zNear;
	float zFar;

	mat4 view;
	mat4 projection;
	mat4 viewProjection;

	bool viewDirty;
	bool projectionDirty;
	bool viewProjectionDirty;

	Camera(float fov = 60.0f, float ar = 1.33f, float znear = 0.1f, float zfar = 100.0f) 
		: fieldOfView(fov), aspectRatio(ar), zNear(znear), zFar(zfar) {
		position = vec3( 0.0f, 0.0f, 0.0f);
		rotation = vec3( 0.0f, 0.0f, 0.0f);
		scale    = vec3( 1.0f, 1.0f, 1.0f);
		target   = vec3( 0.0f, 0.0f, 0.0f);
		up       = vec3( 0.0f, 1.0f, 0.0f);
		
		view.setIdentity();
		projection.setIdentity();
		viewProjection.setIdentity();

		viewDirty = true;
		projectionDirty = true;
		viewProjectionDirty = true;
	}

	void update() {
		if (viewDirty == true) {
			view.setCameraLookAtTransformation(position, target, up);
			viewDirty = false;
			viewProjectionDirty = true;
		}

		if (projectionDirty == true) {
			projection.setPerspective(fieldOfView, aspectRatio, zNear, zFar);
			projectionDirty = false;
			viewProjectionDirty = true;
		}

		if (viewProjectionDirty == true) {
			viewProjection = projection * view;
			viewProjectionDirty = false;
		}
	}
};

#endif //__CAMERA_H__

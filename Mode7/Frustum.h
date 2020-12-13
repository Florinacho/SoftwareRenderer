#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

struct Frustum {
	Vector2f LeftFar;
	Vector2f LeftNear;
	Vector2f RightFar;
	Vector2f RightNear;

	Vector2f normals[4];

	void calculateNormals() {
		Vector2f diff;
		diff = LeftFar - LeftNear;
		normals[0] = Vector2f(-diff.y, diff.x);
		diff = LeftNear - RightNear;
		normals[1] = Vector2f(-diff.y, diff.x);
		diff = RightNear - RightFar;
		normals[2] = Vector2f(-diff.y, diff.x);
		diff = RightFar - LeftFar;
		normals[3] = Vector2f(-diff.y, diff.x);
	}

	void draw(Image* image, const Vector2f& size, const Vector4f& color) {
		image->drawLine(
				LeftFar * size, 
				LeftNear * size, 
				color);
		image->drawLine(
				((LeftFar + LeftNear) / 2.0f) * size,
				((LeftFar + LeftNear) / 2.0f + normals[0]) * size, 
				color);
		image->drawLine(
				RightFar * size, 
				RightNear * size, 
				color);
		image->drawLine(
				((RightFar + RightNear) / 2.0f) * size,
				((RightFar + RightNear) / 2.0f + normals[2]) * size, 
				color);
		image->drawLine(
				LeftFar * size,
				RightFar * size, 
				color);
		image->drawLine(
				((LeftFar + RightFar) / 2.0f) * size,
				((LeftFar + RightFar) / 2.0f + normals[3]) * size, 
				color);
	}

	float isPointInside(const Vector2f& point) {
		Vector2f cp1 = LeftFar - point;
		return 0.0f;
	}
};

#endif // __FRUSTUM_H__
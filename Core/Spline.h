#ifndef __SPLINE_H__
#define __SPLINE_H__

#include <vector>
#include "Vector.h"

template <typename T>
struct Spline {
	std::vector<T> path;
	bool loop;
	Spline() 
		: loop(false) {
	}
	const T getSplinePoint(float t) const {
		int p0, p1, p2, p3;
		if (loop) {
			p1 = (int)t;
			p2 = (p1 + 1 ) % path.size();
			p3 = (p2 + 1 ) % path.size();
			p0 = p1 >= 1 ? p1 -1 : path.size() - 1;
		} else {
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		t = t - (int)t;
		const float tt = t * t;
		const float ttt = tt * t;
		const float q1 = -ttt + 2.0f * tt - t;
		const float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
		const float q3 = -3.0f * ttt + 4.0f * tt + t;
		const float q4 = ttt - tt;
		return (path[p0] * q1 + path[p1] * q2 + path[p2] * q3 + path[p3] * q4) * 0.5f;
	}
	const T getSplineGradient(float t) const {
		int p0, p1, p2, p3;
		if (loop) {
			p1 = (int)t;
			p2 = (p1 + 1 ) % path.size();
			p3 = (p2 + 1 ) % path.size();
			p0 = p1 >= 1 ? p1 -1 : path.size() - 1;
		} else {
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		t = t - (int)t;
		const float tt = t * t;
		const float ttt = tt * t;
		const float q1 = -3.0f * tt + 4.0f*t - 1;
		const float q2 = 9.0f*tt - 10.0f*t;
		const float q3 = -9.0f*tt + 8.0f*t + 1.0f;
		const float q4 = 3.0f*tt - 2.0f*t;
		return path[p0] * q1 + path[p1] * q2 + path[p2] * q3 + path[p3] * q4;
	}
	float getMinT() const {
		return 0.0f;
	}
	float getMaxT() const {
		return loop ? (float)path.size() : (float)path.size() - 3.0f;
	}
};
typedef Spline<Vector2f> Spline2f;
typedef Spline<Vector3f> Spline3f;
typedef Spline<Vector4f> Spline4f;

#endif //__SPLINE_H__

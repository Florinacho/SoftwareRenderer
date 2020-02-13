#ifndef __MATRIX_4_H__
#define __MATRIX_4_H__

#include "Vector.h"

template <typename T>
inline void swap(T& a, T& b) {
	const T c = a;
	a = b;
	b = c;
}

/*
Rotation: counter clockwise
		     ^ y+
		     |   / z-
		     |  /
		     | /
		     |/
	  -x --------o--------> x+
		    /|
		   / |
		  /  |
	      +z /   | y+

Layout: Column-major
	0	4	8	12
	1	5	9	13
	2	6	10	14
	3	7	11	15
*/

static float deg2rad(float degreeAngle) {
	return degreeAngle * M_PI / 180.0f;
}

static float rad2deg(float radianAngle) {
	return radianAngle * 180.0f / M_PI;
}

template <typename T>
class Matrix4 {
	T m[16];

public:
	Matrix4<T>() {
	}

	Matrix4<T>(
		const T  m0, const T  m4, const T  m8, const T m12, 
		const T  m1, const T  m5, const T  m9, const T m13,
		const T  m2, const T  m6, const T m10, const T m14,
		const T  m3, const T  m7, const T m11, const T m15) {

		m[ 0] = m0;
		m[ 1] = m1;
		m[ 2] = m2;
		m[ 3] = m3;
		m[ 4] = m4;
		m[ 5] = m5;
		m[ 6] = m6;
		m[ 7] = m7;
		m[ 8] = m8;
		m[ 9] = m9;
		m[10] = m10;
		m[11] = m11;
		m[12] = m12;
		m[13] = m13;
		m[14] = m14;
		m[15] = m15;
	}

	Matrix4<T>(const T src[16]) {
		m[ 0] = src[ 0];
		m[ 1] = src[ 1];
		m[ 2] = src[ 2];
		m[ 3] = src[ 3];
		m[ 4] = src[ 4];
		m[ 5] = src[ 5];
		m[ 6] = src[ 6];
		m[ 7] = src[ 7];
		m[ 8] = src[ 8];
		m[ 9] = src[ 9];
		m[10] = src[10];
		m[11] = src[11];
		m[12] = src[12];
		m[13] = src[13];
		m[14] = src[14];
		m[15] = src[15];
	}

	Matrix4<T>(const Matrix4<T>& other) {
		m[ 0] = other.m[ 0];
		m[ 1] = other.m[ 1];
		m[ 2] = other.m[ 2];
		m[ 3] = other.m[ 3];
		m[ 4] = other.m[ 4];
		m[ 5] = other.m[ 5];
		m[ 6] = other.m[ 6];
		m[ 7] = other.m[ 7];
		m[ 8] = other.m[ 8];
		m[ 9] = other.m[ 9];
		m[10] = other.m[10];
		m[11] = other.m[11];
		m[12] = other.m[12];
		m[13] = other.m[13];
		m[14] = other.m[14];
		m[15] = other.m[15];
	}

	void setIdentity() {
		m[ 0] = (T)1;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;
		m[ 4] = (T)0;
		m[ 5] = (T)1;
		m[ 6] = (T)0;
		m[ 7] = (T)0;
		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] = (T)1;
		m[11] = (T)0;
		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;
	}

	static const Matrix4<T>& GetIdentity() {
		static const Matrix4<T> identity(
			(T)1, (T)0, (T)0, (T)0, 
			(T)0, (T)1, (T)0, (T)0, 
			(T)0, (T)0, (T)1, (T)0, 
			(T)0, (T)0, (T)0, (T)1);
		return identity;
	}

	void set(const T src[16]) {
		m[ 0] = src[ 0];
		m[ 1] = src[ 1];
		m[ 2] = src[ 2];
		m[ 3] = src[ 3];
		m[ 4] = src[ 4];
		m[ 5] = src[ 5];
		m[ 6] = src[ 6];
		m[ 7] = src[ 7];
		m[ 8] = src[ 8];
		m[ 9] = src[ 9];
		m[10] = src[10];
		m[11] = src[11];
		m[12] = src[12];
		m[13] = src[13];
		m[14] = src[14];
		m[15] = src[15];
	}

	void transpose() {
		swap(m[ 1], m[ 4]);
		swap(m[ 2], m[ 8]);
		swap(m[ 3], m[12]);
		swap(m[ 6], m[ 9]);
		swap(m[ 7], m[13]);
		swap(m[11], m[14]);
	}

	Matrix4<T> GetTranspose() const {
		Matrix4<T> ans = *this;
		ans.transpose();
		return ans;
	}
	
	bool invert() {
		T inv[16];

		inv[ 0] = m[ 5] * m[10] * m[15] - m[ 5] * m[11] * m[14] - m[ 9] * m[ 6] * m[15] + 
			  m[ 9] * m[ 7] * m[14] + m[13] * m[ 6] * m[11] - m[13] * m[ 7] * m[10];
		inv[ 4] =-m[ 4] * m[10] * m[15] + m[ 4] * m[11] * m[14] + m[ 8] * m[ 6] * m[15] - 
			  m[ 8] * m[ 7] * m[14] - m[12] * m[ 6] * m[11] + m[12] * m[ 7] * m[10];
		inv[ 8] = m[ 4] * m[ 9] * m[15] - m[4]  * m[11] * m[13] - m[ 8] * m[ 5] * m[15] + 
			  m[ 8] * m[ 7] * m[13] + m[12] * m[ 5] * m[11] - m[12] * m[ 7] * m[ 9];
		inv[12] =-m[ 4] * m[ 9] * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[ 5] * m[14] - 
			  m[ 8] * m[ 6] * m[13] - m[12] * m[ 5] * m[10] + m[12] * m[ 6] * m[ 9];
		inv[ 1] =-m[ 1] * m[10] * m[15] + m[ 1] * m[11] * m[14] + m[ 9] * m[ 2] * m[15] - 
			  m[ 9] * m[ 3] * m[14] - m[13] * m[ 2] * m[11] + m[13] * m[ 3] * m[10];
		inv[ 5] = m[ 0] * m[10] * m[15] - m[ 0] * m[11] * m[14] - m[ 8] * m[ 2] * m[15] + 
			  m[ 8] * m[ 3] * m[14] + m[12] * m[ 2] * m[11] - m[12] * m[ 3] * m[10];
		inv[ 9] =-m[ 0] * m[ 9] * m[15] + m[ 0] * m[11] * m[13] + m[ 8] * m[ 1] * m[15] - 
			  m[ 8] * m[ 3] * m[13] - m[12] * m[ 1] * m[11] + m[12] * m[ 3] * m[ 9];
		inv[13] = m[ 0] * m[ 9] * m[14] - m[ 0] * m[10] * m[13] - m[ 8] * m[ 1] * m[14] + 
			  m[ 8] * m[ 2] * m[13] + m[12] * m[ 1] * m[10] - m[12] * m[ 2] * m[ 9];
		inv[ 2] = m[ 1] * m[ 6] * m[15] - m[ 1] * m[ 7] * m[14] - m[ 5] * m[ 2] * m[15] + 
			  m[ 5] * m[ 3] * m[14] + m[13] * m[ 2] * m[ 7] - m[13] * m[ 3] * m[ 6];
		inv[ 6] =-m[ 0] * m[ 6] * m[15] + m[ 0] * m[ 7] * m[14] + m[ 4] * m[ 2] * m[15] - 
			  m[ 4] * m[ 3] * m[14] - m[12] * m[ 2] * m[ 7] + m[12] * m[ 3] * m[6];
		inv[10] = m[ 0] * m[ 5] * m[15] - m[ 0] * m[ 7] * m[13] - m[ 4] * m[ 1] * m[15] + 
			  m[ 4] * m[ 3] * m[13] + m[12] * m[ 1] * m[ 7] - m[12] * m[ 3] * m[ 5];
		inv[14] =-m[ 0] * m[ 5] * m[14] + m[ 0] * m[ 6] * m[13] + m[ 4] * m[ 1] * m[14] - 
			  m[ 4] * m[ 2] * m[13] - m[12] * m[ 1] * m[ 6] + m[12] * m[ 2] * m[ 5];
		inv[ 3] =-m[ 1] * m[ 6] * m[11] + m[ 1] * m[ 7] * m[10] + m[ 5] * m[ 2] * m[11] - 
			  m[ 5] * m[ 3] * m[10] - m[ 9] * m[ 2] * m[ 7] + m[ 9] * m[ 3] * m[ 6];
		inv[ 7] = m[ 0] * m[ 6] * m[11] - m[ 0] * m[ 7] * m[10] - m[ 4] * m[ 2] * m[11] + 
			  m[ 4] * m[ 3] * m[10] + m[ 8] * m[ 2] * m[ 7] - m[ 8] * m[ 3] * m[ 6];
		inv[11] =-m[ 0] * m[ 5] * m[11] + m[ 0] * m[ 7] * m[ 9] + m[ 4] * m[ 1] * m[11] - 
			  m[ 4] * m[ 3] * m[ 9] - m[ 8] * m[ 1] * m[ 7] + m[ 8] * m[ 3] * m[ 5];
		inv[15] = m[ 0] * m[ 5] * m[10] - m[ 0] * m[ 6] * m[ 9] - m[ 4] * m[ 1] * m[10] + 
			  m[ 4] * m[ 2] * m[ 9] + m[ 8] * m[ 1] * m[ 6] - m[ 8] * m[ 2] * m[ 5];
		T det = m[ 0] * inv[ 0] + m[ 1] * inv[ 4] + m[ 2] * inv[ 8] + m[ 3] * inv[12];

		if (det == 0) {
			return false;
		}

		det = (T)1.0 / det;

		for (int i = 0; i < 16; ++i) {
			m[i] = inv[i] * det;
		}

		return true;	
	}

	Matrix4<T> getInverse() const {
		Matrix4<T> ans = *this;
		ans.invert();
		return ans;
	}
	
	bool operator == (const Matrix4<T>& other) {
		return (m[ 0] == other.m[ 0] && m[ 1] == other.m[ 1] && m[ 2] == other.m[ 2] && m[ 3] == other.m[ 3]
			&& m[ 4] == other.m[ 4] && m[ 5] == other.m[ 5] && m[ 6] == other.m[ 6] && m[ 7] == other.m[ 7]
			&& m[ 8] == other.m[ 8] && m[ 9] == other.m[ 9] && m[10] == other.m[10] && m[11] == other.m[11]
			&& m[12] == other.m[12] && m[13] == other.m[13] && m[14] == other.m[14] && m[15] == other.m[15]);
	}

	Matrix4<T> operator + (const Matrix4<T>& other) const {
		return Matrix4(
			m[ 0]+other.m[ 0], m[ 1]+other.m[ 1], m[ 2]+other.m[ 2], m[ 3]+other.m[ 3],
			m[ 4]+other.m[ 4], m[ 5]+other.m[ 5], m[ 6]+other.m[ 6], m[ 7]+other.m[ 7],
			m[ 8]+other.m[ 8], m[ 9]+other.m[ 9], m[10]+other.m[10], m[11]+other.m[11],
			m[12]+other.m[12], m[13]+other.m[13], m[14]+other.m[14], m[15]+other.m[15]);
	}

	Matrix4<T> operator - (const Matrix4<T>& other) const {
		return Matrix4(
			m[ 0] - other[ 0], m[ 1] - other[ 1], m[ 2] - other[ 2], m[ 3] - other[ 3],
			m[ 4] - other[ 4], m[ 5] - other[ 5], m[ 6] - other[ 6], m[ 7] - other[ 7],
			m[ 8] - other[ 8], m[ 9] - other[ 9], m[10] - other[10], m[11] - other[11],
			m[12] - other[12], m[13] - other[13], m[14] - other[14], m[15] - other[15]);
	}

	Vector3<T> operator * (const Vector3<T>& other) const {
		return Vector3<T>(
			m[ 0] * other.x + m[ 4] * other.y + m[ 8] * other.z + m[12],
			m[ 1] * other.x + m[ 5] * other.y + m[ 9] * other.z + m[13],
			m[ 2] * other.x + m[ 6] * other.y + m[10] * other.z + m[14]);
	}

	Vector4<T> operator * (const Vector4<T>& other) const {
		return Vector4<T>(
			m[ 0] * other.x + m[ 4] * other.y + m[ 8] * other.z + m[12] * other.w,
			m[ 1] * other.x + m[ 5] * other.y + m[ 9] * other.z + m[13] * other.w,
			m[ 2] * other.x + m[ 6] * other.y + m[10] * other.z + m[14] * other.w,
			m[ 3] * other.x + m[ 7] * other.y + m[11] * other.z + m[15] * other.w);
	}

	Matrix4<T> operator * (const Matrix4<T>& other) const {
		const T ans[16] = {
			m[ 0] * other[ 0] + m[ 4] * other[ 1] + m[ 8] * other[ 2] + m[12] * other[ 3],
			m[ 1] * other[ 0] + m[ 5] * other[ 1] + m[ 9] * other[ 2] + m[13] * other[ 3],
			m[ 2] * other[ 0] + m[ 6] * other[ 1] + m[10] * other[ 2] + m[14] * other[ 3],
			m[ 3] * other[ 0] + m[ 7] * other[ 1] + m[11] * other[ 2] + m[15] * other[ 3],
			m[ 0] * other[ 4] + m[ 4] * other[ 5] + m[ 8] * other[ 6] + m[12] * other[ 7],
			m[ 1] * other[ 4] + m[ 5] * other[ 5] + m[ 9] * other[ 6] + m[13] * other[ 7],
			m[ 2] * other[ 4] + m[ 6] * other[ 5] + m[10] * other[ 6] + m[14] * other[ 7],
			m[ 3] * other[ 4] + m[ 7] * other[ 5] + m[11] * other[ 6] + m[15] * other[ 7],
			m[ 0] * other[ 8] + m[ 4] * other[ 9] + m[ 8] * other[10] + m[12] * other[11],
			m[ 1] * other[ 8] + m[ 5] * other[ 9] + m[ 9] * other[10] + m[13] * other[11],
			m[ 2] * other[ 8] + m[ 6] * other[ 9] + m[10] * other[10] + m[14] * other[11],
			m[ 3] * other[ 8] + m[ 7] * other[ 9] + m[11] * other[10] + m[15] * other[11],
			m[ 0] * other[12] + m[ 4] * other[13] + m[ 8] * other[14] + m[12] * other[15],
			m[ 1] * other[12] + m[ 5] * other[13] + m[ 9] * other[14] + m[13] * other[15],
			m[ 2] * other[12] + m[ 6] * other[13] + m[10] * other[14] + m[14] * other[15],
			m[ 3] * other[12] + m[ 7] * other[13] + m[11] * other[14] + m[15] * other[15]
		};
		return ans;
	}

	const T& operator [] (unsigned int index) const {
		return m[index];
	}

	T& operator [] (unsigned int index) {
		return m[index];
	}

	void setOrthogonal(const float left, const float right, const float top, const float bottom, const float znear, const float zfar) {
		const float rml = right - left;
		const float tmb = top - bottom;
		const float fmn = zfar - znear;

		m[ 0] = (T)2 / rml;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)2 / tmb;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] =-(T)2 / fmn;
		m[11] = (T)0;

		m[12] =-(right + left) / rml;
		m[13] =-(top + bottom) / tmb;
		m[14] =-(zfar + znear) / fmn;
		m[15] = (T)1;
	}

	void setFrustum(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar) {
		const float k0 = 2.0f * zNear;
		const float k1 = right - left;
		const float k2 = top - bottom;
		const float k3 = zFar - zNear;
		m[ 0] = (T)k0 / k1;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)k0 / k2;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)(right + left) / k1;
		m[ 9] = (T)(top + bottom) / k2;
		m[10] =-(T)(zFar + zNear) / k3;
		m[11] =-(T)1;

		m[12] = (T)0;
		m[13] = (T)0;
		m[14] =-(T)(k0 * zFar) / k3;
		m[15] = (T)1;
	}

	void setPerspective(const float fieldOfView, const float aspectRatio, const float zNear, const float zFar) {
		const float ymax = zNear * tanf(fieldOfView * M_PI / 360.0);
		const float xmax = ymax * aspectRatio;
		setFrustum(-xmax, xmax,-ymax, ymax, zNear, zFar);
	}

	void setCameraLookAtTransformation(const Vector3f& position, const Vector3f& target, const Vector3f& up) {
		const Vector3f zaxis = (position - target).normalize();
		const Vector3f xaxis = up.cross(zaxis).normalize();
		const Vector3f yaxis = zaxis.cross(xaxis);

		m[ 0] = (T)xaxis.x;
		m[ 1] = (T)yaxis.x;
		m[ 2] = (T)zaxis.x;
		m[ 3] = (T)0;

		m[ 4] = (T)xaxis.y;
		m[ 5] = (T)yaxis.y;
		m[ 6] = (T)zaxis.y;
		m[ 7] = (T)0;

		m[ 8] = (T)xaxis.z;
		m[ 9] = (T)yaxis.z;
		m[10] = (T)zaxis.z;
		m[11] = (T)0;

		m[12] =-(T)xaxis.dot(position);
		m[13] =-(T)yaxis.dot(position);
		m[14] =-(T)zaxis.dot(position);
		m[15] = (T)1;
	}

	void setViewport(const T x, const T y, const T z, const T w) {
		m[ 0] = (T)(z - x) / (T)2;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)(w - y) / (T)2;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] = (T)(T)1 / (T)2;
		m[11] = (T)0;

		m[12] = (z + x) / 2.0f;
		m[13] = (y + w) / 2.0f;
		m[14] = (T)0.5f;
		m[15] = (T)1;
	}

	void setScale(const Vector3<T>& value) {
		m[ 0] = (T)value.x;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)value.y;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] = (T)value.z;
		m[11] = (T)0;

		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;
	}

	void setTranslation(const Vector4<T>& translation) {
		m[ 0] = (T)1;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)1;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] = (T)1;
		m[11] = (T)0;

		m[12] = (T)translation.x;
		m[13] = (T)translation.y;
		m[14] = (T)translation.z;
		m[15] = (T)translation.w;
	}

	void setRotationX(const float value) {
		const float s = sin(value);
		const float c = cos(value);

		m[ 0] = (T)1;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)c;
		m[ 6] = (T)s;
		m[ 7] = (T)0;

		m[ 8] = (T)0;
		m[ 9] = (T)-s;
		m[10] = (T)c;
		m[11] = (T)0;

		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;
	}

	void setRotationY(const float value) {
		const float s = sin(value);
		const float c = cos(value);

		m[ 0] = (T)c;
		m[ 1] = (T)0;
		m[ 2] = (T)-s;
		m[ 3] = (T)0;

		m[ 4] = (T)0;
		m[ 5] = (T)1;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)s;
		m[ 9] = (T)0;
		m[10] = (T)c;
		m[11] = (T)0;

		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;
	}

	void setRotationZ(const float value) {
		const float s = sin(value);
		const float c = cos(value);

		m[ 0] = (T)c;
		m[ 1] = (T)s;
		m[ 2] = (T)0;
		m[ 3] = (T)0;

		m[ 4] = (T)-s;
		m[ 5] = (T)c;
		m[ 6] = (T)0;
		m[ 7] = (T)0;

		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] = (T)1;
		m[11] = (T)0;

		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;
	}
	//http://www.songho.ca/opengl/gl_anglestoaxes.html
	// This usses the ZXY multiplication order
	void setRotation(const Vector3<T>& value) {
		const float sA = sin(value.x);
		const float cA = cos(value.x);
		const float sB = sin(value.y);
		const float cB = cos(value.y);
		const float sC = sin(value.z);
		const float cC = cos(value.z);

		m[ 0] = cC * cB - sC * sA * sB;
		m[ 1] = sC * cB + cC * sA * sB;
		m[ 2] = -cA * sB ;
		m[ 3] = (T)0;

		m[ 4] =-sC * cA;
		m[ 5] = cC * cA;
		m[ 6] = sA;
		m[ 7] = (T)0;

		m[ 8] = cC * sB + sC * sA * cB;
		m[ 9] = sC * sB - cC * sA * cB;
		m[10] = cA * cB;
		m[11] = (T)0;

		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;
	}

	void setTransformation(const Vector3<T>& position, const Vector3<T>& rotation, const Vector3<T>& scale) {
		const float sA = sin(deg2rad(rotation.x));
		const float cA = cos(deg2rad(rotation.x));
		const float sB = sin(deg2rad(rotation.y));
		const float cB = cos(deg2rad(rotation.y));
		const float sC = sin(deg2rad(rotation.z));
		const float cC = cos(deg2rad(rotation.z));

		m[ 0] = scale.x * (cC * cB - sC * sA * sB);
		m[ 1] = scale.x * (sC * cB + cC * sA * sB);
		m[ 2] = scale.x * (-cA * sB);
		m[ 3] = (T)0;

		m[ 4] = scale.y * (-sC * cA);
		m[ 5] = scale.y * ( cC * cA);
		m[ 6] = scale.y * sA;
		m[ 7] = (T)0;

		m[ 8] = scale.z * (cC * sB + sC * sA * cB);
		m[ 9] = scale.z * (sC * sB - cC * sA * cB);
		m[10] = scale.z * (cA * cB);
		m[11] = (T)0;

		m[12] = (T)position.x;
		m[13] = (T)position.y;
		m[14] = (T)position.z;
		m[15] = (T)1;
	}
};

typedef Matrix4<int> Matrix4i;
typedef Matrix4<unsigned int> Matrix4ui;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;
typedef Matrix4f mat4;

#endif // __MATRIX_H__

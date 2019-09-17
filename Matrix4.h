#ifndef __MATRIX_4_H__
#define __MATRIX_4_H__

#include "Vector.h"

template <typename T>
inline void swap(T& a, T& b) {
	const T c = a;
	a = b;
	b = c;
}

/**
0	4	8	12
1	5	9	13
2	6	10	14
3	7	11	15





Not this
0	1	2	3
4	5	6	7
8	9	10	11
12	13	14	15
/**/

template <typename T>
class Matrix4 {
	T m[16];
public:
	Matrix4<T>() {
	}

	Matrix4<T>(
		const T m0, const T m1, const T m2, const T m3, 
		const T m4, const T m5, const T m6, const T m7,
		const T m8, const T m9, const T m10, const T m11,
		const T m12, const T m13, const T m14, const T m15) {
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
		set(src);
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
		m[ 0] = (T)1.0;
		m[ 1] = (T)0.0;
		m[ 2] = (T)0.0;
		m[ 3] = (T)0.0;
		m[ 4] = (T)0.0;
		m[ 5] = (T)1.0;
		m[ 6] = (T)0.0;
		m[ 7] = (T)0.0;
		m[ 8] = (T)0.0;
		m[ 9] = (T)0.0;
		m[10] = (T)1.0;
		m[11] = (T)0.0;
		m[12] = (T)0.0;
		m[13] = (T)0.0;
		m[14] = (T)0.0;
		m[15] = (T)1.0;
	}

	static const Matrix4<T>& GetIdentity() {
		static const Matrix4<T> identity(
			(T)1.0, (T)0.0, (T)0.0, (T)0.0, 
			(T)0.0, (T)1.0, (T)0.0, (T)0.0, 
			(T)0.0, (T)0.0, (T)1.0, (T)0.0, 
			(T)0.0, (T)0.0, (T)0.0, (T)1.0);
		return identity;
	}

	template <typename U>
	const U* pointer() const {
		return (U*)this;
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
	
	bool invert() {
		T inv[16];

		inv[0] = m[5] * m[10] * m[15] - m[5]  * m[11] * m[14] - m[9]  * m[6]  * m[15] + 
			m[9]  * m[7]  * m[14] + m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];
		inv[4] = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] + m[8]  * m[6]  * m[15] - 
			m[8]  * m[7]  * m[14] - m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];
		inv[8] = m[4]  * m[9] * m[15] - m[4]  * m[11] * m[13] - m[8]  * m[5] * m[15] + 
			m[8]  * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4]  * m[9] * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[5] * m[14] - 
			m[8]  * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] + m[9]  * m[2] * m[15] - 
			m[9]  * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] - m[8]  * m[2] * m[15] + 
			m[8]  * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0]  * m[9] * m[15] + m[0]  * m[11] * m[13] + m[8]  * m[1] * m[15] - 
			m[8]  * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0]  * m[9] * m[14] - m[0]  * m[10] * m[13] - m[8]  * m[1] * m[14] + 
			m[8]  * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1]  * m[6] * m[15] - m[1]  * m[7] * m[14] - m[5]  * m[2] * m[15] + 
			m[5]  * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0]  * m[6] * m[15] + m[0]  * m[7] * m[14] + m[4]  * m[2] * m[15] - 
			m[4]  * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0]  * m[5] * m[15] - m[0]  * m[7] * m[13] - m[4]  * m[1] * m[15] + 
			m[4]  * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0]  * m[5] * m[14] + m[0]  * m[6] * m[13] + m[4]  * m[1] * m[14] - 
			m[4]  * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - 
			m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + 
			m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - 
			m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + 
			m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		T det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0) {
			return false;
		}

		det = (T)1.0 / det;

		for (int i = 0; i < 16; ++i) {
			m[i] = inv[i] * det;
		}

		return true;	
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
			m[0]-other[0],   m[1]-other[1],   m[2]-other[2],   m[3]-other[3],
			m[4]-other[4],   m[5]-other[5],   m[6]-other[6],   m[7]-other[7],
			m[8]-other[8],   m[9]-other[9],   m[10]-other[10], m[11]-other[11],
			m[12]-other[12], m[13]-other[13], m[14]-other[14], m[15]-other[15]);
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
			m[ 0]*other[ 0] + m[ 4]*other[ 1] + m[ 8]*other[ 2] + m[12]*other[ 3],
			m[ 1]*other[ 0] + m[ 5]*other[ 1] + m[ 9]*other[ 2] + m[13]*other[ 3],
			m[ 2]*other[ 0] + m[ 6]*other[ 1] + m[10]*other[ 2] + m[14]*other[ 3],
			m[ 3]*other[ 0] + m[ 7]*other[ 1] + m[11]*other[ 2] + m[15]*other[ 3],
			m[ 0]*other[ 4] + m[ 4]*other[ 5] + m[ 8]*other[ 6] + m[12]*other[ 7],
			m[ 1]*other[ 4] + m[ 5]*other[ 5] + m[ 9]*other[ 6] + m[13]*other[ 7],
			m[ 2]*other[ 4] + m[ 6]*other[ 5] + m[10]*other[ 6] + m[14]*other[ 7],
			m[ 3]*other[ 4] + m[ 7]*other[ 5] + m[11]*other[ 6] + m[15]*other[ 7],
			m[ 0]*other[ 8] + m[ 4]*other[ 9] + m[ 8]*other[10] + m[12]*other[11],
			m[ 1]*other[ 8] + m[ 5]*other[ 9] + m[ 9]*other[10] + m[13]*other[11],
			m[ 2]*other[ 8] + m[ 6]*other[ 9] + m[10]*other[10] + m[14]*other[11],
			m[ 3]*other[ 8] + m[ 7]*other[ 9] + m[11]*other[10] + m[15]*other[11],
			m[ 0]*other[12] + m[ 4]*other[13] + m[ 8]*other[14] + m[12]*other[15],
			m[ 1]*other[12] + m[ 5]*other[13] + m[ 9]*other[14] + m[13]*other[15],
			m[ 2]*other[12] + m[ 6]*other[13] + m[10]*other[14] + m[14]*other[15],
			m[ 3]*other[12] + m[ 7]*other[13] + m[11]*other[14] + m[15]*other[15]
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

		m[ 0] = 2.0f / rml;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0.0f;
		m[ 5] = 2.0f / tmb;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;
		m[ 9] = 0.0f;
		m[10] =-2.0f / fmn;
		m[11] = 0.0f;
		m[12] =-(right + left) / rml;
		m[13] =-(top + bottom) / tmb;
		m[14] =-(zfar + znear) / fmn;
		m[15] = 1.0f;
	}

	void setPerspective2(const float fieldOfView, const float aspectRatio, const float zNear, const float zFar) {
		const float f = 1 / tanf(fieldOfView / 2.0f);
		const float q = zFar / (zFar - zNear);
		m[ 0] = aspectRatio * f;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0.0f;
		m[ 5] = f;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;
		m[ 9] = 0.0f;
		m[10] = q;
		m[11] = - zNear * q;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 1.0f;
		m[15] = 0.0f;
	}

	void setFrustum(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar) {
		const float k0 = 2.0f * zNear;
		const float k1 = right - left;
		const float k2 = top - bottom;
		const float k3 = zFar - zNear;
		m[ 0] = k0 / k1;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0.0f;
		m[ 5] = k0 / k2;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = (right + left) / k1;
		m[ 9] = (top + bottom) / k2;
		m[10] =-(zFar + zNear) / k3;
		m[11] =-1.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] =-(k0 * zFar) / k3;
		m[15] = 1.0f;
	}

	// Right handed
	void setPerspective(const float fieldOfView, const float aspectRatio, const float zNear, const float zFar) {
		const float ymax = zNear * tanf(fieldOfView * M_PI / 360.0);
		const float xmax = ymax * aspectRatio;
		setFrustum(-xmax, xmax,-ymax, ymax, zNear, zFar);
	}

	void setCameraLookAtTransformation(const Vector3f& position, const Vector3f& target, const Vector3f& up) {
		const Vector3f zaxis = (position - target).normalize();
		const Vector3f xaxis = up.cross(zaxis).normalize();
		const Vector3f yaxis = zaxis.cross(xaxis);

		m[ 0] = xaxis.x;
		m[ 1] = yaxis.x;
		m[ 2] = zaxis.x;
		m[ 3] = 0.0f;
		m[ 4] = xaxis.y;
		m[ 5] = yaxis.y;
		m[ 6] = zaxis.y;
		m[ 7] = 0.0f;
		m[ 8] = xaxis.z;
		m[ 9] = yaxis.z;
		m[10] = zaxis.z;
		m[11] = 0.0f;
		m[12] = -xaxis.dot(position);
		m[13] = -yaxis.dot(position);
		m[14] = -zaxis.dot(position);
		m[15] = 1.0f;
	}

	void setTranslation(const Vector2f& translation) {
		m[ 0] = (T)1.0;
		m[ 1] = (T)0.0;
		m[ 2] = (T)0.0;
		m[ 3] = (T)0.0;
		m[ 4] = (T)0.0;
		m[ 5] = (T)1.0;
		m[ 6] = (T)0.0;
		m[ 7] = (T)0.0;
		m[ 8] = (T)0.0;
		m[ 9] = (T)0.0;
		m[10] = (T)1.0;
		m[11] = (T)0.0;
		m[12] = (T)translation.x;
		m[13] = (T)translation.y;
//		m[14] = (T)translation.z;
		m[14] = (T)0.0;
		m[15] = (T)1.0;
	}

	void setTransformation(const Vector3f& translation, const Vector3f& rotation) {
		static const float DEGTORAD = M_PI / 180.0f;
		const T cx = cos(rotation.x * DEGTORAD);
		const T sx = sin(rotation.x * DEGTORAD);
		const T cy = cos(rotation.y * DEGTORAD);
		const T sy = sin(rotation.y * DEGTORAD);
		const T cz = cos(rotation.z * DEGTORAD);
		const T sz = sin(rotation.z * DEGTORAD);

		m[ 0] = cz * cy;
		m[ 1] = -sz;
		m[ 2] = cz * sy;
		m[ 3] = 0.0f;
		m[ 4] = cx * sz * cy + sx * sy;
		m[ 5] = cx * cz;
		m[ 6] = cx * sz * sy - sx * cy;
		m[ 7] = 0.0f;
		m[ 8] = sx * sz * cy - cx * sy;
		m[ 9] = sx * cz;
		m[10] = sx * sz * sy + cx * cy;
		m[11] = 0.0f;
		m[12] = translation.x;
		m[13] = translation.y;
		m[14] = translation.z;
		m[15] = 1.0f;
	}

	void setTransformation(const Vector3f& translation, const Vector3f& rotation, const Vector3f& scale) {
		static const float DEGTORAD = M_PI / 180.0f;
		const T cx = cos(rotation.x * DEGTORAD);
		const T sx = sin(rotation.x * DEGTORAD);
		const T cy = cos(rotation.y * DEGTORAD);
		const T sy = sin(rotation.y * DEGTORAD);
		const T cz = cos(rotation.z * DEGTORAD);
		const T sz = sin(rotation.z * DEGTORAD);

		m[ 0] = cz*cy*scale.x;
		m[ 1] = -sz*scale.x;
		m[ 2] = cz*sy*scale.x;
		m[ 3] = 0.0f;
		m[ 4] = cx*sz*cy+sx*sy*scale.y;
		m[ 5] = cx*cz*scale.y;
		m[ 6] = cx*sz*sy-sx*cy*scale.y;
		m[ 7] = 0.0f;
		m[ 8] = sx*sz*cy-cx*sy*scale.z;
		m[ 9] = sx*cz*scale.z;
		m[10] = sx*sz*sy+cx*cy*scale.z;
		m[11] = 0.0f;
		m[12] = translation.x;
		m[13] = translation.y;
		m[14] = translation.z;
		m[15] = 1.0f;
	}

	void setTransformation(float degree, const Vector2f& center, const Vector2f& translate, const Vector2f& scale) {
		static const float DEGTORAD = M_PI / 180.0f;
		const T c = cos(DEGTORAD * degree);
		const T s = sin(DEGTORAD * degree);

		m[ 0] = (T)(c * scale.x);
		m[ 1] = (T)(s * scale.y);
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;

		m[ 4] = (T)(-s * scale.x);
		m[ 5] = (T)( c * scale.y);
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;

		m[ 8] = (T)( c * scale.x * center.x + -s * center.y + translate.x);
		m[ 9] = (T)( s * scale.y * center.x +  c * center.y + translate.y);
		m[10] = 1.0f;
		m[11] = 0.0f;

		m[12] = translate.x;
		m[13] = translate.y;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	void setViewport(const T x, const T y, const T z, const T w) {
		m[ 0] = (z - x) / (T)2;
		m[ 1] = (T)0;
		m[ 2] = (T)0;
		m[ 3] = (T)0;
		m[ 4] = (T)0;
		m[ 5] = (w - y) / (T)2;
		m[ 6] = (T)0;
		m[ 7] = (T)0;
		m[ 8] = (T)0;
		m[ 9] = (T)0;
		m[10] = (T)1 / (T)2;
		m[11] = (T)0;
		m[12] = (T)0;
		m[13] = (T)0;
		m[14] = (T)0;
		m[15] = (T)1;

		m[12] = (z + x) / 2.0f;
		m[13] = (y + w) / 2.0f;
		m[14] = 0.5f;
	}
};

typedef Matrix4<int> Matrix4i;
typedef Matrix4<unsigned int> Matrix4ui;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

typedef Matrix4f mat4;

#endif // __MATRIX_H__

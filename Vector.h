#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

template <typename T>
struct Vector2 {
	T x, y;
	
	Vector2<T>(T nx = (T)0, T ny = (T)0)
		: x(nx), y(ny) {
	}

	Vector2<T>& operator = (const Vector2<T>& other) {
		x = other.x;
		y = other.y;
		return *this;
	}
	
	Vector2<T> operator - (const Vector2<T>& other) {
		return Vector2<T>(x - other.x, y - other.y);
	}
	
	Vector2<T> operator + (const Vector2<T>& other) {
		return Vector2<T>(x + other.x, y + other.y);
	}
	
	Vector2<T> operator * (const T value) const {
		return Vector2<T>(x * value, y * value);
	}
};

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float> Vector2f;

typedef Vector2f vec2f;
typedef Vector2i vec2i;
typedef Vector2u vec2u;
typedef vec2f vec2;

template <typename T>
struct Vector3 {
	T x, y, z;
	
	Vector3<T>(T nx = (T)0, T ny = (T)0, T nz = (T)0)
		: x(nx), y(ny), z(nz){
	}

	Vector3<T>& operator = (const Vector3<T>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector3<T> operator + (const Vector3<T>& other) const {
		return Vector3<T>(x + other.x, y + other.y, z + other.z);
	}
	
	Vector3<T> operator - (const Vector3<T>& other) const {
		return Vector3<T>(x - other.x, y - other.y, z - other.z);
	}
	
	Vector3<T> operator * (const Vector3<T>& other) const {
		return Vector3<T>(x * other.x, y * other.y, z * other.z);
	}
	
	Vector3<T> operator * (const T value) const {
		return Vector3<T>(x * value, y * value, z * value);
	}

	Vector3<T> operator /= (const T value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}
	
	Vector3<T> lerp(const Vector3<T>& other, float value) const {
		Vector3<T> ans;
		const float inv = 1.0f - value;
		ans.x = x * value + other.x * inv;
		ans.y = y * value + other.y * inv;
		ans.z = z * value + other.z * inv;
		return ans;
	}
	
	Vector3<T> cross(const Vector3<T>& other) const {
		return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	T dot(const Vector3<T>& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Vector3<T>& normalize() {
		float length = x * x + y * y + z * z;
		if (length == 0 )
			return *this;

		length = 1.0f / sqrt(length);

		x = (T)(x * length);
		y = (T)(y * length);
		z = (T)(z * length);
		
		return *this;
	}
};
typedef Vector3<float> Vector3f;
typedef Vector3f vec3;

template <typename T>
struct Vector4 {
	T x, y, z, w;
	
	Vector4<T>(T nx = (T)0, T ny = (T)0, T nz = (T)0, T nw = (T)0)
		: x(nx), y(ny), z(nz), w(nw) {
	}

	Vector4<T> operator + (const Vector4<T>& other) const {
		return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	
	Vector4<T> operator - (const Vector4<T>& other) const {
		return Vector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	
	Vector4<T> operator * (const Vector4<T>& other) const {
		return Vector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
	}
	
	Vector4<T> operator / (const Vector4<T>& other) const {
		return Vector4<T>(x / other.x, y / other.y, z / other.z, w / other.w);
	}
	
	Vector4<T> operator / (const T value) const {
		return Vector4<T>(x / value, y / value, z / value, w / value);
	}
	
	Vector4<T> operator * (const T value) const {
		return Vector4<T>(x * value, y * value, z * value, w * value);
	}
	
	Vector4<T>& operator += (const Vector4<T>& other) {
		x += other.x;
		y +=other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	
	Vector4<T> lerp(const Vector4<T>& other, float value) const {
		Vector4<T> ans;
		const float inv = 1.0f - value;
		ans.x = x * value + other.x * inv;
		ans.y = y * value + other.y * inv;
		ans.z = z * value + other.z * inv;
		ans.w = w * value + other.w * inv;
		return ans;
	}

	Vector4<T>& operator = (const Vector4<T>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
};

typedef Vector4<float> Vector4f;
typedef Vector4f vec4;
	
#endif //__VECTOR_H__

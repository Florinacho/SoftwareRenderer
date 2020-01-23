#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

template <typename T>
struct Vector2 {
	T x, y;
	
	Vector2<T>(T nx = (T)0, T ny = (T)0)
		: x(nx), y(ny) {
	}

	Vector2<T>(const Vector2<T>& other) 
		: x(other.x), y(other.y) {
	}

	Vector2<T>& operator = (const T value) {
		x = value;
		y = value;
		return *this;
	}

	Vector2<T>& operator = (const Vector2<T>& other) {
		x = other.x;
		y = other.y;
		return *this;
	}

	bool operator == (const T value) const {
		return ((x == value) && (y == value));
	}

	bool operator != (const T value) const {
		return ((x != value) || (y != value));
	}
	
	Vector2<T> operator - () const {
		return Vector2<T>(-x, -y);
	}

	Vector2<T> operator + (const T value) const {
		return Vector2<T>(x + value, y + value);
	}

	Vector2<T> operator - (const T value) const {
		return Vector2<T>(x - value, y - value);
	}

	Vector2<T> operator * (const T value) const {
		return Vector2<T>(x * value, y * value);
	}

	Vector2<T> operator / (const T value) const {
		return Vector2<T>(x / value, y / value);
	}

	Vector2<T> operator + (const Vector2<T>& other) const {
		return Vector2<T>(x + other.x, y + other.y);
	}

	Vector2<T> operator - (const Vector2<T>& other) const {
		return Vector2<T>(x - other.x, y - other.y);
	}

	Vector2<T> operator * (const Vector2<T>& other) const {
		return Vector2<T>(x * other.x, y * other.y);
	}

	Vector2<T> operator / (const Vector2<T>& other) const {
		return Vector2<T>(x / other.x, y / other.y);
	}

	Vector2<T>& operator += (const T value) {
		x += value;
		y += value;
		return *this;
	}

	Vector2<T>& operator -= (const T value) {
		x -= value;
		y -= value;
		return *this;
	}

	Vector2<T>& operator *= (const T value) {
		x *= value;
		y *= value;
		return *this;
	}

	Vector2<T>& operator /= (const T value) {
		x /= value;
		y /= value;
		return *this;
	}

	Vector2<T>& operator += (const Vector2<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2<T>& operator -= (const Vector2<T>& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2<T>& operator *= (const Vector2<T>& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2<T>& operator /= (const Vector2<T>& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vector2<T>& normalize() {
		const float length = x * x + y * y;
		if (length == 0 ) {
			return *this;
		}

		length = 1.0f / sqrt(length);

		x = (T)(x * length);
		y = (T)(y * length);
		
		return *this;
	}

	Vector2<T> lerp(const Vector2<T>& other, float value) const {
		Vector2<T> ans;
		const float inv = 1.0f - value;
		ans.x = x * value + other.x * inv;
		ans.y = y * value + other.y * inv;
		return ans;
	}

	T dot(const Vector2<T>& other) const {
		return x * other.x + y * other.y;
	}

	void rotate(const float angle, const Vector2<T>& center) {
		const float s = sinf(angle);
		const float c = cosf(angle);

		x -= center.x;
		y -= center.y;

		x = x * c - y * s;
		y = x * s + y * c;

		x += center.x;
		x += center.y;
	}
};

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float> Vector2f;

typedef Vector2f vec2;

template <typename T>
struct Vector3 {
	T x, y, z;
	
	Vector3<T>(T nx = (T)0, T ny = (T)0, T nz = (T)0)
		: x(nx), y(ny), z(nz){
	}

	Vector3<T>(const Vector2<T>& xy, const T otherZ)
		: x(xy.x), y(xy.y), z(otherZ) {
	}

	Vector3<T> operator + (const T value) const {
		return Vector3<T>(x + value, y + value, z + value);
	}

	Vector3<T> operator - (const T value) const {
		return Vector3<T>(x - value, y - value, z - value);
	}

	Vector3<T> operator * (const T value) const {
		return Vector3<T>(x * value, y * value, z * value);
	}

	Vector3<T> operator / (const T value) const {
		return Vector3<T>(x / value, y / value, z / value);
	}

	Vector3<T>& operator = (const T value) {
		x = value;
		y = value;
		z = value;
		return *this;
	}

	bool operator == (const T value) const {
		return ((x == value) && (y == value) && (z == value));
	}

	bool operator != (const T value) const {
		return ((x != value) || (y != value) || (z != value));
	}

	Vector3<T>& operator += (const T value) {
		x += value;
		y += value;
		z += value;
		return *this;
	}

	Vector3<T>& operator -= (const T value) {
		x -= value;
		y -= value;
		z -= value;
		return *this;
	}

	Vector3<T>& operator *= (const T value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3<T>& operator /= (const T value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	Vector3<T>& operator = (const Vector2<T>& other) {
		x = other.x;
		y = other.y;
		z = (T)0;
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
	
	Vector3<T> operator += (const Vector3<T>& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3<T> operator -= (const Vector3<T>& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3<T> operator *= (const Vector3<T>& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vector3<T> operator /= (const Vector3<T>& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
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

	void calculateNormal(const Vector3<T>& p1, const Vector3<T>& p2, const Vector3<T>& p3) {
		const Vector3<T> u = p2 - p1;
		const Vector3<T> v = p3 - p1;
		x = u.y * v.z - u.z * v.y;
		y = u.z * v.x - u.x * v.z;
		x = u.x * v.y - u.y * v.x;
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

	Vector4<T>(const Vector2<T>& other, const T valueZ = (T)0, const T valueW = (T)0)
		: x(other.x), y(other.y), z(valueZ), w(valueW) {
	}

	Vector4<T>(const Vector2<T>& a, const Vector2<T>& b)
		: x(a.x), y(a.y), z(b.x), w(b.y) {
	}

	Vector4<T>(const Vector3<T>& other, const T valueW = (T)0)
		: x(other.x), y(other.y), z(other.z), w(valueW) {
	}

	Vector4<T>& operator = (const T value) {
		x = value;
		y = value;
		z = value;
		w = value;
		return *this;
	}

	Vector4<T> operator + (const T value) const {
		return Vector4<T>(x + value, y + value, z + value, w + value);
	}

	Vector4<T> operator - (const T value) const {
		return Vector4<T>(x - value, y - value, z - value, w - value);
	}

	Vector4<T> operator * (const T value) const {
		return Vector4<T>(x * value, y * value, z * value, w * value);
	}

	Vector4<T> operator / (const T value) const {
		return Vector4<T>(x / value, y / value, z / value, w / value);
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

	bool operator == (const Vector4<T>& other) const {
		return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
	}

	bool operator != (const Vector4<T>& other) const {
		return ((x != other.x) || (y != other.y) || (z != other.z) || (w != other.w));
	}
	
	Vector4<T>& operator += (const Vector4<T>& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	Vector4<T>& operator *= (const Vector4<T>& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
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

	Vector4<T>& operator /= (const T value) {
		x /= value;
		y /= value;
		z /= value;
		w /= value;
		return *this;
	}

	Vector3<T> getVector3() const {
		return Vector3<T>(x, y, z);
	}

	Vector3<T> xyz() const {
		return Vector3<T>(x, y, z);
	}

	T getWidth() const {
		return z - x;
	}

	T getHeight() const {
		return w - y;
	}
};

typedef Vector4<float> Vector4f;
typedef Vector4f vec4;
	
#endif //__VECTOR_H__

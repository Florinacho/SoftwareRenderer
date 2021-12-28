#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

// https://stackoverflow.com/questions/51641131/how-to-achieve-vector-swizzling-in-c
// https://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B
// http://learnwebgl.brown37.net/12_shader_language/glsl_mathematical_operations.html
// http://learnwebgl.brown37.net/12_shader_language/documents/webgl-reference-card-1_0.pdf

template <typename T>
struct Vector2 {
	template<unsigned int A, unsigned int B>
	struct Swizzle {
		union {
			T data[2];
			struct { T x, y; };
			struct { T r, g; };
			struct { T s, t; };
		};

		Vector2<T>::Swizzle<A, B>& operator = (const Vector2<T>& rhs) {
			data[A] = rhs.x;
			data[B] = rhs.y;
			return *this;
		}

		Vector2<T> operator + () const {
			return Vector2<T>(+data[A], +data[B]);
		}

		Vector2<T> operator - () const {
			return Vector2<T>(-data[A], -data[B]);
		}
		
		Vector2<T> operator + (const Vector2<T>& rhs) const {
			return Vector2<T>(data[A] + rhs.x, data[B] + rhs.y);
		}
		
		Vector2<T> operator - (const Vector2<T>& rhs) const {
			return Vector2<T>(data[A] - rhs.x, data[B] - rhs.y);
		}
		
		Vector2<T> operator * (const Vector2<T>& rhs) const {
			return Vector2<T>(data[A] * rhs.x, data[B] * rhs.y);
		}
		
		Vector2<T> operator / (const Vector2<T>& rhs) const {
			return Vector2<T>(data[A] / rhs.x, data[B] / rhs.y);
		}
		
		Vector2<T>::Swizzle<A, B>& operator += (const Vector2<T>& rhs) {
			data[A] += rhs.x;
			data[B] += rhs.y;
			return *this;
		}
		
		Vector2<T>::Swizzle<A, B>& operator -= (const Vector2<T>& rhs) {
			data[A] -= rhs.x;
			data[B] -= rhs.y;
			return *this;
		}
		
		Vector2<T>::Swizzle<A, B>& operator *= (const Vector2<T>& rhs) {
			data[A] *= rhs.x;
			data[B] *= rhs.y;
			return (*this);
		}
		
		Vector2<T>::Swizzle<A, B>& operator /= (const Vector2<T>& rhs) {
			data[A] /= rhs.x;
			data[B] /= rhs.y;
			return *this;
		}
		
		Vector2<T>::Swizzle<A, B>& operator ++ () {
			++data[A];
			++data[B];
			return *this;
		}
		
		Vector2<T> operator ++ (int) {
			const Vector2<T> ans(data[A], data[B]);
			++data[A];
			++data[B];
			return ans;
		}
		
		Vector2<T>::Swizzle<A, B>& operator -- () {
			--data[A];
			--data[B];
			return *this;
		}
		
		Vector2<T> operator -- (int) {
			const Vector2<T> ans(data[A], data[B]);
			--data[A];
			--data[B];
			return ans;
		}
		
		bool operator == (const Vector2<T>& rhs) const {
			return ((data[A] == rhs.x) && (data[B] == rhs.y));
		}
		
		bool operator != (const Vector2<T>& rhs) const {
			return ((data[A] != rhs.x) || (data[B] != rhs.y));
		}

		const T& operator [] (const unsigned int index) const {
			const T* new_data[] = {&data[A], &data[B]};
			return *new_data[index];
		}

		T& operator [] (const unsigned int index) {
			T* new_data[] = {&data[A], &data[B]};
			return *new_data[index];
		}
		
		operator Vector2<T>() const {
			return Vector2<T>(data[A], data[B]);
		}
		
		Vector2<T>::Swizzle<A, B>& normalize() {
			float length = data[A] * data[A] + data[B] * data[B];
			if (length == 0 ) {
				return *this;
			}

			length = 1.0f / sqrt(length);

			data[A] = (T)(data[A] * length);
			data[B] = (T)(data[B] * length);
			
			return *this;
		}
		
		T dot(const Vector2<T>& other) const {
			return data[A] * other.data[A] + data[B] * other.data[B];
		}
		
		double length() const {
			return data[A] * data[A] + data[B] * data[B];
		}
		
		double magnitude() const {
			return sqrt(length());
		}
	}; // Swizzle
	
	typedef Swizzle<0, 0> XX;
	typedef Swizzle<0, 1> XY;
	typedef Swizzle<0, 2> XZ;
	typedef Swizzle<0, 3> XW;
	typedef Swizzle<1, 0> YX;
	typedef Swizzle<1, 1> YY;
	typedef Swizzle<1, 2> YZ;
	typedef Swizzle<1, 3> YW;
	typedef Swizzle<2, 0> ZX;
	typedef Swizzle<2, 1> ZY;
	typedef Swizzle<2, 2> ZZ;
	typedef Swizzle<2, 3> ZW;
	typedef Swizzle<3, 0> WX;
	typedef Swizzle<3, 1> WY;
	typedef Swizzle<3, 2> WZ;
	typedef Swizzle<3, 3> WW;

	union {
		T data[2];
		struct { T x, y; };
		struct { T r, g; };
		struct { T s, t; };

		XX xx;
		XY xy;
		YX yx;
		YY yy;
	};
	
	Vector2(const T value = (T)0) : x(value), y(value) { }
	
	Vector2(const T value0, const T value1) : x(value0), y(value1) { }
	
	Vector2(const Vector2<T>& other) : x(other.x), y(other.y) { }

	Vector2<T>& operator = (const Vector2<T>& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	Vector2<T> operator + () const {
		return Vector2<T>(+x, +y);
	}
	
	Vector2<T> operator - () const {
		return Vector2<T>(-x, -y);
	}

	Vector2<T> operator + (const Vector2<T>& rhs) const {
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}
	
	Vector2<T> operator - (const Vector2<T>& rhs) const {
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}
	
	Vector2<T> operator * (const Vector2<T>& rhs) const {
		return Vector2<T>(x * rhs.x, y * rhs.y);
	}
	
	Vector2<T> operator / (const Vector2<T>& rhs) const {
		return Vector2<T>(x / rhs.x, y / rhs.y);
	}
	
	Vector2<T>& operator += (const Vector2<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	
	Vector2<T>& operator -= (const Vector2<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	
	Vector2<T>& operator *= (const Vector2<T>& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}
	
	Vector2<T>& operator /= (const Vector2<T>& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}
	
	Vector2<T>& operator ++ () {
		++x;
		++y;
		return *this;
	}
	
	Vector2<T> operator ++ (int) {
		return Vector2<T>(x++, y++);
	}

	Vector2<T>& operator -- () {
		--x;
		--y;
		return *this;
	}
	
	Vector2<T> operator -- (int) {
		return Vector2<T>(x--, y--);
	}

	bool operator == (const Vector2<T>& rhs) const {
		return ((x == rhs.x) && (y == rhs.y));
	}
	
	bool operator != (const Vector2<T>& rhs) const {
		return ((x != rhs.x) || (y != rhs.y));
	}

	const T& operator [] (const unsigned int index) const {
		return data[index];
	}

	T& operator [] (const unsigned int index) {
		return data[index];
	}
	
	Vector2<T>& normalize() {
		float length = x * x + y * y;
		if (length == 0 ) {
			return *this;
		}

		length = 1.0f / sqrt(length);

		x = (T)(x * length);
		y = (T)(y * length);
		
		return *this;
	}
	
	T dot(const Vector2<T>& other) const {
		return x * other.x + y * other.y;
	}
	
	T getDistanceFrom(const Vector2<T>& other) const {
		return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
	}
	
	double length() const {
		return x * x + y * y;
	}
	
	double magnitude() const {
		return sqrt(length());
	}
}; // Vector2

template <typename T>
struct Vector3 {
	template<unsigned int A, unsigned int B, unsigned int C>
	struct Swizzle {
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T s, t, p; };
			typename Vector2<T>::XX xx;
			typename Vector2<T>::XY xy;
			typename Vector2<T>::XZ xz;
			typename Vector2<T>::YX yx;
			typename Vector2<T>::YY yy;
			typename Vector2<T>::YZ yz;
			typename Vector2<T>::ZX zx;
			typename Vector2<T>::ZY zy;
			typename Vector2<T>::ZZ zz;
		};

		Vector3<T>::Swizzle<A, B, C>& operator = (const Vector3<T>& rhs) {
			data[A] = rhs.x;
			data[B] = rhs.y;
			data[C] = rhs.z;
			return *this;
		}
		
		Vector3<T> operator + () const {
			return Vector3<T>(+data[A], +data[B], +data[C]);
		}

		Vector3<T> operator - () const {
			return Vector3<T>(-data[A], -data[B], -data[C]);
		}

		Vector3<T> operator + (const Vector3<T>& rhs) const {
			return Vector3<T>(data[A] + rhs.x, data[B] + rhs.y, data[C] + rhs.z);
		}
		
		Vector3<T> operator - (const Vector3<T>& rhs) const {
			return Vector3<T>(data[A] - rhs.x, data[B] - rhs.y, data[C] - rhs.z);
		}
		
		Vector3<T> operator * (const Vector3<T>& rhs) const {
			return Vector3<T>(data[A] * rhs.x, data[B] * rhs.y, data[C] * rhs.z);
		}
		
		Vector3<T> operator / (const Vector3<T>& rhs) const {
			return Vector3<T>(data[A] / rhs.x, data[B] / rhs.y, data[C] / rhs.z);
		}
		
		Vector3<T>::Swizzle<A, B, C>& operator += (const Vector3<T>& rhs) {
			data[A] += rhs.x;
			data[B] += rhs.y;
			data[C] += rhs.z;
			return *this;
		}
		
		Vector3<T>::Swizzle<A, B, C>& operator -= (const Vector3<T>& rhs) {
			data[A] -= rhs.x;
			data[B] -= rhs.y;
			data[C] -= rhs.z;
			return *this;
		}
		
		Vector3<T>::Swizzle<A, B, C>& operator *= (const Vector3<T>& rhs) {
			data[A] *= rhs.x;
			data[B] *= rhs.y;
			data[C] *= rhs.z;
			return *this;
		}
		
		Vector3<T>::Swizzle<A, B, C>& operator /= (const Vector3<T>& rhs) {
			data[A] /= rhs.x;
			data[B] /= rhs.y;
			data[C] /= rhs.z;
			return *this;
		}
		
		Vector3<T>::Swizzle<A, B, C>& operator ++ () {
			++data[A];
			++data[B];
			++data[C];
			return *this;
		}
		
		Vector3<T> operator ++ (int) {
			Vector3<T> ans(data[A], data[B], data[C]);
			++data[A];
			++data[B];
			++data[C];
			return ans;
		}
		
		Vector3<T>::Swizzle<A, B, C>& operator -- () {
			--data[A];
			--data[B];
			--data[C];
			return *this;
		}
		
		Vector3<T> operator -- (int) {
			Vector3<T> ans(data[A], data[B], data[C]);
			--data[A];
			--data[B];
			--data[C];
			return ans;
		}
		
		bool operator == (const Vector3<T>& rhs) const {
			return ((data[A] == rhs.x) &&
					(data[B] == rhs.y) &&
					(data[C] == rhs.z));
		}
		
		bool operator != (const Vector3<T>& rhs) const {
			return ((data[A] != rhs.x) ||
					(data[B] != rhs.y) ||
					(data[C] != rhs.z));
		}

		const T& operator [] (const unsigned int index) const {
			const T* new_data[] = {&data[A], &data[B], &data[C]};
			return new_data[index];
		}

		T& operator [] (const unsigned int index) {
			T* new_data[] = {&data[A], &data[B], &data[C]};
			return *new_data[index];
		}
		
		operator Vector3<T>() const {
			return Vector3<T>(data[A], data[B], data[C]);
		}
		
		Vector3<T>::Swizzle<A, B, C>& normalize() {
			float length = data[A] * data[A] + data[B] * data[B] + data[C] * data[C];
			if (length == 0 )
				return *this;

			length = 1.0f / sqrt(length);

			data[A] = (T)(data[A] * length);
			data[B] = (T)(data[B] * length);
			data[C] = (T)(data[C] * length);
			
			return *this;
		}

		T dot(const Vector3<T>::Swizzle<A, B, C>& other) const {
			return data[A] * other.data[A] + data[B] * other.data[B] + data[C] * other.data[C];
		}
		
//		Vector3<T>::Swizzle<A, B, C> cross(const Vector3<T>& other) const {
//			return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
//		}

	}; // Swizzle

	typedef Swizzle<0, 0, 0> XXX;
	typedef Swizzle<0, 0, 1> XXY;
	typedef Swizzle<0, 0, 2> XXZ;
	typedef Swizzle<0, 0, 3> XXW;
	typedef Swizzle<0, 1, 0> XYX;
	typedef Swizzle<0, 1, 1> XYY;
	typedef Swizzle<0, 1, 2> XYZ;
	typedef Swizzle<0, 1, 3> XYW;
	typedef Swizzle<0, 2, 0> XZX;
	typedef Swizzle<0, 2, 1> XZY;
	typedef Swizzle<0, 2, 2> XZZ;
	typedef Swizzle<0, 2, 3> XZW;
	typedef Swizzle<0, 3, 0> XWX;
	typedef Swizzle<0, 3, 1> XWY;
	typedef Swizzle<0, 3, 2> XWZ;
	typedef Swizzle<0, 3, 3> XWW;
	typedef Swizzle<1, 0, 0> YXX;
	typedef Swizzle<1, 0, 1> YXY;
	typedef Swizzle<1, 0, 2> YXZ;
	typedef Swizzle<1, 0, 3> YXW;
	typedef Swizzle<1, 1, 0> YYX;
	typedef Swizzle<1, 1, 1> YYY;
	typedef Swizzle<1, 1, 2> YYZ;
	typedef Swizzle<1, 1, 3> YYW;
	typedef Swizzle<1, 2, 0> YZX;
	typedef Swizzle<1, 2, 1> YZY;
	typedef Swizzle<1, 2, 2> YZZ;
	typedef Swizzle<1, 2, 3> YZW;
	typedef Swizzle<1, 3, 0> YWX;
	typedef Swizzle<1, 3, 1> YWY;
	typedef Swizzle<1, 3, 2> YWZ;
	typedef Swizzle<1, 3, 3> YWW;
	typedef Swizzle<2, 0, 0> ZXX;
	typedef Swizzle<2, 0, 1> ZXY;
	typedef Swizzle<2, 0, 2> ZXZ;
	typedef Swizzle<2, 0, 3> ZXW;
	typedef Swizzle<2, 1, 0> ZYX;
	typedef Swizzle<2, 1, 1> ZYY;
	typedef Swizzle<2, 1, 2> ZYZ;
	typedef Swizzle<2, 1, 3> ZYW;
	typedef Swizzle<2, 2, 0> ZZX;
	typedef Swizzle<2, 2, 1> ZZY;
	typedef Swizzle<2, 2, 2> ZZZ;
	typedef Swizzle<2, 2, 3> ZZW;
	typedef Swizzle<2, 3, 0> ZWX;
	typedef Swizzle<2, 3, 1> ZWY;
	typedef Swizzle<2, 3, 2> ZWZ;
	typedef Swizzle<2, 3, 3> ZWW;
	typedef Swizzle<3, 0, 0> WXX;
	typedef Swizzle<3, 0, 1> WXY;
	typedef Swizzle<3, 0, 2> WXZ;
	typedef Swizzle<3, 0, 3> WXW;
	typedef Swizzle<3, 1, 0> WYX;
	typedef Swizzle<3, 1, 1> WYY;
	typedef Swizzle<3, 1, 2> WYZ;
	typedef Swizzle<3, 1, 3> WYW;
	typedef Swizzle<3, 2, 0> WZX;
	typedef Swizzle<3, 2, 1> WZY;
	typedef Swizzle<3, 2, 2> WZZ;
	typedef Swizzle<3, 2, 2> WZW;
	typedef Swizzle<3, 3, 0> WWX;
	typedef Swizzle<3, 3, 1> WWY;
	typedef Swizzle<3, 3, 2> WWZ;
	typedef Swizzle<3, 3, 3> WWW;
	
	union {
		T data[3];
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T s, t, p; };
		typename Vector2<T>::XX xx;
		typename Vector2<T>::XY xy;
		typename Vector2<T>::YX yx;
		typename Vector2<T>::YY yy;
		XXX xxx;
		XXY xxy;
		XXZ xxz;
		XYX xyx;
		XYY xyy;
		XYZ xyz;
		XZX xzx;
		XZY xzy;
		XZZ xzz;
		YXX yxx;
		YXY yxy;
		YXZ yxz;
		YYX yyx;
		YYY yyy;
		YYZ yyz;
		YZX yzx;
		YZY yzy;
		YZZ yzz;
		ZXX zxx;
		ZXY zxy;
		ZXZ zxz;
		ZYX zyx;
		ZYY zyy;
		ZYZ zyz;
		ZZX zzx;
		ZZY zzy;
		ZZZ zzz;
	};
	
	Vector3(const T value = (T)0) : x(value), y(value), z(value) { }
	
	Vector3(const T value0, const T value1, const T value2) : x(value0), y(value1), z(value2) { }
	
	Vector3(const Vector3<T>& other) : x(other.x), y(other.y), z(other.z) { }
	
	Vector3(const Vector2<T>& other, const T nz) : x(other.x), y(other.y), z(nz) { }
	
	Vector3<T>& operator = (const Vector3<T>& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	
	Vector3<T> operator + () const {
		return Vector3<T>(+x, +y, +z);
	}
	
	Vector3<T> operator - () const {
		return Vector3<T>(-x, -y, -z);
	}

	Vector3<T> operator + (const Vector3<T>& rhs) const {
		return Vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	
	Vector3<T> operator - (const Vector3<T>& rhs) const {
		return Vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	Vector3<T> operator * (const Vector3<T>& rhs) const {
		return Vector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
	}
	
	Vector3<T> operator / (const Vector3<T>& rhs) const {
		return Vector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
	}
	
	Vector3<T>& operator += (const Vector3<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	Vector3<T>& operator -= (const Vector3<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	
	Vector3<T>& operator *= (const Vector3<T>& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	
	Vector3<T>& operator /= (const Vector3<T>& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}
	
	Vector3<T>& operator ++ () {
		++x;
		++y;
		++z;
		return *this;
	}
	
	Vector3<T> operator ++ (int) {
		return Vector3<T>(x++, y++, z++);
	}

	Vector3<T>& operator -- () {
		--x;
		--y;
		--z;
		return *this;
	}
	
	Vector3<T> operator -- (int) {
		return Vector3<T>(x--, y--, z--);
	}

	bool operator == (const Vector3<T>& rhs) const {
		return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
	}
	
	bool operator != (const Vector3<T>& rhs) const {
		return ((x != rhs.x) || (y != rhs.y) || (z != rhs.z));
	}

	const T& operator [] (const unsigned int index) const {
		return data[index];
	}

	T& operator [] (const unsigned int index) {
		return data[index];
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
	
	T dot(const Vector3<T>& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	
	Vector3<T> cross(const Vector3<T>& other) const {
		return Vector3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	
	void rotateXZBy(float degrees, const Vector3<T>& center=Vector3<T>())
	{
		degrees *= (M_PI / 180.0f);

		const T cs = (T)cosf(degrees);
		const T sn = (T)sinf(degrees);

		x -= center.x;
		z -= center.z;

#define COMPILER_BUG
#ifdef COMPILER_BUG
		const T cX = (x * cs);
		const T sZ = (z * sn);
		const T sX = (x * sn);
		const T cZ = (z * cs);

		x = cX - sZ;
		z = sX + cZ;
#else
		x = ((x * cs) - (z * sn));
		z = ((x * sn) + (z * cs));
#endif
		x += center.x;
		z += center.z;
	}
	
	T getDistanceFrom(const Vector3<T>& other) const {
		return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z));
	}
}; // Vector3

template <typename T>
struct Vector4 {
	template<unsigned int A, unsigned int B, unsigned int C, unsigned int D>
	struct Swizzle {
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, p, q; };
			typename Vector2<T>::XX xx;
			typename Vector2<T>::XY xy;
			typename Vector2<T>::XZ xz;
			typename Vector2<T>::YX yx;
			typename Vector2<T>::YY yy;
			typename Vector2<T>::YZ yz;
			typename Vector2<T>::ZX zx;
			typename Vector2<T>::ZY zy;
			typename Vector3<T>::XXX xxx;
			typename Vector3<T>::XXY xxy;
			typename Vector3<T>::XXZ xxz;
			typename Vector3<T>::XYX xyx;
			typename Vector3<T>::XYY xyy;
			typename Vector3<T>::XYZ xyz;
			typename Vector3<T>::XZX xzx;
			typename Vector3<T>::XZY xzy;
			typename Vector3<T>::XZZ xzz;
			typename Vector3<T>::YXX yxx;
			typename Vector3<T>::YXY yxy;
			typename Vector3<T>::YXZ yxz;
			typename Vector3<T>::YYX yyx;
			typename Vector3<T>::YYY yyy;
			typename Vector3<T>::YYZ yyz;
			typename Vector3<T>::YZX yzx;
			typename Vector3<T>::YZY yzy;
			typename Vector3<T>::YZZ yzz;
			typename Vector3<T>::ZXX zxx;
			typename Vector3<T>::ZXY zxy;
			typename Vector3<T>::ZXZ zxz;
			typename Vector3<T>::ZYX zyx;
			typename Vector3<T>::ZYY zyy;
			typename Vector3<T>::ZYZ zyz;
			typename Vector3<T>::ZZX zzx;
			typename Vector3<T>::ZZY zzy;
			typename Vector3<T>::ZZZ zzz;
		};
		
		Vector4<T>::Swizzle<A, B, C, D>& operator = (const Vector4<T>& rhs) {
			data[A] = rhs.x;
			data[B] = rhs.y;
			data[C] = rhs.z;
			data[D] = rhs.w;
			return *this;
		}

		Vector4<T> operator + () const {
			return Vector4<T>(+data[A], +data[B], +data[C], +data[D]);
		}

		Vector4<T> operator - () const {
			return Vector4<T>(-data[A], -data[B], -data[C], -data[D]);
		}
		
		Vector4<T> operator + (const Vector4<T>& rhs) const {
			return Vector4<T>(data[A] + rhs.x, 
								data[B] + rhs.y,
								data[C] + rhs.z,
								data[D] + rhs.w);
		}
		
		Vector4<T> operator - (const Vector4<T>& rhs) const {
			return Vector4<T>(data[A] - rhs.x, 
								data[B] - rhs.y,
								data[C] - rhs.z,
								data[D] - rhs.w);
		}
		
		Vector4<T> operator * (const Vector4<T>& rhs) const {
			return Vector4<T>(data[A] * rhs.x, 
								data[B] * rhs.y,
								data[C] * rhs.z,
								data[D] * rhs.w);
		}
		
		Vector4<T> operator / (const Vector4<T>& rhs) const {
			return Vector4<T>(data[A] / rhs.x, 
								data[B] / rhs.y,
								data[C] / rhs.z,
								data[D] / rhs.w);
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& operator += (const Vector4<T>& rhs) {
			data[A] += rhs.x;
			data[B] += rhs.y;
			data[C] += rhs.z;
			data[D] += rhs.w;
			return *this;
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& operator -= (const Vector4<T>& rhs) {
			data[A] -= rhs.x;
			data[B] -= rhs.y;
			data[C] -= rhs.z;
			data[D] -= rhs.w;
			return *this;
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& operator *= (const Vector4<T>& rhs) {
			data[A] *= rhs.x;
			data[B] *= rhs.y;
			data[C] *= rhs.z;
			data[D] *= rhs.w;
			return *this;
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& operator /= (const Vector4<T>& rhs) {
			data[A] /= rhs.x;
			data[B] /= rhs.y;
			data[C] /= rhs.z;
			data[D] /= rhs.w;
			return *this;
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& operator ++ () {
			++data[A];
			++data[B];
			++data[C];
			++data[D];
			return *this;
		}
		
		Vector4<T> operator ++ (int) {
			const Vector4<T> ans(data[A], data[B], data[C], data[D]);
			++data[A];
			++data[B];
			++data[C];
			++data[D];
			return ans;
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& operator -- () {
			--data[A];
			--data[B];
			--data[C];
			--data[D];
			return *this;
		}
		
		Vector4<T> operator -- (int) {
			const Vector4<T> ans(data[A], data[B], data[C], data[D]);
			--data[A];
			--data[B];
			--data[C];
			--data[D];
			return ans;
		}
		
		bool operator == (const Vector4<T>& rhs) const {
			return ((data[A] == rhs.x) &&
					(data[B] == rhs.y) &&
					(data[C] == rhs.z) &&
					(data[D] == rhs.w));
		}
		
		bool operator != (const Vector4<T>& rhs) const {
			return ((data[A] != rhs.x) ||
					(data[B] != rhs.y) ||
					(data[C] != rhs.z) ||
					(data[D] != rhs.w));
		}

		const T& operator [] (const unsigned int index) const {
			const T* new_data[] = {&data[A], &data[B], &data[C], &data[D]};
			return new_data[index];
		}

		T& operator [] (const unsigned int index) {
			T* new_data[] = {&data[A], &data[B], &data[C], &data[D]};
			return *new_data[index];
		}
		
		operator Vector4<T>() const {
			return Vector4<T>(data[A], data[B], data[C], data[D]);
		}
		
		Vector4<T>::Swizzle<A, B, C, D>& normalize() {
			float length = data[A] * data[A] + data[B] * data[B] + data[C] * data[C] + data[D] * data[D];
			if (length == 0 )
				return *this;

			length = 1.0f / sqrt(length);

			data[A] = (T)(data[A] * length);
			data[B] = (T)(data[B] * length);
			data[C] = (T)(data[C] * length);
			data[D] = (T)(data[D] * length);
			
			return *this;
		}
		
		T dot(const Vector4<T>::Swizzle<A, B, C, D>& other) const {
			return data[A] * other.data[A] + data[B] * other.data[B] + data[C] * other.data[C] + data[D] * other.data[D];
		}
	}; // Swizzle

	typedef Swizzle<0, 0, 0, 0> XXXX;
	typedef Swizzle<0, 0, 0, 1> XXXY;
	typedef Swizzle<0, 0, 0, 2> XXXZ;
	typedef Swizzle<0, 0, 0, 3> XXXW;
	typedef Swizzle<0, 0, 1, 0> XXYX;
	typedef Swizzle<0, 0, 1, 1> XXYY;
	typedef Swizzle<0, 0, 1, 2> XXYZ;
	typedef Swizzle<0, 0, 1, 3> XXYW;
	typedef Swizzle<0, 0, 2, 0> XXZX;
	typedef Swizzle<0, 0, 2, 1> XXZY;
	typedef Swizzle<0, 0, 2, 2> XXZZ;
	typedef Swizzle<0, 0, 2, 3> XXZW;
	typedef Swizzle<0, 0, 3, 0> XXWX;
	typedef Swizzle<0, 0, 3, 1> XXWY;
	typedef Swizzle<0, 0, 3, 2> XXWZ;
	typedef Swizzle<0, 0, 3, 3> XXWW;
	typedef Swizzle<0, 1, 0, 0> XYXX;
	typedef Swizzle<0, 1, 0, 1> XYXY;
	typedef Swizzle<0, 1, 0, 2> XYXZ;
	typedef Swizzle<0, 1, 0, 3> XYXW;
	typedef Swizzle<0, 1, 1, 0> XYYX;
	typedef Swizzle<0, 1, 1, 1> XYYY;
	typedef Swizzle<0, 1, 1, 2> XYYZ;
	typedef Swizzle<0, 1, 1, 3> XYYW;
	typedef Swizzle<0, 1, 2, 0> XYZX;
	typedef Swizzle<0, 1, 2, 1> XYZY;
	typedef Swizzle<0, 1, 2, 2> XYZZ;
	typedef Swizzle<0, 1, 2, 3> XYZW;
	typedef Swizzle<0, 1, 3, 0> XYWX;
	typedef Swizzle<0, 1, 3, 1> XYWY;
	typedef Swizzle<0, 1, 3, 2> XYWZ;
	typedef Swizzle<0, 1, 3, 3> XYWW;
	typedef Swizzle<0, 2, 0, 0> XZXX;
	typedef Swizzle<0, 2, 0, 1> XZXY;
	typedef Swizzle<0, 2, 0, 2> XZXZ;
	typedef Swizzle<0, 2, 0, 3> XZXW;
	typedef Swizzle<0, 2, 1, 0> XZYX;
	typedef Swizzle<0, 2, 1, 1> XZYY;
	typedef Swizzle<0, 2, 1, 2> XZYZ;
	typedef Swizzle<0, 2, 1, 3> XZYW;
	typedef Swizzle<0, 2, 2, 0> XZZX;
	typedef Swizzle<0, 2, 2, 1> XZZY;
	typedef Swizzle<0, 2, 2, 2> XZZZ;
	typedef Swizzle<0, 2, 2, 3> XZZW;
	typedef Swizzle<0, 2, 3, 0> XZWX;
	typedef Swizzle<0, 2, 3, 1> XZWY;
	typedef Swizzle<0, 2, 3, 2> XZWZ;
	typedef Swizzle<0, 2, 3, 3> XZWW;
	typedef Swizzle<0, 3, 0, 0> XWXX;
	typedef Swizzle<0, 3, 0, 1> XWXY;
	typedef Swizzle<0, 3, 0, 2> XWXZ;
	typedef Swizzle<0, 3, 0, 3> XWXW;
	typedef Swizzle<0, 3, 1, 0> XWYX;
	typedef Swizzle<0, 3, 1, 1> XWYY;
	typedef Swizzle<0, 3, 1, 2> XWYZ;
	typedef Swizzle<0, 3, 1, 3> XWYW;
	typedef Swizzle<0, 3, 2, 0> XWZX;
	typedef Swizzle<0, 3, 2, 1> XWZY;
	typedef Swizzle<0, 3, 2, 2> XWZZ;
	typedef Swizzle<0, 3, 2, 3> XWZW;
	typedef Swizzle<0, 3, 3, 0> XWWX;
	typedef Swizzle<0, 3, 3, 1> XWWY;
	typedef Swizzle<0, 3, 3, 2> XWWZ;
	typedef Swizzle<0, 3, 3, 3> XWWW;
	typedef Swizzle<1, 0, 0, 0> YXXX;
	typedef Swizzle<1, 0, 0, 1> YXXY;
	typedef Swizzle<1, 0, 0, 2> YXXZ;
	typedef Swizzle<1, 0, 0, 3> YXXW;
	typedef Swizzle<1, 0, 1, 0> YXYX;
	typedef Swizzle<1, 0, 1, 1> YXYY;
	typedef Swizzle<1, 0, 1, 2> YXYZ;
	typedef Swizzle<1, 0, 1, 3> YXYW;
	typedef Swizzle<1, 0, 2, 0> YXZX;
	typedef Swizzle<1, 0, 2, 1> YXZY;
	typedef Swizzle<1, 0, 2, 2> YXZZ;
	typedef Swizzle<1, 0, 2, 3> YXZW;
	typedef Swizzle<1, 0, 3, 0> YXWX;
	typedef Swizzle<1, 0, 3, 1> YXWY;
	typedef Swizzle<1, 0, 3, 2> YXWZ;
	typedef Swizzle<1, 0, 3, 3> YXWW;
	typedef Swizzle<1, 1, 0, 0> YYXX;
	typedef Swizzle<1, 1, 0, 1> YYXY;
	typedef Swizzle<1, 1, 0, 2> YYXZ;
	typedef Swizzle<1, 1, 0, 3> YYXW;
	typedef Swizzle<1, 1, 1, 0> YYYX;
	typedef Swizzle<1, 1, 1, 1> YYYY;
	typedef Swizzle<1, 1, 1, 2> YYYZ;
	typedef Swizzle<1, 1, 1, 3> YYYW;
	typedef Swizzle<1, 1, 2, 0> YYZX;
	typedef Swizzle<1, 1, 2, 1> YYZY;
	typedef Swizzle<1, 1, 2, 2> YYZZ;
	typedef Swizzle<1, 1, 2, 3> YYZW;
	typedef Swizzle<1, 1, 3, 0> YYWX;
	typedef Swizzle<1, 1, 3, 1> YYWY;
	typedef Swizzle<1, 1, 3, 2> YYWZ;
	typedef Swizzle<1, 1, 3, 3> YYWW;
	typedef Swizzle<1, 2, 0, 0> YZXX;
	typedef Swizzle<1, 2, 0, 1> YZXY;
	typedef Swizzle<1, 2, 0, 2> YZXZ;
	typedef Swizzle<1, 2, 0, 3> YZXW;
	typedef Swizzle<1, 2, 1, 0> YZYX;
	typedef Swizzle<1, 2, 1, 1> YZYY;
	typedef Swizzle<1, 2, 1, 2> YZYZ;
	typedef Swizzle<1, 2, 1, 3> YZYW;
	typedef Swizzle<1, 2, 2, 0> YZZX;
	typedef Swizzle<1, 2, 2, 1> YZZY;
	typedef Swizzle<1, 2, 2, 2> YZZZ;
	typedef Swizzle<1, 2, 2, 3> YZZW;
	typedef Swizzle<1, 2, 3, 0> YZWX;
	typedef Swizzle<1, 2, 3, 1> YZWY;
	typedef Swizzle<1, 2, 3, 2> YZWZ;
	typedef Swizzle<1, 2, 3, 3> YZWW;
	typedef Swizzle<1, 3, 0, 0> YWXX;
	typedef Swizzle<1, 3, 0, 1> YWXY;
	typedef Swizzle<1, 3, 0, 2> YWXZ;
	typedef Swizzle<1, 3, 0, 3> YWXW;
	typedef Swizzle<1, 3, 1, 0> YWYX;
	typedef Swizzle<1, 3, 1, 1> YWYY;
	typedef Swizzle<1, 3, 1, 2> YWYZ;
	typedef Swizzle<1, 3, 1, 3> YWYW;
	typedef Swizzle<1, 3, 2, 0> YWZX;
	typedef Swizzle<1, 3, 2, 1> YWZY;
	typedef Swizzle<1, 3, 2, 2> YWZZ;
	typedef Swizzle<1, 3, 2, 3> YWZW;
	typedef Swizzle<1, 3, 3, 0> YWWX;
	typedef Swizzle<1, 3, 3, 1> YWWY;
	typedef Swizzle<1, 3, 3, 2> YWWZ;
	typedef Swizzle<1, 3, 3, 3> YWWW;
	typedef Swizzle<2, 0, 0, 0> ZXXX;
	typedef Swizzle<2, 0, 0, 1> ZXXY;
	typedef Swizzle<2, 0, 0, 2> ZXXZ;
	typedef Swizzle<2, 0, 0, 3> ZXXW;
	typedef Swizzle<2, 0, 1, 0> ZXYX;
	typedef Swizzle<2, 0, 1, 1> ZXYY;
	typedef Swizzle<2, 0, 1, 2> ZXYZ;
	typedef Swizzle<2, 0, 1, 3> ZXYW;
	typedef Swizzle<2, 0, 2, 0> ZXZX;
	typedef Swizzle<2, 0, 2, 1> ZXZY;
	typedef Swizzle<2, 0, 2, 2> ZXZZ;
	typedef Swizzle<2, 0, 2, 3> ZXZW;
	typedef Swizzle<2, 0, 3, 0> ZXWX;
	typedef Swizzle<2, 0, 3, 1> ZXWY;
	typedef Swizzle<2, 0, 3, 2> ZXWZ;
	typedef Swizzle<2, 0, 3, 3> ZXWW;
	typedef Swizzle<2, 1, 0, 0> ZYXX;
	typedef Swizzle<2, 1, 0, 1> ZYXY;
	typedef Swizzle<2, 1, 0, 2> ZYXZ;
	typedef Swizzle<2, 1, 0, 3> ZYXW;
	typedef Swizzle<2, 1, 1, 0> ZYYX;
	typedef Swizzle<2, 1, 1, 1> ZYYY;
	typedef Swizzle<2, 1, 1, 2> ZYYZ;
	typedef Swizzle<2, 1, 1, 3> ZYYW;
	typedef Swizzle<2, 1, 2, 0> ZYZX;
	typedef Swizzle<2, 1, 2, 1> ZYZY;
	typedef Swizzle<2, 1, 2, 2> ZYZZ;
	typedef Swizzle<2, 1, 2, 3> ZYZW;
	typedef Swizzle<2, 1, 3, 0> ZYWX;
	typedef Swizzle<2, 1, 3, 1> ZYWY;
	typedef Swizzle<2, 1, 3, 2> ZYWZ;
	typedef Swizzle<2, 1, 3, 3> ZYWW;
	typedef Swizzle<2, 2, 0, 0> ZZXX;
	typedef Swizzle<2, 2, 0, 1> ZZXY;
	typedef Swizzle<2, 2, 0, 2> ZZXZ;
	typedef Swizzle<2, 2, 0, 3> ZZXW;
	typedef Swizzle<2, 2, 1, 0> ZZYX;
	typedef Swizzle<2, 2, 1, 1> ZZYY;
	typedef Swizzle<2, 2, 1, 2> ZZYZ;
	typedef Swizzle<2, 2, 1, 3> ZZYW;
	typedef Swizzle<2, 2, 2, 0> ZZZX;
	typedef Swizzle<2, 2, 2, 1> ZZZY;
	typedef Swizzle<2, 2, 2, 2> ZZZZ;
	typedef Swizzle<2, 2, 2, 3> ZZZW;
	typedef Swizzle<2, 2, 3, 0> ZZWX;
	typedef Swizzle<2, 2, 3, 1> ZZWY;
	typedef Swizzle<2, 2, 3, 2> ZZWZ;
	typedef Swizzle<2, 2, 3, 3> ZZWW;
	typedef Swizzle<2, 3, 0, 0> ZWXX;
	typedef Swizzle<2, 3, 0, 1> ZWXY;
	typedef Swizzle<2, 3, 0, 2> ZWXZ;
	typedef Swizzle<2, 3, 0, 3> ZWXW;
	typedef Swizzle<2, 3, 1, 0> ZWYX;
	typedef Swizzle<2, 3, 1, 1> ZWYY;
	typedef Swizzle<2, 3, 1, 2> ZWYZ;
	typedef Swizzle<2, 3, 1, 3> ZWYW;
	typedef Swizzle<2, 3, 2, 0> ZWZX;
	typedef Swizzle<2, 3, 2, 1> ZWZY;
	typedef Swizzle<2, 3, 2, 2> ZWZZ;
	typedef Swizzle<2, 3, 2, 3> ZWZW;
	typedef Swizzle<2, 3, 3, 0> ZWWX;
	typedef Swizzle<2, 3, 3, 1> ZWWY;
	typedef Swizzle<2, 3, 3, 2> ZWWZ;
	typedef Swizzle<2, 3, 3, 3> ZWWW;
	typedef Swizzle<3, 0, 0, 0> WXXX;
	typedef Swizzle<3, 0, 0, 1> WXXY;
	typedef Swizzle<3, 0, 0, 2> WXXZ;
	typedef Swizzle<3, 0, 0, 3> WXXW;
	typedef Swizzle<3, 0, 1, 0> WXYX;
	typedef Swizzle<3, 0, 1, 1> WXYY;
	typedef Swizzle<3, 0, 1, 2> WXYZ;
	typedef Swizzle<3, 0, 1, 3> WXYW;
	typedef Swizzle<3, 0, 2, 0> WXZX;
	typedef Swizzle<3, 0, 2, 1> WXZY;
	typedef Swizzle<3, 0, 2, 2> WXZZ;
	typedef Swizzle<3, 0, 2, 3> WXZW;
	typedef Swizzle<3, 0, 3, 0> WXWX;
	typedef Swizzle<3, 0, 3, 1> WXWY;
	typedef Swizzle<3, 0, 3, 2> WXWZ;
	typedef Swizzle<3, 0, 3, 3> WXWW;
	typedef Swizzle<3, 1, 0, 0> WYXX;
	typedef Swizzle<3, 1, 0, 1> WYXY;
	typedef Swizzle<3, 1, 0, 2> WYXZ;
	typedef Swizzle<3, 1, 0, 3> WYXW;
	typedef Swizzle<3, 1, 1, 0> WYYX;
	typedef Swizzle<3, 1, 1, 1> WYYY;
	typedef Swizzle<3, 1, 1, 2> WYYZ;
	typedef Swizzle<3, 1, 1, 3> WYYW;
	typedef Swizzle<3, 1, 2, 0> WYZX;
	typedef Swizzle<3, 1, 2, 1> WYZY;
	typedef Swizzle<3, 1, 2, 2> WYZZ;
	typedef Swizzle<3, 1, 2, 3> WYZW;
	typedef Swizzle<3, 1, 3, 0> WYWX;
	typedef Swizzle<3, 1, 3, 1> WYWY;
	typedef Swizzle<3, 1, 3, 2> WYWZ;
	typedef Swizzle<3, 1, 3, 3> WYWW;
	typedef Swizzle<3, 2, 0, 0> WZXX;
	typedef Swizzle<3, 2, 0, 1> WZXY;
	typedef Swizzle<3, 2, 0, 2> WZXZ;
	typedef Swizzle<3, 2, 0, 3> WZXW;
	typedef Swizzle<3, 2, 1, 0> WZYX;
	typedef Swizzle<3, 2, 1, 1> WZYY;
	typedef Swizzle<3, 2, 1, 2> WZYZ;
	typedef Swizzle<3, 2, 1, 3> WZYW;
	typedef Swizzle<3, 2, 2, 0> WZZX;
	typedef Swizzle<3, 2, 2, 1> WZZY;
	typedef Swizzle<3, 2, 2, 2> WZZZ;
	typedef Swizzle<3, 2, 2, 3> WZZW;
	typedef Swizzle<3, 2, 3, 0> WZWX;
	typedef Swizzle<3, 2, 3, 1> WZWY;
	typedef Swizzle<3, 2, 3, 2> WZWZ;
	typedef Swizzle<3, 2, 3, 3> WZWW;
	typedef Swizzle<3, 3, 0, 0> WWXX;
	typedef Swizzle<3, 3, 0, 1> WWXY;
	typedef Swizzle<3, 3, 0, 2> WWXZ;
	typedef Swizzle<3, 3, 0, 3> WWXW;
	typedef Swizzle<3, 3, 1, 0> WWYX;
	typedef Swizzle<3, 3, 1, 1> WWYY;
	typedef Swizzle<3, 3, 1, 2> WWYZ;
	typedef Swizzle<3, 3, 1, 3> WWYW;
	typedef Swizzle<3, 3, 2, 0> WWZX;
	typedef Swizzle<3, 3, 2, 1> WWZY;
	typedef Swizzle<3, 3, 2, 2> WWZZ;
	typedef Swizzle<3, 3, 2, 3> WWZW;
	typedef Swizzle<3, 3, 3, 0> WWWX;
	typedef Swizzle<3, 3, 3, 1> WWWY;
	typedef Swizzle<3, 3, 3, 2> WWWZ;
	typedef Swizzle<3, 3, 3, 3> WWWW;

	union {
		T data[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T s, t, p, q; };
		typename Vector2<T>::XX xx;
		typename Vector2<T>::XY xy;
		typename Vector2<T>::XZ xz;
		typename Vector2<T>::XW xw;
		typename Vector2<T>::YX yx;
		typename Vector2<T>::YY yy;
		typename Vector2<T>::YZ yz;
		typename Vector2<T>::YW yw;
		typename Vector2<T>::ZX zx;
		typename Vector2<T>::ZY zy;
		typename Vector2<T>::ZZ zz;
		typename Vector2<T>::ZW zw;
		typename Vector2<T>::WX wx;
		typename Vector2<T>::WY wy;
		typename Vector2<T>::WZ wz;
		typename Vector2<T>::WW ww;
		typename Vector3<T>::XXX xxx;
		typename Vector3<T>::XXY xxy;
		typename Vector3<T>::XXZ xxz;
		typename Vector3<T>::XXW xxw;
		typename Vector3<T>::XYX xyx;
		typename Vector3<T>::XYY xyy;
		typename Vector3<T>::XYZ xyz;
		typename Vector3<T>::XYW xyw;
		typename Vector3<T>::XZX xzx;
		typename Vector3<T>::XZY xzy;
		typename Vector3<T>::XZZ xzz;
		typename Vector3<T>::XZW xzw;
		typename Vector3<T>::YXX yxx;
		typename Vector3<T>::YXY yxy;
		typename Vector3<T>::YXZ yxz;
		typename Vector3<T>::YXW yxw;
		typename Vector3<T>::YYX yyx;
		typename Vector3<T>::YYY yyy;
		typename Vector3<T>::YYZ yyz;
		typename Vector3<T>::YYW yyw;
		typename Vector3<T>::YZX yzx;
		typename Vector3<T>::YZY yzy;
		typename Vector3<T>::YZZ yzz;
		typename Vector3<T>::YZW yzw;
		typename Vector3<T>::ZXX zxx;
		typename Vector3<T>::ZXY zxy;
		typename Vector3<T>::ZXZ zxz;
		typename Vector3<T>::ZXW zxw;
		typename Vector3<T>::ZYX zyx;
		typename Vector3<T>::ZYY zyy;
		typename Vector3<T>::ZYZ zyz;
		typename Vector3<T>::ZYW zyw;
		typename Vector3<T>::ZZX zzx;
		typename Vector3<T>::ZZY zzy;
		typename Vector3<T>::ZZZ zzz;
		typename Vector3<T>::ZZW zzw;
		XXXX xxxx;
		XXXY xxxy;
		XXXZ xxxz;
		XXXW xxxw;
		XXYX xxyx;
		XXYY xxyy;
		XXYZ xxyz;
		XXYW xxyw;
		XXZX xxzx;
		XXZY xxzy;
		XXZZ xxzz;
		XXZW xxzw;
		XXWX xxwx;
		XXWY xxwy;
		XXWZ xxwz;
		XXWW xxww;
		XYXX xyxx;
		XYXY xyxy;
		XYXZ xyxz;
		XYXW xyxw;
		XYYX xyyx;
		XYYY xyyy;
		XYYZ xyyz;
		XYYW xyyw;
		XYZX xyzx;
		XYZY xyzy;
		XYZZ xyzz;
		XYZW xyzw;
		XYWX xywx;
		XYWY xywy;
		XYWZ xywz;
		XYWW xyww;
		XZXX xzxx;
		XZXY xzxy;
		XZXZ xzxz;
		XZXW xzxw;
		XZYX xzyx;
		XZYY xzyy;
		XZYZ xzyz;
		XZYW xzyw;
		XZZX xzzx;
		XZZY xzzy;
		XZZZ xzzz;
		XZZW xzzw;
		XZWX xzwx;
		XZWY xzwy;
		XZWZ xzwz;
		XZWW xzww;
		XWXX xwxx;
		XWXY xwxy;
		XWXZ xwxz;
		XWXW xwxw;
		XWYX xwyx;
		XWYY xwyy;
		XWYZ xwyz;
		XWYW xwyw;
		XWZX xwzx;
		XWZY xwzy;
		XWZZ xwzz;
		XWZW xwzw;
		XWWX xwwx;
		XWWY xwwy;
		XWWZ xwwz;
		XWWW xwww;
		YXXX yxxx;
		YXXY yxxy;
		YXXZ yxxz;
		YXXW yxxw;
		YXYX yxyx;
		YXYY yxyy;
		YXYZ yxyz;
		YXYW yxyw;
		YXZX yxzx;
		YXZY yxzy;
		YXZZ yxzz;
		YXZW yxzw;
		YXWX yxwx;
		YXWY yxwy;
		YXWZ yxwz;
		YXWW yxww;
		YYXX yyxx;
		YYXY yyxy;
		YYXZ yyxz;
		YYXW yyxw;
		YYYX yyyx;
		YYYY yyyy;
		YYYZ yyyz;
		YYYW yyyw;
		YYZX yyzx;
		YYZY yyzy;
		YYZZ yyzz;
		YYZW yyzw;
		YYWX yywx;
		YYWY yywy;
		YYWZ yywz;
		YYWW yyww;
		YZXX yzxx;
		YZXY yzxy;
		YZXZ yzxz;
		YZXW yzxw;
		YZYX yzyx;
		YZYY yzyy;
		YZYZ yzyz;
		YZYW yzyw;
		YZZX yzzx;
		YZZY yzzy;
		YZZZ yzzz;
		YZZW yzzw;
		YZWX yzwx;
		YZWY yzwy;
		YZWZ yzwz;
		YZWW yzww;
		YWXX ywxx;
		YWXY ywxy;
		YWXZ ywxz;
		YWXW ywxw;
		YWYX ywyx;
		YWYY ywyy;
		YWYZ ywyz;
		YWYW ywyw;
		YWZX ywzx;
		YWZY ywzy;
		YWZZ ywzz;
		YWZW ywzw;
		YWWX ywwx;
		YWWY ywwy;
		YWWZ ywwz;
		YWWW ywww;
		ZXXX zxxx;
		ZXXY zxxy;
		ZXXZ zxxz;
		ZXXW zxxw;
		ZXYX zxyx;
		ZXYY zxyy;
		ZXYZ zxyz;
		ZXYW zxyw;
		ZXZX zxzx;
		ZXZY zxzy;
		ZXZZ zxzz;
		ZXZW zxzw;
		ZXWX zxwx;
		ZXWY zxwy;
		ZXWZ zxwz;
		ZXWW zxww;
		ZYXX zyxx;
		ZYXY zyxy;
		ZYXZ zyxz;
		ZYXW zyxw;
		ZYYX zyyx;
		ZYYY zyyy;
		ZYYZ zyyz;
		ZYYW zyyw;
		ZYZX zyzx;
		ZYZY zyzy;
		ZYZZ zyzz;
		ZYZW zyzw;
		ZYWX zywx;
		ZYWY zywy;
		ZYWZ zywz;
		ZYWW zyww;
		ZZXX zzxx;
		ZZXY zzxy;
		ZZXZ zzxz;
		ZZXW zzxw;
		ZZYX zzyx;
		ZZYY zzyy;
		ZZYZ zzyz;
		ZZYW zzyw;
		ZZZX zzzx;
		ZZZY zzzy;
		ZZZZ zzzz;
		ZZZW zzzw;
		ZZWX zzwx;
		ZZWY zzwy;
		ZZWZ zzwz;
		ZZWW zzww;
		ZWXX zwxx;
		ZWXY zwxy;
		ZWXZ zwxz;
		ZWXW zwxw;
		ZWYX zwyx;
		ZWYY zwyy;
		ZWYZ zwyz;
		ZWYW zwyw;
		ZWZX zwzx;
		ZWZY zwzy;
		ZWZZ zwzz;
		ZWZW zwzw;
		ZWWX zwwx;
		ZWWY zwwy;
		ZWWZ zwwz;
		ZWWW zwww;
		WXXX wxxx;
		WXXY wxxy;
		WXXZ wxxz;
		WXXW wxxw;
		WXYX wxyx;
		WXYY wxyy;
		WXYZ wxyz;
		WXYW wxyw;
		WXZX wxzx;
		WXZY wxzy;
		WXZZ wxzz;
		WXZW wxzw;
		WXWX wxwx;
		WXWY wxwy;
		WXWZ wxwz;
		WXWW wxww;
		WYXX wyxx;
		WYXY wyxy;
		WYXZ wyxz;
		WYXW wyxw;
		WYYX wyyx;
		WYYY wyyy;
		WYYZ wyyz;
		WYYW wyyw;
		WYZX wyzx;
		WYZY wyzy;
		WYZZ wyzz;
		WYZW wyzw;
		WYWX wywx;
		WYWY wywy;
		WYWZ wywz;
		WYWW wyww;
		WZXX wzxx;
		WZXY wzxy;
		WZXZ wzxz;
		WZXW wzxw;
		WZYX wzyx;
		WZYY wzyy;
		WZYZ wzyz;
		WZYW wzyw;
		WZZX wzzx;
		WZZY wzzy;
		WZZZ wzzz;
		WZZW wzzw;
		WZWX wzwx;
		WZWY wzwy;
		WZWZ wzwz;
		WZWW wzww;
		WWXX wwxx;
		WWXY wwxy;
		WWXZ wwxz;
		WWXW wwxw;
		WWYX wwyx;
		WWYY wwyy;
		WWYZ wwyz;
		WWYW wwyw;
		WWZX wwzx;
		WWZY wwzy;
		WWZZ wwzz;
		WWZW wwzw;
		WWWX wwwx;
		WWWY wwwy;
		WWWZ wwwz;
		WWWW wwww;
	}; // Swizzle
	
	Vector4(const T value = (T)0) : x(value), y(value), z(value), w(value) { }
	
	Vector4(const T value0, const T value1, const T value2, const T value3)
		: x(value0), y(value1), z(value2), w(value3) { }
	
	Vector4(const Vector4<T>& other)
		: x(other.x), y(other.y), z(other.z), w(other.w) { }
		
	Vector4(const Vector3<T>& other, const T nw)
		: x(other.x), y(other.y), z(other.z), w(nw) { }
	
	Vector4<T>& operator = (const Vector4<T>& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}
	
	Vector4<T> operator + () const {
		return Vector4<T>(+x, +y, +z, +w);
	}
	
	Vector4<T> operator - () const {
		return Vector4<T>(-x, -y, -z, -w);
	}

	Vector4<T> operator + (const Vector4<T>& rhs) const {
		return Vector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}
	
	Vector4<T> operator - (const Vector4<T>& rhs) const {
		return Vector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}
	
	Vector4<T> operator * (const Vector4<T>& rhs) const {
		return Vector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}
	
	Vector4<T> operator / (const Vector4<T>& rhs) const {
		return Vector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}
	
	Vector4<T>& operator += (const Vector4<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	
	Vector4<T>& operator -= (const Vector4<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	
	Vector4<T>& operator *= (const Vector4<T>& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	
	Vector4<T>& operator /= (const Vector4<T>& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}
	
	Vector4<T>& operator ++ () {
		++x;
		++y;
		++z;
		++w;
		return *this;
	}
	
	Vector4<T> operator ++ (int) {
		return Vector4<T>(x++, y++, z++, w++);
	}

	Vector4<T> operator -- () {
		--x;
		--y;
		--z;
		--w;
		return *this;
	}
	
	Vector4<T> operator -- (int) {
		return Vector4<T>(x--, y--, z--, w--);
	}

	bool operator == (const Vector4<T>& rhs) const {
		return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w));
	}
	
	bool operator != (const Vector4<T>& rhs) const {
		return ((x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w));
	}

	const T& operator [] (const unsigned int index) const {
		return data[index];
	}

	T& operator [] (const unsigned int index) {
		return data[index];
	}
	
	T getWidth() const {
		return z - x;
	}

	T getHeight() const {
		return w - y;
	}
	
	Vector4<T>& normalize() {
		float length = x * x + y * y + z * z + w * w;
		if (length == 0 )
			return *this;

		length = 1.0f / sqrt(length);

		x = (T)(x * length);
		y = (T)(y * length);
		z = (T)(z * length);
		w = (T)(w * length);
		
		return *this;
	}
	
	T dot(const Vector4<T>& other) const {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	
	T getDistanceFrom(const Vector4<T>& other) const {
		return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y) + (other.z - z) * (other.z - z) + (other.w - w) * (other.w - w));
	}
}; // Vector4

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<unsigned char> Vector2ub;
typedef Vector2<float> Vector2f;

typedef Vector3<float> Vector3f;
typedef Vector4<unsigned char> Vector4ub;
typedef Vector4<float> Vector4f;

typedef Vector2<bool> bvec2;
typedef Vector2<signed int> ivec2;
typedef Vector2<unsigned int> uvec2;
typedef Vector2<float> vec2;
typedef Vector2<double> dvec2;

typedef Vector3<bool> bvec3;
typedef Vector3<signed int> ivec3;
typedef Vector3<unsigned int> uvec3;
typedef Vector3<float> vec3;
typedef Vector3<double> dvec3;

typedef Vector4<bool> bvec4;
typedef Vector4<signed int> ivec4;
typedef Vector4<unsigned int> uvec4;
typedef Vector4<unsigned char> ubvec4;
typedef Vector4<float> vec4;
typedef Vector4<double> dvec4;

#endif //__VECTOR_H__

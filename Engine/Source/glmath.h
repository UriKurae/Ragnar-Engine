// ----------------------------------------------------------------------------------------------------------------------------
//
// Version 2.04
//
// ----------------------------------------------------------------------------------------------------------------------------
#pragma once
#define _USE_MATH_DEFINES

#include <math.h>

// ----------------------------------------------------------------------------------------------------------------------------

class Vec2
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		struct
		{
			float s, t;
		};
		struct
		{
			float r, g;
		};
	};
	Vec2() : x(0.0f), y(0.0f)
	{}
	~Vec2()
	{}
	Vec2(float num) : x(num), y(num)
	{}
	Vec2(float x, float y) : x(x), y(y)
	{}
	Vec2(const Vec2 &u) : x(u.x), y(u.y)
	{}
	Vec2& operator=(const Vec2 &u)
	{
		x = u.x; y = u.y;
		return *this;
	}
	Vec2 operator-()
	{
		return Vec2(-x, -y);
	}
	float* operator&()
	{
		return (float*)this;
	};
	Vec2& operator+=(float num)
	{
		x += num; y += num;
		return *this;
	}
	Vec2& operator+=(const Vec2 &u)
	{
		x += u.x; y += u.y;
		return *this;
	}
	Vec2& operator-=(float num)
	{
		x -= num; y -= num;
		return *this;
	}
	Vec2& operator-=(const Vec2 &u)
	{
		x -= u.x; y -= u.y;
		return *this;
	}
	Vec2& operator*=(float num)
	{
		x *= num; y *= num;
		return *this;
	}
	Vec2& operator*=(const Vec2 &u)
	{
		x *= u.x; y *= u.y;
		return *this;
	}
	Vec2& operator/=(float num)
	{
		x /= num; y /= num;
		return *this;
	}
	Vec2& operator/=(const Vec2 &u)
	{
		x /= u.x; y /= u.y;
		return *this;
	}
	friend Vec2 operator+(const Vec2 &u, float num)
	{
		return Vec2(u.x + num, u.y + num);
	}
	friend Vec2 operator+(float num, const Vec2 &u)
	{
		return Vec2(num + u.x, num + u.y);
	}
	friend Vec2 operator+(const Vec2 &u, const Vec2 &v)
	{
		return Vec2(u.x + v.x, u.y + v.y);
	}
	friend Vec2 operator-(const Vec2 &u, float num)
	{
		return Vec2(u.x - num, u.y - num);
	}
	friend Vec2 operator-(float num, const Vec2 &u)
	{
		return Vec2(num - u.x, num - u.y);
	}
	friend Vec2 operator-(const Vec2 &u, const Vec2 &v)
	{
		return Vec2(u.x - v.x, u.y - v.y);
	}
	friend Vec2 operator*(const Vec2 &u, float num)
	{
		return Vec2(u.x * num, u.y * num);
	}
	friend Vec2 operator*(float num, const Vec2 &u)
	{
		return Vec2(num * u.x, num * u.y);
	}
	friend Vec2 operator*(const Vec2 &u, const Vec2 &v)
	{
		return Vec2(u.x * v.x, u.y * v.y);
	}
	friend Vec2 operator/(const Vec2 &u, float num)
	{
		return Vec2(u.x / num, u.y / num);
	}
	friend Vec2 operator/(float num, const Vec2 &u)
	{
		return Vec2(num / u.x, num / u.y);
	}
	friend Vec2 operator/(const Vec2 &u, const Vec2 &v)
	{
		return Vec2(u.x / v.x, u.y / v.y);
	}
};

// ----------------------------------------------------------------------------------------------------------------------------

float Dot(const Vec2 &u, const Vec2 &v);
float Length(const Vec2 &u);
float Length2(const Vec2 &u);
Vec2 Mix(const Vec2 &u, const Vec2 &v, float a);
Vec2 Normalize(const Vec2 &u);
Vec2 Reflect(const Vec2 &i, const Vec2 &n);
Vec2 Refract(const Vec2 &i, const Vec2 &n, float eta);
Vec2 Rotate(const Vec2 &u, float angle);

// ----------------------------------------------------------------------------------------------------------------------------

class Vec3
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		struct
		{
			float s, t, p;
		};
		struct
		{
			float r, g, b;
		};
	};
	Vec3() : x(0.0f), y(0.0f), z(0.0f)
	{}
	~Vec3()
	{}
	Vec3(float num) : x(num), y(num), z(num)
	{}
	Vec3(float x, float y, float z) : x(x), y(y), z(z)
	{}
	Vec3(const Vec2 &u, float z) : x(u.x), y(u.y), z(z)
	{}
	Vec3(const Vec3 &u) : x(u.x), y(u.y), z(u.z)
	{}
	void Set(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}
	Vec3& operator=(const Vec3 &u)
	{
		x = u.x; y = u.y; z = u.z;
		return *this;
	}
	Vec3 operator-()
	{
		return Vec3(-x, -y, -z);
	}
	float* operator&()
	{
		return (float*)this;
	}
	Vec3& operator+=(float num)
	{
		x += num; y += num; z += num;
		return *this;
	}
	Vec3& operator+=(const Vec3 &u)
	{
		x += u.x; y += u.y; z += u.z;
		return *this;
	}
	Vec3& operator-=(float num)
	{
		x -= num; y -= num; z -= num;
		return *this;
	}
	Vec3& operator-=(const Vec3 &u)
	{
		x -= u.x; y -= u.y; z -= u.z;
		return *this;
	}
	Vec3& operator*=(float num)
	{
		x *= num; y *= num; z *= num;
		return *this;
	}
	Vec3& operator*=(const Vec3 &u)
	{
		x *= u.x; y *= u.y; z *= u.z;
		return *this;
	}
	Vec3& operator/=(float num)
	{
		x /= num; y /= num; z /= num;
		return *this;
	}
	Vec3& operator/=(const Vec3 &u)
	{
		x /= u.x; y /= u.y; z /= u.z;
		return *this;
	}
	friend Vec3 operator+(const Vec3 &u, float num)
	{
		return Vec3(u.x + num, u.y + num, u.z + num);
	}
	friend Vec3 operator+(float num, const Vec3 &u)
	{
		return Vec3(num + u.x, num + u.y, num + u.z);
	}
	friend Vec3 operator+(const Vec3 &u, const Vec3 &v)
	{
		return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
	}
	friend Vec3 operator-(const Vec3 &u, float num)
	{
		return Vec3(u.x - num, u.y - num, u.z - num);
	}
	friend Vec3 operator-(float num, const Vec3 &u)
	{
		return Vec3(num - u.x, num - u.y, num - u.z);
	}
	friend Vec3 operator-(const Vec3 &u, const Vec3 &v)
	{
		return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
	}
	friend Vec3 operator*(const Vec3 &u, float num)
	{
		return Vec3(u.x * num, u.y * num, u.z * num);
	}
	friend Vec3 operator*(float num, const Vec3 &u)
	{
		return Vec3(num * u.x, num * u.y, num * u.z);
	}
	friend Vec3 operator*(const Vec3 &u, const Vec3 &v)
	{
		return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
	}
	friend Vec3 operator/(const Vec3 &u, float num)
	{
		return Vec3(u.x / num, u.y / num, u.z / num);
	}
	friend Vec3 operator/(float num, const Vec3 &u)
	{
		return Vec3(num / u.x, num / u.y, num / u.z);
	}
	friend Vec3 operator/(const Vec3 &u, const Vec3 &v)
	{
		return Vec3(u.x / v.x, u.y / v.y, u.z / v.z);
	}
};

// ----------------------------------------------------------------------------------------------------------------------------

Vec3 Cross(const Vec3 &u, const Vec3 &v);
float Dot(const Vec3 &u, const Vec3 &v);
float Length(const Vec3 &u);
float Length2(const Vec3 &u);
Vec3 Mix(const Vec3 &u, const Vec3 &v, float a);
Vec3 Normalize(const Vec3 &u);
Vec3 Reflect(const Vec3 &i, const Vec3 &n);
Vec3 Refract(const Vec3 &i, const Vec3 &n, float eta);
Vec3 Rotate(const Vec3 &u, float angle, const Vec3 &v);

// ----------------------------------------------------------------------------------------------------------------------------

class Vec4
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float s, t, p, q;
		};
		struct
		{
			float r, g, b, a;
		};
	};
	Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}
	~Vec4()
	{}
	Vec4(float num) : x(num), y(num), z(num), w(num)
	{}
	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{}
	Vec4(const Vec2 &u, float z, float w) : x(u.x), y(u.y), z(z), w(w)
	{}
	Vec4(const Vec3 &u, float w) : x(u.x), y(u.y), z(u.z), w(w)
	{}
	Vec4(const Vec4 &u) : x(u.x), y(u.y), z(u.z), w(u.w)
	{}
	Vec4& operator=(const Vec4 &u)
	{
		x = u.x; y = u.y; z = u.z; w = u.w;
		return *this;
	}
	Vec4 operator-()
	{
		return Vec4(-x, -y, -z, -w);
	}
	float* operator&()
	{
		return (float*)this;
	}
	Vec4& operator+=(float num)
	{
		x += num; y += num; z += num; w += num;
		return *this;
	}
	Vec4& operator+=(const Vec4 &u)
	{
		x += u.x; y += u.y; z += u.z; w += u.w;
		return *this;
	}
	Vec4& operator-=(float num)
	{
		x -= num; y -= num; z -= num; w -= num;
		return *this;
	}
	Vec4& operator-=(const Vec4 &u)
	{
		x -= u.x; y -= u.y; z -= u.z; w -= u.w;
		return *this;
	}
	Vec4& operator*=(float num)
	{
		x *= num; y *= num; z *= num; w *= num;
		return *this;
	}
	Vec4& operator*=(const Vec4 &u)
	{
		x *= u.x; y *= u.y; z *= u.z; w *= u.w;
		return *this;
	}
	Vec4& operator/=(float num)
	{
		x /= num; y /= num; z /= num; w /= num;
		return *this;
	}
	Vec4& operator/=(const Vec4 &u)
	{
		x /= u.x; y /= u.y; z /= u.z; w /= u.w;
		return *this;
	}
	friend Vec4 operator+(const Vec4 &u, float num)
	{
		return Vec4(u.x + num, u.y + num, u.z + num, u.w + num);
	}
	friend Vec4 operator+(float num, const Vec4 &u)
	{
		return Vec4(num + u.x, num + u.y, num + u.z, num + u.w);
	}
	friend Vec4 operator+(const Vec4 &u, const Vec4 &v)
	{
		return Vec4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w);
	}
	friend Vec4 operator-(const Vec4 &u, float num)
	{
		return Vec4(u.x - num, u.y - num, u.z - num, u.w - num);
	}
	friend Vec4 operator-(float num, const Vec4 &u)
	{
		return Vec4(num - u.x, num - u.y, num - u.z, num - u.w);
	}
	friend Vec4 operator-(const Vec4 &u, const Vec4 &v)
	{
		return Vec4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w);
	}
	friend Vec4 operator*(const Vec4 &u, float num)
	{
		return Vec4(u.x * num, u.y * num, u.z * num, u.w * num);
	}
	friend Vec4 operator*(float num, const Vec4 &u)
	{
		return Vec4(num * u.x, num * u.y, num * u.z, num * u.w);
	}
	friend Vec4 operator*(const Vec4 &u, const Vec4 &v)
	{
		return Vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
	}
	friend Vec4 operator/(const Vec4 &u, float num)
	{
		return Vec4(u.x / num, u.y / num, u.z / num, u.w / num);
	}
	friend Vec4 operator/(float num, const Vec4 &u)
	{
		return Vec4(num / u.x, num / u.y, num / u.z, num / u.w);
	}
	friend Vec4 operator/(const Vec4 &u, const Vec4 &v)
	{
		return Vec4(u.x / v.x, u.y / v.y, u.z / v.z, u.w / v.w);
	}
};

// ----------------------------------------------------------------------------------------------------------------------------

class Mat2x2;
class Mat3x3;
class Mat4x4;

// ----------------------------------------------------------------------------------------------------------------------------

class Mat2x2
{
public:
	float m[4];
	Mat2x2();
	~Mat2x2();
	Mat2x2(const Mat2x2 &matrix);
	Mat2x2(const Vec2 &col1, const Vec2 &col2);
	Mat2x2(float c1r1, float c1r2, float c2r1, float c2r2);
	explicit Mat2x2(const Mat3x3 &matrix);
	explicit Mat2x2(const Mat4x4 &matrix);
	Mat2x2& operator=(const Mat2x2 &matrix);
	float& operator[](int i);
	float* operator&();
	friend Mat2x2 operator*(const Mat2x2 &matrix1, const Mat2x2 &matrix2);
	friend Vec2 operator*(const Mat2x2 &matrix, const Vec2 &u);
};

// ----------------------------------------------------------------------------------------------------------------------------

Mat2x2 Inverse(const Mat2x2 &matrix);
Mat2x2 Transpose(const Mat2x2 &matrix);

// ----------------------------------------------------------------------------------------------------------------------------

class Mat3x3
{
public:
	float m[9];
	Mat3x3();
	~Mat3x3();
	Mat3x3(const Mat3x3 &matrix);
	Mat3x3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3);
	Mat3x3(float c1r1, float c1r2, float c1r3, float c2r1, float c2r2, float c2r3, float c3r1, float c3r2, float c3r3);
	explicit Mat3x3(const Mat2x2 &matrix);
	explicit Mat3x3(const Mat4x4 &matrix);
	Mat3x3& operator=(const Mat3x3 &matrix);
	float& operator[](int i);
	float* operator&();
	friend Mat3x3 operator*(const Mat3x3 &matrix1, const Mat3x3 &matrix2);
	friend Vec3 operator*(const Mat3x3 &matrix, const Vec3 &u);
};

// ----------------------------------------------------------------------------------------------------------------------------

Mat3x3 Inverse(const Mat3x3 &matrix);
Mat3x3 Transpose(const Mat3x3 &matrix);

// ----------------------------------------------------------------------------------------------------------------------------

class Mat4x4
{
public:
	float m[16];
	Mat4x4();
	~Mat4x4();
	Mat4x4(const Mat4x4 &matrix);
	Mat4x4(const Vec4 &col1, const Vec4 &col2, const Vec4 &col3, const Vec4 &col4);
	Mat4x4(float c1r1, float c1r2, float c1r3, float c1r4, float c2r1, float c2r2, float c2r3, float c2r4, float c3r1, float c3r2, float c3r3, float c3r4, float c4r1, float c4r2, float c4r3, float c4r4);
	explicit Mat4x4(const Mat2x2 &matrix);
	explicit Mat4x4(const Mat3x3 &matrix);
	Mat4x4& operator=(const Mat4x4 &matrix);
	float& operator[](int i);
	float* operator&();
	const float* operator&() const;
	friend Mat4x4 operator*(const Mat4x4 &matrix1, const Mat4x4 &matrix2);
	friend Vec4 operator*(const Mat4x4 &matrix, const Vec4 &u);

	Mat4x4& Inverse();
	Mat4x4& Look(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
	Mat4x4& Ortho(float left, float right, float bottom, float top, float n, float f);
	Mat4x4& Perspective(float fovy, float aspect, float n, float f);
	Mat4x4& Rotate(float angle, const Vec3 &u);
	Mat4x4& Scale(float x, float y, float z);
	Mat4x4& Translate(float x, float y, float z);
	Mat4x4& Transpose();
	Vec3 Translation() const;
};

// ----------------------------------------------------------------------------------------------------------------------------

extern Mat4x4 biasMatrix;
extern Mat4x4 biasMatrixInverse;
extern Mat4x4 identityMatrix;

// ----------------------------------------------------------------------------------------------------------------------------

Mat4x4 Inverse(const Mat4x4 &matrix);
Mat4x4 Look(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
Mat4x4 Ortho(float left, float right, float bottom, float top, float n, float f);
Mat4x4 Perspective(float fovy, float aspect, float n, float f);
Mat4x4 Rotate(float angle, const Vec3 &u);
Mat4x4 Scale(float x, float y, float z);
Mat4x4 Translate(float x, float y, float z);
Mat4x4 Transpose(const Mat4x4 &matrix);
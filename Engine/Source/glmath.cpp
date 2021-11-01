#include "glmath.h"

#include "Profiling.h"

// ----------------------------------------------------------------------------------------------------------------------------

float Dot(const Vec2 &u, const Vec2 &v)
{
	return u.x * v.x + u.y * v.y;
}

float Length(const Vec2 &u)
{
	return sqrt(u.x * u.x + u.y * u.y);
}

float Length2(const Vec2 &u)
{
	return u.x * u.x + u.y * u.y;
}

Vec2 Mix(const Vec2 &u, const Vec2 &v, float a)
{
	return u * (1.0f - a) + v * a;
}

Vec2 Normalize(const Vec2 &u)
{
	return u / sqrt(u.x * u.x + u.y * u.y);
}

Vec2 Reflect(const Vec2 &i, const Vec2 &n)
{
	return i - 2.0f * Dot(n, i) * n;
}

Vec2 Refract(const Vec2 &i, const Vec2 &n, float eta)
{
	Vec2 r;

	float ndoti = Dot(n, i), k = 1.0f - eta * eta * (1.0f - ndoti * ndoti);

	if(k >= 0.0f)
	{
		r = eta * i - n * (eta * ndoti + sqrt(k));
	}

	return r;
}

Vec2 Rotate(const Vec2 &u, float angle)
{
	angle = angle / 180.0f * (float)M_PI;

	float c = cos(angle), s = sin(angle);

	return Vec2(u.x * c - u.y * s, u.x * s + u.y * c);
}

// ----------------------------------------------------------------------------------------------------------------------------

Vec3 Cross(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

float Dot(const Vec3 &u, const Vec3 &v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float Length(const Vec3 &u)
{
	return sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

float Length2(const Vec3 &u)
{
	return u.x * u.x + u.y * u.y + u.z * u.z;
}

Vec3 Mix(const Vec3 &u, const Vec3 &v, float a)
{
	return u * (1.0f - a) + v * a;
}

Vec3 Normalize(const Vec3 &u)
{
	return u / sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

Vec3 Reflect(const Vec3 &i, const Vec3 &n)
{
	return i - 2.0f * Dot(n, i) * n;
}

Vec3 Refract(const Vec3 &i, const Vec3 &n, float eta)
{
	Vec3 r;

	float ndoti = Dot(n, i), k = 1.0f - eta * eta * (1.0f - ndoti * ndoti);

	if(k >= 0.0f)
	{
		r = eta * i - n * (eta * ndoti + sqrt(k));
	}

	return r;
}

Vec3 Rotate(const Vec3 &u, float angle, const Vec3 &v)
{
	return *(Vec3*)&(Rotate(angle, v) * Vec4(u, 1.0f));
}

// ----------------------------------------------------------------------------------------------------------------------------

Mat2x2::Mat2x2()
{
	m[0] = 1.0f; m[2] = 0.0f;
	m[1] = 0.0f; m[3] = 1.0f;
}

Mat2x2::~Mat2x2()
{}

Mat2x2::Mat2x2(const Mat2x2 &matrix)
{
	for(int i = 0; i < 4; i++)
	{
		m[i] = matrix.m[i];
	}
}

Mat2x2::Mat2x2(const Vec2 &col1, const Vec2 &col2)
{
	m[0] = col1.x; m[2] = col2.x;
	m[1] = col1.y; m[3] = col2.y;
}

Mat2x2::Mat2x2(float c1r1, float c1r2, float c2r1, float c2r2)
{
	m[0] = c1r1; m[2] = c2r1;
	m[1] = c1r2; m[3] = c2r2;
}

Mat2x2::Mat2x2(const Mat3x3 &matrix)
{
	m[0] = matrix.m[0]; m[2] = matrix.m[3];
	m[1] = matrix.m[1]; m[3] = matrix.m[4];
}

Mat2x2::Mat2x2(const Mat4x4 &matrix)
{
	m[0] = matrix.m[0]; m[2] = matrix.m[4];
	m[1] = matrix.m[1]; m[3] = matrix.m[5];
}

Mat2x2& Mat2x2::operator=(const Mat2x2 &matrix)
{
	for(int i = 0; i < 4; i++)
	{
		m[i] = matrix.m[i];
	}

	return *this;
}

float& Mat2x2::operator[](int i)
{
	return m[i];
}

float* Mat2x2::operator&()
{
	return (float*)this;
}

Mat2x2 operator*(const Mat2x2 &matrix1, const Mat2x2 &matrix2)
{
	Mat2x2 matrix3;

	matrix3.m[0] = matrix1.m[0] * matrix2.m[0] + matrix1.m[2] * matrix2.m[1];
	matrix3.m[1] = matrix1.m[1] * matrix2.m[0] + matrix1.m[3] * matrix2.m[1];
	matrix3.m[2] = matrix1.m[0] * matrix2.m[2] + matrix1.m[2] * matrix2.m[3];
	matrix3.m[3] = matrix1.m[1] * matrix2.m[2] + matrix1.m[3] * matrix2.m[3];

	return matrix3;
}

Vec2 operator*(const Mat2x2 &matrix, const Vec2 &u)
{
	Vec2 v;

	v.x = matrix.m[0] * u.x + matrix.m[2] * u.y;
	v.y = matrix.m[1] * u.x + matrix.m[3] * u.y;

	return v;
}

// ----------------------------------------------------------------------------------------------------------------------------
//
// 0 2
// 1 3
//
// ----------------------------------------------------------------------------------------------------------------------------

Mat2x2 Inverse(const Mat2x2 &matrix)
{
	const float *m = matrix.m;

	float det = m[0] * m[3] - m[2] * m[1];

	Mat2x2 inverse;

	inverse.m[0] = m[3] / det;
	inverse.m[1] = -m[1] / det;
	inverse.m[2] = -m[2] / det;
	inverse.m[3] = m[0] / det;

	return inverse;
}

Mat2x2 Transpose(const Mat2x2 &matrix)
{
	Mat2x2 transpose;

	transpose.m[0] = matrix.m[0];
	transpose.m[1] = matrix.m[2];
	transpose.m[2] = matrix.m[1];
	transpose.m[3] = matrix.m[3];

	return transpose;
}

// ----------------------------------------------------------------------------------------------------------------------------

Mat3x3::Mat3x3()
{
	m[0] = 1.0f; m[3] = 0.0f; m[6] = 0.0f;
	m[1] = 0.0f; m[4] = 1.0f; m[7] = 0.0f;
	m[2] = 0.0f; m[5] = 0.0f; m[8] = 1.0f;
}

Mat3x3::~Mat3x3()
{}

Mat3x3::Mat3x3(const Mat3x3 &matrix)
{
	for(int i = 0; i < 9; i++)
	{
		m[i] = matrix.m[i];
	}
}

Mat3x3::Mat3x3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3)
{
	m[0] = col1.x; m[3] = col2.x; m[6] = col3.x;
	m[1] = col1.y; m[4] = col2.y; m[7] = col3.y;
	m[2] = col1.z; m[5] = col2.z; m[8] = col3.z;
}

Mat3x3::Mat3x3(float c1r1, float c1r2, float c1r3, float c2r1, float c2r2, float c2r3, float c3r1, float c3r2, float c3r3)
{
	m[0] = c1r1; m[3] = c2r1; m[6] = c3r1;
	m[1] = c1r2; m[4] = c2r2; m[7] = c3r2;
	m[2] = c1r3; m[5] = c2r3; m[8] = c3r3;
}

Mat3x3::Mat3x3(const Mat2x2 &matrix)
{
	m[0] = matrix.m[0]; m[3] = matrix.m[2]; m[6] = 0.0f;
	m[1] = matrix.m[1]; m[4] = matrix.m[3]; m[7] = 0.0f;
	m[2] = 0.0f; m[5] = 0.0f; m[8] = 1.0f;
}

Mat3x3::Mat3x3(const Mat4x4 &matrix)
{
	m[0] = matrix.m[0]; m[3] = matrix.m[4]; m[6] = matrix.m[8];
	m[1] = matrix.m[1]; m[4] = matrix.m[5]; m[7] = matrix.m[9];
	m[2] = matrix.m[2]; m[5] = matrix.m[6]; m[8] = matrix.m[10];
}

Mat3x3& Mat3x3::operator=(const Mat3x3 &matrix)
{
	for(int i = 0; i < 9; i++)
	{
		m[i] = matrix.m[i];
	}

	return *this;
}

float& Mat3x3::operator[](int i)
{
	return m[i];
}

float* Mat3x3::operator&()
{
	return (float*)this;
}

Mat3x3 operator*(const Mat3x3 &matrix1, const Mat3x3 &matrix2)
{
	Mat3x3 matrix3;

	matrix3.m[0] = matrix1.m[0] * matrix2.m[0] + matrix1.m[3] * matrix2.m[1] + matrix1.m[6] * matrix2.m[2];
	matrix3.m[1] = matrix1.m[1] * matrix2.m[0] + matrix1.m[4] * matrix2.m[1] + matrix1.m[7] * matrix2.m[2];
	matrix3.m[2] = matrix1.m[2] * matrix2.m[0] + matrix1.m[5] * matrix2.m[1] + matrix1.m[8] * matrix2.m[2];
	matrix3.m[3] = matrix1.m[0] * matrix2.m[3] + matrix1.m[3] * matrix2.m[4] + matrix1.m[6] * matrix2.m[5];
	matrix3.m[4] = matrix1.m[1] * matrix2.m[3] + matrix1.m[4] * matrix2.m[4] + matrix1.m[7] * matrix2.m[5];
	matrix3.m[5] = matrix1.m[2] * matrix2.m[3] + matrix1.m[5] * matrix2.m[4] + matrix1.m[8] * matrix2.m[5];
	matrix3.m[6] = matrix1.m[0] * matrix2.m[6] + matrix1.m[3] * matrix2.m[7] + matrix1.m[6] * matrix2.m[8];
	matrix3.m[7] = matrix1.m[1] * matrix2.m[6] + matrix1.m[4] * matrix2.m[7] + matrix1.m[7] * matrix2.m[8];
	matrix3.m[8] = matrix1.m[2] * matrix2.m[6] + matrix1.m[5] * matrix2.m[7] + matrix1.m[8] * matrix2.m[8];

	return matrix3;
}

Vec3 operator*(const Mat3x3 &matrix, const Vec3 &u)
{
	Vec3 v;

	v.x = matrix.m[0] * u.x + matrix.m[3] * u.y + matrix.m[6] * u.z;
	v.y = matrix.m[1] * u.x + matrix.m[4] * u.y + matrix.m[7] * u.z;
	v.z = matrix.m[2] * u.x + matrix.m[5] * u.y + matrix.m[8] * u.z;

	return v;
}

// ----------------------------------------------------------------------------------------------------------------------------
//
// 0 3 6 | + - +
// 1 4 7 | - + -
// 2 5 8 | + - +
//
// ----------------------------------------------------------------------------------------------------------------------------

float Det2x2sub(const float *m, int i0, int i1, int i2, int i3)
{
	return m[i0] * m[i3] - m[i2] * m[i1];
}

Mat3x3 Inverse(const Mat3x3 &matrix)
{
	const float *m = matrix.m;

	float det = 0.0f;

	det += m[0] * Det2x2sub(m, 4, 5, 7, 8);
	det -= m[3] * Det2x2sub(m, 1, 2, 7, 8);
	det += m[6] * Det2x2sub(m, 1, 2, 4, 5);

	Mat3x3 inverse;

	inverse.m[0] = Det2x2sub(m, 4, 5, 7, 8) / det;
	inverse.m[1] = -Det2x2sub(m, 1, 2, 7, 8) / det;
	inverse.m[2] = Det2x2sub(m, 1, 2, 4, 5) / det;
	inverse.m[3] = -Det2x2sub(m, 3, 5, 6, 8) / det;
	inverse.m[4] = Det2x2sub(m, 0, 2, 6, 8) / det;
	inverse.m[5] = -Det2x2sub(m, 0, 2, 3, 5) / det;
	inverse.m[6] = Det2x2sub(m, 3, 4, 6, 7) / det;
	inverse.m[7] = -Det2x2sub(m, 0, 1, 6, 7) / det;
	inverse.m[8] = Det2x2sub(m, 0, 1, 3, 4) / det;

	return inverse;
}

Mat3x3 Transpose(const Mat3x3 &matrix)
{
	Mat3x3 transpose;

	transpose.m[0] = matrix.m[0];
	transpose.m[1] = matrix.m[3];
	transpose.m[2] = matrix.m[6];
	transpose.m[3] = matrix.m[1];
	transpose.m[4] = matrix.m[4];
	transpose.m[5] = matrix.m[7];
	transpose.m[6] = matrix.m[2];
	transpose.m[7] = matrix.m[5];
	transpose.m[8] = matrix.m[8];

	return transpose;
}

// ----------------------------------------------------------------------------------------------------------------------------

Mat4x4::Mat4x4()
{
	m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f; m[12] = 0.0f;
	m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f; m[13] = 0.0f;
	m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

Mat4x4::~Mat4x4()
{}

Mat4x4::Mat4x4(const Mat4x4 &matrix)
{
	for(int i = 0; i < 16; i++)
	{
		m[i] = matrix.m[i];
	}
}

Mat4x4::Mat4x4(const Vec4 &col1, const Vec4 &col2, const Vec4 &col3, const Vec4 &col4)
{
	m[0] = col1.x; m[4] = col2.x; m[8] = col3.x; m[12] = col4.x;
	m[1] = col1.y; m[5] = col2.y; m[9] = col3.y; m[13] = col4.y;
	m[2] = col1.z; m[6] = col2.z; m[10] = col3.z; m[14] = col4.z;
	m[3] = col1.w; m[7] = col2.w; m[11] = col3.w; m[15] = col4.w;
}

Mat4x4::Mat4x4(float c1r1, float c1r2, float c1r3, float c1r4, float c2r1, float c2r2, float c2r3, float c2r4, float c3r1, float c3r2, float c3r3, float c3r4, float c4r1, float c4r2, float c4r3, float c4r4)
{
	m[0] = c1r1; m[4] = c2r1; m[8] = c3r1; m[12] = c4r1;
	m[1] = c1r2; m[5] = c2r2; m[9] = c3r2; m[13] = c4r2;
	m[2] = c1r3; m[6] = c2r3; m[10] = c3r3; m[14] = c4r3;
	m[3] = c1r4; m[7] = c2r4; m[11] = c3r4; m[15] = c4r4;
}

Mat4x4::Mat4x4(const Mat2x2 &matrix)
{
	m[0] = matrix.m[0]; m[4] = matrix.m[2]; m[8] = 0.0f; m[12] = 0.0f;
	m[1] = matrix.m[1]; m[5] = matrix.m[3]; m[9] = 0.0f; m[13] = 0.0f;
	m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

Mat4x4::Mat4x4(const Mat3x3 &matrix)
{
	m[0] = matrix.m[0]; m[4] = matrix.m[3]; m[8] = matrix.m[6]; m[12] = 0.0f;
	m[1] = matrix.m[1]; m[5] = matrix.m[4]; m[9] = matrix.m[7]; m[13] = 0.0f;
	m[2] = matrix.m[2]; m[6] = matrix.m[5]; m[10] = matrix.m[8]; m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

Mat4x4& Mat4x4::operator=(const Mat4x4 &matrix)
{
	for(int i = 0; i < 16; i++)
	{
		m[i] = matrix.m[i];
	}

	return *this;
}

float& Mat4x4::operator[](int i)
{
	return m[i];
}

float* Mat4x4::operator&()
{
	return (float*)this;
}

const float* Mat4x4::operator&() const
{
	return (float*)this;
}

Mat4x4 operator*(const Mat4x4 &matrix1, const Mat4x4 &matrix2)
{
	Mat4x4 matrix3;

	matrix3.m[0] = matrix1.m[0] * matrix2.m[0] + matrix1.m[4] * matrix2.m[1] + matrix1.m[8] * matrix2.m[2] + matrix1.m[12] * matrix2.m[3];
	matrix3.m[1] = matrix1.m[1] * matrix2.m[0] + matrix1.m[5] * matrix2.m[1] + matrix1.m[9] * matrix2.m[2] + matrix1.m[13] * matrix2.m[3];
	matrix3.m[2] = matrix1.m[2] * matrix2.m[0] + matrix1.m[6] * matrix2.m[1] + matrix1.m[10] * matrix2.m[2] + matrix1.m[14] * matrix2.m[3];
	matrix3.m[3] = matrix1.m[3] * matrix2.m[0] + matrix1.m[7] * matrix2.m[1] + matrix1.m[11] * matrix2.m[2] + matrix1.m[15] * matrix2.m[3];
	matrix3.m[4] = matrix1.m[0] * matrix2.m[4] + matrix1.m[4] * matrix2.m[5] + matrix1.m[8] * matrix2.m[6] + matrix1.m[12] * matrix2.m[7];
	matrix3.m[5] = matrix1.m[1] * matrix2.m[4] + matrix1.m[5] * matrix2.m[5] + matrix1.m[9] * matrix2.m[6] + matrix1.m[13] * matrix2.m[7];
	matrix3.m[6] = matrix1.m[2] * matrix2.m[4] + matrix1.m[6] * matrix2.m[5] + matrix1.m[10] * matrix2.m[6] + matrix1.m[14] * matrix2.m[7];
	matrix3.m[7] = matrix1.m[3] * matrix2.m[4] + matrix1.m[7] * matrix2.m[5] + matrix1.m[11] * matrix2.m[6] + matrix1.m[15] * matrix2.m[7];
	matrix3.m[8] = matrix1.m[0] * matrix2.m[8] + matrix1.m[4] * matrix2.m[9] + matrix1.m[8] * matrix2.m[10] + matrix1.m[12] * matrix2.m[11];
	matrix3.m[9] = matrix1.m[1] * matrix2.m[8] + matrix1.m[5] * matrix2.m[9] + matrix1.m[9] * matrix2.m[10] + matrix1.m[13] * matrix2.m[11];
	matrix3.m[10] = matrix1.m[2] * matrix2.m[8] + matrix1.m[6] * matrix2.m[9] + matrix1.m[10] * matrix2.m[10] + matrix1.m[14] * matrix2.m[11];
	matrix3.m[11] = matrix1.m[3] * matrix2.m[8] + matrix1.m[7] * matrix2.m[9] + matrix1.m[11] * matrix2.m[10] + matrix1.m[15] * matrix2.m[11];
	matrix3.m[12] = matrix1.m[0] * matrix2.m[12] + matrix1.m[4] * matrix2.m[13] + matrix1.m[8] * matrix2.m[14] + matrix1.m[12] * matrix2.m[15];
	matrix3.m[13] = matrix1.m[1] * matrix2.m[12] + matrix1.m[5] * matrix2.m[13] + matrix1.m[9] * matrix2.m[14] + matrix1.m[13] * matrix2.m[15];
	matrix3.m[14] = matrix1.m[2] * matrix2.m[12] + matrix1.m[6] * matrix2.m[13] + matrix1.m[10] * matrix2.m[14] + matrix1.m[14] * matrix2.m[15];
	matrix3.m[15] = matrix1.m[3] * matrix2.m[12] + matrix1.m[7] * matrix2.m[13] + matrix1.m[11] * matrix2.m[14] + matrix1.m[15] * matrix2.m[15];

	return matrix3;
}

Vec4 operator*(const Mat4x4 &matrix, const Vec4 &u)
{
	Vec4 v;

	v.x = matrix.m[0] * u.x + matrix.m[4] * u.y + matrix.m[8] * u.z + matrix.m[12] * u.w;
	v.y = matrix.m[1] * u.x + matrix.m[5] * u.y + matrix.m[9] * u.z + matrix.m[13] * u.w;
	v.z = matrix.m[2] * u.x + matrix.m[6] * u.y + matrix.m[10] * u.z + matrix.m[14] * u.w;
	v.w = matrix.m[3] * u.x + matrix.m[7] * u.y + matrix.m[11] * u.z + matrix.m[15] * u.w;

	return v;
}

// ----------------------------------------------------------------------------------------------------------------------------

Mat4x4 biasMatrix = Mat4x4(0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f);
Mat4x4 biasMatrixInverse = Mat4x4(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, -1.0f, -1.0f, -1.0f, 1.0f);
Mat4x4 identityMatrix = Mat4x4(
	1.0f, 0.0f, 0.0f, 0.0f, 
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 
	0.0f, 0.0f, 0.0f, 1.0f);

// ----------------------------------------------------------------------------------------------------------------------------
//
// 0 4  8 12 | + - + -
// 1 5  9 13 | - + - +
// 2 6 10 14 | + - + -
// 3 7 11 15 | - + - +
//
// ----------------------------------------------------------------------------------------------------------------------------

float Det3x3sub(const float *m, int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8)
{
	float det = 0.0f;

	det += m[i0] * Det2x2sub(m, i4, i5, i7, i8);
	det -= m[i3] * Det2x2sub(m, i1, i2, i7, i8);
	det += m[i6] * Det2x2sub(m, i1, i2, i4, i5);

	return det;
}

Mat4x4& Mat4x4::Inverse()
{
	const float *m = m;

	float det = 0.0f;

	det += m[0] * Det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15);
	det -= m[4] * Det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	det += m[8] * Det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	det -= m[12] * Det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11);

	Mat4x4 inverse;

	inverse.m[0] = Det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15) / det;
	inverse.m[1] = -Det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15) / det;
	inverse.m[2] = Det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15) / det;
	inverse.m[3] = -Det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11) / det;
	inverse.m[4] = -Det3x3sub(m, 4, 6, 7, 8, 10, 11, 12, 14, 15) / det;
	inverse.m[5] = Det3x3sub(m, 0, 2, 3, 8, 10, 11, 12, 14, 15) / det;
	inverse.m[6] = -Det3x3sub(m, 0, 2, 3, 4, 6, 7, 12, 14, 15) / det;
	inverse.m[7] = Det3x3sub(m, 0, 2, 3, 4, 6, 7, 8, 10, 11) / det;
	inverse.m[8] = Det3x3sub(m, 4, 5, 7, 8, 9, 11, 12, 13, 15) / det;
	inverse.m[9] = -Det3x3sub(m, 0, 1, 3, 8, 9, 11, 12, 13, 15) / det;
	inverse.m[10] = Det3x3sub(m, 0, 1, 3, 4, 5, 7, 12, 13, 15) / det;
	inverse.m[11] = -Det3x3sub(m, 0, 1, 3, 4, 5, 7, 8, 9, 11) / det;
	inverse.m[12] = -Det3x3sub(m, 4, 5, 6, 8, 9, 10, 12, 13, 14) / det;
	inverse.m[13] = Det3x3sub(m, 0, 1, 2, 8, 9, 10, 12, 13, 14) / det;
	inverse.m[14] = -Det3x3sub(m, 0, 1, 2, 4, 5, 6, 12, 13, 14) / det;
	inverse.m[15] = Det3x3sub(m, 0, 1, 2, 4, 5, 6, 8, 9, 10) / det;

	operator=(inverse);

	return *this;
}

Mat4x4& Mat4x4::Look(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
	Vec3 Z = Normalize(eye - center);
	Vec3 X = Normalize(Cross(up, Z));
	Vec3 Y = Cross(Z, X);

	m[0] = X.x;
	m[1] = Y.x;
	m[2] = Z.x;
	m[4] = X.y;
	m[5] = Y.y;
	m[6] = Z.y;
	m[8] = X.z;
	m[9] = Y.z;
	m[10] = Z.z;
	m[12] = -Dot(X, eye);
	m[13] = -Dot(Y, eye);
	m[14] = -Dot(Z, eye);

	return *this;
}

Mat4x4& Mat4x4::Ortho(float left, float right, float bottom, float top, float n, float f)
{
	m[0] = 2.0f / (right - left);
	m[5] = 2.0f / (top - bottom);
	m[10] = -2.0f / (f - n);
	m[12] = -(right + left) / (right - left);
	m[13] = -(top + bottom) / (top - bottom);
	m[14] = -(f + n) / (f - n);

	return *this;
}

Mat4x4& Mat4x4::Perspective(float fovy, float aspect, float n, float f)
{
	float coty = 1.0f / tan(fovy * (float)M_PI / 360.0f);

	m[0] = coty / aspect;
	m[5] = coty;
	m[10] = (n + f) / (n - f);
	m[11] = -1.0f;
	m[14] = 2.0f * n * f / (n - f);
	m[15] = 0.0f;

	return *this;
}

Mat4x4& Mat4x4::Rotate(float angle, const Vec3 &u)
{
	angle = angle / 180.0f * (float)M_PI;

	Vec3 v = Normalize(u);

	float c = 1.0f - cos(angle), s = sin(angle);

	m[0] = 1.0f + c * (v.x * v.x - 1.0f);
	m[1] = c * v.x * v.y + v.z * s;
	m[2] = c * v.x * v.z - v.y * s;
	m[4] = c * v.x * v.y - v.z * s;
	m[5] = 1.0f + c * (v.y * v.y - 1.0f);
	m[6] = c * v.y * v.z + v.x * s;
	m[8] = c * v.x * v.z + v.y * s;
	m[9] = c * v.y * v.z - v.x * s;
	m[10] = 1.0f + c * (v.z * v.z - 1.0f);

	return *this;
}

Mat4x4& Mat4x4::Scale(float x, float y, float z)
{
	m[0] = x;
	m[5] = y;
	m[10] = z;

	return *this;
}

Mat4x4& Mat4x4::Translate(float x, float y, float z)
{
	m[12] = x;
	m[13] = y;
	m[14] = z;

	return *this;
}

Mat4x4& Mat4x4::Transpose()
{
	Mat4x4 transpose;

	transpose.m[0] = m[0];
	transpose.m[1] = m[4];
	transpose.m[2] = m[8];
	transpose.m[3] = m[12];
	transpose.m[4] = m[1];
	transpose.m[5] = m[5];
	transpose.m[6] = m[9];
	transpose.m[7] = m[13];
	transpose.m[8] = m[2];
	transpose.m[9] = m[6];
	transpose.m[10] = m[10];
	transpose.m[11] = m[14];
	transpose.m[12] = m[3];
	transpose.m[13] = m[7];
	transpose.m[14] = m[11];
	transpose.m[15] = m[15];

	operator=(transpose);

	return *this;
}

Vec3 Mat4x4::Translation() const
{
	return(Vec3(m[12], m[13], m[14]));
}


Mat4x4 Inverse(const Mat4x4 &matrix)
{
	const float *m = matrix.m;

	float det = 0.0f;

	det += m[0] * Det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15);
	det -= m[4] * Det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	det += m[8] * Det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	det -= m[12] * Det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11);

	Mat4x4 inverse;

	inverse.m[0] = Det3x3sub(m, 5, 6, 7, 9, 10, 11, 13, 14, 15) / det;
	inverse.m[1] = -Det3x3sub(m, 1, 2, 3, 9, 10, 11, 13, 14, 15) / det;
	inverse.m[2] = Det3x3sub(m, 1, 2, 3, 5, 6, 7, 13, 14, 15) / det;
	inverse.m[3] = -Det3x3sub(m, 1, 2, 3, 5, 6, 7, 9, 10, 11) / det;
	inverse.m[4] = -Det3x3sub(m, 4, 6, 7, 8, 10, 11, 12, 14, 15) / det;
	inverse.m[5] = Det3x3sub(m, 0, 2, 3, 8, 10, 11, 12, 14, 15) / det;
	inverse.m[6] = -Det3x3sub(m, 0, 2, 3, 4, 6, 7, 12, 14, 15) / det;
	inverse.m[7] = Det3x3sub(m, 0, 2, 3, 4, 6, 7, 8, 10, 11) / det;
	inverse.m[8] = Det3x3sub(m, 4, 5, 7, 8, 9, 11, 12, 13, 15) / det;
	inverse.m[9] = -Det3x3sub(m, 0, 1, 3, 8, 9, 11, 12, 13, 15) / det;
	inverse.m[10] = Det3x3sub(m, 0, 1, 3, 4, 5, 7, 12, 13, 15) / det;
	inverse.m[11] = -Det3x3sub(m, 0, 1, 3, 4, 5, 7, 8, 9, 11) / det;
	inverse.m[12] = -Det3x3sub(m, 4, 5, 6, 8, 9, 10, 12, 13, 14) / det;
	inverse.m[13] = Det3x3sub(m, 0, 1, 2, 8, 9, 10, 12, 13, 14) / det;
	inverse.m[14] = -Det3x3sub(m, 0, 1, 2, 4, 5, 6, 12, 13, 14) / det;
	inverse.m[15] = Det3x3sub(m, 0, 1, 2, 4, 5, 6, 8, 9, 10) / det;

	return inverse;
}

Mat4x4 Look(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
	Vec3 Z = Normalize(eye - center);
	Vec3 X = Normalize(Cross(up, Z));
	Vec3 Y = Cross(Z, X);

	Mat4x4 view;

	view.m[0] = X.x;
	view.m[1] = Y.x;
	view.m[2] = Z.x;
	view.m[4] = X.y;
	view.m[5] = Y.y;
	view.m[6] = Z.y;
	view.m[8] = X.z;
	view.m[9] = Y.z;
	view.m[10] = Z.z;
	view.m[12] = -Dot(X, eye);
	view.m[13] = -Dot(Y, eye);
	view.m[14] = -Dot(Z, eye);

	return view;
}

Mat4x4 Ortho(float left, float right, float bottom, float top, float n, float f)
{
	Mat4x4 ortho;

	ortho.m[0] = 2.0f / (right - left);
	ortho.m[5] = 2.0f / (top - bottom);
	ortho.m[10] = -2.0f / (f - n);
	ortho.m[12] = -(right + left) / (right - left);
	ortho.m[13] = -(top + bottom) / (top - bottom);
	ortho.m[14] = -(f + n) / (f - n);

	return ortho;
}

Mat4x4 Perspective(float fovy, float aspect, float n, float f)
{
	Mat4x4 perspective;

	float coty = 1.0f / tan(fovy * (float)M_PI / 360.0f);

	perspective.m[0] = coty / aspect;
	perspective.m[5] = coty;
	perspective.m[10] = (n + f) / (n - f);
	perspective.m[11] = -1.0f;
	perspective.m[14] = 2.0f * n * f / (n - f);
	perspective.m[15] = 0.0f;

	return perspective;
}

Mat4x4 Rotate(float angle, const Vec3 &u)
{
	Mat4x4 rotate;

	angle = angle / 180.0f * (float)M_PI;

	Vec3 v = Normalize(u);

	float c = 1.0f - cos(angle), s = sin(angle);

	rotate.m[0] = 1.0f + c * (v.x * v.x - 1.0f);
	rotate.m[1] = c * v.x * v.y + v.z * s;
	rotate.m[2] = c * v.x * v.z - v.y * s;
	rotate.m[4] = c * v.x * v.y - v.z * s;
	rotate.m[5] = 1.0f + c * (v.y * v.y - 1.0f);
	rotate.m[6] = c * v.y * v.z + v.x * s;
	rotate.m[8] = c * v.x * v.z + v.y * s;
	rotate.m[9] = c * v.y * v.z - v.x * s;
	rotate.m[10] = 1.0f + c * (v.z * v.z - 1.0f);

	return rotate;
}

Mat4x4 Scale(float x, float y, float z)
{
	Mat4x4 scale;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;

	return scale;
}

Mat4x4 Translate(float x, float y, float z)
{
	Mat4x4 translate;

	translate.m[12] = x;
	translate.m[13] = y;
	translate.m[14] = z;

	return translate;
}

Mat4x4 Transpose(const Mat4x4 &matrix)
{
	Mat4x4 transpose;

	transpose.m[0] = matrix.m[0];
	transpose.m[1] = matrix.m[4];
	transpose.m[2] = matrix.m[8];
	transpose.m[3] = matrix.m[12];
	transpose.m[4] = matrix.m[1];
	transpose.m[5] = matrix.m[5];
	transpose.m[6] = matrix.m[9];
	transpose.m[7] = matrix.m[13];
	transpose.m[8] = matrix.m[2];
	transpose.m[9] = matrix.m[6];
	transpose.m[10] = matrix.m[10];
	transpose.m[11] = matrix.m[14];
	transpose.m[12] = matrix.m[3];
	transpose.m[13] = matrix.m[7];
	transpose.m[14] = matrix.m[11];
	transpose.m[15] = matrix.m[15];

	return transpose;
}
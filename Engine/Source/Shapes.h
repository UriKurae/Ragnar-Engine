#pragma once
#include "glmath.h"
#include "Color.h"
#include <vector>
#include "Math/float4x4.h"
#include "glmath.h"

enum ShapesTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Capsule,
	Primitive_Pyramid,
};

class Shape
{
public:

	Shape();
	~Shape() {};

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetPos(float3 pos);
	void			SetScale(float x, float y, float z);
	void			SetScale(float3 scale);
	void			FromRS(const Quat& rotate, const float3& scale);
	float4x4		mat4x4ToFloat4x4();

	ShapesTypes	GetType() const;

public:
	
	Color color;
	Mat4x4 transform;
	bool axis,wire;

protected:
	ShapesTypes type;
};

// ============================================
class PCube : public Shape
{
public :
	PCube();
	PCube(float3 _size, float3 pos = float3::zero);
	PCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size = {1, 1, 1 };
};

// ============================================
class PSphere : public Shape
{
public:
	PSphere();
	PSphere(float radius);
	PSphere(float radius, int sectors, int stacks);

public:
	float radius = 0.5f;
	int sectors = 36;
	int stacks = 18;
};

// ============================================
class PCylinder : public Shape
{
public:
	PCylinder();
	PCylinder(float radius, float height, int sectorCount = 24);
	void InnerRender() const;

public:
	float radius = 1;
	float height = 1;
	int sectorCount = 24;
};

class PCapsule : public Shape
{
public:
	PCapsule();
	PCapsule(float radius, float height);

public:
	float radius = 1;
	float height = 1;
};

class PPyramid : public Shape
{
public:
	PPyramid();
	PPyramid(float radius, float height);

public:
	float radius = 1;
	float height = 1;
};

// ============================================
class PLine : public Shape
{
public:
	PLine();
	PLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class PPlane : public Shape
{
public:
	PPlane();
	PPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal = { 0,1,0 };
	float constant = 0;
};
#include "Application.h"
#include "GL/glew.h"
#include "Shapes.h"
#include <cmath>
#include "Math/float3x3.h"
#include "Math/TransformOps.h"

// ------------------------------------------------------------
Shape::Shape() : transform(identityMatrix), color(white), wire(false), axis(false), type(ShapesTypes::Primitive_Point)
{
}

// ------------------------------------------------------------
ShapesTypes Shape::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Shape::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.m);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Shape::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Shape::SetPos(float x, float y, float z)
{
	transform.Translate(x, y, z);
}

void Shape::SetPos(float3 pos)
{
	transform.Translate(pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------
void Shape::SetScale(float x, float y, float z)
{
	transform.Scale(x, y, z);
}

void Shape::SetScale(float3 scale)
{
	transform.Scale(scale.x, scale.y, scale.z);
}

void Shape::FromRS(const Quat& rotate, const float3& scale)
{
	int k = 0;
	float3x3 RS = float3x3(rotate) * float3x3::Scale(scale);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			transform.m[k++] = RS[j][i];
		}
		k++;
	}
}

float4x4 Shape::mat4x4ToFloat4x4()
{
	float4x4 trans;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			trans[j][i] = transform[k++];
		}
	}
	return trans;
}

// CUBE ============================================
PCube::PCube() : Shape()
{
	type = ShapesTypes::Primitive_Cube;
}

PCube::PCube(float3 size, float3 pos) : Shape(), size(size)
{
	type = ShapesTypes::Primitive_Cube;
	glTranslatef(pos.x, pos.y, pos.z);
}

PCube::PCube(float sizeX, float sizeY, float sizeZ) : size(sizeX, sizeY, sizeZ)
{
	type = ShapesTypes::Primitive_Cube;
}

void PCube::InnerRender() const
{
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f(sx, -sy, sz);
	glVertex3f(sx, sy, sz);
	glVertex3f(-sx, sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, sy, -sz);
	glVertex3f(sx, sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy, sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx, sy, -sz);
	glVertex3f(sx, sy, sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy, sz);
	glVertex3f(-sx, sy, sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy, sz);
	glVertex3f(sx, sy, sz);
	glVertex3f(sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx, -sy, sz);
	glVertex3f(-sx, -sy, sz);

	glEnd();
}

// SPHERE ============================================
PSphere::PSphere() : Shape()
{
	type = ShapesTypes::Primitive_Sphere;
}

PSphere::PSphere(float radius) : Shape(), radius(radius)
{
	type = ShapesTypes::Primitive_Sphere;
}

PSphere::PSphere(float radius, int sectors, int stacks) : Shape(), radius(radius), sectors(sectors), stacks(stacks)
{
	type = ShapesTypes::Primitive_Sphere;
}

// CYLINDER ============================================
PCylinder::PCylinder() : Shape()
{
	type = ShapesTypes::Primitive_Cylinder;
}

PCylinder::PCylinder(float radius, float height, int sectorCount) : Shape(), radius(radius), height(height), sectorCount(sectorCount)
{
	type = ShapesTypes::Primitive_Cylinder;
}

void PCylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);

	for (int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();
}

PCapsule::PCapsule()
{
	type = ShapesTypes::Primitive_Capsule;
}

PCapsule::PCapsule(float radius, float height) : Shape(), radius(radius), height(height)
{
	type = ShapesTypes::Primitive_Capsule;
}

// PYRAMID ============================================
PPyramid::PPyramid() : Shape()
{
	type = ShapesTypes::Primitive_Cylinder;
}

PPyramid::PPyramid(float radius, float height) : Shape(), radius(radius), height(height)
{
	type = ShapesTypes::Primitive_Cylinder;
}

// LINE ==================================================
PLine::PLine() : Shape(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = ShapesTypes::Primitive_Line;
}

PLine::PLine(float x, float y, float z) : Shape(), origin(0, 0, 0), destination(x, y, z)
{
	type = ShapesTypes::Primitive_Line;
}

void PLine::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
PPlane::PPlane() : Shape(), normal(0, 1, 0), constant(1)
{
	type = ShapesTypes::Primitive_Plane;
}

PPlane::PPlane(float x, float y, float z, float d) : Shape(), normal(x, y, z), constant(d)
{
	type = ShapesTypes::Primitive_Plane;
}

void PPlane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 50.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}
#include "Primitive.h"

#include "glew/include/GL/glew.h"
#include "Globals.h"

#include "Profiling.h"

PGrid::PGrid(float w, float h) : width(w), height(h), Primitive()
{
	CreateGrid();
	transform = { 0.0f, 0.0f, 0.0f };
	vertex = new VertexBuffer(vertices.data(), sizeof(float3) * vertices.size());
}

PGrid::~PGrid()
{
	RELEASE(vertex);
}

void PGrid::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	vertex->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_LINES, 0, vertex->GetCount());

	DrawAxis();

	vertex->Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);

}

void PGrid::CreateGrid()
{
	
	for (int j = 0; j < width; ++j)
	{
		float3 line = {};
		line = { (-width / 2) + (j), 0, (-width / 2)};
		vertices.push_back(line);

		line = { (-width / 2) + (j), 0, (width / 2) };
		vertices.push_back(line);
	}

	for (int i = 0; i < height; ++i)
	{
		float3 line = {};
		line = { (-width / 2), 0, (-width / 2) + (i) };
		vertices.push_back(line);

		line = { (width / 2), 0, (-width / 2) + (i) };
		vertices.push_back(line);
	}	
}

void Primitive::DrawAxis()
{
	glLineWidth(3.0f);

	glBegin(GL_LINES);

	// X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x + 1.0f, transform.y, transform.z);

	// Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x, transform.y + 1.0f, transform.z);

	// X axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(transform.x, transform.y, transform.z);
	glVertex3f(transform.x, transform.y, transform.z + 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);


	glEnd();

	glLineWidth(1.0f);
}
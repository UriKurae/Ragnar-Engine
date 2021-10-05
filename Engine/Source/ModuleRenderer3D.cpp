#include "Application.h"
#include "ModuleRenderer3D.h"

#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "glew/include/GL/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "mmgr/mmgr.h"

ModuleRenderer3D::ModuleRenderer3D(bool startEnabled) : Module(startEnabled)
{
	name = "Renderer";
	context = NULL;

	depthTest = true;
	cullFace = true;
	colorMaterial = true;
	lighting = true;
	texture2D = true;
	stencil = false;
	blending = false;

	wireMode = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JsonParsing& node)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			LOG("Error: %s\n", glewGetErrorString(err));
		}

		LOG("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();
		
		ImGui_ImplSDL2_InitForOpenGL(app->window->window, context);
		ImGui_ImplOpenGL3_Init();

		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(*app->window->GetWindowWidth(), *app->window->GetWindowHeight());

	cubeId = 0;
	glGenBuffers(1, &cubeId);

	index = 0;
	glGenBuffers(1, &index);

	PCube* cube = new PCube({0,0,0}, {0,0,0}, {1,1,1});
	primitives.push_back(cube);

	PPyramid* pyramid = new PPyramid({ 4,0,0 }, { 0,0,0 }, { 1,1,1 });
	primitives.push_back(pyramid);
	
	cyl = new PCylinder(50, 2.0f, 1.0f);
	//primitives.push_back(cyl);
	
	// Bind cylinder
	//glGenBuffers(1, &cylinder);
	//glBindBuffer(GL_VERTEX_ARRAY, cyl->GetIndexBuffer());
	//cyl->BuildVerticesSmooth();
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* cyl->indices.size(), &cyl->indices, GL_STATIC_DRAW);

	// Bind Sphere buffer and add data for it
	sphere = 0;
	glGenBuffers(1, &sphere);
	DrawSphere(1.0f, 20, 20);
	glBindBuffer(GL_VERTEX_ARRAY, sphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices, GL_STATIC_DRAW);
	
	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(app->camera->position.x, app->camera->position.y, app->camera->position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	return true;
}

bool ModuleRenderer3D::Update(float dt)
{
	// Uncomment for direct mode Opengl Cube
	//DrawCubeDirectMode();

	// Uncomment for Vertex Array
	//GLfloat vertices[108] =
	//{
	//	// Front
	//	1.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,

	//	0.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,

	//	// Right
	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f,-1.0f,

	//	1.0f, 0.0f,-1.0f,
	//	1.0f, 1.0f,-1.0f,
	//	1.0f, 1.0f, 0.0f,

	//	// top
	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f,-1.0f,
	//	0.0f, 1.0f,-1.0f,

	//	0.0f, 1.0f,-1.0f,
	//	0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,

	//	// Back
	//	0.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f, 0.0f,

	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f,-1.0f,

	//	// Left
	//	0.0f, 0.0f,-1.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,

	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f,-1.0f,
	//	0.0f, 0.0f,-1.0f,

	//	// Bottom
	//	0.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f, 0.0f,

	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f,-1.0f
	//};

	//
	//// Bind Buffer and store data
	//glBindBuffer(GL_ARRAY_BUFFER, cubeId);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * 3, vertices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, cubeId);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDisableClientState(GL_VERTEX_ARRAY);
	
	// Uncomment for Index array	
	/*GLfloat indexVertex[24] =
	{
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		0.0f, 1.0f,-1.0f,
		0.0f, 0.0f,-1.0f
	};

	GLuint indices[36] =
	{
		0,1,2,
		2,3,0,

		0,3,4,
		4,5,0,

		0,5,6,
		6,1,0,

		7,6,5,
		5,4,7,

		7,2,1,
		1,6,7,

		7,4,3,
		3,2,7
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, indices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, &indexVertex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);*/
	

	// Uncomment for Sphere
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &cyl->vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &cyl->normals[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &cyl->texCoords[0]);
	glDrawElements(GL_TRIANGLES, cyl->indices.size(), GL_UNSIGNED_INT, &cyl->indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return true;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	//for (int i = 0; i < primitives.size(); ++i)
	//{
	//	primitives[i]->Draw();
	//}

	app->editor->Draw();

	SDL_GL_SwapWindow(app->window->window);

	return true;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (int i = 0; i < primitives.size(); ++i)
	{
		RELEASE(primitives[i]);
	}

	primitives.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::LoadConfig(JsonParsing& node)
{
	return true;
}

bool ModuleRenderer3D::SaveConfig(JsonParsing& node) const
{
	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = Perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetDepthTest()
{
	if (depthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::SetCullFace()
{
	if (cullFace) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SetLighting()
{
	if (lighting) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::SetColorMaterial()
{
	if (colorMaterial) glEnable(GL_COLOR_MATERIAL);
	else glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetTexture2D()
{
	if (texture2D) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetStencil()
{
	if (stencil) glEnable(GL_STENCIL_TEST);
	else glDisable(GL_STENCIL_TEST);
}

void ModuleRenderer3D::SetBlending()
{
	if (blending) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void ModuleRenderer3D::SetWireMode()
{
	//for (int i = 0; i < primitives.size(); ++i)
	//{
	//	primitives[i]->wire = wireMode;
	//}
}

void ModuleRenderer3D::DrawCubeDirectMode()
{
	GLfloat v0[3] = { 1.0f, 1.0f, 0.0f };
	GLfloat v1[3] = { 0.0f, 1.0f, 0.0f };
	GLfloat v2[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat v3[3] = { 1.0f, 0.0f, 0.0f };
	GLfloat v4[3] = { 1.0f, 0.0f,-1.0f };
	GLfloat v5[3] = { 1.0f, 1.0f,-1.0f };
	GLfloat v6[3] = { 0.0f, 1.0f,-1.0f };
	GLfloat v7[3] = { 0.0f, 0.0f,-1.0f };

	glBegin(GL_TRIANGLES);

	// Front
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v2);
	glVertex3fv(v3);
	glVertex3fv(v0);

	// Right
	glVertex3fv(v0);
	glVertex3fv(v3);
	glVertex3fv(v4);

	glVertex3fv(v4);
	glVertex3fv(v5);
	glVertex3fv(v0);

	// Top
	glVertex3fv(v0);
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v6);
	glVertex3fv(v1);
	glVertex3fv(v0);

	// Back
	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v5);

	glVertex3fv(v5);
	glVertex3fv(v4);
	glVertex3fv(v7);

	// Left
	glVertex3fv(v7);
	glVertex3fv(v2);
	glVertex3fv(v1);

	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v7);

	// Bottom
	glVertex3fv(v7);
	glVertex3fv(v4);
	glVertex3fv(v3);

	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v7);

	glEnd();
}

void ModuleRenderer3D::DrawSphere(float radius, unsigned int rings, unsigned int sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);
	texcoords.resize(rings * sectors * 3);

	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	std::vector<GLfloat>::iterator t = texcoords.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

	/*	texcoords.push_back(s * S);
		texcoords.push_back(r * R);

		vertices.push_back(x * radius);
		vertices.push_back(y * radius);
		vertices.push_back(z * radius);

		normals.push_back(x);
		normals.push_back(y);
		normals.push_back(z);*/
	
		*t++ = s * S;
		*t++ = r * R;

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		
		/*indices.push_back(r * sectors + s);
		indices.push_back(r * sectors + (s + 1));
		indices.push_back((r + 1) * sectors + (s + 1));
		indices.push_back((r + 1) * sectors + s);*/
		
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}
}

void ModuleRenderer3D::AddPrimitive(Primitive* primitive)
{
	primitives.push_back(primitive);
}
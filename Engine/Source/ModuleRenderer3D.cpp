#include "Application.h"
#include "ModuleRenderer3D.h"

#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Framebuffer.h"
#include "glew/include/GL/glew.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Imgui/ImguiStyle.h"

#include "IL/ilut.h"

#include "Profiling.h"

ModuleRenderer3D::ModuleRenderer3D(bool startEnabled) : Module(startEnabled), mainCameraFbo(nullptr)
{
	name = "Renderer";
	context = NULL;
	fbo = nullptr;
	grid = nullptr;

	depthTest = true;
	cullFace = true;
	colorMaterial = true;
	lighting = true;
	texture2D = true;
	stencil = false;
	blending = false;
	wireMode = false;
	vsync = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	RELEASE(mainCameraFbo);
}

// Called before render is available
bool ModuleRenderer3D::Init(JsonParsing& node)
{
	DEBUG_LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		DEBUG_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			DEBUG_LOG("Error: %s\n", glewGetErrorString(err));
		}

		DEBUG_LOG("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		ilutRenderer(ILUT_OPENGL);

		//Use Vsync
		vsync = node.GetJsonBool("vsync");
		if(SDL_GL_SetSwapInterval(vsync) < 0)
			DEBUG_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		//ImGui::StyleColorsDark();
		ImGui::DarkTheme();

		ImGui_ImplSDL2_InitForOpenGL(app->window->window, context);
		ImGui_ImplOpenGL3_Init();

		DEBUG_LOG("Vendor: %s", glGetString(GL_VENDOR));
		DEBUG_LOG("Renderer: %s", glGetString(GL_RENDERER));
		DEBUG_LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		DEBUG_LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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
		
		GLfloat lightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat materialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);

		GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
		
		depthTest = node.GetJsonBool("depth test");
		cullFace = node.GetJsonBool("cull face");
		lighting = node.GetJsonBool("lighting");
		colorMaterial = node.GetJsonBool("color material");
		texture2D = node.GetJsonBool("texture 2D");
		stencil = node.GetJsonBool("stencil");
		blending = node.GetJsonBool("blending");
		wireMode = node.GetJsonBool("wire mode");

		lights[0].Active(true);
		
		if (depthTest) SetDepthTest();
		if (cullFace) SetCullFace();
		if (lighting) SetLighting();
		if (colorMaterial) SetColorMaterial();
		if (texture2D) SetTexture2D();
		if (stencil) SetStencil();
		if (blending) SetBlending();
		if (wireMode) SetWireMode();
	}

	// Uncomment for using framebuffer
	//glGenFramebuffers(1, &framebuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//glGenTextures(1, &textureColorbuffer);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1165, 641, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//// attach it to currently bound framebuffer object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//glBindTexture(GL_TEXTURE_2D, 0);

	//
	//glGenTextures(1, &rboDepthStencil);
	//glBindTexture(GL_TEXTURE_2D, rboDepthStencil);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 1165, 641, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepthStencil, 0);

	//glBindTexture(GL_TEXTURE_2D, 0);


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//if (err != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	DEBUG_LOG("Framebuffer is Incomplete. Error %s", glGetString(err));
	//}
	//else DEBUG_LOG("Framebuffer is Complete");

	//// Projection matrix for
	int w = *app->window->GetWindowWidth();
	int h = *app->window->GetWindowHeight();
	OnResize(w, h);

	
	//mainCameraFbo = new Framebuffer(w, h);
	fbo = new Framebuffer(w, h);
	

	grid = new PGrid(200, 200);

	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->matrixViewFrustum.Transposed().ptr());
	glPopMatrix();*/

	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(app->scene->mainCamera->matrixViewFrustum.Transposed().ptr());
	//glPopMatrix();

	return true;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	RG_PROFILING_FUNCTION("Rendering");

	// Editor Camera FBO
	
	fbo->Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	grid->Draw();
	app->scene->Draw();

	fbo->Unbind();
	
	app->editor->Draw(fbo);

	//// Camera Component FBO

	/*mainCameraFbo->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	app->editor->Draw(mainCameraFbo);
	mainCameraFbo->Unbind();*/


	SDL_GL_SwapWindow(app->window->window);

	return true;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	DEBUG_LOG("Destroying 3D Renderer");

	RELEASE(grid);
	RELEASE(fbo);
	RELEASE(mainCameraFbo);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::LoadConfig(JsonParsing& node)
{
	vsync = node.GetJsonBool("vsync");
	depthTest = node.GetJsonBool("depth test");
	cullFace = node.GetJsonBool("cull face");
	lighting = node.GetJsonBool("lighting");
	colorMaterial = node.GetJsonBool("color material");
	texture2D = node.GetJsonBool("texture 2D");
	stencil = node.GetJsonBool("stencil");
	blending = node.GetJsonBool("blending");
	wireMode = node.GetJsonBool("wire mode");

	SetVsync();
	SetDepthTest();
	SetCullFace();
	SetLighting();
	SetColorMaterial();
	SetTexture2D();
	SetStencil();
	SetBlending();
	SetWireMode();

	return true;
}

bool ModuleRenderer3D::SaveConfig(JsonParsing& node) const
{
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "vsync", vsync);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "depth test", depthTest);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "cull face", cullFace);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "lighting", lighting);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "color material", colorMaterial);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "texture 2D", texture2D);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "stencil", stencil);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "blending", blending);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "wire mode", wireMode);

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
	if (wireMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::SetVsync()
{
	SDL_GL_SetSwapInterval(vsync);
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
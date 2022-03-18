#include "ModuleRenderer3D.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleSceneManager.h"
#include "ModuleNavMesh.h"
#include "ModuleUI.h"

#include "LightComponent.h"
#include "TransformComponent.h"
#include "ButtonComponent.h"
#include "CheckBoxComponent.h"
#include "ImageComponent.h"
#include "SliderComponent.h"

#include "ResourceManager.h"
#include "Scene.h"
#include "Shader.h"
#include "Lights.h"
#include "Framebuffer.h"
#include "NavMeshBuilder.h"

#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "Imgui/ImguiStyle.h"
#include "IL/ilut.h"
#include "Geometry/LineSegment.h"

#include "Profiling.h"

ModuleRenderer3D::ModuleRenderer3D(bool startEnabled) : Module(startEnabled), mainCameraFbo(nullptr), rayCast(true)
{
	name = "Renderer";
	context = NULL;
	fbo = nullptr;

	depthTest = true;
	cullFace = true;
	colorMaterial = true;
	lighting = true;
	texture2D = true;
	stencil = true;
	blending = false;
	wireMode = false;
	vsync = false;
	rayCast = false;
	navMesh = false;
	drawGrid = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
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
		ImGuiIO& io = ImGui::GetIO(); 
		io.IniFilename = "Settings/imgui.ini";
		(void)io;
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
		
		depthTest = node.GetJsonBool("depth test");
		cullFace = node.GetJsonBool("cull face");
		lighting = node.GetJsonBool("lighting");
		colorMaterial = node.GetJsonBool("color material");
		texture2D = node.GetJsonBool("texture 2D");
		stencil = node.GetJsonBool("stencil");
		blending = node.GetJsonBool("blending");
		wireMode = node.GetJsonBool("wire mode");
		drawGrid = node.GetJsonBool("draw grid");
		
		if (depthTest) SetDepthTest();
		if (cullFace) SetCullFace();
		if (lighting) SetLighting();
		if (colorMaterial) SetColorMaterial();
		if (texture2D) SetTexture2D();
		if (stencil) SetStencil();
		if (blending) SetBlending();
		if (wireMode) SetWireMode();		
	}
	//// Projection matrix for
	int w = *app->window->GetWindowWidth();
	int h = *app->window->GetWindowHeight();
	OnResize(w, h);
	
	fbo = new Framebuffer(w, h, 1);
	fbo->Unbind();
	mainCameraFbo = new Framebuffer(w, h, 0);
	mainCameraFbo->Unbind();	

	grid.SetPos(0, 0, 0);
	grid.constant = 0;
	grid.axis = true;

	dirLight = new DirectionalLight();
	goDirLight = app->sceneManager->GetCurrentScene()->CreateGameObject(0);
	goDirLight->SetName("Directional Light");

	TransformComponent* tr = goDirLight->GetComponent<TransformComponent>();
	tr->SetPosition({ 50,50,50 });
	ComponentLight* lightComp = (ComponentLight*)goDirLight->CreateComponent(ComponentType::LIGHT);
	lightComp->SetLight(dirLight);

	return ret;
}

bool ModuleRenderer3D::PreUpdate(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Editor Camera FBO
	fbo->Bind();
	PushCamera(app->camera->matrixProjectionFrustum, app->camera->matrixViewFrustum);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(app->camera->matrixProjectionFrustum.Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->matrixViewFrustum.Transposed().ptr());
	return true;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	RG_PROFILING_FUNCTION("Rendering");

	std::set<GameObject*> objects;
	if(drawGrid) grid.Render();

	// TODO: wtf quadtree man.
	app->sceneManager->GetCurrentScene()->GetQuadtree().Intersect(objects, app->sceneManager->GetCurrentScene()->mainCamera);

	if (rayCast)
	{
		math::LineSegment line = app->camera->rayCastToDraw.ToLineSegment(50.0f);
		glLineWidth(2.5f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.64f, 0.0f);
		glVertex3f(line.a.x, line.a.y, line.a.z);
		glVertex3f(line.b.x, line.b.y, line.b.z);
		glColor3f(1.0f, 1.0f, 1.0f);

		glEnd();
		glLineWidth(1.0f);
	}

	GameObject* objSelected = app->editor->GetGO();
	if (app->camera->visualizeFrustum)
	{
		for (std::set<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if ((*it) != objSelected)(*it)->Draw(nullptr);
		}
	}
	else
	{
		app->sceneManager->GetCurrentScene()->Draw();
	}

	if (navMesh && app->navMesh->GetNavMeshBuilder() != nullptr)
	{
		app->navMesh->GetNavMeshBuilder()->DebugDraw();

		Pathfinder* pathfinding = app->navMesh->GetPathfinding();
		for (int i = 0; i < pathfinding->agents.size(); i++)
			pathfinding->RenderPath(app->navMesh->GetPathfinding()->agents[i]);
	}

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	if (stencil && objSelected && objSelected->GetActive())
	{
		glColor3f(0.25f, 0.87f, 0.81f);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		objSelected->DrawOutline();

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		if (depthTest) glEnable(GL_DEPTH_TEST);
		glColor3f(1.0f, 1.0f, 1.0f);
		objSelected->Draw(nullptr);
	}

	fbo->Unbind();
		
	// Camera Component FBO
	mainCameraFbo->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	PushCamera(app->sceneManager->GetCurrentScene()->mainCamera->matrixProjectionFrustum, app->sceneManager->GetCurrentScene()->mainCamera->matrixViewFrustum);

	for (std::set<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->Draw(app->sceneManager->GetCurrentScene()->mainCamera);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPopMatrix();

	// DRAW UI
	ButtonComponent* aux = nullptr;
	//CanvasComponent* aux1 = nullptr;
	CheckboxComponent* aux2 = nullptr;
	ImageComponent* aux3 = nullptr;
	//InputBoxComponent* aux4 = nullptr;
	SliderComponent* aux5 = nullptr;
	for (int a = 0; a < app->userInterface->UIGameObjects.size(); a++)
	{
		/* glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(frustum.ProjectionMatrix().Transposed().ptr());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(app->scene->mainCamera->matrixViewFrustum.Transposed().ptr()); */

		aux = app->userInterface->UIGameObjects[a]->GetComponent<ButtonComponent>();
		//aux1 = go->GetComponent<CanvasComponent>();
		aux2 = app->userInterface->UIGameObjects[a]->GetComponent<CheckboxComponent>();
		aux3 = app->userInterface->UIGameObjects[a]->GetComponent<ImageComponent>();
		//aux4 = go->GetComponent<InputBoxComponent>();
		aux5 = app->userInterface->UIGameObjects[a]->GetComponent<SliderComponent>();

		if (aux != nullptr)
		{

			app->userInterface->UIGameObjects[a]->Draw(nullptr);
			app->userInterface->RenderText(aux->GetButtonText().textt, aux->GetButtonText().X, aux->GetButtonText().Y, aux->GetButtonText().Scale, aux->GetButtonText().Color);
			aux = nullptr;
		}
		/* else if (aux1 != nullptr)
		{
			textExample = aux1->text;
			color.x = aux1->color.r;
			color.y = aux1->color.g;
			color.z = aux1->color.b;
			aux1 = nullptr;
		} */
		else if (aux2 != nullptr)
		{
			app->userInterface->UIGameObjects[a]->Draw(nullptr);
			aux2 = nullptr;
		}
		else if (aux3 != nullptr)
		{
			app->userInterface->UIGameObjects[a]->Draw(nullptr);
			aux3 = nullptr;
		}
		/* else if (aux4 != nullptr)
		{
			textExample = aux4->text;
			color.x = aux4->textColor.r;
			color.y = aux4->textColor.g;
			color.z = aux4->textColor.b;
			aux4 = nullptr;
		} */
		else if (aux5 != nullptr)
		{
			app->userInterface->UIGameObjects[a]->Draw(nullptr);
			aux5 = nullptr;
		}
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	mainCameraFbo->Unbind();

	// Draw both buffers
	app->editor->Draw(fbo, mainCameraFbo);

	SDL_GL_SwapWindow(app->window->window);

	return true;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	DEBUG_LOG("Destroying 3D Renderer");

	RELEASE(fbo);
	RELEASE(mainCameraFbo);
	//RELEASE(defaultShader);
	RELEASE(dirLight);

	for(auto& pl : pointLights)
	{
		delete pl;
		pl = nullptr;
	}
	pointLights.clear();

	for (auto& pl : spotLights)
	{
		delete pl;
		pl = nullptr;
	}
	spotLights.clear();

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
	navMesh = node.GetJsonBool("navmesh");
	drawGrid = node.GetJsonBool("draw grid");

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

bool ModuleRenderer3D::SaveConfig(JsonParsing& node)
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
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "navmesh", navMesh);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "draw grid", drawGrid);

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
	if (stencil)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}
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

Material* ModuleRenderer3D::GetDefaultMaterial()
{
	return defaultMaterial;
}

uint ModuleRenderer3D::GetDefaultShader()
{
	return defaultShader;
}

Shader* ModuleRenderer3D::AddShader(const std::string& path)
{
	//Shader* shader = new Shader(path);
	std::string p = path;
	ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, p, std::string());
	//shaders.push_back(shader);

	//if (path.find("default"))
	//{
	//	defaultShader = shader;
	//}
	//
	//return shader;
	return 0;
}

void ModuleRenderer3D::AddMaterial(Material* material)
{
	materials.emplace_back(material);
}

void ModuleRenderer3D::AddPointLight(PointLight* pl)
{
	pointLights.push_back(pl);
}

void ModuleRenderer3D::AddSpotLight(SpotLight* sl)
{
	spotLights.push_back(sl);
}

void ModuleRenderer3D::ClearPointLights()
{
	pointLights.clear();
}

void ModuleRenderer3D::ClearSpotLights()
{
	spotLights.clear();
}

void ModuleRenderer3D::RemovePointLight(PointLight* light)
{
	for (std::vector<PointLight*>::iterator it = pointLights.begin(); it != pointLights.end(); ++it)
	{
		if ((*it) == light)
		{
			delete light;
			light = 0;
			*it = 0;
			pointLights.erase(it);
			break;
		}
	}
}

void ModuleRenderer3D::PushCamera(const float4x4& proj, const float4x4& view)
{
	//glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(proj.Transposed().ptr());

	//glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(view.Transposed().ptr());
}

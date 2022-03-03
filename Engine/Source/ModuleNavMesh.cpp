#include "Application.h"
#include "Globals.h"
#include "ModuleNavMesh.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"

#define MAX_KEYS 300

ModuleNavMesh::ModuleNavMesh(bool startEnabled) : Module(startEnabled)
{
	name = "NavMesh";
}

// Destructor
ModuleNavMesh::~ModuleNavMesh()
{

}

// Called before render is available
bool ModuleNavMesh::Init(JsonParsing& node)
{
	bool ret = true;

	return ret;
}

// Called every draw update
bool ModuleNavMesh::PreUpdate(float dt)
{

	return true;
}

// Called before quitting
bool ModuleNavMesh::CleanUp()
{

	return true;
}

bool ModuleNavMesh::LoadConfig(JsonParsing& node)
{
	return true;
}

bool ModuleNavMesh::SaveConfig(JsonParsing& node)
{
	return true;
}

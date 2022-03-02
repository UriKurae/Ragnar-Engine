#pragma once

#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postProcess.h"
#include "GameObject.h"
class Component;

class ComponentTransform2D : public Component
{
public:
	ComponentTransform2D(GameObject* theObject, float3 pos, float3 sca, float3 rot, GameObject* Own);
	ComponentTransform2D(float3 pos, float3 sca, float3 rot, GameObject* Own);
	~ComponentTransform2D();
	/*ComponentType getComponentType();*/
	void Enable() override;
	void Update(bool releaseMouse);
	void Disable() override;
	void OnEditor() override;
	/*void Load(const char* path) override;
	void Save(const char* path) override;*/
	Quat FromEulerToQuat(float3 eulerAngles);
	float3 FromQuatToEuler(Quat quatAngles);
	std::vector<float3> vertices_aux;
	float4x4 transMatrix;
	void setOwner();
	//void CreateAABB(ComponentType type, GameObject* go, bool firstTime)override;
	void setLast();
	float buttonWidth;
	float buttonHeight;
	Quat rotationQuat;
	float4x4 transmat;
	float* matrix;
	/*float3 lastRotation;
	float3 lastPosition;
	float3 lastScale;
	float3 position;*/

	//float3 scale;
	float3 rotationEuler;
	float3 position;
	float3 scale;
	float3 lastScale;
	float generalScale;
	float lastGeneralScale;
	float3 lastPosition;
	float3 lastRotation;
	GameObject* own;
};

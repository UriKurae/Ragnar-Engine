#pragma once

#include "ModuleNavMesh.h"
#include "MonoManager.h"

#include "NavAgentComponent.h"

#include "TransformBindings.h"

MonoArray* CalculateAgentPath(MonoObject* go, MonoObject* dest)
{
	NavAgentComponent* agent = GetComponentMono<NavAgentComponent*>(go);
	float3 destination = app->moduleMono->UnboxVector(dest);
	std::vector<float3> wayPoints = agent->pathfinding->CalculatePath(agent, destination);

	MonoClass* vecClass = mono_class_from_name(app->moduleMono->image, SCRIPTS_NAMESPACE, "Vector3");
	MonoArray* ret = mono_array_new(app->moduleMono->domain, vecClass, wayPoints.size());

	for (int i = 0; i < wayPoints.size(); ++i)
		mono_array_set(ret, MonoObject*, i, app->moduleMono->Float3ToCS(wayPoints[i]));
	
	return ret;
}

MonoBoolean GetAgentTargetSetted(MonoObject* go)
{
	NavAgentComponent* agent = GetComponentMono<NavAgentComponent*>(go);
	return agent->agentProperties->targetPosSet;
}

bool SetAgentPath(MonoObject* go, MonoArray* path)
{
	NavAgentComponent* agent = GetComponentMono<NavAgentComponent*>(go);

	std::vector<float3> wayPoints;
	int size = mono_array_length(path);
	for (int i = 0; i < size; ++i)
	{
		float3 v = app->moduleMono->UnboxVector(mono_array_get(path, MonoObject*, i));
		wayPoints.push_back(v);
	}

	return agent->pathfinding->SetPath(agent, wayPoints);
}

MonoObject* GetAgentDestination(MonoObject* go)
{
	NavAgentComponent* agent = GetComponentMono<NavAgentComponent*>(go);
	return app->moduleMono->Float3ToCS(agent->agentProperties->targetPos);
}

bool MoveAgentTo(MonoObject* go, MonoObject* dest)
{
	NavAgentComponent* agent = GetComponentMono<NavAgentComponent*>(go);
	float3 destination = app->moduleMono->UnboxVector(dest);
	return agent->pathfinding->MoveTo(agent, destination);
}
#pragma once
#include "GameObject.h"
#include "glmath.h"

class Particle {

public:

	// This constructor is made to create the particle reference only
	Particle(GameObject* parent) {
		lifeTime = 1.5f;
		rotation = { 90.0f,0.0f,0.0f };
		rotationQuat = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
		acceleration = { 0.0f,0.0f,0.0f };
		size = { 0.5f,0.5f,0.5f };
		velocity = { 0.0f,0.0f,0.0f };
		position = { 0.0f,0.0f,0.0f };
		color = { 0, 0, 0 };
		tex = nullptr;
		isActive = true;
	}

	// This constructor should be used by default
	Particle(Particle* particleReference, GameObject* parent)
	{
		lifeTime = particleReference->lifeTime;
		rotation = particleReference->rotation;
		acceleration = particleReference->acceleration;
		size = particleReference->size;
		velocity = particleReference->velocity;
		position = particleReference->position;
		color = particleReference->color;
		tex = particleReference->tex;
		isActive = particleReference->isActive;
	}

	~Particle() {}

	bool OnLoad(JsonParsing& node)
	{
		lifeTime = node.GetJsonNumber("Particle: LifeTime");
		//rotation = node.GetJson3Number(node, "Particle: Rotation");
		acceleration = node.GetJson3Number(node, "Particle: Acceleration");
		size = node.GetJson3Number(node, "Particle: Size");
		velocity = node.GetJson3Number(node, "Particle: Velocity");
		position = node.GetJson3Number(node, "Particle: Position");
		isActive = node.GetJsonBool("Particle: Is Active");
		return true;
	}

	bool OnSave(JsonParsing& node, JSON_Array* array)
	{
		JsonParsing file = JsonParsing();
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Particle: LifeTime", lifeTime);
		//file.SetNewJson3Number(file, "Particle: Rotation", Quat:: rotation);
		file.SetNewJson3Number(file, "Particle: Acceleration", acceleration);
		file.SetNewJson3Number(file, "Particle: Size", size);
		file.SetNewJson3Number(file, "Particle: Velocity", velocity);
		file.SetNewJson3Number(file, "Particle: Position", position);
		file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Particle: Is Active", isActive);
		node.SetValueToArray(array, file.GetRootValue());
		return true;
	}

public:

	float lifeTime;
	Quat rotationQuat;
	float3 rotation, acceleration;
	float3 size;
	float3 velocity, position;
	Vec4 color;
	bool isActive;
	std::shared_ptr<Texture> tex;
	GameObject* plane;
};


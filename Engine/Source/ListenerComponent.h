#pragma once

#include "Component.h"

class TransformComponent;
class IndexBuffer;
class VertexBuffer;

class ListenerComponent : public Component
{
public:
	ListenerComponent(GameObject* own, TransformComponent* trans);
	~ListenerComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;
private:
	TransformComponent* transform;

	bool activeListener;
};
#pragma once
#include "Component.h"

class TransformComponent;

class ListenerComponent : public Component
{
public:
	ListenerComponent(GameObject* own, TransformComponent* trans);
	~ListenerComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	inline void ChangePosition() { changePosition = true; }
private:
	TransformComponent* transform;
	bool changePosition;
	bool activeListener;
};
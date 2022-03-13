#pragma once

#include "Component.h"

class TransformComponent;
class ActionMaps;

class InputActionComponent : public Component
{
public:

	InputActionComponent(GameObject* own);
	~InputActionComponent();

	void OnEditor() override;
	bool Update(float dt) override;

	bool OnLoad(JsonParsing& node) override;
	bool OnSave(JsonParsing& node, JSON_Array* array) override;

	bool LoadInputAsset(const char* path);

private:

	std::vector<ActionMaps*> currentActionMaps;
	std::string currentAssetName;

	bool assetWindowActive;

};
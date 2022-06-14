#pragma once
#include "Menu.h"
#include <vector>

class GameObject;

enum class ItemType {
	NONE = 0,
	TAG = 1,
	LAYER = 2
};

class InspectorMenu : public Menu
{
public:
	InspectorMenu();
	~InspectorMenu();

	bool Update(float dt);

	std::vector<std::string> GetTags() { return tags; };
	std::vector<std::string> GetLayers() { return layers; };

	void SetTags(std::vector<std::string> tag) { tags = tag; };
	void SetLayers(std::vector<std::string> lay) { layers = lay; };

private:
	void DrawDefaultInspector(GameObject* obj);
	void DrawEditLists();

	std::string DrawList(const char* label, std::vector<std::string>* list, std::string& item);
	void AddItem(const char* label);

	void DrawListTagLayer(const char* label, std::vector<std::string>& list);
	void AddTag(std::string newTag);
	void AddLayer(std::string newLayer);

	std::vector<std::string> tags;
	std::vector<std::string> layers;
	char newTag[16] = "";
	char newLayer[16] = "";

	ItemType item = ItemType::NONE;
};
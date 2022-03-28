
#include "Application.h"
#include "FileSystem.h"
#include "InputActionComponent.h"
#include "InputActionMenu.h"
#include "MonoManager.h"
#include "Globals.h"

#include "IconsFontAwesome5.h"
#include "Profiling.h"
#include <ScriptComponent.cpp>

InputActionComponent::InputActionComponent(GameObject* own)
{
	owner = own;
	type = ComponentType::INPUT_ACTION;
	assetWindowActive = false;
}

InputActionComponent::~InputActionComponent()
{
}

void InputActionComponent::OnEditor()
{
	ImGui::PushID(this);
	if (ImGui::CollapsingHeader(ICON_FA_GAMEPAD" Player Input"))
	{
		ImGui::Text("Actions:");
		ImGui::SameLine();
		std::string text = currentAssetName + (std::string)ICON_FA_EDIT;
		auto posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(text.c_str()).x
			- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
		if (posX > ImGui::GetCursorPosX())
			ImGui::SetCursorPosX(posX);
		ImGui::Text(currentAssetName.c_str());
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_EDIT))
		{
			assetWindowActive = true;
			inputAssetsList.clear();
			LoadAllInputAssets("Assets");
		}

		if (currentActionMaps.size() > 0)
		{
			if (ImGui::TreeNodeEx("Events", ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (size_t i = 0; i < currentActionMaps.size(); i++)
				{
					ImGui::PushID(i);
					if (ImGui::TreeNodeEx(currentActionMaps[i]->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						for (size_t j = 0; j < currentActionMaps[i]->GetActions()->size(); j++)
						{
							ImGui::PushID(j);
							if (ImGui::TreeNodeEx(currentActionMaps[i]->GetActions()->at(j)->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
							{
								if (ImGui::BeginCombo("Script", scriptsNameList[i][j].c_str()))
								{
									const char* scriptName;
									for (int k = 0; k < app->moduleMono->userScripts.size(); k++)
									{
										scriptName = mono_class_get_name(app->moduleMono->userScripts[k]);
										if (ImGui::Selectable(scriptName))
										{
											scriptsNameList[i][j] = scriptName;
											scriptsMethodsList[i][j].clear();

											MonoClass* klass = mono_class_from_name(app->moduleMono->image, USER_SCRIPTS_NAMESPACE, scriptName);
											MonoMethodDesc* mdesc = mono_method_desc_new(":Start", false);
											monoMethodList[i][j] = mono_method_desc_search_in_class(mdesc, klass);
											mono_method_desc_free(mdesc);

											app->moduleMono->DebugAllMethods(USER_SCRIPTS_NAMESPACE, scriptName, scriptsMethodsList[i][j]);
										}
									}
									ImGui::EndCombo();
								}
								const char* methodName = nullptr;
								if (scriptsMethodsList[i][j].size() > 0)
								{
									methodName = scriptsMethodsList[i][j][currentMethodList[i][j]].c_str();
								}
								else
								{
									methodName = "";
								}
								if (ImGui::BeginCombo("Method", methodName))
								{
									const char* scriptName;
									for (int actualScript = 0; actualScript < scriptsMethodsList[i][j].size(); actualScript++)
									{
										scriptName = scriptsMethodsList[i][j][actualScript].c_str();
										if (ImGui::Selectable(scriptName))
										{
											currentMethodList[i][j] = actualScript;
											//scriptsNameList[i][j] = scriptName;
										}
									}
									ImGui::EndCombo();
								}
								ImGui::TreePop();
							}
							ImGui::PopID();
						}
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::PopID();

	if (assetWindowActive)
	{
		ImGui::Begin("Input Action Assets", &assetWindowActive, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x &&
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) assetWindowActive = false;
		}

		if (ImGui::BeginTable("table3", 3))
		{
			for (int item = 0; item < inputAssetsList.size(); item++)
			{
				ImGui::TableNextColumn();
				std::string assetName = inputAssetsList[item];
				app->fs->GetFilenameWithoutExtension(assetName);
				if (ImGui::Selectable(assetName.c_str()))
				{
					assetWindowActive = false;
					LoadInputAsset(inputAssetsList[item].c_str());
				}
			}
			ImGui::EndTable();
		}

		ImGui::End();
	}
}

bool InputActionComponent::Update(float dt)
{
	return false;
}

bool InputActionComponent::OnLoad(JsonParsing& node)
{
	currentActionMaps.clear();

	JSON_Array* jsonArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Action Maps");

	size_t size = node.GetJsonArrayCount(jsonArray);
	for (int i = 0; i < size; ++i)
	{
		JsonParsing go = node.GetJsonArrayValue(jsonArray, i);
		std::shared_ptr<ActionMaps> aM(new ActionMaps());
		aM->OnLoad(go);
		currentActionMaps.push_back(aM);
	}
	currentAssetName = node.GetJsonString("Path");

	return false;
}

bool InputActionComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	JSON_Array* newArray = file.SetNewJsonArray(file.GetRootValue(), "Action Maps");
	for (int i = 0; i < currentActionMaps.size(); i++)
	{
		currentActionMaps[i]->OnSave(file, newArray);
	}
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", currentAssetPath.c_str());

	node.SetValueToArray(array, file.GetRootValue());

	return false;
}

bool InputActionComponent::LoadInputAsset(const char* path)
{
	currentActionMaps.clear();
	scriptsNameList.clear();
	scriptsMethodsList.clear();
	currentMethodList.clear();
	monoMethodList.clear();

	JsonParsing sceneFile = JsonParsing();

	if (sceneFile.ParseFile(path) > 0)
	{
		JSON_Array* jsonArray = sceneFile.GetJsonArray(sceneFile.ValueToObject(sceneFile.GetRootValue()), "Action Maps");

		size_t size = sceneFile.GetJsonArrayCount(jsonArray);
		scriptsNameList.resize(size);
		scriptsMethodsList.resize(size);
		currentMethodList.resize(size);
		monoMethodList.resize(size);
		for (int i = 0; i < size; ++i)
		{
			JsonParsing go = sceneFile.GetJsonArrayValue(jsonArray, i);
			std::shared_ptr<ActionMaps> aM(new ActionMaps());
			aM->OnLoad(go);
			currentActionMaps.push_back(aM);
			scriptsNameList[i].resize(aM->GetActions()->size());
			scriptsMethodsList[i].resize(aM->GetActions()->size());
			currentMethodList[i].resize(aM->GetActions()->size());
			monoMethodList[i].resize(aM->GetActions()->size());
		}
		currentAssetPath = path;
		currentAssetName = path;
		app->fs->GetFilenameWithExtension(currentAssetName);
	}
	else
	{
		DEBUG_LOG("Input Asset couldn't be loaded");
	}
	return false;
}

void InputActionComponent::LoadAllInputAssets(const char* folder)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	std::string path = folder + std::string("/");
	app->fs->DiscoverFilesAndDirs(path.c_str(), files, dirs);
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if ((*it).find(".inputaction") != std::string::npos)
		{
			inputAssetsList.push_back(*it);
		}
	}
	for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		LoadAllInputAssets((*it).c_str());
	}
}

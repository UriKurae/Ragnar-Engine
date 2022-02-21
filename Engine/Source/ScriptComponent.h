#pragma once
#include "Component.h"
#include<vector>
#include<string>

#include "MonoManager.h"
#include <mono/metadata/object-forward.h>

class GameObject;
typedef struct _MonoClassField MonoClassField;

class ScriptComponent : public Component
{
public:

	ScriptComponent(GameObject* owner, const char* scriptName);
	virtual ~ScriptComponent();

	bool Update(float dt) override;

	bool OnLoad(JsonParsing& nObj) override;
	bool OnSave(JSON_Object* nObj);

	void SetField(MonoClassField* field, GameObject* value);
	void DropField(SerializedField& fieldName, const char* dropType);

	void LoadScriptData(const char*);

private:
//#ifndef STANDALONE
	void OnEditor() override;
	void DisplayField(SerializedField& field, const char* dropType);
//#endif // !STANDALONE

public:
	std::vector<std::string> methods;
	std::vector<SerializedField> fields;

	MonoMethod* updateMethod;
	uint32_t noGCobject;

	static ScriptComponent* runningScript;
};
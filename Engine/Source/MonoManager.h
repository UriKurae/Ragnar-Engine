#pragma once
#include "Application.h"
#include "Module.h"

#include <mono/utils/mono-publib.h>
#include "Math/float3.h"

#include<mono/metadata/object-forward.h>
#include <mono/metadata/blob.h>
#include<mono/metadata/threads.h>

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoClassField MonoClassField;

class GameObject;
class Component;
class ScriptComponent;

#define USER_SCRIPTS_NAMESPACE ""
#define SCRIPTS_NAMESPACE "RagnarEngine"

union FieldValue
{
	int iValue;
	float fValue;
	bool bValue;
	char strValue[50];
	GameObject* goValue;
	MonoArray* arrValue;
};
struct SerializedField
{
	SerializedField();
	SerializedField(MonoClassField* _field, MonoObject* _object, ScriptComponent* parent);

	MonoClassField* field;
	MonoTypeEnum type = MonoTypeEnum::MONO_TYPE_END;
	FieldValue fiValue;
	ScriptComponent* parentSC;
	std::string displayName;
};

class MonoManager : public Module
{
public:
	MonoManager(bool start_enabled = true);
	~MonoManager();

	bool Init(JsonParsing& node) override;

	bool CleanUp() override;

	static Quat UnboxQuat(MonoObject* _obj);
	static float3 UnboxVector(MonoObject* _obj);
	static float2 UnboxVector2D(MonoObject* _obj);

	static void LoadFieldData(SerializedField& _field, MonoObject* _object);
	static void DebugAllMethods(const char* nsName, const char* className, std::vector<std::string>& _data);
	static void DebugAllFields(const char* className, std::vector<SerializedField>& _data, MonoObject* obj, ScriptComponent* script, const char* nameSpace);

	void CreateAssetsScript(const char* localPath);
	void AddScriptToSLN(const char* scriptLocalPath);
	void RemoveScriptFromSLN(const char* scriptLocalPath);

	GameObject* GameObjectFromCSGO(MonoObject* goObj);
	GameObject* GameObjectFromCSComponent(MonoObject* goComponent);
	MonoObject* QuatToCS(Quat& inVec) const;
	MonoObject* Float3ToCS(float3& inVec) const;
	MonoObject* GoToCSGO(GameObject* inGo) const;
	MonoObject* ComponentToCS(Component* comp) const;

	void UpdateListScripts();

	void ReCompileCS();

public:
	MonoDomain* domain;
	MonoDomain* jitDomain;
	MonoAssembly* assembly;
	MonoImage* image;
	MonoThread* domainThread;

	std::vector<MonoClass*> userScripts;

private:
	void InitMono();

};
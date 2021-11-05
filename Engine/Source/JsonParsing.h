#pragma once

#include "json/parson.h"
#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Math/Quat.h"

class JsonParsing
{
public:
	JsonParsing();
	JsonParsing(const char* string);
	~JsonParsing();

	size_t Save(char** buf);

	JSON_Object* SetNewJsonNode(JSON_Object* parent, const char* nodeName) const;
	JSON_Status SetNewJsonString(JSON_Object* node, const char* name, const char* string) const;
	JSON_Status SetNewJsonNumber(JSON_Object* node, const char* name, double number) const;
	JSON_Status SetNewJson3Number(JsonParsing& node, const char* name, float3 number) const;
	JSON_Status SetNewJson4Number(JsonParsing& node, const char* name, Quat number) const;
	JSON_Status SetNewJsonBool(JSON_Object* node, const char* name, bool boolean) const;
	JSON_Array* SetNewJsonArray(JSON_Value* parent, const char* name) const;
	JSON_Status SetValueToArray(JSON_Array* jsonArray, JSON_Value* value);
	JsonParsing SetChild(JSON_Value* parent, const char* name);

	JSON_Object* GetJsonObject(JSON_Object* parentObject, const char* node) const;
	JSON_Value* GetRootValue() const;
	const char* GetJsonString(const char* name) const;
	double GetJsonNumber(const char* name) const;
	bool GetJsonBool(const char* name) const;
	JsonParsing GetChild(JSON_Value* parent, const char* name);

	//GetValor("Width")

	JSON_Value* ParseFile(const char* fileName);
	JSON_Object* ValueToObject(JSON_Value* value) const;
	JSON_Status SerializeFile(JSON_Value* value, const char* fileName) const;

	JSON_Value* InitObject();
private:

	JSON_Value* rootObject;

};

#pragma once

#include "json/parson.h"

class JsonParsing
{
public:
	JsonParsing();
	~JsonParsing();

	JSON_Object* SetNewJsonNode(JSON_Object* parent, const char* nodeName) const;
	JSON_Status SetNewJsonString(JSON_Object* node, const char* name, const char* string) const;
	JSON_Status SetNewJsonNumber(JSON_Object* node, const char* name, double number) const;
	JSON_Status SetNewJsonBool(JSON_Object* node, const char* name, bool boolean) const;
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

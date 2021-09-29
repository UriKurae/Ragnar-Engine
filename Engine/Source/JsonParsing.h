#pragma once

#include "json/parson.h"

class JsonParsing
{
public:
	JsonParsing();
	~JsonParsing();

	JSON_Object* SetObject(JSON_Object* object, const char* name);

	JSON_Object* GetObject(const JSON_Value& value) const;

	JSON_Value* InitObject();
private:

	JSON_Value* rootObject;

};

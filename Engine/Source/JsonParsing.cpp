#include "Globals.h"
#include "JsonParsing.h"

#include "mmgr/mmgr.h"

JsonParsing::JsonParsing()
{
	rootObject = json_value_init_object();
}

JsonParsing::JsonParsing(const char* string)
{
	if (string != nullptr)
	{
		rootObject = json_parse_string(string);
	}
}

JsonParsing::~JsonParsing()
{
}

size_t JsonParsing::Save(char** buf)
{
	size_t written = json_serialization_size(rootObject);
	*buf = new char[written];
	json_serialize_to_buffer(rootObject, *buf, written);
	return written;
}

JSON_Value* JsonParsing::InitObject()
{
	JSON_Value* object = json_value_init_object();
	rootObject = object;

	return object;
}


JSON_Object* JsonParsing::SetNewJsonNode(JSON_Object* parent, const char* nodeName) const
{
	json_object_set_value(parent, nodeName, json_value_init_object());;

	return json_object_get_object(ValueToObject(rootObject), nodeName);
}

JSON_Status JsonParsing::SetNewJsonString(JSON_Object* node, const char* name, const char* string) const
{
	return json_object_set_string(node, name, string);
}

JSON_Status JsonParsing::SetNewJsonNumber(JSON_Object* node, const char* name, double number) const
{
	return json_object_set_number(node, name, number);
}

JSON_Status JsonParsing::SetNewJsonBool(JSON_Object* node, const char* name, bool boolean) const
{
	return json_object_set_boolean(node, name, boolean);
}

JsonParsing JsonParsing::SetChild(JSON_Value* parent, const char* name)
{
	json_object_set_value(ValueToObject(parent), name, json_value_init_object());

	return GetChild(parent, name);
}

JSON_Value* JsonParsing::ParseFile(const char* fileName)
{
	JSON_Value* config = json_parse_file(fileName);
	rootObject = config;

	return config;
}

JSON_Object* JsonParsing::ValueToObject(JSON_Value* value) const
{
	JSON_Object* object = json_value_get_object(value);
	
	if(object != NULL)
		return object;

	LOG("Couldn't retrieve the object");

	return nullptr;
}

JSON_Status JsonParsing::SerializeFile(JSON_Value* value, const char* fileName) const
{
	return json_serialize_to_file_pretty(value, fileName);
}

JSON_Value* JsonParsing::GetRootValue() const
{
	return rootObject;
}

const char* JsonParsing::GetJsonString(const char* name) const
{
	return json_object_get_string(ValueToObject(rootObject), name);
}

double JsonParsing::GetJsonNumber(const char* name) const
{
	return json_object_get_number(ValueToObject(rootObject), name);
}

bool JsonParsing::GetJsonBool(const char* name) const
{
	return json_object_get_boolean(ValueToObject(rootObject), name);
}

JsonParsing JsonParsing::GetChild(JSON_Value* parent, const char* name)
{
	JsonParsing child; 
	
	child.rootObject = json_object_get_value(ValueToObject(rootObject), name);
	
	return child;
}

JSON_Object* JsonParsing::GetJsonObject(JSON_Object* parentObject, const char* node) const
{
	JSON_Object* object = json_object_get_object(parentObject, node);

	if (object) return object;

	LOG("Could not find node object");
	return nullptr;
}
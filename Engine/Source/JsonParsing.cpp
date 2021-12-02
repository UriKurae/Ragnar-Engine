#include "Globals.h"
#include "JsonParsing.h"

#include "Profiling.h"

JsonParsing::JsonParsing()
{
	rootObject = json_value_init_object();
}

JsonParsing::JsonParsing(const char* string)
{
	rootObject = nullptr;
	if (string != nullptr)
	{
		rootObject = json_parse_string(string);
	}
}

JsonParsing::JsonParsing(JSON_Value* value)
{
	rootObject = value;
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

size_t JsonParsing::SaveFile(const char* name)
{
	size_t written = json_serialization_size(rootObject);
	json_serialize_to_file(rootObject, name);
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

JSON_Status JsonParsing::SetNewJson3Number(JsonParsing& node, const char* name, float3 number) const
{
	JSON_Array* array = SetNewJsonArray(node.GetRootValue(), name);

	json_array_append_number(array, number.x);
	json_array_append_number(array, number.y);
	
	return json_array_append_number(array, number.z);
}

JSON_Status JsonParsing::SetNewJson4Number(JsonParsing& node, const char* name, Quat number) const
{
	JSON_Array* array = SetNewJsonArray(node.GetRootValue(), name);

	json_array_append_number(array, number.x);
	json_array_append_number(array, number.y);
	json_array_append_number(array, number.z);

	return json_array_append_number(array, number.w);
}

JSON_Status JsonParsing::SetNewJsonBool(JSON_Object* node, const char* name, bool boolean) const
{
	return json_object_set_boolean(node, name, boolean);
}

JSON_Array* JsonParsing::SetNewJsonArray(JSON_Value* parent, const char* name) const
{
	json_object_dotset_value(ValueToObject(parent), name, json_value_init_array());

	return json_object_dotget_array(ValueToObject(parent), name);
}

JSON_Status JsonParsing::SetValueToArray(JSON_Array* jsonArray, JSON_Value* value)
{
	return json_array_append_value(jsonArray, value);
}

JsonParsing JsonParsing::SetChild(JSON_Value* parent, const char* name)
{
	json_object_set_value(ValueToObject(parent), name, json_value_init_object());

	return GetChild(parent, name);
}

size_t JsonParsing::ParseFile(const char* fileName)
{
	JSON_Value* config = json_parse_file(fileName);
	rootObject = config;
	
	return json_serialization_size(rootObject);
}

JSON_Object* JsonParsing::ValueToObject(JSON_Value* value) const
{
	JSON_Object* object = json_value_get_object(value);
	
	if(object != NULL)
		return object;

	DEBUG_LOG("Couldn't retrieve the object");

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

JsonParsing JsonParsing::GetJsonArrayValue(JSON_Array* array, int index) const
{
	return JsonParsing(json_array_get_value(array, index));
}

float3 JsonParsing::GetJson3Number(JsonParsing& node, const char* name)
{
	JSON_Array* array = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), name);

	float x = json_array_get_number(array, 0);
	float y = json_array_get_number(array, 1);
	float z = json_array_get_number(array, 2);

	return float3(x,y,z);
}

float4 JsonParsing::GetJson4Number(JsonParsing& node, const char* name)
{
	JSON_Array* array = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), name);

	float x = json_array_get_number(array, 0);
	float y = json_array_get_number(array, 1);
	float z = json_array_get_number(array, 2);
	float w = json_array_get_number(array, 3);

	return float4(x, y, z, w);
}

JSON_Object* JsonParsing::GetJsonObject(JSON_Object* parentObject, const char* node) const
{
	JSON_Object* object = json_object_get_object(parentObject, node);

	if (object) return object;

	DEBUG_LOG("Could not find node object");
	return nullptr;
}

JSON_Array* JsonParsing::GetJsonArray(JSON_Object* parentObject, const char* node) const
{
	return json_object_dotget_array(parentObject, node);
}

size_t JsonParsing::GetJsonArrayCount(JSON_Array* array) const
{
	return json_array_get_count(array);
}
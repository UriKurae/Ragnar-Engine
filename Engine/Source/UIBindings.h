#pragma once

#include "ScriptComponent.h"

#include "ButtonComponent.h"
#include "SliderComponent.h"
#include "CheckBoxComponent.h"
#include "Transform2DComponent.h"
#include "TextComponent.h"

#include <metadata\object-forward.h>
#include <metadata\object.h>
#include <metadata/class.h>

MonoObject* Get2DPosition(MonoObject* go)
{
	ComponentTransform2D* tr = GetComponentMono<ComponentTransform2D*>(go);
	//float3 position = ;
	MonoObject* vec = app->moduleMono->Float3ToCS(tr->GetPosition());
	char* name = mono_string_to_utf8(mono_object_to_string(vec, 0));

	return vec;
}

void Set2DPosition(MonoObject* go, MonoObject* position)
{
	if (ComponentTransform2D* tr = GetComponentMono<ComponentTransform2D*>(go))
	{		
		tr->SetPosition(app->moduleMono->UnboxVector(position));
	}
}
 // Button ==============================
int GetButtonState(MonoObject* go)
{
	ButtonComponent* tr = GetComponentMono<ButtonComponent*>(go);
	//float3 position = ;
	

	return (int)tr->GetState();
}
void SetButtonText(MonoObject* go, MonoString* text)
{
	ButtonComponent* tr = GetComponentMono<ButtonComponent*>(go);
	tr->GetButtonText().textt.clear();
	char* aux = mono_string_to_utf8(text);
	tr->SetText(aux);
	
}
const char* GetButtonText(MonoObject* go)
{
	ButtonComponent* tr = GetComponentMono<ButtonComponent*>(go);
	return tr->GetButtonText().textt.c_str();
}

void SetTextPosition(MonoObject* go, float posx,float posy)
{
	ButtonComponent* tr = GetComponentMono<ButtonComponent*>(go);
	float3 i = { posx ,posy,0 };
	tr->SetTextPosition(i);



}
float3 GetTextPosition(MonoObject* go)
{
	const char* name = mono_class_get_name(mono_object_get_class(go));
	ButtonComponent* tr = GetComponentMono<ButtonComponent*>(go);
	//float3 position = ;


	return tr->GetTextPosition();
}

const bool GetIsChecked(MonoObject* go)
{
	CheckboxComponent* tr = GetComponentMono<CheckboxComponent*>(go);
	//float3 position = ;

	return tr->GetChecked();
}
int GetCheckboxState(MonoObject* go)
{
	CheckboxComponent* tr = GetComponentMono<CheckboxComponent*>(go);
	//float3 position = ;


	return (int)tr->GetState();
}

// Slider =========================================
float GetSliderActualValue(MonoObject* go)
{
	SliderComponent* tr = GetComponentMono<SliderComponent*>(go);
	//float3 position = ;


	return tr->GetBarProgress();
}

// Text ==================================
void SetText(MonoObject* go, MonoString* text)
{
	TextComponent* tr = GetComponentMono<TextComponent*>(go);
	char* aux = mono_string_to_utf8(text);
	tr->SetText(aux);

}
const char* GetText(MonoObject* go)
{
	TextComponent* tr = GetComponentMono<TextComponent*>(go);
	return tr->GetText();
}


// Transform 2D ========================================
Vec3 GetSize(MonoObject* go)
{
	ComponentTransform2D* tr = GetComponentMono<ComponentTransform2D*>(go);	
	Vec3 vec2(tr->GetButtonWidth(), tr->GetButtonHeight(),0);
	return vec2;
}

void SetSize(MonoObject* go, MonoObject* size)
{
	if (ComponentTransform2D* tr = GetComponentMono<ComponentTransform2D*>(go))
	{
		tr->SetButtonWidth(app->moduleMono->UnboxVector2D(size).x);
		tr->SetButtonHeight(app->moduleMono->UnboxVector2D(size).y);		
	}
}



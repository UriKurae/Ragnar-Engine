#include "ImageComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleInput.h"
#include "ModuleUI.h"
#include "Texture.h"
#include "GameObject.h"
#include "MaterialComponent.h"
#include "Transform2DComponent.h"
#include"ResourceManager.h"
#include "GL/glew.h"
void Animation::Update(float dt) 
{
	
	currentDt += dt;
	for (int a = 0;a < images.size();a++) {
		
		if (animatonState == a && currentDt >= timeBetwen) {
			currentDt =0;
			if (images.size() == a+1) {
				if(loop)
					animatonState = 0;
				else
					animatonState = -1;
			}
			else {
				animatonState++;
			}
		}
	}
	if (animatonState == -1) {
		isPlayng = false;
	}
}
MaterialComponent* Animation::Draw() 
{
	return images[animatonState];
}
ImageComponent::ImageComponent(GameObject* own)
{
	type = ComponentType::UI_IMAGE;
	own->isUI = true;
	this->text = "Image Component";

	if (!own->GetComponent<ComponentTransform2D>()) // If comes from Load not enter
	{
		own->CreateComponent(ComponentType::TRANFORM2D);
		principal=(MaterialComponent*)own->CreateComponent(ComponentType::MATERIAL);
	}

	app->userInterface->UIGameObjects.push_back(own);
	planeToDraw = new MyPlane(float3{ 0,0,0 }, float3{ 1,1,1 });
	planeToDraw->own = own;
	app->userInterface->OrderButtons();
}

ImageComponent::~ImageComponent()
{
	text.clear();
	RELEASE(planeToDraw);
}

bool ImageComponent::Update(float dt)
{
	for (int a = 0;a < animations.size();a++) {
		if(animations[a]->isPlayng)
			animations[a]->Update(dt);
	}
	return true;
}
void ImageComponent::SetActualColor(float Red, float Green, float Blue)
{
	actualColor.r = Red / 255;
	actualColor.g = Green / 255;
	actualColor.b = Blue / 255;
}
void ImageComponent::Draw(CameraComponent* gameCam)
{
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);
	for (int a = 0;a < animations.size();a++) {
		if (animations[a]->isPlayng)
			principal=animations[a]->Draw();
	}

	planeToDraw->DrawPlane2D(principal->GetTexture().get());

	glColor4f(actualColor.r, actualColor.g, actualColor.b, actualColor.a);
	planeToDraw->DrawPlane2D(owner->GetComponent<MaterialComponent>()->GetTexture().get());

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}
int ImageComponent::LoadTexture(std::string newTexture) 
{
	MaterialComponent* auxiliar = (MaterialComponent*)owner->CreateComponent(ComponentType::MATERIAL);
	ResourceManager* aux =ResourceManager::GetInstance();
	auxiliar->SetTexture(aux->LoadResource(std::string(newTexture)));
	materialList.push_back(auxiliar);

	
	return materialList.size();
}
void ImageComponent::UseTexture(int ID) 
{
	principal = materialList[ID-1];
}
void ImageComponent::OnEditor()
{
	if (ImGui::CollapsingHeader("ImageComponent"))
	{
		ImGui::SliderFloat("Alpha", &alpha, 0.5f, 1.0f);
		if (ImGui::Button("create animation")) {
			Animation* aux = new Animation();
			animations.push_back(aux);
		}
		for (int b = 0;b < animations.size();b++) {
			ImGui::Checkbox("isLoop", &animations[b]->loop);
			ImGui::InputFloat("time Betwen (seconds)", &animations[b]->timeBetwen);
			if (ImGui::Button("set image")) {
				MaterialComponent* matAux = (MaterialComponent*)owner->CreateComponent(ComponentType::MATERIAL);
				animations[b]->images.push_back(matAux);
			}
			for (int c = 0;c < animations[b]->images.size();c++) {
				animations[b]->images[c]->OnEditor();
			}
		}
		ComponentOptions(this);
		ImGui::Separator();
	}
}
bool ImageComponent::OnLoad(JsonParsing& node)
{
	alpha = node.GetJsonNumber("alpha");
	
	for (int a = 0; a < owner->components.size(); a++) {
		if (owner->components[a]->type == ComponentType::MATERIAL) 
		{
			principal = (MaterialComponent*)owner->components[a];
			break;
		}
		
	}
	
	return true;
}

bool ImageComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "alpha", alpha);
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

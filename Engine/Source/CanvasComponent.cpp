//#include "Application.h"
//#include "SDL.h"
//#include "CanvasComponent.h"
//#include"glew/include/GL/glew.h"
//CanvasComponent::CanvasComponent(int id, std::string text)
//{
//	name = "Canvas Component";
//	type = ComponentType::UI_CANVAS;
//	this->text = "Canvas Component";
//	state = State::NORMAL;
//	color = White;
//}
//
//CanvasComponent::~CanvasComponent()
//{
//	text.clear();
//}
//
//bool CanvasComponent::Update(float dt)
//{
//
//	if (!active)
//		state = State::DISABLED;
//	else
//		state = State::NORMAL;
//
//	if (state != State::DISABLED)
//	{
//		if (App->userInterface->focusedGameObject == owner)
//		{
//			state = State::FOCUSED;
//
//			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
//			{
//				state = State::PRESSED;
//
//				ComponentTransform2D* aux = owner->getTransform2D();
//				aux->position.x = App->userInterface->fMousePos.x - (App->editor->viewport.z / 2) + (aux->scale.x / 2) - 100;
//				aux->position.y = ((App->userInterface->fMousePos.y - (App->editor->viewport.w / 2)) * -1) - (aux->scale.y / 2) + 100;
//
//				aux->Update(false);
//			}
//		}
//		else state = State::NORMAL;
//
//		if (App->userInterface->UIGameObjectSelected == owner)
//		{
//			state = State::SELECTED;
//			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
//				OnClick();
//		}
//	}
//	return true;
//}
//
//void CanvasComponent::Draw()
//{
//	MyPlane* planeToDraw = nullptr;
//	int auxId = owner->id;
//
//	for (int i = 0; i < App->editor->planes.size(); i++)
//		if (App->editor->planes[i]->id == auxId) planeToDraw = App->editor->planes[i];
//
//	glAlphaFunc(GL_GREATER, 0.5);
//	glEnable(GL_ALPHA_TEST);
//
//	glColor4f(color.r, color.g, color.b, color.a);
//
//	planeToDraw->DrawPlane2D();
//
//	glDisable(GL_ALPHA_TEST);
//	glColor3f(255, 255, 255);
//}
//
//void CanvasComponent::OnEditor(int i)
//{
//	// Manage if colors are being edited or not
//	static bool normalEditable = false;
//
//	ImGui::Checkbox("Active", &active);
//
//	ImGui::Text("Image Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Image Color", ImVec4(color.r, color.g, color.b, color.a)))
//		normalEditable = !normalEditable;
//
//	if (normalEditable)
//	{
//		ImGui::ColorPicker3("Image Color", &color);
//	}
//}
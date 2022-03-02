//#include "InputBoxComponent.h"
//#include "SDL.h"
//#include"ModuleInput.h"
//#include "Application.h"
//#include "ModuleCamera3D.h"
//#include "CameraComponent.h"
//#include"glew/include/GL/glew.h"
//InputBoxComponent::InputBoxComponent(int id, std::string _text)
//{
//	name = "InputBox Component";
//	type = ComponentType::UI_INPUTBOX;
//	drawRect = false;
//	state = State::NORMAL;
//	aux.setText(_text, 5, 5, 0.5, { 255,255,255 });
//}
//
//InputBoxComponent::~InputBoxComponent()
//{
//	
//}
//
//void InputBoxComponent::Update()
//{
//	aux.SetOnlyPosition(float2(GetParentPosition().x, GetParentPosition().y));
//
//	if (state != State::DISABLED)
//	{
//		// Check collision between mouse and button bounds
//		if (app->userInterface->focusedGameObject == owner)
//		{
//			state = State::FOCUSED;
//			color = focusedColor;
//			
//			if (app->input->GetMouseButton(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
//			{
//				state = State::PRESSED;
//				color = pressedColor;
//				OnClick();
//			}
//		}
//		else
//		{
//			state = State::NORMAL;
//			color = normalColor;
//		}
//		if (app->userInterface->UIGameObjectSelected == owner)
//		{
//			state = State::SELECTED;
//			color = selectedColor;
//			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
//				OnClick();
//		}
//	}
//	else
//		color = disabledColor;
//
//	if (textEditable)
//	{
//		normalColor.Set(255, 0, 255, 255);
//		if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN && strlen(text) > 0)
//		{
//			text[strlen(text) - 1] = '\0';
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'Q';
//			else cadenaTemporal[0] = ' ';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'Q';
//			else cadenaTemporal[0] = 'q';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'W';
//			else cadenaTemporal[0] = 'w';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'E';
//			else cadenaTemporal[0] = 'e';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'R';
//			else cadenaTemporal[0] = 'r';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_T) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'T';
//			else cadenaTemporal[0] = 't';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_Y) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'Y';
//			else cadenaTemporal[0] = 'y';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_U) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'U';
//			else cadenaTemporal[0] = 'u';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_I) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'I';
//			else cadenaTemporal[0] = 'i';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_O) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'O';
//			else cadenaTemporal[0] = 'o';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_P) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'P';
//			else cadenaTemporal[0] = 'p';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'A';
//			else cadenaTemporal[0] = 'a';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'S';
//			else cadenaTemporal[0] = 's';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'D';
//			else cadenaTemporal[0] = 'd';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'F';
//			else cadenaTemporal[0] = 'f';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_G) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'G';
//			else cadenaTemporal[0] = 'g';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_H) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'H';
//			else cadenaTemporal[0] = 'h';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_J) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'J';
//			else cadenaTemporal[0] = 'j';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_K) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'K';
//			else cadenaTemporal[0] = 'k';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_L) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'L';
//			else cadenaTemporal[0] = 'l';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'Z';
//			else cadenaTemporal[0] = 'z';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_X) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'X';
//			else cadenaTemporal[0] = 'x';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_C) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'C';
//			else cadenaTemporal[0] = 'c';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_V) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'V';
//			else cadenaTemporal[0] = 'v';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_B) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'B';
//			else cadenaTemporal[0] = 'b';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_N) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'N';
//			else cadenaTemporal[0] = 'n';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_M) == KeyState::KEY_DOWN)
//		{
//			char cadenaTemporal[2];
//			if (GetKeyState(VK_CAPITAL) & 0x0001 != 0) cadenaTemporal[0] = 'M';
//			else cadenaTemporal[0] = 'm';
//			cadenaTemporal[1] = '\0';
//			strcat(text, cadenaTemporal);
//			aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_0) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '0';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '1';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '2';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_3) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '3';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '4';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_5) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '5';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_6) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '6';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_7) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '7';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_8) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '8';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_9) == KeyState::KEY_DOWN)
//		{
//		char cadenaTemporal[2];
//		cadenaTemporal[0] = '9';
//		cadenaTemporal[1] = '\0';
//		strcat(text, cadenaTemporal);
//		aux.setOnlyText(text);
//		}
//		else if (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) textEditable = false;
//	}
//	else normalColor = prevColor;
//}
//
//void InputBoxComponent::Draw()
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
//void InputBoxComponent::OnEditor(int i)
//{
//	// Manage if colors are being edited or not
//	static bool textColorEditable = false;
//
//	// Manage if colors are being edited or not
//	static bool normalEditable = false;
//	static bool pressedEditable = false;
//	static bool focusedEditable = false;
//	static bool disabledEditable = false;
//	static bool selectedEditable = false;
//
//	ImGui::Checkbox("Active", &active);
//
//	ImGui::Text("Normal Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Normal Color", ImVec4(normalColor.r, normalColor.g, normalColor.b, normalColor.a)))
//		normalEditable = !normalEditable;
//
//	ImGui::Text("Pressed Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Pressed Color", ImVec4(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a)))
//		pressedEditable = !pressedEditable;
//
//	ImGui::Text("Focused Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Focused Color", ImVec4(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a)))
//		focusedEditable = !focusedEditable;
//
//	ImGui::Text("Disabled Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Disabled Color", ImVec4(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a)))
//		disabledEditable = !disabledEditable;
//
//	ImGui::Text("Selected Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Selected Color", ImVec4(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a)))
//		selectedEditable = !selectedEditable;
//
//	ImGui::Separator();
//
//	ImGui::Text("Text Color"); ImGui::SameLine();
//	if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a))) {
//		
//		textColorEditable = !textColorEditable;
//	}
//	aux.setOnlyColor({ textColor.r, textColor.g, textColor.b });
//
//	if (normalEditable)
//	{
//		ImGui::ColorPicker3("Normal Color", &normalColor);
//	}
//	if (pressedEditable)
//	{
//		ImGui::ColorPicker3("Pressed Color", &pressedColor);
//	}
//	if (focusedEditable)
//	{
//		ImGui::ColorPicker3("Focused Color", &focusedColor);
//	}
//	if (disabledEditable)
//	{
//		ImGui::ColorPicker3("Disabled Color", &disabledColor);
//	}
//	if (selectedEditable)
//	{
//		ImGui::ColorPicker3("Selected Color", &selectedColor);
//	}
//	if (textColorEditable)
//	{
//		ImGui::ColorPicker3("Text Color", &textColor);
//	}
//
//	ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
//	ImGui::DragFloat("Font Size", &aux.Scale,0.1,0,10);
//	aux.setOnlyText(text);
//}
//
//void InputBoxComponent::OnClick()
//{
//	textEditable = true;
//}
//
//float2 InputBoxComponent::GetParentPosition()
//{
//	ComponentTransform2D* transform = owner->getTransform2D();
//	return { transform->position.x - (strlen(text) * 6), transform->position.y - 5};
//}
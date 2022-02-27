#include "ModuleUI.h"

#include"glew/include/GL/glew.h"
#include "Globals.h"
#include "Application.h"

#include "CameraComponent.h"
#include "ModuleCamera3D.h"
#include "ButtonComponent.h"
#include"ModuleScene.h"
#include "ImageComponent.h"
#include "InputBoxComponent.h"
#include "MaterialComponent.h"
#include "SliderComponent.h"
#include "CheckBoxComponent.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include"GameObject.h"
#include"TransformComponent.h"
#include <stack>
#include"freetype-2.10.0/include/ft2build.h"
#include"CanvasComponent.h"

#include"ModuleRenderer3D.h"

#include "MathGeoLib/src/Geometry/Frustum.h"
#include FT_FREETYPE_H 
MyPlane::MyPlane(float3 pos, float3 sca) {
	position = pos;
	scale = sca;

	//type = PrimitivesTypes::PRIMITIVE_MYPLANE3D;

	/*vertices.push_back({ (1 + pos.x) * sca.x ,pos.y,(1 + pos.z) * sca.z });
	vertices.push_back({ (1 + pos.x) * sca.x ,pos.y, (pos.z - 1) * sca.z });
	vertices.push_back({ (pos.x - 1) * sca.x ,pos.y,(pos.z - 1) * sca.z });
	vertices.push_back({ (pos.x - 1) * sca.x ,pos.y,(1 + pos.z) * sca.z });*/
	vertices.push_back({ -0.5,-0.5,0 });
	vertices.push_back({ 0.5,-0.5,0 });
	vertices.push_back({ -0.5,0.5,0 });
	vertices.push_back({ 0.5,0.5,0 });

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);

	/*App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.w = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.x = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.y = 0;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot.z = 0;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.x = sca.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.y = sca.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca.z = sca.z;

	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.x = pos.x;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.y = pos.y;
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans.z = pos.z;*/

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertices.data(), (size_t)vertices.size());
}

void MyPlane::DrawPlane2D() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable states
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// TODO: 2D en lugar de 3D para UI
	ComponentTransform2D* d=own->GetComponent<ComponentTransform2D>();

	glPushMatrix();
	
	glMultMatrixf(d->matrix);


	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, TBO); // TexCoords
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindTexture(GL_TEXTURE_2D, aTextureId); // Textures and Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	ComponentTransform2D* auxTrans = app->userInterface->UIGameObjects[app->userInterface->UIGameObjects.size() - 1]->GetComponent<ComponentTransform2D>();
	glPushMatrix();
	float4x4 aux = float4x4::FromTRS(float3(auxTrans->position.x, auxTrans->position.y, auxTrans->position.z), auxTrans->rotationQuat, float3(auxTrans->scale.x, auxTrans->scale.y, 1));
	glMultMatrixf(aux.Transposed().ptr());

	//Draw
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

	

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}
Shader::Shader()
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode = "#version 330 core\n  in vec4 vertex;\n out vec2 TexCoords;\n  uniform mat4 projection;\n  void main() {\n  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n  TexCoords = vertex.zw; };";
	std::string fragmentCode = "#version 330 core\n  in vec2 TexCoords;\n  out vec4 color;\n  uniform sampler2D text;\n  uniform vec3 textColor;\n  void main() {\n  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n  color = vec4(textColor, 1.0) * sampled;};"; //color = vec4(textColor, 1.0) * sampled;};";

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);


}
// activate the shader
// ------------------------------------------------------------------------
void Shader::Use()
{
	glUseProgram(ID);
}
// deactivate the shader
// ------------------------------------------------------------------------
void Shader::StopUse()
{
	glUseProgram(0);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			//std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			//std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}


////////////////////////////////////////////////


ModuleUI::ModuleUI(bool startEnabled) : Module(startEnabled)
{
	focusedGameObject = nullptr;
	UIGameObjectSelected = nullptr;
}

ModuleUI::~ModuleUI()
{
	focusedGameObject = nullptr;
	UIGameObjectSelected = nullptr;
}

bool ModuleUI::Start()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		//LOG("ERROR::FREETYPE: Could not init FreeType Library");
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Assets/Fonts/OpenSans-Bold.ttf", 0, &face))
	{
		//LOG("ERROR::FREETYPE: Failed to load font");
		return false;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				IVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				IVec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = new Shader();
	return true;
}
void ModuleUI::RenderText(std::string text, float x, float y, float scale, float3 color)
{
	// activate corresponding render state	
	
	shader->Use();
	Frustum frustum;
	CameraComponent* camera= app->scene->camera->GetComponent<CameraComponent>();
	
	frustum.pos = camera->camera.pos;
	frustum.front = camera->camera.front; //COGED EL FRONT DE LA CAMARA DE JUEGO
	frustum.up = camera->camera.up; //COGED EL UP DE LA CAMARA DE JUEGO
	frustum.type = FrustumType::OrthographicFrustum;
	
	frustum.orthographicHeight = camera->currentScreenHeight;//PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	frustum.orthographicWidth = camera->currentScreenWidth;//PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	frustum.nearPlaneDistance = 0.1;
	frustum.farPlaneDistance = 1000000.f;
	
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	//frustum.SetViewPlaneDistances(0.1, 1000000.f);

	/*float verticalFov = 2 * Atan((Tan(DegToRad(90.0f) / 2)) * (camera->currentScreenHeight / camera->currentScreenWidth));
	frustum.SetVerticalFovAndAspectRatio(verticalFov, (camera->currentScreenWidth / camera->currentScreenHeight));*/

	/*frustum.SetOrthographic(camera->currentScreenWidth, camera->currentScreenHeight);
	frustum.SetPerspective(DegToRad(90.0f), 0.0f);
	frustum.SetFrame(camera->camera.pos, camera->camera.front, camera->camera.up);*/

	/*math::float4x4 h = frustum.ComputeProjectionMatrix();
	math::float4x4 v = frustum.ComputeViewMatrix();*/

	auto p = frustum.ProjectionMatrix();
	glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, p.Transposed().ptr());
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shader->StopUse();
}

bool ModuleUI::PreUpdate(float dt)
{
	/*if (app->gameMode)
	{*/
		CameraComponent* camera = app->scene->camera->GetComponent<CameraComponent>();
		
		float2 mousePos = { (float)app->input->GetMouseX() ,(float)app->input->GetMouseY() };
		float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
		
		float4 viewport = app->editor->gameView->bounds;
		fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y };
		/*DEBUG_LOG("%f",viewport.x);
		DEBUG_LOG("%f", viewport.y);
		DEBUG_LOG("%f", viewport.w);
		DEBUG_LOG("%f", viewport.z);*/
		
		if (mousePos.x > viewport.x && mousePos.x < viewport.x + viewport.z && mousePos.y > viewport.y && mousePos.y < viewport.y + viewport.w)
		{
			for (int i = 0; i < UIGameObjects.size(); i++)
			{
				GameObject* go = UIGameObjects[i];
				ComponentTransform2D* transform2D = go->GetComponent<ComponentTransform2D>();

				float3 position = transform2D->position;
				ComponentTransform2D*button=(ComponentTransform2D*)go->GetComponent<ComponentTransform2D>();
				float posXMin = ((viewport.z / 2) + (position.x)) - (button->buttonWidth / 2);
				float posXMax = ((viewport.z / 2) + (position.x)) + (button->buttonWidth / 2);

				float posYMin = ((viewport.w / 2) + (-position.y)) - (button->buttonHeight / 2);
				float posYMax = ((viewport.w / 2) + (-position.y)) + (button->buttonHeight / 2);

				ImageComponent* image = nullptr;
				image = go->GetComponent<ImageComponent>();
				if ((fMousePos.x > posXMin && fMousePos.x < posXMax && fMousePos.y > posYMin && fMousePos.y < posYMax))
				{
					hitObjs.push_back(go);
				}
			}

			if (hitObjs.size() > 0)
			{
				std::vector<float> distance;
				float nearestDistance = 100000.0f;
				int nearObj = 0;
				for (int i = 0; i < hitObjs.size(); ++i)
				{
					ComponentTransform2D* transform2D = hitObjs[i]->GetComponent<ComponentTransform2D>();

					float3 position = transform2D->position;
					distance.push_back(position.z);
					if (distance[i] < nearestDistance)
					{
						nearestDistance = distance[i];
						nearObj = i;
					}
				}
				focusedGameObject = hitObjs[nearObj];
				UIGameObjectSelected = nullptr;
				hitObjs.clear();
			}
			else
				focusedGameObject = nullptr;
		}
	/*}*/

	return true;
}

bool ModuleUI::Update(float dt)
{
	// Update All UI Components

	if (!UIGameObjects.empty() && app->input->GetKey(SDL_SCANCODE_TAB) == KeyState::KEY_DOWN)
	{
		if (!UIGameObjectSelected)
			UIGameObjectSelected = UIGameObjects[0];
		else
		{
			if (nextUIGameObject != UIGameObjects.size())
			{
				UIGameObjectSelected = UIGameObjects[nextUIGameObject];
				nextUIGameObject++;
			}
			else
			{
				UIGameObjectSelected = UIGameObjects[0];
				nextUIGameObject = 1;
			}
		}
	}
	ButtonComponent* aux = nullptr;
	/*CanvasComponent* aux1 = nullptr;
	CheckboxComponent* aux2 = nullptr;
	ImageComponent* aux3= nullptr;
	InputBoxComponent* aux4 = nullptr;
	SliderComponent*aux5= nullptr;*/
	for (int i = 0; i < UIGameObjects.size(); i++)
	{
		GameObject* go = UIGameObjects[i];
		
		aux = go->GetComponent<ButtonComponent>();
		/*aux1 = go->GetComponent<CanvasComponent>();
		aux2 = go->GetComponent<CheckboxComponent>();
		aux3 = go->GetComponent<ImageComponent>();
		aux4 = go->GetComponent<InputBoxComponent>();
		aux5 = go->GetComponent<SliderComponent>();*/
		if (aux != nullptr) 
		{
			textExample = aux->text;
			color.x = aux->textColor.r;
			color.y = aux->textColor.g;
			color.z = aux->textColor.b;
			aux = nullptr;
		}
		/*else if (aux1 != nullptr) 
		{
			textExample = aux1->text;
			color.x = aux1->color.r;
			color.y = aux1->color.g;
			color.z = aux1->color.b;
			aux1 = nullptr;
		}
		else if (aux2 != nullptr)
		{
			textExample = aux2->text;
			color.x = aux2->textColor.r;
			color.y = aux2->textColor.g;
			color.z = aux2->textColor.b;
			aux2 = nullptr;
		}
		else if (aux3 != nullptr)
		{
			textExample = aux3->text;
			color.x = aux3->color.r;
			color.y = aux3->color.g;
			color.z = aux3->color.b;
			aux3 = nullptr;
		}
		else if (aux4 != nullptr)
		{
			textExample = aux4->text;
			color.x = aux4->textColor.r;
			color.y = aux4->textColor.g;
			color.z = aux4->textColor.b;
			aux4 = nullptr;
		}
		else if (aux5 != nullptr)
		{
			textExample = aux5->text;
			color.x = aux5->textColor.r;
			color.y = aux5->textColor.g;
			color.z = aux5->textColor.b;
			aux5 = nullptr;
		}*/
		/*if (button != -1)
		{
			go->components[button]->Update();
			ButtonComponent* auxiliar = go->GetButtonComponent(go);
			textExample = auxiliar->text;
			color.x = auxiliar->textColor.r;
			color.y = auxiliar->textColor.g;
			color.z = auxiliar->textColor.b;
		}
		if (checkbox != -1)
		{
			go->components[checkbox]->Update();
			CheckboxComponent* auxiliar = go->GetCheckboxComponent(go);
			textExample = auxiliar->text;
			color.x = auxiliar->textColor.r;
			color.y = auxiliar->textColor.g;
			color.z = auxiliar->textColor.b;
		}
		if (image != -1)
			go->components[image]->Update();
		if (inputbox != -1)
		{
			go->components[inputbox]->Update();
			InputBoxComponent* auxiliar = go->GetInputboxComponent(go);
			textExample = auxiliar->text;
			color.x = auxiliar->textColor.r;
			color.y = auxiliar->textColor.g;
			color.z = auxiliar->textColor.b;
		}

		if (slider != -1)
		{
			go->components[slider]->Update();
			SliderComponent* auxiliar = go->GetSliderComponent(go);
			textExample = auxiliar->text;
			color.x = auxiliar->textColor.r;
			color.y = auxiliar->textColor.g;
			color.z = auxiliar->textColor.b;
		}
		if (canvas != -1)
		{
			go->components[canvas]->Update();
		}*/
	}
	
		
	
	
	return true;
}

bool ModuleUI::PostUpdate()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
	
	CameraComponent* camera = app->scene->camera->GetComponent<CameraComponent>();
	//App->viewportBuffer->Bind(App->camera->GameCam);
	
	app->renderer3D->mainCameraFbo->Bind();
	Frustum frustum;
	
	//frustum.pos = camera->camera.pos;
	//frustum.front = camera->camera.front; //COGED EL FRONT DE LA CAMARA DE JUEGO
	//frustum.up = camera->camera.up; //COGED EL UP DE LA CAMARA DE JUEGO
	
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetViewPlaneDistances(0.1, 1000.f);
	

	float verticalFov = 2 * Atan((Tan(DegToRad(90.0f) / 2)) * (camera->currentScreenHeight / camera->currentScreenWidth));
	frustum.SetVerticalFovAndAspectRatio(verticalFov, (camera->currentScreenWidth / camera->currentScreenHeight));

	frustum.SetOrthographic(camera->currentScreenWidth, camera->currentScreenHeight);
	//frustum.SetPerspective(DegToRad(90.0f), 0.0f);
	frustum.SetFrame(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f), float3(0.0f, 1.0f, 0.0f));

	/////////////////////////////////////////////////////

	//frustum.pos = float3::zero;
	//frustum.front = float3::unitZ; //COGED EL FRONT DE LA CAMARA DE JUEGO
	//frustum.up = float3::unitY;
	frustum.type = OrthographicFrustum;
	//
	//frustum.orthographicHeight = camera->currentScreenHeight; //PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	//frustum.orthographicWidth = camera->currentScreenWidth; //PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	//frustum.nearPlaneDistance = 0.1;
	//frustum.farPlaneDistance = 1000.f;

	math::float4x4 h=frustum.ComputeProjectionMatrix();
	math::float4x4 v=frustum.ComputeViewMatrix();
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(h.Transposed().ptr());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(v.Transposed().ptr());
	//glLoadIdentity();

	
	std::vector<GameObject*> orderedGameObjects;
	std::vector<uint> orderedIndices;

	for (int i = 0; i < UIGameObjects.size(); i++)
	{
		orderedIndices.push_back(i);
	}

	if (UIGameObjects.size() > 1)
	{
		for (int i = 0; i < (UIGameObjects.size() - 1); i++)
		{
			for (int j = 0; j < (UIGameObjects.size() - i - 1); j++)
			{
				GameObject* go = UIGameObjects[i];
				TransformComponent* transform2D = go->GetComponent<TransformComponent>();

				float3 position = transform2D->GetPosition();

				GameObject* go1 = UIGameObjects[i+1];
				TransformComponent* transform2D1 = go1->GetComponent<TransformComponent>();

				float3 position1 = transform2D1->GetPosition();
				if (position.z <= position1.z)
				{
					uint temp = j;
					orderedIndices[j] = orderedIndices[j + 1];
					orderedIndices[j + 1] = temp;
				}
			}
		}

		for (int i = 0; i < orderedIndices.size(); i++)
		{
			orderedGameObjects.push_back(UIGameObjects[orderedIndices[i]]);
		}
		ButtonComponent* aux = nullptr;
		/*CanvasComponent* aux1 = nullptr;
		CheckboxComponent* aux2 = nullptr;
		ImageComponent* aux3 = nullptr;
		InputBoxComponent* aux4 = nullptr;
		SliderComponent* aux5 = nullptr;*/
		for (int i = 0; i < orderedGameObjects.size(); i++)
		{
			GameObject* go = orderedGameObjects[i];

			aux = go->GetComponent<ButtonComponent>();
			/*aux1 = go->GetComponent<CanvasComponent>();
			aux2 = go->GetComponent<CheckboxComponent>();
			aux3 = go->GetComponent<ImageComponent>();
			aux4 = go->GetComponent<InputBoxComponent>();
			aux5 = go->GetComponent<SliderComponent>();*/

			if (aux != nullptr)
			{
				aux->Draw();
				RenderText(aux->buttonText.textt, aux->buttonText.X, aux->buttonText.Y, aux->buttonText.Scale, aux->buttonText.Color);
				
				aux = nullptr;
			}
			//else if (aux1 != nullptr)
			//{
			//	/*aux1->Draw();
			//	RenderText(aux1->checkboxText.textt, aux1->checkboxText.X, auxiliar->checkboxText.Y, auxiliar->checkboxText.Scale, auxiliar->checkboxText.Color);
			//	textExample = aux1->text;
			//	color.x = aux1->color.r;
			//	color.y = aux1->color.g;
			//	color.z = aux1->color.b;*/
			//	aux1 = nullptr;
			//}
			//else if (aux2 != nullptr)
			//{
			//	aux2->Draw();
			//	RenderText(aux2->checkboxText.textt, aux2->checkboxText.X, aux2->checkboxText.Y, aux2->checkboxText.Scale, aux2->checkboxText.Color);
			//	
			//	aux2 = nullptr;
			//}
			//else if (aux3 != nullptr)
			//{
			//	aux3->Draw();
			//	aux3 = nullptr;
			//}
			//else if (aux4 != nullptr)
			//{
			//	
			//	aux4->Draw();
			//	RenderText(aux4->aux.textt, aux4->aux.X, aux4->aux.Y, aux4->aux.Scale, aux4->aux.Color);
			//	aux4 = nullptr;
			//}
			//else if (aux5 != nullptr)
			//{
			//	aux5->Draw();
			//	aux5 = nullptr;
			//}
			
			
		}

		orderedGameObjects.clear();
		orderedIndices.clear();
	}
	else
	{
		for (int i = 0; i < UIGameObjects.size(); i++)
		{
			GameObject* go = UIGameObjects[i];
			ButtonComponent* aux = nullptr;

			aux = go->GetComponent<ButtonComponent>();
			/*aux1 = go->GetComponent<CanvasComponent>();
			aux2 = go->GetComponent<CheckboxComponent>();
			aux3 = go->GetComponent<ImageComponent>();
			aux4 = go->GetComponent<InputBoxComponent>();
			aux5 = go->GetComponent<SliderComponent>();*/

			if (aux != nullptr)
			{
				aux->Draw();
				RenderText(aux->buttonText.textt, aux->buttonText.X, aux->buttonText.Y, aux->buttonText.Scale, aux->buttonText.Color);

				aux = nullptr;
			}
		}
	}
	
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	app->renderer3D->mainCameraFbo->Unbind();
	
	//App->viewportBuffer->UnBind();

	return true;
}

bool ModuleUI::CleanUp()
{
	return true;
}

void ModuleUI::DeleteUIGameObjects()
{
	//int UIGameObjectsQuantity = UIGameObjects.size();

	//for (int a = UIGameObjectsQuantity -1 ; a >= 0; a--)
	//{
	//	app->editor->objectSelected = UIGameObjects[a];
	//	int i;
	//	int id = App->editor->objectSelected->id;
	//	for (i = 0; i < App->scene->gameObjects.size(); i++)
	//	{
	//		if (id == App->scene->gameObjects[i]->id)
	//		{
	//			App->editor->objectSelected = nullptr;


	//			for (int i = 0; i < App->userInterface->UIGameObjects.size(); i++)
	//			{
	//				if (App->userInterface->UIGameObjects[i]->id == id)
	//				{
	//					GameObject* go = App->userInterface->UIGameObjects[i];
	//					uint comp = go->SearchComponent(go, ComponentType::UI_IMAGE);
	//					if (comp == -1 || (comp != -1 && go->components[comp]->UIid != 10))
	//					{
	//						ComponentTransform2D* component2d = App->userInterface->UIGameObjects[i]->getTransform2D();
	//						component2d->position.x = 70000;
	//						component2d->position.z = 70000;
	//					}
	//					//App->userInterface->UIGameObjects.erase(App->userInterface->UIGameObjects.begin() + i);
	//					break;
	//				}
	//			}

	//			//delete (*(App->scene->gameObjects.begin() + i));
	//			//App->scene->gameObjects.erase(App->scene->gameObjects.begin() + i);
	//			break;
	//		}
	//	}
	//}

	UIGameObjectSelected = nullptr;
}
#include "ModuleUI.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleSceneManager.h"
#include "Scene.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"

#include "Transform2DComponent.h"
#include "ButtonComponent.h"
#include "ImageComponent.h"
#include "SliderComponent.h"
#include "CheckBoxComponent.h"
#include "TextComponent.h"

#include "freetype-2.10.0/include/ft2build.h"
#include "Texture.h"
#include "GameView.h"

#include "GL/glew.h"
#include "Profiling.h"

#include FT_FREETYPE_H 

MyPlane::MyPlane(float3 pos, float3 sca) {
	position = pos;
	scale = sca;

	vertices.push_back({ -0.5,0.5,-2});
	vertices.push_back({ -0.5,-0.5,-2 });
	vertices.push_back({ 0.5,-0.5,-2 });

	vertices.push_back({ 0.5,-0.5,-2 });
	vertices.push_back({ 0.5,0.5,-2 });
	vertices.push_back({ -0.5,0.5,-2 });

	texCoords.push_back(0);
	texCoords.push_back(0);

	texCoords.push_back(1);
	texCoords.push_back(1);

	texCoords.push_back(1);
	texCoords.push_back(0);

	texCoords.push_back(0);
	texCoords.push_back(1);

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glGenBuffers(1, &TBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::string vertexSource = R"(
		#version 430 core
		layout(location = 0) in vec3 position;
		layout(location = 1) in vec2 texCoords;

		uniform mat4 projection;
		uniform mat4 model;

		out vec2 vTexCoords;

		void main()
		{
			gl_Position = projection * model * vec4(position, 1.0f);
			vTexCoords = texCoords;
		}
	)";

	std::string fragSource = R"(
		#version 430 core

		out vec4 fragColor;
        uniform vec4 Color;
		uniform sampler2D tex;		

		in vec2 vTexCoords;
		void main()
		{
			fragColor = texture(tex, vTexCoords) * Color;
		}
	)";

	shader = new Shadert(vertexSource, fragSource);
}

MyPlane::~MyPlane()
{
	RELEASE(shader);
}

void MyPlane::DrawPlane2D(Texture* texture)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ComponentTransform2D* auxTrans = own->GetComponent<ComponentTransform2D>();


	ButtonComponent* theButton = nullptr;
	SliderComponent* theSlider = nullptr;
	CheckboxComponent* theCheckbox = nullptr;
	ImageComponent* theImage = nullptr;

	float4x4 transform = float4x4::FromTRS(auxTrans->GetInternalPosition(), auxTrans->GetRotationQuat(), float3(auxTrans->GetScale().x, auxTrans->GetScale().y, 1));
	
	theButton = own->GetComponent<ButtonComponent>();
	theSlider=own->GetComponent<SliderComponent>();
	theCheckbox = own->GetComponent<CheckboxComponent>();
	theImage = own->GetComponent<ImageComponent>();
	if(texture)
		texture->Bind();
	shader->Use();

	CameraComponent* cam = app->sceneManager->GetCurrentScene()->camera->GetComponent<CameraComponent>();

	if (theButton)
	{
		glUniform4f(glGetUniformLocation(shader->ID, "Color"), theButton->GetActualColor().r, theButton->GetActualColor().g, theButton->GetActualColor().b, 1);
	}
	else if (theSlider) 
	{
		if (theSlider->GetFirstDraw()) {
			int cont = 0;
			for (int a = 0; a < own->components.size(); a++) {
				if (own->components[a]->type == ComponentType::TRANFORM2D)
				{
					cont++;
					if (cont == 1) {

					}
					else
					{
						ComponentTransform2D* r = (ComponentTransform2D*)own->components[a];
						transform = float4x4::FromTRS(r->GetInternalPosition(), r->GetRotationQuat(), float3(r->GetScale().x, r->GetScale().y, 1));
						break;
					}
				}
			}
		}
		
		glUniform4f(glGetUniformLocation(shader->ID, "Color"), theSlider->GetActualColor().r, theSlider->GetActualColor().g, theSlider->GetActualColor().b, 1);
	}
	else if (theCheckbox)
	{
		glUniform4f(glGetUniformLocation(shader->ID, "Color"), theCheckbox->GetActualColor().r, theCheckbox->GetActualColor().g, theCheckbox->GetActualColor().b, 1);
	}
	else if (theImage)
	{
		glUniform4f(glGetUniformLocation(shader->ID, "Color"), theImage->GetActualColor().r, theImage->GetActualColor().g, theImage->GetActualColor().b, 1);
	}
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, cam->matrixProjectionFrustum.Transposed().ptr());
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, transform.Transposed().ptr());

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_COORD_ARRAY, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	shader->StopUse();
}

Shadert::Shadert(const std::string& vertexSource, const std::string& fragmentSource)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode = vertexSource;
	std::string fragmentCode = fragmentSource;
	if(vertexSource.empty())
		vertexCode = "#version 330 core\n  in vec4 vertex;\n out vec2 TexCoords;\n uniform mat4 model;\n  uniform mat4 projection;\n  void main() {\n  gl_Position = projection * model *vec4(vertex.xy, 0.0, 1.0);\n  TexCoords = vertex.zw; };";
	if(fragmentSource.empty())
		fragmentCode = "#version 330 core\n  in vec2 TexCoords;\n  out vec4 color;\n  uniform sampler2D text;\n  uniform vec3 textColor;\n  void main() {\n  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n  color = vec4(textColor, 1.0) * sampled;};"; //color = vec4(textColor, 1.0) * sampled;};";

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
void Shadert::Use()
{
	glUseProgram(ID);
}
// deactivate the shader
// ------------------------------------------------------------------------
void Shadert::StopUse()
{
	glUseProgram(0);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shadert::CheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			
			DEBUG_LOG("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s ", type, infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);

			DEBUG_LOG("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s ", type, infoLog);
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
	if (FT_New_Face(ft, "Assets/Resources/Fonts/Montserrat-Bold.ttf", 0, &face))
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
				DEBUG_LOG("ERROR::FREETYTPE: Failed to load Glyph");
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

	shader = new Shadert("", "");
	return true;
}
void ModuleUI::RenderText(std::string text, float x, float y, float scale, float3 color)
{
	// activate corresponding render state	
	
	shader->Use();
	Frustum frustum;
	CameraComponent* camera= app->sceneManager->GetCurrentScene()->camera->GetComponent<CameraComponent>();
	
	frustum.pos = camera->GetFrustum()->pos;
	
	frustum.front = camera->GetFrustum()->front; //COGED EL FRONT DE LA CAMARA DE JUEGO
	frustum.up = camera->GetFrustum()->up; //COGED EL UP DE LA CAMARA DE JUEGO
	frustum.type = FrustumType::OrthographicFrustum;
	
	frustum.orthographicHeight = camera->GetCurrentScreenHeight();//PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	frustum.orthographicWidth = camera->GetCurrentScreenWidth();//PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	frustum.nearPlaneDistance = -1.0f;
	frustum.farPlaneDistance = 1.0f;
	
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	
	math::float4x4 model = math::float4x4::identity;
	math::float3 scl = math::float3(1, 1, 1.0f);
	math::float3 center = math::float3(x, y, 1.0f);
	model = model.Scale(scl, center);
	model.SetTranslatePart(center);

	auto p = frustum.ProjectionMatrix();
	glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, p.Transposed().ptr());
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_TRUE, (const float*)&model);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	DrawCharacters(text, x, scale, y);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shader->StopUse();
}
// Draw all text letters 
void ModuleUI::DrawCharacters(std::string& text, float& x, float scale, float y)
{
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
}

bool ModuleUI::PreUpdate(float dt)
{
	/*if (app->gameMode)
	{*/
		CameraComponent* camera = app->sceneManager->GetCurrentScene()->camera->GetComponent<CameraComponent>();
		
		float2 mousePos = { (float)app->input->GetMouseX() ,(float)app->input->GetMouseY() };
		float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
		
		float4 viewport = app->editor->GetGameView()->GetBounds();
		fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y };
		
		
		// Check if mouse is hovered on Game View
		if (app->editor->GetGameView()->GetState())
		{
			HitPosibleFocusedObjects(viewport);
			SetFocusedObject();
		}
	

	return true;
}

// Check if mouse is hovered on some object UI
void ModuleUI::HitPosibleFocusedObjects(math::float4& viewport)
{
	for (int i = 0; i < UIGameObjects.size(); i++)
	{
		GameObject* go = UIGameObjects[i];
		ComponentTransform2D* transform2D = go->GetComponent<ComponentTransform2D>();

		float3 position = transform2D->GetPosition();
		ComponentTransform2D* button = (ComponentTransform2D*)go->GetComponent<ComponentTransform2D>();

		float posXMin = ((viewport.z / 2) + (position.x * 1.7)) - (button->GetButtonWidth() / 2);
		float posXMax = ((viewport.z / 2) + (position.x * 1.7)) + (button->GetButtonWidth() / 2);

		float posYMin = ((viewport.w / 2) + (-position.y * 1.7)) - (button->GetButtonHeight() / 2);
		float posYMax = ((viewport.w / 2) + (-position.y * 1.7)) + (button->GetButtonHeight() / 2);

		ImageComponent* image = nullptr;
		image = go->GetComponent<ImageComponent>();
		if ((fMousePos.x > posXMin && fMousePos.x < posXMax && fMousePos.y > posYMin && fMousePos.y < posYMax))
		{
			hitObjs.push_back(go);
		}
	}
}
// Check depending on the distance of the object from the camera what object is focused 
void ModuleUI::SetFocusedObject()
{
	if (hitObjs.size() > 0)
	{
		std::vector<float> distance;
		float nearestDistance = -100000.0f;
		int nearObj = 0;
		for (int i = 0; i < hitObjs.size(); ++i)
		{
			ComponentTransform2D* transform2D = hitObjs[i]->GetComponent<ComponentTransform2D>();

			float3 position = transform2D->GetPosition();
			distance.push_back(position.z);
			if (distance[i] > nearestDistance)
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
	CheckboxComponent* aux2 = nullptr;
	ImageComponent* aux3= nullptr;	
	SliderComponent*aux5= nullptr;
	for (int i = 0; i < UIGameObjects.size(); i++)
	{
		GameObject* go = UIGameObjects[i];
		
		aux = go->GetComponent<ButtonComponent>();
		
		aux2 = go->GetComponent<CheckboxComponent>();
		aux3 = go->GetComponent<ImageComponent>();
	
		aux5 = go->GetComponent<SliderComponent>();
		if (aux != nullptr) 
		{
			textExample = aux->GetText();
			color = aux->GetTextColor();
		}
		else if (aux2 != nullptr)
		{
			textExample = aux2->GetText();
			color = aux2->GetTextColor();
		}
		else if (aux3 != nullptr)
		{
			textExample = aux3->GetText();
			color = aux3->GetColor();
		}
		else if (aux5 != nullptr)
		{
			textExample = aux5->GetText().textt;
			color = aux5->GetTextColor();
		}		
	}	
	
	return true;
}

void ModuleUI::Draw()
{
	ButtonComponent* aux = nullptr;
	TextComponent* aux1 = nullptr;
	CheckboxComponent* aux2 = nullptr;
	ImageComponent* aux3 = nullptr;
	SliderComponent* aux5 = nullptr;

	for (int a = 0; a < UIGameObjects.size(); a++)
	{
		GameObject* UI = app->userInterface->UIGameObjects[a];
		aux = UI->GetComponent<ButtonComponent>();
		aux1 = UI->GetComponent<TextComponent>();
		aux2 = UI->GetComponent<CheckboxComponent>();
		aux3 = UI->GetComponent<ImageComponent>();
		aux5 = UI->GetComponent<SliderComponent>();

		if (aux != nullptr)
		{
			UI->Draw(nullptr);
			RenderText(aux->GetButtonText().textt, aux->GetButtonText().X, aux->GetButtonText().Y, aux->GetButtonText().Scale, aux->GetButtonText().Color);
			aux = nullptr;
		}
		else if (aux1 != nullptr)
		{
			RenderText(text->textToShow.textt, text->textToShow.X, text->textToShow.Y, text->textToShow.Scale, text->textToShow.Color);
			text = nullptr;
		}
		else if (aux2 != nullptr)
		{
			UI->Draw(nullptr);
			aux2 = nullptr;
		}
		else if (aux3 != nullptr)
		{
			UI->Draw(nullptr);
			aux3 = nullptr;
		}
		else if (aux5 != nullptr)
		{
			UI->Draw(nullptr);
			aux5 = nullptr;
		}
	}
}


bool ModuleUI::CleanUp()
{
	RELEASE(shader);
	return true;
}

void ModuleUI::DeleteUIGameObjects(GameObject* ui)
{
	UIGameObjects.erase(FindUI(ui));
	UIGameObjectSelected = nullptr;
}
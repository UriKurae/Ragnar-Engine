#include "MaterialComponent.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Viewport.h"

#include "CameraComponent.h"

#include "FileSystem.h"

#include "Imgui/imgui.h"

#include "IconsFontAwesome5.h"

#include "Profiling.h"

#define MAX_TIME_TO_REFRESH_SHADER 10

MaterialComponent::MaterialComponent(GameObject* own, bool defaultMat) : diff(nullptr), showTexMenu(false), showShaderMenu(false), defaultMat(defaultMat)
{
	type = ComponentType::MATERIAL;
	owner = own;
	checker = false;
	showShaderEditor = false;
	active = true;

	//shader = new Shader("Assets/Resources/Shaders/default.shader");
	shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/Shaders/default.shader")));

	diff = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/white.png")));

	ambientColor = { 0.4,0.4,0.4 };
	diffuseColor = ambientColor;
	specularColor = { 0.5,0.5,0.5 };
	shininess = 5.0f;

	refreshShaderTimer = 0.0f;
}

MaterialComponent::MaterialComponent(MaterialComponent* mat) : showTexMenu(false), showShaderMenu(false), defaultMat(false)
{
	checker = mat->checker;
	diff = mat->diff;

	shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/Shaders/default.shader")));
	//shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->GetResource("Assets/Resources/Shaders/default.shader"));

	ambientColor = { 0.4,0.4,0.4 };
	diffuseColor = ambientColor;
	specularColor = { 0.5,0.5,0.5 };
	shininess = 5.0f;
}

MaterialComponent::~MaterialComponent()
{
	if (diff.use_count() - 1 == 1) diff->UnLoad();

	MeshComponent* mesh = owner->GetComponent<MeshComponent>();
	if (mesh)
	{
		mesh->SetMaterial(nullptr);
	}

	//RELEASE(shader);
}

void MaterialComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader(ICON_FA_LAYER_GROUP" Material"))
	{
		Checkbox(this, "Active", active);
		if (diff != nullptr)
		{
			ImGui::Text("Select texture: ");
			ImGui::SameLine();
			if (ImGui::Button(diff ? diff->GetName().c_str() : ""))
			{
				showTexMenu = true;
			}
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", diff->GetAssetsPath().c_str());
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diff->GetWidth());
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", diff->GetHeight());
			ImGui::Text("Reference Count: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (Warning: There's already one instance of it on the resources map)", diff.use_count());
			ImGui::Image((ImTextureID)diff->GetId(), ImVec2(128, 128));
		}
		else
		{
			ImGui::Text("Select texture: ");
			ImGui::SameLine();
			if (ImGui::Button("No Texture"))
			{
				showTexMenu = true;
			}
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "There's no texture");
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", 0);
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", 0);
		}

		

		//ImGui::Separator();

		ImGui::Text("Select shader: ");
		ImGui::SameLine();
		if (ImGui::Button(shader->GetName().c_str()))
		{
			showShaderMenu = true;
		}

		ImGui::Text("Reference Count: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (Warning: There's already one instance of it on the resources map)", shader.use_count());

		ImGui::BulletText("Last time modified: %s", shader->GetLastModifiedDate());

		if (ImGui::Button("Edit Shader", { 100,25 }))	EditorShader();

		ImGui::Separator();
	}

	if (showShaderEditor)
	{
		ImGui::Begin("Shader Editor", &showShaderEditor, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
		//Update
		auto cpos = editor.GetCursorPosition();
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::Button("Save"))
			{
				std::string shader = editor.GetText();

				app->fs->RemoveFile(fileToEdit.c_str());
				app->fs->Save(fileToEdit.c_str(), shader.c_str(), editor.GetText().size());

				//glDetachShader(shadertoRecompily->GetId(), meshCom->shadertoRecompily->parameters.vertexID);
				//glDetachShader(shadertoRecompily->GetId(), meshCom->shadertoRecompily->parameters.fragmentID);
				//glDeleteProgram(shadertoRecompily->GetId());

				//meshCom->LoadShader(meshCom->fileToEdit.c_str());
				//meshCom->GetShader()->parameters.uniforms = ShaderImporter::GetShaderUniforms(meshCom->GetShader()->parameters.shaderID);
			}
			ImGui::EndMenuBar();
		}

		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), fileToEdit.c_str());

		editor.Render("TextEditor");
		ImGui::End();
	}

	if (showTexMenu)
	{
		ImGui::Begin("Textures", &showTexMenu);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x &&
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) showTexMenu = false;
		}

		std::vector<std::string> files;
		app->fs->DiscoverFiles("Library/Textures/", files);
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if ((*it).find(".rgtexture") != std::string::npos)
			{
				app->fs->GetFilenameWithoutExtension(*it);
				*it = (*it).substr((*it).find_last_of("_") + 1, (*it).length());
				uint uid = std::stoll(*it);
				std::shared_ptr<Resource> res = ResourceManager::GetInstance()->GetResource(uid);
				if (ImGui::Selectable(res->GetName().c_str()))
				{
					res->Load();
					if (diff.use_count() - 1 == 1) diff->UnLoad();
					SetTexture(res);
				}
			}
		}

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Delete").x - 25);
		if (ImGui::Button(ICON_FA_TRASH" Delete"))
			owner->RemoveComponent(this);

		ImGui::End();
	}

	if (showShaderMenu)
	{
		ImGui::Begin("Shaders", &showShaderMenu);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetIO().MousePos;
		if (!(mouse.x < winPos.x + size.x && mouse.x > winPos.x &&
			mouse.y < winPos.y + size.y && mouse.y > winPos.y))
		{
			if (ImGui::GetIO().MouseClicked[0]) showShaderMenu = false;
		}

		std::vector<std::string> files;
		app->fs->DiscoverFiles("Assets/Resources/Shaders", files);
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if ((*it).find(".shader") != std::string::npos)
			{
				if (ImGui::Selectable((*it).c_str()))
				{
					uint uid = ResourceManager::GetInstance()->CreateResource(ResourceType::SHADER, std::string("Assets/Resources/Shaders/" + *it), std::string());
					shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->LoadResource(uid));
				}
			}
		}

		ImGui::End();
	}


	ImGui::PopID();
}

bool MaterialComponent::Update(float dt)
{
	if (refreshShaderTimer <= MAX_TIME_TO_REFRESH_SHADER)
	{
		refreshShaderTimer += dt;
	}
	else
	{
		refreshShaderTimer = 0.0f;
		shader->Refresh();
	}
	return true;
}

bool MaterialComponent::OnLoad(JsonParsing& node)
{
	diff = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Path"))));
	active = node.GetJsonBool("Active");

	return true;
}

bool MaterialComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Path", diff->GetAssetsPath().c_str());
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);
	
	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void MaterialComponent::Bind(CameraComponent* gameCam)
{
	// Crash when creating a primitive
	if (!this)
		return;

	if (diff)
		diff->Bind();

	shader->Bind();

	// Could not do view and proj each frame.
	float4x4 model = owner->GetComponent<TransformComponent>()->GetGlobalTransform();
	shader->SetUniformMatrix4f("model", model.Transposed());

	float4x4 view = float4x4::identity;
	float4x4 proj = float4x4::identity;
	if (gameCam)
	{
		view = gameCam->matrixViewFrustum;
		proj = gameCam->matrixProjectionFrustum;
	}
	else
	{
		view = app->camera->matrixViewFrustum;
		proj = app->camera->matrixProjectionFrustum;
	}
	shader->SetUniformMatrix4f("view", view.Transposed());
	shader->SetUniformMatrix4f("projection", proj.Transposed());
	float4x4 normalMat = view;
	normalMat.Inverse();
	shader->SetUniformMatrix3f("normalMatrix", normalMat.Float3x3Part().Transposed());


	shader->SetUniformVec3f("material.ambient", ambientColor);
	shader->SetUniformVec3f("material.diffuse", diffuseColor);
	shader->SetUniformVec3f("material.specular", specularColor);
	shader->SetUniform1f("material.shininess", shininess);
	shader->SetUniform1f("material.gammaCorrection", 0);

	if (app->renderer3D->goDirLight)
	{
		shader->SetUniformVec3f("dirLight.direction", app->renderer3D->dirLight->dir);
		shader->SetUniformVec3f("dirLight.ambient", app->renderer3D->dirLight->ambient);
		shader->SetUniformVec3f("dirLight.diffuse", app->renderer3D->dirLight->diffuse);
		shader->SetUniformVec3f("dirLight.specular", app->renderer3D->dirLight->specular);
	}
	else
	{
		shader->SetUniformVec3f("dirLight.ambient", float3::zero);
		shader->SetUniformVec3f("dirLight.diffuse", float3::zero);
		shader->SetUniformVec3f("dirLight.specular", float3::zero);
	}

	std::vector<PointLight*> pls = app->renderer3D->GetPointLights();
	for (int i = 0; i < pls.size(); ++i)
	{
		std::string name = "pointLights[" + std::to_string(i) + "]";
		shader->SetUniformVec3f(name + ".position", pls[i]->position);

		shader->SetUniform1f(name + ".intensity", pls[i]->intensity);
		shader->SetUniform1f(name + ".constant", pls[i]->constant);
		shader->SetUniform1f(name + ".lin", pls[i]->lin);
		shader->SetUniform1f(name + ".quadratic", pls[i]->quadratic);

		float3 props = { pls[i]->constant, pls[i]->lin, pls[i]->quadratic };
		shader->SetUniformVec3f(name + ".properties", props);

		shader->SetUniformVec3f(name + ".ambient", pls[i]->ambient);
		shader->SetUniformVec3f(name + ".diffuse", pls[i]->diffuse);
		shader->SetUniformVec3f(name + ".specular", pls[i]->specular);

	}

	std::vector<SpotLight*> sls = app->renderer3D->GetSpotLights();
	for (int i = 0; i < sls.size(); ++i)
	{
		std::string name = "spotLights[" + std::to_string(i) + "]";
		shader->SetUniformVec3f(name + ".position", sls[i]->position);
		shader->SetUniformVec3f(name + ".direction", sls[i]->direction);

		shader->SetUniform1f(name + ".intensity", sls[i]->intensity);
		shader->SetUniform1f(name + ".cutOff", (sls[i]->cutOff));
		shader->SetUniform1f(name + ".outerCutOff", (sls[i]->outerCutOff));

		shader->SetUniformVec3f(name + ".ambient", sls[i]->ambient);
		shader->SetUniformVec3f(name + ".diffuse", sls[i]->diffuse);
		shader->SetUniformVec3f(name + ".specular", sls[i]->specular);
	}


}

void MaterialComponent::Unbind()
{
	// Crash when creating a primitive
	if (!this) return;
	if (diff) diff->Unbind();
	shader->Unbind();
}

void MaterialComponent::SetTexture(std::shared_ptr<Resource> tex)
{
	diff = std::static_pointer_cast<Texture>(tex);
}

void MaterialComponent::EditorShader()
{
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::GLSL();

	fileToEdit = shader->GetAssetsPath();
	editor.SetShowWhitespaces(false);

	/*std::ifstream text(fileToEdit.c_str());
	if (text.good())
	{
		std::string str((std::istreambuf_iterator<char>(text)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}*/

	showShaderEditor = true;

	shadertoRecompily = shader;
}

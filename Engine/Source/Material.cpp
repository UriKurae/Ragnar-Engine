//#include "Application.h"
//#include "ModuleRenderer3D.h"
//#include "ModuleEditor.h"
//#include "ModuleCamera3D.h"
//
//#include "CameraComponent.h"
//#include "ComponentLight.h"
//
//#include "ResourceManager.h"
//#include "Material.h"
//
//#include "Shader.h"
//#include "Texture.h"
//#include "Lights.h"
//
//#include "Timer.h"
//
//Material::Material(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::MATERIAL, assets, library)
//{
//	name = "New Material";
//
//	shader = app->renderer3D->GetDefaultShader();
//
//	uuid = ResourceManager::GetInstance()->GenerateUUID();
//
//	ambientColor = { 0.4,0.4,0.4 };
//	diffuseColor = ambientColor;
//	specularColor = { 0.5,0.5,0.5 };
//	shininess = 5.0f;
//
//	frequency = 2.0f;
//	speed = 0.05f;
//	amplitude = 0.2f;
//	textureAlpha = 1.0f;
//	foamSpeed = 0.0f;
//	noiseAmount = 1.0f;
//	foamDir = { 0.001, 0.0 };
//
//	timer = Timer();
//}
//
//Material::~Material()
//{
//	// Shader is deleted in renderer
//}
//
//void Material::Bind(const float4x4& transform, ComponentCamera* cam)
//{
//	if (!shader)
//		return;
//
//	//ComponentCamera* cam = 0;
//	//switch (app->editor->GetSceneState())
//	//{
//	//	case SceneState::EDIT:
//	//	{
//	//		cam = app->camera->editorCam;
//	//		break;
//	//	}
//	//	case SceneState::PAUSE:
//	//	case SceneState::PLAY:
//	//	case SceneState::STEP_ONE_FRAME:
//	//	{
//	//		cam = app->camera->gameCam;
//	//		break;
//	//	}
//	//}
//
//	ComponentCamera* matCam = cam;
//
//	if (cam == nullptr)
//		matCam = app->camera->editorCam;
//
//	shader->Bind();
//
//	shader->SetUniformMatrix4f("model", transform.Transposed());
//	float4x4 view = matCam->frustum.ViewMatrix();
//	shader->SetUniformMatrix4f("view", view.Transposed());
//	shader->SetUniformMatrix4f("projection", matCam->frustum.ProjectionMatrix().Transposed());
//	float4x4 normalMat = view;
//	normalMat.Inverse();
//	shader->SetUniformMatrix3f("normalMatrix", normalMat.Float3x3Part().Transposed());
//	shader->SetUniformVec3f("camPos", app->camera->GetCurrentCamera()->GetPosition());
//
//	shader->SetUniformVec3f("ambientColor", ambientColor);
//	shader->SetUniform1f("textureAlpha", textureAlpha);
//
//	shader->SetUniformVec3f("material.ambient", ambientColor);
//	shader->SetUniformVec3f("material.diffuse", diffuseColor);
//	shader->SetUniformVec3f("material.specular", specularColor);
//	shader->SetUniform1f("material.shininess", shininess);
//	shader->SetUniform1i("material.gammaCorrection", app->renderer3D->gammaCorrection);
//	shader->SetUniform1f("material.gammaCorrectionAmount", app->renderer3D->gammaCorrectionAmount);
//
//
//	if (app->renderer3D->goDirLight)
//	{
//		shader->SetUniformVec3f("dirLight.direction", app->renderer3D->dirLight->dir);
//		shader->SetUniformVec3f("dirLight.ambient", app->renderer3D->dirLight->ambient);
//		shader->SetUniformVec3f("dirLight.diffuse", app->renderer3D->dirLight->diffuse);
//		shader->SetUniformVec3f("dirLight.specular", app->renderer3D->dirLight->specular);
//	}
//	else
//	{
//		shader->SetUniformVec3f("dirLight.ambient", float3::zero);
//		shader->SetUniformVec3f("dirLight.diffuse", float3::zero);
//		shader->SetUniformVec3f("dirLight.specular", float3::zero);
//	}
//
//	std::vector<PointLight*> pls = app->renderer3D->GetPointLights();
//	for (int i = 0; i < pls.size(); ++i)
//	{
//		std::string name = "pointLights[" + std::to_string(i) + "]";
//		shader->SetUniformVec3f(name + ".position", pls[i]->position);
//		
//		shader->SetUniform1f(name + ".intensity", pls[i]->intensity);
//		shader->SetUniform1f(name + ".constant", pls[i]->constant);
//		shader->SetUniform1f(name + ".lin", pls[i]->lin);
//		shader->SetUniform1f(name + ".quadratic", pls[i]->quadratic);
//
//		float3 props = { pls[i]->constant, pls[i]->lin, pls[i]->quadratic };
//		shader->SetUniformVec3f(name + ".properties", props);
//
//		shader->SetUniformVec3f(name + ".ambient", pls[i]->ambient);
//		shader->SetUniformVec3f(name + ".diffuse", pls[i]->diffuse);
//		shader->SetUniformVec3f(name + ".specular", pls[i]->specular);
//
//	}
//
//	/*std::vector<SpotLight*> sls = app->renderer3D->GetSpotLights();
//	for (int i = 0; i < sls.size(); ++i)
//	{
//		std::string name = "spotLights[" + std::to_string(i) + "]";
//		shader->SetUniformVec3f(name + ".position", sls[i]->position);
//
//		shader->SetUniform1f(name + ".intensity", sls[i]->intensity);
//		shader->SetUniform1f(name + ".cutOff", sls[i]->cutOff);
//		shader->SetUniform1f(name + ".outerCutOff", sls[i]->outerCutOff);
//
//		shader->SetUniformVec3f(name + ".ambient", sls[i]->ambient);
//		shader->SetUniformVec3f(name + ".diffuse", sls[i]->diffuse);
//		shader->SetUniformVec3f(name + ".specular", sls[i]->specular);
//	}*/
//
//	if (shader->GetName() == "wave")
//	{
//		shader->SetUniform1i("time", timer.Read());
//		shader->SetUniform1f("amplitude", amplitude);
//		shader->SetUniform1f("frequency", frequency);
//		shader->SetUniform1f("speed", speed);
//		shader->SetUniform1f("foamSpeed", foamSpeed);
//		shader->SetUniform1f("noiseAmount", noiseAmount);
//		shader->SetUnifromVec2f("foamDir", foamDir);
//	}
//}
//
//void Material::Unbind()
//{
//	shader->Unbind();
//}
//
//void Material::SetShader(Shader* s)
//{
//	shader = s;
//	shader->Refresh();
//}

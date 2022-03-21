#include "Application.h"
#include "ModuleCamera3D.h"
#include "FileSystem.h"
#include "ParticleEmitter.h"
#include "ResourceManager.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Globals.h"
#include "Math/MathFunc.h"
#include "imgui/imgui.h"
#include "Profiling.h"

ParticleEmitter::ParticleEmitter(GameObject* owner) :
	particlesPerSecond(50),
	maxParticles(200),
	minLifeTime(0.7f),
	maxLifeTime(1.7f),
	isActive(true),
	showTexMenu(false),
	emitterName(""),
	own(owner),
	toDelete(false)
{
	poolIndex = maxParticles - 1;
	particlePool.resize(maxParticles);

	//particleReference = new Particle(own);
	effects.resize(4);

	timer = 1.0f / particlesPerSecond;
	currTimer = timer;
	particleReference.color = { 1,0,0,1 };
	//particleReference.colorBegin = { 1,0,0,1 };
	//particleReference.colorEnd = { 0,1,0,1 };
	//particleReference.sizeBegin = 0.5f, particleReference.sizeVariation = 0.3f, particleReference.sizeEnd = 0.0f;
	particleReference.size = 0.5f;
	particleReference.lifeTime = 1.0f;
	particleReference.velocity = { 0.0f, 0.1f, 0.0f };
	particleReference.acceleration = { 0.0f, 0.0f, 0.0f };
	particleReference.position = { 0.0f, 0.0f, 0.0f };

	data.vertexArray = nullptr;
	data.vertexBuffer = nullptr;
	data.indexBuffer = nullptr;
	SetUpBuffers();

	data.shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/Shaders/particlesBehaviour.shader")));

	data.vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	data.vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	data.vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	data.vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	texture = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/white.png")));

	data.drawCalls = 0;

	showTexMenu = false;

	effect = nullptr;
}

ParticleEmitter::~ParticleEmitter()
{
	for (int i = 0; i < effects.size(); i++)
	{
		if (effects[i] != nullptr)
			RELEASE(effects[i]);
	}

	RELEASE(data.vertexArray);
	RELEASE(data.vertexBuffer);
	RELEASE(data.indexBuffer);
	RELEASE_ARRAY(data.vertexBufferBase);
	data.vertexBufferPtr = nullptr;

	effect = nullptr;
	effects.clear();
}

void ParticleEmitter::Emit(float dt)
{
	currTimer -= dt;
	if (currTimer <= 0.0f)
	{
		Particle& particle = particlePool[poolIndex];
		particle.active = true;

		particle.position = particleReference.position;
		particle.rotation = particleReference.deltaRotation + random.Float() * 2 * pi;

		particle.acceleration = particleReference.acceleration;

		particle.velocity = particleReference.velocity;
		particle.velocity.x += particleReference.acceleration.x * (random.Float() - 0.5f) * 2;
		particle.velocity.y += particleReference.acceleration.y * (random.Float() - 0.5f) * 2;

		particle.color = particleReference.color;
		//particle.colorBegin = particleReference.colorBegin;
		//particle.colorEnd = particleReference.colorEnd;

		particle.lifeTime = particleReference.lifeTime;
		particle.lifeRemaining = particleReference.lifeTime;
		//particle.sizeBegin = particleReference.sizeBegin + particleReference.sizeVariation * (random.Float() - 0.5f);
		particle.size = particleReference.size;
		//particle.sizeEnd = particleReference.sizeEnd;

		poolIndex = --poolIndex % particlePool.size();

		currTimer = timer;
		
		for (int i = 0; i < particlePool.size(); i++)
		{
			for (int j = 0; j < effects.size(); j++)
			{
				if (effects[j] != nullptr)
				{
					effects[j]->Init(particlePool[i]);
				}
			}
		}
	}
}

void ParticleEmitter::DrawParticle(const float3& pos, float rotation, const float3& size, const float4& color, Quat newRotation)
{
	if (data.indexCount >= data.maxIndices)
		NextBatch();

	const float2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	/*Quat q;
	q.SetFromAxisAngle({ 0,1,0 }, rotation);
	float4x4 transform = float4x4::FromTRS(pos, q, size);*/

	Quat q = newRotation * Quat::RotateAxisAngle({ 0.0f,0.0f,1.0f }, rotation);
	float4x4 transform = float4x4::FromTRS(pos, q, size);

	const int quadVertexCount = 4;
	for (int i = 0; i < quadVertexCount; i++)
	{
		float4 p = transform * data.vertexPositions[i];
		data.vertexBufferPtr->position = p.Float3Part();
		data.vertexBufferPtr->texCoords = texCoords[i];
		data.vertexBufferPtr->color = color;

		data.vertexBufferPtr++;
	}

	data.indexCount += 6;
}

void ParticleEmitter::SetUpBuffers()
{
	if (data.vertexArray)
	{
		RELEASE( data.vertexArray);
		RELEASE( data.vertexBuffer);
		RELEASE( data.indexBuffer);
		RELEASE_ARRAY( data.vertexBufferBase);
		data.vertexBufferPtr = nullptr;
	}

	particlePool.resize(maxParticles);
	poolIndex = maxParticles - 1;

	data.maxQuads = maxParticles;
	data.maxVertices = data.maxQuads * 4;
	data.maxIndices = data.maxQuads * 6;
	

	data.vertexArray = new VertexArray();

	data.vertexBuffer = new VertexBuffer();
	data.vertexBuffer->SetData(data.maxVertices);
	data.vertexBuffer->SetLayout({
		{ShaderDataType::VEC3F, "position"},
		{ShaderDataType::VEC2F, "texCoords"},
		{ShaderDataType::VEC4F, "color"}
		});
	data.vertexArray->AddVertexBuffer(*data.vertexBuffer);

	data.vertexBufferBase = new ParticleVertex[data.maxVertices];

	unsigned int* indices = new unsigned int[data.maxIndices];

	int offset = 0;
	for (int i = 0; i < data.maxIndices; i += 6)
	{
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	data.indexBuffer = new IndexBuffer(indices, data.maxIndices);
	data.vertexArray->SetIndexBuffer(*data.indexBuffer);

	delete[]indices;
}

void ParticleEmitter::Render(CameraComponent* gameCam)
{
	StartBatch();

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

	BillboardParticleComponent* billboard = static_cast<BillboardParticleComponent*>(own->GetComponent<BillboardParticleComponent>());

	if (data.maxQuads > 0)
	{
		int i = 0;
		for (; i < particlePool.size(); ++i)
		{
			Particle& particle = particlePool[i];

			if (!particle.active)
				continue;

			float life = particle.lifeRemaining / particle.lifeTime;
			//float4 color = float4::Lerp(particle.color, particle.color, life);
			//color.w *= life;
			//float size = Lerp(particle.sizeEnd, particle.sizeBegin, life);

			if (billboard != nullptr)
			{
				DrawParticle(particle.position, particle.rotation, { particle.size,particle.size,0.0f }, particle.color, billboard->GetAlignment());
			}
		}

		uint32_t dataSize = (uint32_t)((uint8_t*)data.vertexBufferPtr - (uint8_t*)data.vertexBufferBase);
		data.vertexBuffer->SetData(data.vertexBufferBase, dataSize);

		data.vertexArray->Bind();

		texture->Bind();
		data.shader->Bind();
		data.shader->SetUniformMatrix4f("view", view.Transposed());
		data.shader->SetUniformMatrix4f("projection", proj.Transposed());
		TransformComponent* tr = own->GetComponent<TransformComponent>();
		data.shader->SetUniformMatrix4f("model", tr->GetGlobalTransform().Transposed());

		//for (int i = 0; i < data.textureSlots; ++i)
		//	data.textureSlots[i]->;

		data.vertexBuffer->Bind();
		data.indexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, data.indexCount, GL_UNSIGNED_INT, 0);
		data.indexBuffer->Unbind();
		data.shader->Unbind();
		data.vertexArray->Unbind();
		texture->Unbind();

		data.drawCalls++;
	}
}

void ParticleEmitter::UpdateParticle(float dt)
{
	for (int i = 0; i < particlePool.size(); ++i) 
	{
		if (particlePool[i].active)
		{
			for (int j = 0; j < effects.size(); j++)
			{
				if (effects[j] != nullptr && isEffectActive(effects[j]->type))
				{
					effects[j]->Update(particlePool[i], dt);
				}
			}
			particlePool[i].velocity += particlePool[i].acceleration * dt;
			particlePool[i].position += particlePool[i].velocity;
		}
	}
}

void ParticleEmitter::Update(float dt)
{
	UpdateParticle(dt);
	for (int i = 0; i < particlePool.size(); ++i)
	//for (auto& particle : particlePool)
	{
		Particle& particle = particlePool[i];

		if (!particle.active)
			continue;

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= dt;
		//particle.position += particle.velocity * dt;
		//particle.velocity += particleReference.acceleration * dt;
		particle.rotation += particleReference.deltaRotation * dt;
	}
}

void ParticleEmitter::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "##";
	suffixLabel += emitterIndex;
	std::string guiName = "Emitter " + suffixLabel;

	ImGui::Indent();
	if (ImGui::CollapsingHeader(guiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		if (ImGui::CollapsingHeader("Batch Stats"))
		{
			ImGui::Text("Draw calls: %i", data.drawCalls);
			ImGui::Text("Max Quads: %i", data.maxQuads);
		}
		ImGui::Unindent();

		strcpy(charsOfName, guiName.c_str());
		guiName = suffixLabel + "Name";
		guiName = "Delete Emitter" + suffixLabel;
		if (ImGui::Button(ICON_FA_TRASH" Delete Emitter"))
		{
			toDelete = true;
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		/*ImGui::Indent();*/

		//guiName = "Particle max lifetime" + suffixLabel;
		//ImGui::DragFloat(guiName.c_str(), &maxLifeTime, 0.1f, 0.0f, 10.0f);

		//particleReference->lifeTime = random.Float(minLifeTime, maxLifeTime);

		guiName = "Particles per Second" + suffixLabel;
		if (ImGui::DragFloat(guiName.c_str(), &particlesPerSecond))
			SetParticlesPerSecond(particlesPerSecond);

		guiName = "Max Particles" + suffixLabel;
		ImGui::DragInt(guiName.c_str(), &maxParticles, 1.0f, 0.0f);
		if (ImGui::IsItemDeactivated())
			if(maxParticles > 0)
				SetUpBuffers();

		//float size[2] = { particleReference->size.x, particleReference->size.y };

		//guiName = "Size" + suffixLabel;

		guiName = "Particle lifetime" + suffixLabel;
		ImGui::DragFloat(guiName.c_str(), &particleReference.lifeTime, 0.01f, 0.0f, 10.0f);

		ImGui::DragFloat3("Velocity", particleReference.velocity.ptr(), 0.01f, -1.0f, 1.0f);
		ImGui::DragFloat3("Acceleration", particleReference.acceleration.ptr(), 0.01f);

		ImGui::DragFloat("Rotation Amount", &particleReference.deltaRotation, 0.01f);
	
		/*guiName = "Color (RGBA)" + suffixLabel;
		ImGui::ColorEdit4("Beginning Color", particleReference.colorBegin.ptr());

		ImGui::ColorEdit4("Ending Color", particleReference.colorEnd.ptr());*/

		//ImGui::Indent();

		ImGui::Text("Texture");
		if (ImGui::ImageButton((void*)texture->GetId(), { 150,150 }))
			showTexMenu = true;

		if (showTexMenu)
			ShowTextureMenu();

		//if (particleReference->tex != nullptr)
		//{
		//	if (ImGui::IsItemHovered())
		//	{
		//		ImGui::BeginTooltip();
		//		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
		//		ImGui::Text("Click on the image to erase it");
		//		ImGui::PopStyleColor();
		//		ImGui::EndTooltip();
		//	}
		//}
		//else
		//{
		//	ImGui::Image((ImTextureID)0, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
		//}

		for (int i = (int)ParticleEffectType::NO_TYPE + 1; i <= (int)ParticleEffectType::COLOR_OVER_LIFETIME; i++)
		{
			if (isEffectActive((ParticleEffectType)i))
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Spacing();

				effects[(int)(ParticleEffectType)i]->OnEditor(emitterIndex);

				if (effects[(int)(ParticleEffectType)i]->toDelete)
				{
					delete effects[(int)(ParticleEffectType)i];
					effects[(int)(ParticleEffectType)i] = nullptr;
				}
			}
		}

		ImGui::Spacing();
		ImGui::Spacing();

		guiName = "Add Effect##";
		guiName += emitterIndex;

		std::string textNameDisplay = "NEW EFFECT";

		textNameDisplay += emitterIndex;
		if (ImGui::BeginCombo(guiName.c_str(), textNameDisplay.c_str()))
		{
			for (int j = (int)ParticleEffectType::NO_TYPE + 1; j <= (int)ParticleEffectType::COLOR_OVER_LIFETIME; j++)
			{
				guiName = (GetNameFromEffect((ParticleEffectType)j)) + suffixLabel;

				int index = isEffectActive((ParticleEffectType)j);
				if (isEffectActive((ParticleEffectType)j) == false)
				{
					//if the effect hasn't been created
					if (ImGui::Selectable(guiName.c_str()))
					{
						CreateParticleEffect((ParticleEffectType)j);
					}
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::Unindent();
}

void ParticleEmitter::SetParticlesPerSecond(float particlesPerSec)
{
	particlesPerSecond = particlesPerSec;
	timer = 1.0f / particlesPerSecond;
}

void ParticleEmitter::CreateParticleEffect(ParticleEffectType type)
{
	switch (type)
	{
	case ParticleEffectType::NO_TYPE:
		effects[(int)ParticleEffectType::NO_TYPE] = effect;
		break;
	case ParticleEffectType::VELOCITY_OVER_LIFETIME:
		effect = new ParticleEffect_Velocity();
		effects[(int)ParticleEffectType::VELOCITY_OVER_LIFETIME] = effect;
		break;
	case ParticleEffectType::ACCELERATION_OVER_LIFETIME:
		effect = new ParticleEffect_Acceleration();
		effects[(int)ParticleEffectType::ACCELERATION_OVER_LIFETIME] = effect;
		break;
	case ParticleEffectType::COLOR_OVER_LIFETIME:
		effect = new ParticleEffect_Color();
		effects[(int)ParticleEffectType::COLOR_OVER_LIFETIME] = effect;
		break;
	case ParticleEffectType::SIZE_OVER_LIFETIME:
		effect = new ParticleEffect_Size();
		effects[(int)ParticleEffectType::SIZE_OVER_LIFETIME] = effect;
		break;
	default:
		break;
	}
}

bool ParticleEmitter::isEffectActive(ParticleEffectType type)
{
	for (int i = 0; i < effects.size(); ++i)
	{
		if (effects[i] != nullptr && effects[i]->type == type)
		{
			return true;
		}
	}
	return false;
}

std::string ParticleEmitter::GetNameFromEffect(ParticleEffectType type)
{
	switch (type)
	{
	case ParticleEffectType::NO_TYPE:
		return "No Type";
		break;
	case ParticleEffectType::VELOCITY_OVER_LIFETIME:
		return "Velocity Effect";
		break;
	case ParticleEffectType::ACCELERATION_OVER_LIFETIME:
		return "Acceleration Effect";
		break;
	case ParticleEffectType::COLOR_OVER_LIFETIME:
		return "Color Effect";
		break;
	case ParticleEffectType::SIZE_OVER_LIFETIME:
		return "Size Effect";
		break;
	default:
		break;
	}
}

ParticleEffect* ParticleEmitter::GetParticleEffect(ParticleEffectType type)
{
	for (int i = 0; i < effects.size(); i++)
	{
		if (effects[i] != nullptr && effects[i]->type == type)
			return effects[i];
	}
}

bool ParticleEmitter::OnLoad(JsonParsing& node)
{
	maxParticles = node.GetJsonNumber("Emitter: Max Particles");
	particlesPerSecond = node.GetJsonNumber("Emitter: Particles Per Second");
	isActive = node.GetJsonBool("Emitter: Is Active");
	toDelete = node.GetJsonBool("Emitter: To Delete");
	//minLifeTime = node.GetJsonNumber("Emitter: Min Life Time");
	//maxLifeTime = node.GetJsonNumber("Emitter: Max Life Time");
	timer = node.GetJsonNumber("Emitter: Timer");
	currTimer = node.GetJsonNumber("Emitter: Current Timer");

	particleReference.position = node.GetJson3Number(node, "Particle Reference Position");
	particleReference.velocity = node.GetJson3Number(node, "Particle Reference Velocity");
	particleReference.acceleration = node.GetJson3Number(node, "Particle Reference Acceleration");
	particleReference.color = node.GetJson4Number(node, "Particle Reference Color Begin");
	particleReference.size = node.GetJsonNumber("Particle Reference Size Begin");
	particleReference.lifeTime = node.GetJsonNumber ("Particle Reference Lifetime");
	particleReference.deltaRotation = node.GetJsonNumber ("Particle Reference Rotation Amount");

	texture = std::static_pointer_cast<Texture>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Texture Assets Path"))));
	data.shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->LoadResource(std::string(node.GetJsonString("Shader Assets Path"))));
	
	JSON_Array* effectsArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Effects");
	int effectsCount = node.GetJsonArrayCount(effectsArray);

	for (int i = 0; i < effects.size(); ++i)
	{
		RELEASE(effects[i]);
		effects[i] = nullptr;
	}

	for (int i = 0; i < effectsCount; ++i)
	{
		// TODO: Switch checking the type of the effect and creating it, afterwards push it to the vector
		JsonParsing c = node.GetJsonArrayValue(effectsArray, i);
		CreateParticleEffect((ParticleEffectType)(int)c.GetJsonNumber("Effect Type"));
		effects[(int)c.GetJsonNumber("Effect Type")]->OnLoad(node);
		//effects[i]->OnLoad(node);
	}

	return true;
}

bool ParticleEmitter::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Max Particles", maxParticles);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Particles Per Second", particlesPerSecond);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Emitter: Is Active", isActive);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Emitter: To Delete", toDelete);
	//file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Min Life Time", minLifeTime);
	//file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Max Life Time", maxLifeTime);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Timer", timer);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Current Timer", currTimer);

	file.SetNewJson3Number(file, "Particle Reference Position", particleReference.position);
	file.SetNewJson3Number(file, "Particle Reference Velocity", particleReference.velocity);
	file.SetNewJson3Number(file, "Particle Reference Acceleration", particleReference.acceleration);
	file.SetNewJson4Number(file, "Particle Reference Color Begin", particleReference.color);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Particle Reference Size Begin", particleReference.size);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Particle Reference Lifetime", particleReference.lifeTime);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Particle Reference Rotation Amount", particleReference.deltaRotation);

	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Texture Assets Path", texture->GetAssetsPath().c_str());
	file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Shader Assets Path", data.shader->GetAssetsPath().c_str());


	JSON_Array* effectsArray = file.SetNewJsonArray(file.GetRootValue(), "Effects");

	for (int i = 0; i < effects.size(); ++i)
	{
		if (effects[i] != nullptr)
			effects[i]->OnSave(file, effectsArray);
	}

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

void ParticleEmitter::AddInstancedAttribute(unsigned int vao, unsigned int vbo, int attributeIndex, int dataSize, int instancedDataLength, int offset)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(attributeIndex);
	glVertexAttribPointer(attributeIndex, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(float), (void*)(offset * sizeof(float)));
	glVertexAttribDivisor(attributeIndex, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleEmitter::ShowTextureMenu()
{
	ImGui::Begin("Change Texture", &showTexMenu);

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
				if (texture.use_count() - 1 == 1) texture->UnLoad();
				texture = std::static_pointer_cast<Texture>(res);
			}
		}
	}

	ImGui::End();
}

void ParticleEmitter::StartBatch()
{
	data.indexCount = 0;
	data.vertexBufferPtr = data.vertexBufferBase;
	data.drawCalls = 0;
}

void ParticleEmitter::NextBatch()
{
	Render(0);
	StartBatch();
}

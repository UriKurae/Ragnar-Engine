#include "Application.h"
#include "ModuleCamera3D.h"

#include "ParticleEmitter.h"

#include "ResourceManager.h"
#include "CameraComponent.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Math/MathFunc.h"

#include <GL/glew.h>

ParticleEmitter::ParticleEmitter(GameObject* owner) :
	particlesPerSecond(50),
	maxParticles(200),
	minLifeTime(0.7f),
	maxLifeTime(1.7f),
	isActive(true),
	showTexMenu(false),
	emitterName(""),
	own(owner)
	//VAO(0u),
	//instanceVBO(0u),
	//vertexVBO(0u)
{
	poolIndex = maxParticles - 1;
	particlePool.resize(maxParticles);

	//particleReference = new Particle(own);
	effects.resize(5);

	timer = 1.0f / particlesPerSecond;
	currTimer = timer;

	TransformComponent* tr = owner->GetComponent<TransformComponent>();
	float3 pos = tr->GetPosition();

	/*float v[] = {
		pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};*/

	std::vector<Vertex> vertices;
	Vertex vertex;

	vertex.position = { pos.x - 0.5f, pos.y - 0.5f, pos.z + 0.0f };
	vertex.normal = float3::zero;
	vertex.texCoords = float2::zero;
	vertices.push_back(vertex);

	vertex.position = { pos.x + 0.5f, pos.y - 0.5f, pos.z + 0.0f };
	vertex.normal = float3::zero;
	vertex.texCoords = float2::zero;
	vertices.push_back(vertex);

	vertex.position = { pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.0f };
	vertex.normal = float3::zero;
	vertex.texCoords = float2::zero;
	vertices.push_back(vertex);

	vertex.position = { pos.x - 0.5f, pos.y + 0.5f, pos.z + 0.0f };
	vertex.normal = float3::zero;
	vertex.texCoords = float2::zero;
	vertices.push_back(vertex);


	data.maxQuads = maxParticles;
	data.maxVertices = data.maxQuads * 4;
	data.maxIndices = data.maxQuads * 6;

	data.vertexArray = new VertexArray();
	
	data.vertexBuffer = new VertexBuffer();
	data.vertexBuffer->SetData(data.maxVertices);
	data.vertexBuffer->SetLayout({
		{ShaderDataType::VEC3F, "position"},
		{ShaderDataType::VEC3F, "normal"},
		{ShaderDataType::VEC2F, "texCoords"}
	});
	data.vertexArray->AddVertexBuffer(*data.vertexBuffer);
	//vertexBuffer->Unbind();

	data.vertexBufferBase = new Vertex[data.maxVertices];

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
	//indexBuffer->Unbind();

	data.shader = std::static_pointer_cast<Shader>(ResourceManager::GetInstance()->LoadResource(std::string("Assets/Resources/Shaders/particlesBehaviour.shader")));

	data.vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	data.vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	data.vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	data.vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

}

ParticleEmitter::~ParticleEmitter()
{
	RELEASE(data.vertexArray);
	RELEASE(data.vertexBuffer);
	RELEASE(data.indexBuffer);
}

void ParticleEmitter::Emit(float dt, const ParticleProps& props)
{
	Particle& particle = particlePool[poolIndex];
	particle.active = true;

	particle.position = props.position;
	particle.rotation = random.Float() * 2 * pi;

	particle.velocity = props.velocity;
	particle.velocity.x += props.acceleration.x * (random.Float() - 0.5f);
	particle.velocity.y += props.acceleration.y * (random.Float() - 0.5f);

	particle.colorBegin = props.colorBegin;
	particle.colorEnd = props.colorEnd;

	particle.lifeTime = props.lifeTime;
	particle.lifeRemaining = props.lifeTime;
	particle.sizeBegin = props.sizeBegin + props.sizeVariation * (random.Float() - 0.5f);
	particle.sizeEnd = props.sizeEnd;

	poolIndex = --poolIndex % particlePool.size();

	//currTimer -= dt;
	//if (currTimer <= 0.0f) {
	//
	//	for (int i = 0; i < particlesBuff.size(); i++) {
	//		// When the particle is allocated in memory, but it's not being used at the moment
	//		// Reuse an exisiting particle to make the smaller complexity, which results in more optimized code 
	//
	//		TransformComponent* transform = (TransformComponent*)own->GetComponent<TransformComponent>();
	//		if (transform != nullptr)
	//		{
	//			if (particlesBuff[i].isActive == false)
	//			{
	//				particlesBuff[i].isActive = true;
	//				particlesBuff[i].position = transform->position;
	//				particlesBuff[i].rotation = particleReference->rotation;
	//				particlesBuff[i].size = particleReference->size;
	//				particlesBuff[i].color = particleReference->color;
	//				particlesBuff[i].lifeTime = particleReference->lifeTime;
	//
	//				for (int j = 0; j < effects.size(); j++)
	//				{
	//					if (effects[j] != nullptr)
	//					{
	//						effects[j]->Init(particlesBuff[i]);
	//					}
	//				}
	//				return;
	//			}
	//		}
	//	}
	//
	//	if (particlesBuff.size() < maxParticles) {
	//		// Create new particle
	//		Particle* particle = new Particle(particleReference, own);
	//		particlesBuff.push_back(*particle);
	//	}
	//	currTimer = timer;
	//}
}

void ParticleEmitter::DrawParticle(const float3& pos, float rotation, const float3& size)
{
	if (data.indexCount >= data.maxIndices)
		NextBatch();

	Quat q;
	q.SetFromAxisAngle({ 0,0,1 }, rotation);
	float4x4 transform = float4x4::FromTRS(pos, q, size);

	const int quadVertexCount = 4;
	for (int i = 0; i < quadVertexCount; i++)
	{
		float4 p = transform * data.vertexPositions[i];
		data.vertexBufferPtr->position = p.Float3Part();
		data.vertexBufferPtr->normal = float3::zero;
		data.vertexBufferPtr->texCoords = float2::zero;
		//data.vertexBufferPtr->Color = color;
		//data.vertexBufferPtr->TexCoord = textureCoords[i];
		//data.vertexBufferPtr->TexIndex = textureIndex;
		//data.vertexBufferPtr->TilingFactor = tilingFactor;
		//data.vertexBufferPtr->EntityID = entityID;
		data.vertexBufferPtr++;
	}

	data.indexCount += 6;
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

	
	int i = 0;
	for (; i < particlePool.size(); ++i)
	//for (auto& particle : particlePool)
	{
		Particle& particle = particlePool[i];

		if (!particle.active)
			continue;

		float life = particle.lifeRemaining / particle.lifeTime;
		//float4 color = float4::Lerp(particle.colorEnd, particle.colorBegin, life);
		float size = Lerp(particle.sizeEnd, particle.sizeBegin, life);
		
		//Quat q = Quat({ 0,0,1 }, particle.rotation);
		//float4x4 tr = float4x4::FromTRS({ particle.position.x, particle.position.y, 0.0f }, q, { size, size, 1.0f });
		
		DrawParticle(particle.position, particle.rotation, { size,size,0.0f });

		//data.shader->SetUniformMatrix4f("model", tr.Transposed());
		//data.shader->SetUniformVec4f("color", color);
		//data.vertexArray->Bind();
		//data.vertexBuffer->Bind();
		//data.indexBuffer->Bind();
		//glDrawElements(GL_TRIANGLES, data.indexCount, GL_UNSIGNED_INT, 0);
		//data.shader->Unbind();
		//data.vertexArray->Unbind();
		//data.vertexBuffer->Unbind();
		//data.indexBuffer->Unbind();
	}

	uint32_t dataSize = (uint32_t)((uint8_t*)data.vertexBufferPtr - (uint8_t*)data.vertexBufferBase);
	data.vertexBuffer->SetData(data.vertexBufferBase, dataSize);

	data.vertexArray->Bind();

	data.shader->Bind();
	data.shader->SetUniformMatrix4f("view", view.Transposed());
	data.shader->SetUniformMatrix4f("projection", proj.Transposed());
	
	data.vertexBuffer->Bind();
	data.indexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, data.indexCount, GL_UNSIGNED_INT, 0);
	data.indexBuffer->Unbind();
	data.vertexBuffer->Unbind();
	data.shader->Unbind();
	data.vertexArray->Unbind();

}

void ParticleEmitter::UpdateParticle(float dt)
{
}

void ParticleEmitter::Update(float dt)
{
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
		particle.position += particle.velocity * dt;
		particle.rotation += 0.001 * dt;

		int a = 0;
		a += 2;
	}
}

void ParticleEmitter::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "##";
	suffixLabel += emitterIndex;
	std::string guiName = "Emitter " + suffixLabel;

	if (ImGui::CollapsingHeader(guiName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		strcpy(charsOfName, guiName.c_str());
		guiName = suffixLabel + "Name";
		guiName = "Delete Emitter" + suffixLabel;
		if (ImGui::Button(guiName.c_str()))
		{
			toDelete = true;
		}

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Indent();

		guiName = "Particle min lifetime" + suffixLabel;
		ImGui::DragFloat(guiName.c_str(), &minLifeTime, 0.1f, 0.0f, 10.0f);

		guiName = "Particle max lifetime" + suffixLabel;
		ImGui::DragFloat(guiName.c_str(), &maxLifeTime, 0.1f, 0.0f, 10.0f);

		//particleReference->lifeTime = random.Float(minLifeTime, maxLifeTime);

		guiName = "Particles per Second" + suffixLabel;
		if (ImGui::DragFloat(guiName.c_str(), &particlesPerSecond))
			SetParticlesPerSecond(particlesPerSecond);

		guiName = "Max Particles" + suffixLabel;
		if (ImGui::DragInt(guiName.c_str(), &maxParticles)) {}

		//float size[2] = { particleReference->size.x, particleReference->size.y };

		guiName = "Size" + suffixLabel;
		//if (ImGui::DragFloat2(guiName.c_str(), size))
		//{
		//	particleReference->size.x = size[0];
		//	particleReference->size.y = size[1];
		//}

		//float color[4] = { particleReference->color.r, particleReference->color.g, particleReference->color.b, particleReference->color.a };

		guiName = "Color (RGBA)" + suffixLabel;
		//if (ImGui::ColorPicker3(guiName.c_str(), color))
		//{
		//	particleReference->color.r = color[0];
		//	particleReference->color.g = color[1];
		//	particleReference->color.b = color[2];
		//	particleReference->color.a = color[3];
		//}

		ImGui::Indent();

		ImGui::Text("Texture");

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

		for (int i = (int)ParticleEffectType::NO_TYPE + 1; i <= (int)ParticleEffectType::ACCELERATION_OVER_LIFETIME; i++)
		{
			if (isEffectActive((ParticleEffectType)i))
			{
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Spacing();

				effects[(int)(ParticleEffectType)i]->OnEditor(emitterIndex);
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();

		guiName = "Add Effect##";
		guiName += emitterIndex;

		std::string textNameDisplay = "NEW EFFECT";

		textNameDisplay += emitterIndex;
		if (ImGui::BeginCombo(guiName.c_str(), textNameDisplay.c_str()))
		{
			for (int j = (int)ParticleEffectType::NO_TYPE + 1; j <= (int)ParticleEffectType::ACCELERATION_OVER_LIFETIME; j++)
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
}

void ParticleEmitter::SetParticlesPerSecond(float particlesPerSec)
{
	particlesPerSecond = particlesPerSec;
	timer = 1.0f / particlesPerSecond;
}

void ParticleEmitter::CreateParticleEffect(ParticleEffectType type)
{
	ParticleEffect* effect = nullptr;

	switch (type)
	{
	case ParticleEffectType::NO_TYPE:
		effects[(int)ParticleEffectType::NO_TYPE] = effect;
		break;
	case ParticleEffectType::COLOR_OVER_LIFETIME:
		//TODO
		effects[(int)ParticleEffectType::COLOR_OVER_LIFETIME] = effect;
		break;
	case ParticleEffectType::SIZE_OVER_LIFETIME:
		//TODO
		//effect = (ParticleEffect*)new ParticleEffect_Size();
		effects[(int)ParticleEffectType::SIZE_OVER_LIFETIME] = effect;
		break;
	case ParticleEffectType::VELOCITY_OVER_LIFETIME:
		effect = (ParticleEffect*)new ParticleEffect_Velocity();
		effects[(int)ParticleEffectType::VELOCITY_OVER_LIFETIME] = effect;
		break;
	case ParticleEffectType::ACCELERATION_OVER_LIFETIME:
		effect = (ParticleEffect*)new ParticleEffect_Acceleration();
		effects[(int)ParticleEffectType::ACCELERATION_OVER_LIFETIME] = effect;
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
	case ParticleEffectType::COLOR_OVER_LIFETIME:
		return "Color Effect";
		break;
	case ParticleEffectType::SIZE_OVER_LIFETIME:
		return "Size Effect";
		break;
	case ParticleEffectType::VELOCITY_OVER_LIFETIME:
		return "Velocity Effect";
		break;
	case ParticleEffectType::ACCELERATION_OVER_LIFETIME:
		return "Acceleration Effect";
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
	minLifeTime = node.GetJsonNumber("Emitter: Min Life Time");
	maxLifeTime = node.GetJsonNumber("Emitter: Max Life Time");
	timer = node.GetJsonNumber("Emitter: Timer");
	currTimer = node.GetJsonNumber("Emitter: Current Timer");

	for (int i = 0; i < effects.size(); i++)
		effects[i]->OnLoad(node);

	return true;
}

bool ParticleEmitter::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Max Particles", maxParticles);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Particles Per Second", particlesPerSecond);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Emitter: Is Active", isActive);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Emitter: To Delete", toDelete);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Min Life Time", minLifeTime);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Max Life Time", maxLifeTime);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Timer", timer);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Emitter: Current Timer", currTimer);
	node.SetValueToArray(array, file.GetRootValue());


	for (int i = 0; i < effects.size(); i++)
	{
		if (effects[i] != nullptr)
			effects[i]->OnSave(node, array);
	}

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

void ParticleEmitter::StartBatch()
{
	data.indexCount = 0;
	data.vertexBufferPtr = data.vertexBufferBase;
}

void ParticleEmitter::NextBatch()
{
	Render(0);
	StartBatch();
}

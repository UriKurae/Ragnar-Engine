#include "ParticleEffect_SpawningShape.h"

ParticleEffect_SpawningShape::ParticleEffect_SpawningShape(TransformComponent* transform) : ParticleEffect(ParticleEffectType::SPAWNING_SHAPE),
hasInitialSpeed(false), 
particlesVelocity(0.0f), 
randomVelocityMultiplier(0.0f),
spawnShape(nullptr)
{
	memset(shapeOffset, 0.0f, sizeof(shapeOffset));
	transformComponent = transform;
}

ParticleEffect_SpawningShape::~ParticleEffect_SpawningShape()
{
	if (spawnShape != nullptr)
	{
		delete(spawnShape);
		spawnShape = nullptr;
	}
	transformComponent = nullptr;
}

void ParticleEffect_SpawningShape::Init(Particle& particle)
{
	LCG random;

	if (spawnShape != nullptr)
	{
		float initSpeed = 0.0f;
		if (hasInitialSpeed)
		{
			initSpeed = particlesVelocity;

			if (randomVelocityMultiplier != 0.0f)
			{
				initSpeed += initSpeed * randomVelocityMultiplier * random.Float(-1.0f, 1.0f);
			}
		}

		if (transformComponent != nullptr)
		{
			spawnShape->Spawn(particle, hasInitialSpeed, initSpeed, transformComponent->GetGlobalTransform(), shapeOffset);
		}
	}
}

void ParticleEffect_SpawningShape::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "Shape Effect##";
	suffixLabel += emitterIndex;
	if (ImGui::CollapsingHeader(suffixLabel.c_str(), ImGuiTreeNodeFlags_Bullet))
	{
		suffixLabel = "Delete Shape Effect##";
		suffixLabel += emitterIndex;
		if (ImGui::Button(suffixLabel.c_str()))//TODO all particle effects delete buttons should look like this fix them
			this->toDelete = true;


		ImGui::Spacing();
		ImGui::Indent();

		suffixLabel = "Has Initial Speed##PaShapeEf";
		suffixLabel += emitterIndex;
		ImGui::Checkbox(suffixLabel.c_str(), &hasInitialSpeed);

		if (hasInitialSpeed)
		{
			suffixLabel = "Initial Out Speed##PaShapeEf";
			suffixLabel += emitterIndex;
			ImGui::DragFloat(suffixLabel.c_str(), &particlesVelocity);

			suffixLabel = "Random Speed Multiplier##PaShapeEf";
			suffixLabel += emitterIndex;
			ImGui::DragFloat(suffixLabel.c_str(), &randomVelocityMultiplier);

		}

		suffixLabel = "Shape Offset##PaShapeEf";
		suffixLabel += emitterIndex;
		ImGui::DragFloat3(suffixLabel.c_str(), shapeOffset);


		//=========================================== Combo
		suffixLabel = "Shape Type##PaShapeEf";
		suffixLabel += emitterIndex;

		std::string textNameDisplay = "NONE";
		if (spawnShape != nullptr)
		{
			GetShapeTypeString(spawnShape->GetType(), textNameDisplay);
		}

		textNameDisplay += "##PaShapeEf";
		textNameDisplay += emitterIndex;
		if (ImGui::BeginCombo(suffixLabel.c_str(), textNameDisplay.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			suffixLabel = "NONE##PaShapeEf";
			suffixLabel += emitterIndex;
			const bool noneSelected = (spawnShape == nullptr);

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (noneSelected)
				ImGui::SetItemDefaultFocus();

			//================================
			for (int n = 0; n < (int)SPAWN_SHAPE_TYPE::CIRCUMFERENCE; ++n)
			{
				SPAWN_SHAPE_TYPE iterType = (SPAWN_SHAPE_TYPE)n;

				bool isShapeSelected = false;
				if (spawnShape != nullptr)
				{
					isShapeSelected = (spawnShape->GetType() == iterType);
				}

				GetShapeTypeString(iterType, suffixLabel);
				suffixLabel += "##ListPaShapeEf";
				suffixLabel += emitterIndex;

				if (ImGui::Selectable(suffixLabel.c_str(), isShapeSelected))
				{
					ChangeSpawnShape((SPAWN_SHAPE_TYPE)n);
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isShapeSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		//=========================================== end combo

		if (spawnShape != nullptr)
		{
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Indent();

			spawnShape->OnEditor(emitterIndex);

			ImGui::Unindent();
			ImGui::Spacing();
			ImGui::Separator();
		}
		ImGui::Unindent();
	}
}

SpawnShape* ParticleEffect_SpawningShape::ChangeSpawnShape(SPAWN_SHAPE_TYPE newType)
{
	if (spawnShape == nullptr)
	{
		spawnShape = CreateSpawnShape(newType);
	}
	else if (newType != spawnShape->GetType())
	{
		delete(spawnShape);
		spawnShape = CreateSpawnShape(newType);
	}

	return spawnShape;
}

SpawnShape* ParticleEffect_SpawningShape::CreateSpawnShape(SPAWN_SHAPE_TYPE newType)
{
	SpawnShape* newShape = nullptr;
	//TODO construct different shapes here factory method
	switch (newType)
	{
	case SPAWN_SHAPE_TYPE::SPHERE:
		newShape = new ParticleEffect_SphericalSpawn();
		break;
	case SPAWN_SHAPE_TYPE::CONE:
		//TODO
		break;
	case SPAWN_SHAPE_TYPE::CIRCUMFERENCE:
		//TODO
		break;

	default:
		break;
	}
	return newShape;
}

void ParticleEffect_SpawningShape::GetShapeTypeString(SPAWN_SHAPE_TYPE newType, std::string& out)
{
	out = "";

	switch (newType)
	{
	case SPAWN_SHAPE_TYPE::SPHERE:
		out = "SPHERE";
		break;
	case SPAWN_SHAPE_TYPE::CONE:
		out = "CONE";
		break;
	case SPAWN_SHAPE_TYPE::CIRCUMFERENCE:
		out = "CIRCUMFERENCE";
		break;
	default:
		break;
	}
}

bool ParticleEffect_SpawningShape::OnLoad(JsonParsing& node)
{
	return false;
}

bool ParticleEffect_SpawningShape::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

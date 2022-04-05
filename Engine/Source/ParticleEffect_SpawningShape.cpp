#include "ParticleEffect_SpawningShape.h"

#include "Globals.h"

ParticleEffect_SpawningShape::ParticleEffect_SpawningShape(TransformComponent* transform) : ParticleEffect(ParticleEffectType::SPAWNING_SHAPE),
hasInitialSpeed(false), 
particlesVelocity(0.0f), 
randomVelocityMultiplier(0.0f),
spawnShape(nullptr),
transformComponent(transform)
{
	memset(shapeOffset, 0.0f, sizeof(shapeOffset));
	transformComponent = transform;
	type = ParticleEffectType::SPAWNING_SHAPE;
}

ParticleEffect_SpawningShape::~ParticleEffect_SpawningShape()
{
	if (spawnShape != nullptr)
	{
		RELEASE(spawnShape);
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
			spawnShape->Spawn(particle, hasInitialSpeed, 0.01f, transformComponent->GetGlobalTransform(), shapeOffset);
		}
	}
}

void ParticleEffect_SpawningShape::OnEditor(int emitterIndex)
{
	std::string suffixLabel = "Shape Effect##";
	suffixLabel += emitterIndex;
	if (ImGui::CollapsingHeader(suffixLabel.c_str(), ImGuiTreeNodeFlags_Bullet))
	{
		suffixLabel = "Delete effect##";
		suffixLabel += emitterIndex;

		if (ImGui::Button(suffixLabel.c_str()))//TODO all particle effects delete buttons should look like this fix them
			this->toDelete = true;


		ImGui::Spacing();
		ImGui::Indent();

		suffixLabel = "Has Initial Speed##PaShapeEf";
		suffixLabel += emitterIndex;
		//ImGui::Checkbox(suffixLabel.c_str(), &hasInitialSpeed);

		/*if (hasInitialSpeed)
		{
			suffixLabel = "Initial Out Speed##PaShapeEf";
			suffixLabel += emitterIndex;
			ImGui::DragFloat(suffixLabel.c_str(), &particlesVelocity);

			suffixLabel = "Random Speed Multiplier##PaShapeEf";
			suffixLabel += emitterIndex;
			ImGui::DragFloat(suffixLabel.c_str(), &randomVelocityMultiplier);

		}*/

		suffixLabel = "Offset##PaShapeEf";
		suffixLabel += emitterIndex;
		ImGui::DragFloat3(suffixLabel.c_str(), shapeOffset);

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
		RELEASE(spawnShape);
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
	case SPAWN_SHAPE_TYPE::NONE:
		out = "NONE";
		break;
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
	particlesVelocity = node.GetJsonNumber("PEShape: Velocity");
	randomVelocityMultiplier = node.GetJsonNumber("PEShape: Random Velocity Multiplier");

	shapeOffset[0] = node.GetJsonNumber("PEShape: Offset X");
	shapeOffset[1] = node.GetJsonNumber("PEShape: Offset Y");
	shapeOffset[2] = node.GetJsonNumber("PEShape: Offset Z");

	if ((SPAWN_SHAPE_TYPE)node.GetJsonNumber("PEShape: Shape Type") != SPAWN_SHAPE_TYPE::NONE)
		ChangeSpawnShape((SPAWN_SHAPE_TYPE)node.GetJsonNumber("PEShape: Shape Type"));
	else
	{
		if (spawnShape != nullptr)
			RELEASE(spawnShape);
		spawnShape = new SpawnShape(SPAWN_SHAPE_TYPE::NONE);
	}

	return true;
}

bool ParticleEffect_SpawningShape::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Velocity", particlesVelocity);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Random Velocity Multiplier", randomVelocityMultiplier);

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Offset X", shapeOffset[0]);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Offset Y", shapeOffset[1]);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Offset Z", shapeOffset[2]);

	/*file.SetNewJson3Number(file, "PEShape: Position", transformComponent->GetPosition());
	file.SetNewJson4Number(file, "PEShape: Rotation", transformComponent->GetRotation());
	file.SetNewJson3Number(file, "PEShape: Scale", transformComponent->GetScale());

	float4 row(transformComponent->GetGlobalTransform().At(1, 1), transformComponent->GetGlobalTransform().At(1, 2), transformComponent->GetGlobalTransform().At(1, 3), transformComponent->GetGlobalTransform().At(1, 4));
	file.SetNewJson4Number(file, "PEShape: Gl Mat 1x4", row);
	row = { transformComponent->GetGlobalTransform().At(2, 1), transformComponent->GetGlobalTransform().At(2, 2), transformComponent->GetGlobalTransform().At(2, 3), transformComponent->GetGlobalTransform().At(2, 4) };
	file.SetNewJson4Number(file, "PEShape: Gl Mat 2x4", row);
	row = { transformComponent->GetGlobalTransform().At(3, 1), transformComponent->GetGlobalTransform().At(3, 2), transformComponent->GetGlobalTransform().At(3, 3), transformComponent->GetGlobalTransform().At(3, 4) };
	file.SetNewJson4Number(file, "PEShape: Gl Mat 3x4", row);
	row = { transformComponent->GetGlobalTransform().At(4, 1), transformComponent->GetGlobalTransform().At(4, 2), transformComponent->GetGlobalTransform().At(4, 3), transformComponent->GetGlobalTransform().At(4, 4) };
	file.SetNewJson4Number(file, "PEShape: Gl Mat 4x4", row);
	
	row = { transformComponent->GetLocalTransform().At(1, 1), transformComponent->GetLocalTransform().At(1, 2), transformComponent->GetLocalTransform().At(1, 3), transformComponent->GetLocalTransform().At(1, 4) };
	file.SetNewJson4Number(file, "PEShape: Local Mat 1x4", row);
	row = { transformComponent->GetLocalTransform().At(2, 1), transformComponent->GetLocalTransform().At(2, 2), transformComponent->GetLocalTransform().At(2, 3), transformComponent->GetLocalTransform().At(2, 4) };
	file.SetNewJson4Number(file, "PEShape: Local Mat 2x4", row);
	row = { transformComponent->GetLocalTransform().At(3, 1), transformComponent->GetLocalTransform().At(3, 2), transformComponent->GetLocalTransform().At(3, 3), transformComponent->GetLocalTransform().At(3, 4) };
	file.SetNewJson4Number(file, "PEShape: Local Mat 3x4", row);
	row = { transformComponent->GetLocalTransform().At(4, 1), transformComponent->GetLocalTransform().At(4, 2), transformComponent->GetLocalTransform().At(4, 3), transformComponent->GetLocalTransform().At(4, 4) };
	file.SetNewJson4Number(file, "PEShape: Local Mat 4x4", row);

	file.SetNewJson3Number(file, "PEShape: Rotation Editor", transformComponent->GetRotEditor());*/

	if (spawnShape != nullptr)
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Shape Type", (int)spawnShape->GetType());

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Effect Type", (int)type);

	node.SetValueToArray(array, file.GetRootValue());
	return true;
}

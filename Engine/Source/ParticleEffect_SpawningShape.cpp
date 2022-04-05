#include "ParticleEffect_SpawningShape.h"
#include "Globals.h"

ParticleEffect_SpawningShape::ParticleEffect_SpawningShape(TransformComponent* transform) : ParticleEffect(ParticleEffectType::SPAWNING_SHAPE),
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
		if (transformComponent != nullptr)
			spawnShape->Spawn(particle, transformComponent->GetGlobalTransform(), shapeOffset);
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
	switch (newType)
	{
	case SPAWN_SHAPE_TYPE::SPHERE:
		newShape = new ParticleEffect_SphericalSpawn();
		break;
	case SPAWN_SHAPE_TYPE::CONE:
		newShape = new ParticleEffect_ConeSpawn();
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
	shapeOffset[0] = node.GetJsonNumber("PEShape: Offset X");
	shapeOffset[1] = node.GetJsonNumber("PEShape: Offset Y");
	shapeOffset[2] = node.GetJsonNumber("PEShape: Offset Z");

	SPAWN_SHAPE_TYPE tope = (SPAWN_SHAPE_TYPE)node.GetJsonNumber("PEShape: Shape Type");
	if (tope != SPAWN_SHAPE_TYPE::NONE)
	{
		ChangeSpawnShape((SPAWN_SHAPE_TYPE)node.GetJsonNumber("PEShape: Shape Type"));
		spawnShape->OnLoad(node);
	}
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

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Offset X", shapeOffset[0]);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Offset Y", shapeOffset[1]);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Offset Z", shapeOffset[2]);

	if (spawnShape != nullptr)
	{
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "PEShape: Shape Type", (int)spawnShape->GetType());
		spawnShape->OnSave(file, array);
	}

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Effect Type", (int)type);

	node.SetValueToArray(array, file.GetRootValue());
	return true;
}

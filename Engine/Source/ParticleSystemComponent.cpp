#include "ParticleSystemComponent.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* own, TransformComponent* trans, uint numParticles)
{
	type = ComponentType::PARTICLE_SYSTEM;
	transform = trans;
	active = true; // Component is active
    isActive = false; // Simulation is active

}

ParticleSystemComponent::~ParticleSystemComponent()
{
}

void ParticleSystemComponent::SetEmitter(ParticleEmitter* emitter)
{
	emitters.push_back(emitter);
}

bool ParticleSystemComponent::Update(float dt)
{
	if (isActive && (((float)timer.GetTime()) / 1000.0f < maxDuration || looping == true))
	{
		for (int i = 0; i < emitters.size(); i++)
        {
            emitters[i]->Emit(dt);
			emitters[i]->Update(dt);            
		}
	}

	return true;
}

void ParticleSystemComponent::Draw(CameraComponent* gameCam)
{
	if (isActive)
		for (auto& e : emitters)
			e->Render(gameCam);
}

void ParticleSystemComponent::OnEditor()
{
    if (ImGui::CollapsingHeader("Particle System"))
    {
        if (ImGui::Checkbox("Active", &active));

        ImGui::PushItemWidth(90);

        std::string playButtonName = isActive ? "Pause" : "Play";
        if (ImGui::Button(playButtonName.c_str()))
        {
            isActive = !isActive;

			//if (isActive)
			//    Stop();
			//else
			//    Play();
        }

        ImGui::SameLine();
        ImGui::Text("Played for: %.2f", timer.ReadTime() * 0.001f);

        ImGui::Checkbox("Looping", &looping);
        ImGui::SliderFloat("Duration", &maxDuration, 0.0f, 10.0f);

/*        if (ImGui::Button("Save template"))
        {
            if (app->scene->SceneDirectory().empty())
            {
                std::string filePath = Dialogs::SaveFile("Orange Juice Scene (*.orangeJuice)\0*.orangeJuice\0");
                if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
            }
            else app->scene->SaveScene(app->scene->SceneDirectory().c_str());
        }*/

        ImGui::Spacing();
        std::string guiName = "";
        std::string suffixLabel = "";
        for (int i = 0; i < emitters.size(); ++i)
        {
            suffixLabel = "##";
            suffixLabel += i;
            ImGui::Separator();
            ImGui::Spacing();

            emitters[i]->OnEditor(i);
        }

        if (ImGui::Button("Create Emitter")) {
            emitters.push_back(new ParticleEmitter(owner));
        }

        ImGui::Separator();
    }
}

void ParticleSystemComponent::Play()
{
}

void ParticleSystemComponent::Stop()
{
}

bool ParticleSystemComponent::OnLoad(JsonParsing& node)
{
	return false;
}

bool ParticleSystemComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	return false;
}

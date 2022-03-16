#include "ParticleSystemComponent.h"
#include "BillboardParticleComponent.h"
#include "GameObject.h"
#include "FileSystem.h"
#include "Globals.h"

ParticleSystemComponent::ParticleSystemComponent(GameObject* own, TransformComponent* trans, uint numParticles)
{
	type = ComponentType::PARTICLE_SYSTEM;
	transform = trans;
	active = true; // Component is active
    isActive = false; // Simulation is active
    saveConfig = false;
    loadConfig = false;

    if (own->GetComponent<BillboardParticleComponent>() == nullptr)
        own->CreateComponent(ComponentType::BILLBOARD);
}

ParticleSystemComponent::~ParticleSystemComponent()
{
	for (int i = 0; i < emitters.size(); i++)
	{
		delete emitters[i];
	}
	emitters.clear();
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

    for (std::vector<ParticleEmitter*>::iterator it = emitters.begin(); it != emitters.end(); ++it)
    {
        if ((*it)->toDelete)
        {
            RELEASE((*it));
            emitters.erase(it);
            break;
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
        if (ImGui::Button(ICON_FA_SAVE" Save Configuration"))
            saveConfig = !saveConfig;

        if (ImGui::Button("Load Configuration"))
            loadConfig = !loadConfig;

        if (saveConfig)
            SaveConfiguration();

        if (loadConfig)
            LoadConfiguration();

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

        if (ImGui::Button(ICON_FA_PLUS" Create Emitter")) {
            emitters.push_back(new ParticleEmitter(owner));
        }

/*        if (ImGui::Button("Save template"))
        {
            if (app->scene->SceneDirectory().empty())
            {
                std::string filePath = Dialogs::SaveFile("Orange Juice Scene (*.orangeJuice)\0*.orangeJuice\0");
                if (!filePath.empty()) app->scene->SaveScene(filePath.c_str());
            }
            else app->scene->SaveScene(app->scene->SceneDirectory().c_str());
        }*/

        //ImGui::Spacing();
        std::string guiName = "";
        std::string suffixLabel = "";
        for (int i = 0; i < emitters.size(); ++i)
        {
            suffixLabel = "##";
            suffixLabel += i;
            ImGui::Separator();
            //ImGui::Spacing();

            emitters[i]->OnEditor(i);
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Unindent();
        ImGui::Separator();
        ComponentOptions(this);
        ImGui::Separator();
    }
}

void ParticleSystemComponent::Play()
{
}

void ParticleSystemComponent::Stop()
{
}

void ParticleSystemComponent::SaveConfiguration()
{
    ImGui::Separator();

    const char extension[13] = ".rgparticles";
    static char name[25] = "\0";

    ImGui::Text("Name: "); ImGui::SameLine();

    std::string id("##");
    id += extension;

    ImGui::InputText(id.c_str(), name, sizeof(char) * 25);
    if (ImGui::Button("Create"))
    {
        std::string nameFile = name;
        std::string path = PARTICLES_FOLDER;
        path += name;

        if (path.find('.') == path.npos)
            path += extension;

        //TODO: Check if the extension is correct, to avoid a .cs.glsl file
        if (path.find(extension) != path.npos)
        {
            JsonParsing particuleFile;

            particuleFile = particuleFile.SetChild(particuleFile.GetRootValue(), "Particle");
            JSON_Array* array = particuleFile.SetNewJsonArray(particuleFile.GetRootValue(), "Emitters");

            bool nameDone = false;
            int count = 0;
            while (!nameDone)
            {
                if (app->fs->Exists(path.c_str()) != 0)
                {
                    count++;
                    nameFile = nameFile.substr(0, nameFile.find_last_of("(")) + "(" + std::to_string(count) + ")";
                    path = PARTICLES_FOLDER;
                    path += nameFile + extension;
                }
                else
                {
                    nameDone = true;
                }
            }

            OnSave(particuleFile, array);

            char* buf;
            uint size = particuleFile.Save(&buf);

            if (app->fs->Save(path.c_str(), buf, size) > 0)
                DEBUG_LOG("Particle Configuration saved succesfully");
            else
                DEBUG_LOG("Particle Configuration couldn't be saved");

            RELEASE_ARRAY(buf);
            name[0] = '\0';
        }

        saveConfig = false;
        ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
        saveConfig = false;
        ImGui::CloseCurrentPopup();
    }

    ImGui::Separator();
}

void ParticleSystemComponent::LoadConfiguration()
{
    ImGui::Separator();

    std::string guiName = "Configuration";
    std::vector<std::string> files;
    app->fs->DiscoverFiles(PARTICLES_FOLDER, files);

    if (ImGui::BeginCombo(guiName.c_str(), "Select Configuration"))
    {
        std::string name;

        for (int i = 0; i < files.size(); ++i)
        {
            name = files[i];
            app->fs->GetFilenameWithoutExtension(name);
            guiName = name;

            if (ImGui::Selectable(guiName.c_str()))
            {
                std::string path = PARTICLES_FOLDER + files[i];
                JsonParsing particleFile = JsonParsing();

                if (particleFile.ParseFile(path.c_str()) > 0)
                {
                    OnLoad(particleFile);
                }
                else
                {
                    DEBUG_LOG("Particle Configuration couldn't be loaded");
                }

                loadConfig = false;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
}

bool ParticleSystemComponent::OnLoad(JsonParsing& node)
{
    JSON_Array* emittersArray = node.GetJsonArray(node.ValueToObject(node.GetRootValue()), "Emitters");
    size_t size = node.GetJsonArrayCount(emittersArray);

    for (int i = 0; i < size; ++i)
    {
        JsonParsing file = node.GetJsonArrayValue(emittersArray, i);
        ParticleEmitter* emitter = new ParticleEmitter(owner);
        emitter->OnLoad(file);
        emitters.push_back(emitter);
    }

    //for (auto& emitter : emitters)
    //    emitter->OnLoad(node);
	return true;
}

bool ParticleSystemComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
    JsonParsing file = JsonParsing();

    file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
    file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

    JSON_Array* emittersArray = file.SetNewJsonArray(file.GetRootValue(), "Emitters");

    for (auto& emitter : emitters)
        emitter->OnSave(node, emittersArray);

    node.SetValueToArray(array, file.GetRootValue());

	return true;
}

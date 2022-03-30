#include "DialogueSystem.h"

#include "Application.h"
#include "FileSystem.h"
#include "Globals.h"
#include "Imgui/imgui.h"
#include "IconsFontAwesome5.h"

DialogueSystem* DialogueSystem::instance = nullptr;

DialogueSystem* DialogueSystem::GetInstance()
{
	if (!instance) instance = new DialogueSystem();

	return instance;
}

void DialogueSystem::ReleaseInstance()
{
	RELEASE(instance);
}

DialogueSystem::DialogueSystem()
{
	createDialogue = false;
}

DialogueSystem::~DialogueSystem()
{
}

void DialogueSystem::OnEditor()
{
	ImGui::Begin("Create Dialogue", &createDialogue);

	ShowDialogueFiles();

	int counterId = 0;
	for (std::vector<Dialogue>::iterator it = dialogues.begin(); it != dialogues.end(); ++it)
	{
		//ImGui::InputInt("Id", &(*it).id);

		ImGui::Text("Dialogue ID: %d", counterId);

		(*it).id = counterId;

		char textAuthor[32];
		char text[256];

		int counter = 0;
		for (std::vector<DialogueLine>::iterator it2 = (*it).dialogue.begin(); it2 != (*it).dialogue.end(); ++it2)
		{
			ImGui::PushID(counter);

			strcpy_s(textAuthor, (*it2).author.c_str());
			ImGui::InputText("Author", textAuthor, IM_ARRAYSIZE(textAuthor));
			(*it2).author = textAuthor;

			strcpy_s(text, (*it2).line.c_str());
			ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
			(*it2).line = text;

			ImGui::PopID();
			counter++;
		}

		if (ImGui::Button(ICON_FA_PLUS" Add Line"))
		{
			std::string aux = "";
			//dialogues.push_back(aux);
		}

		if (ImGui::Button(ICON_FA_FILE" Create Dialogue Archive"))
		{

		}

		counterId++;
	}

	ImGui::End();
}

void DialogueSystem::ShowDialogueFiles()
{
	std::string guiName = "Dialogues";
	std::vector<std::string> files;
	app->fs->DiscoverFiles(DIALOGUES_FOLDER, files);

	guiName = guiName.substr(0, guiName.find_last_of("."));

	if (ImGui::BeginCombo(guiName.c_str(), "Select Dialogue To Edit"))
	{
		std::string name;

		if (ImGui::Selectable("New Dialogue File"))
		{
			for (std::vector<Dialogue>::iterator it = dialogues.begin(); it != dialogues.end(); ++it)
			{
				(*it).dialogue.clear();
			}
			dialogues.clear();

			DialogueLine auxLine;
			auxLine.line = "";
			auxLine.author = "";

			Dialogue auxDialog;
			auxDialog.id = 0;
			auxDialog.dialogue.push_back(auxLine);

			dialogues.push_back(auxDialog);
		}

		for (int i = 0; i < files.size(); ++i)
		{
			name = files[i];
			app->fs->GetFilenameWithoutExtension(name);
			guiName = name;

			if (ImGui::Selectable(guiName.c_str()))
			{
				std::string path = DIALOGUES_FOLDER + files[i];

				JsonParsing particleFile = JsonParsing();

				if (particleFile.ParseFile(path.c_str()) > 0)
				{
					LoadDialogue(path);
				}
			}
		}

		ImGui::EndCombo();
	}
}

void DialogueSystem::LoadDialogue(std::vector<std::string>& lines, std::string path)
{
	JsonParsing dialogFile = JsonParsing();

	if (dialogFile.ParseFile(path.c_str()) > 0)
	{
		JSON_Array* jsonArray = dialogFile.GetJsonArray(dialogFile.ValueToObject(dialogFile.GetRootValue()), "Dialogues");


	}
}

void DialogueSystem::LoadDialogue(std::string path)
{
	JsonParsing dialogFile = JsonParsing();

	if (dialogFile.ParseFile(path.c_str()) > 0)
	{
		JSON_Array* jsonArray = dialogFile.GetJsonArray(dialogFile.ValueToObject(dialogFile.GetRootValue()), "Dialogues");

		fileName = dialogFile.GetJsonString("Name");

		size_t size = dialogFile.GetJsonArrayCount(jsonArray);
		for (int i = 0; i < size; ++i)
		{
			JsonParsing dialogue = dialogFile.GetJsonArrayValue(jsonArray, i);

			Dialogue* aux = &dialogues[i];

			aux->id = dialogue.GetJsonNumber("Id");

			JSON_Array* linesArray = dialogue.GetJsonArray(dialogue.ValueToObject(dialogue.GetRootValue()), "Dialogue Lines");
			
			size_t lineSize = dialogue.GetJsonArrayCount(linesArray);
			for (int j = 0; j < lineSize; ++j)
			{
				JsonParsing dialogueLine = dialogue.GetJsonArrayValue(linesArray, j);

				DialogueLine* auxLine = &aux->dialogue[j];

				auxLine->author = dialogueLine.GetJsonString("Author");
				auxLine->line = dialogueLine.GetJsonString("Line");
			}
		}
	}
}

void DialogueSystem::SaveDialogue(Dialogue& dialogue)
{
	JsonParsing dialogueFile;
	JSON_Array* arrayDialogue = dialogueFile.SetNewJsonArray(dialogueFile.GetRootValue(), "Dialogues");

	dialogueFile.SetNewJsonString(dialogueFile.ValueToObject(dialogueFile.GetRootValue()), "Name", fileName.c_str());

	for (std::vector<Dialogue>::iterator it = dialogues.begin(); it != dialogues.end(); ++it)
	{
		JsonParsing file = JsonParsing();

		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Id", (*it).id);

		JSON_Array* arrayLines = file.SetNewJsonArray(file.GetRootValue(), "Dialogue Lines");

		for (std::vector<DialogueLine>::iterator it2 = (*it).dialogue.begin(); it2 != (*it).dialogue.end(); ++it2)
		{
			JsonParsing file2 = JsonParsing();

			file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Author", (*it2).author.c_str());
			file.SetNewJsonString(file.ValueToObject(file.GetRootValue()), "Line", (*it2).line.c_str());

			file.SetValueToArray(arrayLines, file2.GetRootValue());
		}

		dialogueFile.SetValueToArray(arrayDialogue, file.GetRootValue());
	}

	char* buf;
	uint size = dialogueFile.Save(&buf);

	std::string savePath = DIALOGUES_ASSETS_FOLDER;
	savePath += fileName;
	savePath += ".rgdialogue";

	if (app->fs->Save(savePath.c_str(), buf, size) > 0)
		DEBUG_LOG("Dialogue saved succesfully");
	else
		DEBUG_LOG("Dialogue couldn't be saved");

	savePath = DIALOGUES_FOLDER;
	savePath += fileName;
	savePath += ".rgdialogue";
	app->fs->Save(savePath.c_str(), buf, size);

	RELEASE_ARRAY(buf);
}

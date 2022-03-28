#pragma once

#include <vector>
#include <string>

class DialogueLine
{
public:
	std::string author;
	std::string line;
};

class Dialogue
{
public:
	int id;
	std::vector<DialogueLine> dialogue;
};

class DialogueSystem
{
public:
	static DialogueSystem* GetInstance();
	static void ReleaseInstance();
	~DialogueSystem();

	void OnEditor();
	void LoadDialogue(std::vector<std::string>& dialogs, std::string path);
	void SaveDialogue(Dialogue& dialogue);

	bool createDialogue;

private:
	DialogueSystem();

	int numOfLines;
	std::vector<Dialogue> dialogues;

	Dialogue currDialogue;

	static DialogueSystem* instance;
};
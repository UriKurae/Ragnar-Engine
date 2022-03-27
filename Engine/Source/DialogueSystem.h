#pragma once

#include <vector>
#include <string>

class Dialogue
{
public:

	std::vector<std::string> dialogue;
};

class DialogueSystem
{
public:
	static DialogueSystem* GetInstance();
	static void ReleaseInstance();
	~DialogueSystem();

	void OnEditor();
	void LoadDialogue(std::vector<std::string>& dialogs, std::string path);

	bool createDialogue;

private:
	DialogueSystem();

	int numOfLines;
	std::vector<std::string> dialogueLines;

	static DialogueSystem* instance;
};
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
	void ShowDialogueFiles();

	void LoadDialogue(std::string path);
	void SaveDialogue();

	void Reset();

	inline Dialogue* GetCurrentDialogue() { return currDialogue; }
	Dialogue* GetDialogueById(int id);
	inline void SetDialogueAsCurrent(Dialogue* newDialog) { currDialogue = newDialog; }
	void StartDialogue();
	void NextLine();
	inline std::string GetCurrentLine() { return currLine->line; }
	inline std::string GetOwnerOfLine() { return currLine->author; }

	void ImportToLibrary();

	bool createDialogue;

private:
	DialogueSystem();

	bool newDialogueFile;

	std::string fileName;
	std::vector<Dialogue> dialogues;

	Dialogue* currDialogue;
	DialogueLine* currLine;

	static DialogueSystem* instance;
};
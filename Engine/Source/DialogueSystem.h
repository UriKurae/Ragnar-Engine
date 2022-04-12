#pragma once

#include <vector>
#include <string>
#include <list>

class DialogueLine
{
public:
	std::string author = "";
	std::string line = "";
};

class Dialogue
{
public:
	int id;
	std::vector<DialogueLine> dialogue;
};


//MHF
class DialogueLineXML
{
public:
	int authorId = 0;
	std::string line = "";
};

class DialogueXML
{
public:
	int id;
	std::vector<DialogueLineXML*> dialogue;
};

//-------------------------

class DialogueSystem
{
public:
	static DialogueSystem* GetInstance();
	static void ReleaseInstance();
	~DialogueSystem();

	void OnEditor();
	void ShowDialogueFiles();

	void LoadDialogue(std::string path);

	//MHF
	void LoadDialogueXML(std::string path);
	//void LoadLinesXML(pugi::xml_node& node, DialogueXML* dlg);
	//

	void SaveDialogue();

	void Reset();

	inline Dialogue* GetCurrentDialogue() { return currDialogue; }
	Dialogue* GetDialogueById(int id);
	inline void SetDialogueAsCurrent(Dialogue* newDialog) { currDialogue = newDialog; }
	void StartDialogue();
	void NextLine();
	inline std::string GetCurrentLine() { return currLine->line; }
	inline std::string GetOwnerOfLine() { return currLine->author; }

	bool createDialogue;

private:
	DialogueSystem();

	bool newDialogueFile;

	std::string fileName;
	std::vector<Dialogue> dialogues;

	//MHF
	pugi::xml_document dialoguesXML;
	std::vector<DialogueXML*> aDialogueXML;
	std::list<std::string> authorList = {"a","b","c"};
	//

	Dialogue* currDialogue;
	DialogueLine* currLine;

	static DialogueSystem* instance;
};
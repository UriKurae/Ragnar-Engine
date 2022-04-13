#pragma once

#include <vector>
#include <string>
#include <list>

//MHF
#include "PugiXML/pugixml.hpp"

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
	void LoadDialogueXML();
	void LoadLinesXML(pugi::xml_node& node, DialogueXML* dlg);
	void SetCurrentDialogueIdXML(int id) { 
		for (int i = 0; i < aDialogueXML.size(); i++){
			if (id == aDialogueXML[i]->id){
				currDialogXML = aDialogueXML[i];
			}
		}
	}
	// If it returns false it means that the dialog is finished
	bool NextLineXML(){
		indexLine ++;

		if (currDialogXML->dialogue.size() < indexLine)
			return false;

		currLineXML = currDialogXML->dialogue[indexLine];
		return true;
	}
	inline std::string GetCurrentLineXML() { return currLineXML->line; }
	inline std::string GetOwnerOfLineXML() { return authorList[currLineXML->authorId]; }
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

	Dialogue* currDialogue;
	DialogueLine* currLine;

	//MHF
	int indexLine=0;
	std::string authorList[16];
	pugi::xml_document dialoguesXML;
	std::vector<DialogueXML*> aDialogueXML;
	DialogueXML* currDialogXML;
	DialogueLineXML* currLineXML;
	//


	static DialogueSystem* instance;
};
#pragma once
#include "Menu.h"
#include "TextEditor.h"

#include <vector>

class TextEditorMenu : public Menu
{
public:
	TextEditorMenu();
	~TextEditorMenu();

	bool Start();
	bool Update(float dt);

	void SetFile(const char* file);

	bool IsFocused() const;

	void GetAllFiles();

	TextEditor textEditor;


	std::string fileToEdit;
	std::string fileName;

private:

	std::vector<std::string> allScriptsFiles;
	bool isFocused = false;
};

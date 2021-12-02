#pragma once

#include <string>
#include <windows.h>
#include <commdlg.h>
#include "Application.h"
#include "ModuleWindow.h"
#include <SDL/include/SDL_syswm.h>

namespace Dialogs
{
	static std::string OpenFile(const char* path)
	{
		OPENFILENAME openFile;
		char sizeFile[260] = {};
		ZeroMemory(&openFile, sizeof(OPENFILENAME));
		openFile.lStructSize = sizeof(OPENFILENAME);

		openFile.hwndOwner = GetActiveWindow();
		openFile.lpstrFile = sizeFile;
		openFile.nMaxFile = sizeof(openFile);
		openFile.lpstrFilter = path;
		openFile.lpstrDefExt = "ragnar";
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileName(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		return std::string();
	}

	static std::string SaveFile(const char* path)
	{
		OPENFILENAME openFile;
		char sizeFile[260] = {};
		ZeroMemory(&openFile, sizeof(OPENFILENAME));
		openFile.lStructSize = sizeof(OPENFILENAME);

		//SDL_SysWMinfo win;
		//SDL_VERSION(&win.version);
		//SDL_GetWindowWMInfo(app->window->window, &win);
		openFile.hwndOwner = GetActiveWindow();
		openFile.lpstrFile = sizeFile;
		openFile.nMaxFile = sizeof(openFile);
		openFile.lpstrFilter = path;
		openFile.lpstrDefExt = "ragnar";
		openFile.nFilterIndex = 1;
		openFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileName(&openFile) == TRUE)
		{
			return openFile.lpstrFile;
		}
		return std::string();
	}
}
#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"

#include "Profiling.h"

enum class MainState
{
	CREATE = 1,
	INIT,
	UPDATE,
	CLEAN,
	FAIL,
	EXIT
};

Application* app = nullptr;

int main(int argc, char ** argv)
{
	DEBUG_LOG("Starting game '%s'...", TITLE);

	int mainReturn = EXIT_FAILURE;
	MainState state = MainState::CREATE;

	while (state != MainState::EXIT)
	{
		switch (state)
		{
		case MainState::CREATE:
			DEBUG_LOG("CREATION PHASE ===============================");

			app = new Application();

			if (app != NULL)
				state = MainState::INIT;
			else
				state = MainState::FAIL;

			break;

			// Awake all modules -----------------------------------------------
		case MainState::INIT:
			DEBUG_LOG("AWAKE PHASE ===============================");
			if (app->Init() == true)
				state = MainState::UPDATE;
			else
			{
				DEBUG_LOG("ERROR: Awake failed");
				state = MainState::FAIL;
			}

			break;

			// Loop all modules until we are asked to leave ---------------------
		case MainState::UPDATE:
		{
			RG_PROFILING_FRAME("MainThread");
			if (app->Update() == false)
				state = MainState::CLEAN;
		}
		break;

		// Cleanup allocated memory -----------------------------------------
		case MainState::CLEAN:
			DEBUG_LOG("CLEANUP PHASE ===============================");
			if (app->CleanUp() == true)
			{
				RELEASE(app);
				mainReturn = EXIT_SUCCESS;
				state = MainState::EXIT;
			}
			else
				state = MainState::FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
		case MainState::FAIL:
			DEBUG_LOG("Exiting with errors :(");
			mainReturn = EXIT_FAILURE;
			state = MainState::EXIT;
			break;
		}
	}

	DEBUG_LOG("Exiting game '%s'...\n", TITLE);
	return mainReturn;
}
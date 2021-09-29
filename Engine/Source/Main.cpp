#include <stdlib.h>
#include "Application.h"

#include "SDL/include/SDL.h"

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
	LOG("Starting game '%s'...", TITLE);

	int mainReturn = EXIT_FAILURE;
	MainState state = MainState::CREATE;

	while (state != MainState::EXIT)
	{
		switch (state)
		{
		case MainState::CREATE:
			LOG("CREATION PHASE ===============================");

			app = new Application();

			if (app != NULL)
				state = MainState::INIT;
			else
				state = MainState::FAIL;

			break;

			// Awake all modules -----------------------------------------------
		case MainState::INIT:
			LOG("AWAKE PHASE ===============================");
			if (app->Init() == true)
				state = MainState::UPDATE;
			else
			{
				LOG("ERROR: Awake failed");
				state = MainState::FAIL;
			}

			break;

			// Loop all modules until we are asked to leave ---------------------
		case MainState::UPDATE:
		{
			if (app->Update() == false)
				state = MainState::CLEAN;
		}
		break;

		// Cleanup allocated memory -----------------------------------------
		case MainState::CLEAN:
			LOG("CLEANUP PHASE ===============================");
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
			LOG("Exiting with errors :(");
			mainReturn = EXIT_FAILURE;
			state = MainState::EXIT;
			break;
		}
	}

	LOG("Exiting game '%s'...\n", TITLE);
	return mainReturn;
}
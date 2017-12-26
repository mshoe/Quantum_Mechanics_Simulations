#include "InputEngine.h"

InputEngine::InputEngine()
{
	init = Init();
}

InputEngine::~InputEngine()
{
	CleanUp();
}

bool InputEngine::Init()
{
	return true;
}

void InputEngine::CleanUp()
{
}

void InputEngine::Input()
{
	SDL_Event event;
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                  */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				loop = false;
				break;
			case SDLK_SPACE:
				pause = !pause;
				if (pause)
					std::cout << "paused" << std::endl;
				else
					std::cout << "unpaused" << std::endl;
			case SDLK_RIGHT:
				angle += 5.f;
				break;
			case SDLK_LEFT:
				angle -= 5.f;
				break;
			default:
				break;
			}
		case SDL_MOUSEMOTION:

			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W])
	{
	}
}

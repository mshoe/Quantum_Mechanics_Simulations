#include <SDL2/SDL.h>
#include <iostream>

class InputEngine
{
public:
	InputEngine();
	~InputEngine();

	bool Init();
	bool init;
	void CleanUp();

	void Input();

	bool pause = false;
	bool loop = true;

	float angle = 0.f;
private:

};
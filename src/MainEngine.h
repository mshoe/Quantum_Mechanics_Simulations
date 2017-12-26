#pragma once
#include <iostream>
#include <string>
#include <chrono>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "engines.h"
#include "InputEngine.h"

#include "InfWell2DEngine.h"
#include "WavePackets2DSim.h"
#include "HydrogenAtom.h"



class MainEngine
{
public:
	MainEngine();
	~MainEngine();

	// Management
	bool Init();
	bool init;
	void CleanUp();

	// Sub-engine functions
	void Render();
	void Input();

	// Looper and timer elements
	void Loop();
	bool loop;
	std::chrono::high_resolution_clock::time_point time1;
	std::chrono::high_resolution_clock::time_point time2;
	float deltaTime;
	float totalTime;

	// SDL
	// Our SDL_Window ( just like with SDL2 wihout OpenGL)
	SDL_Window *mainWindow;
	int image_width;
	int image_height;

	// imgui
	void Init_imgui();

	void menu();

	// Our opengl context handle
	SDL_GLContext mainContext;
	std::string programName = "QM Simulations by Michael Xu";
	bool SetOpenGLAttributes();
	void PrintSDL_GL_Attributes();
	void CheckSDLError(int line);

	

private:
	// value that describes the current simulation
	unsigned int new_simulation = 0;
	unsigned int current_simulation = 0;

	RenderEngine *renderEngine = NULL;
	InputEngine *inputEngine = NULL;
};

enum Simulation {
	INFINITE_WELL_2D = 0,
	TEST = 1
};
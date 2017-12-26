#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include "Shader.h"

class RenderEngine {
public:
	RenderEngine() { Init(); };
	~RenderEngine() { CleanUp(); };

	virtual bool Init() { return true; };
	virtual void CleanUp() {};

	// Program data from MainEngine
	//int image_width = 0, image_height = 0;
	float time = 0;
	float angle = 0;

	//virtual void UpdateDimensions(int image_width, int image_height) {};
	virtual void UpdateTime(float time) {};
	virtual void SetTime(float time) {};
	virtual void UpdateAngle(float angle) {};

	virtual void RenderSim() {std::cout << "render";};
	virtual void RenderUI() {};

	
protected:

	// VAO's
	GLuint VAO;
	Shader *shader = NULL;
};
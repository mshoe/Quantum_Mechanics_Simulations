#include "RenderEngine.h"

class InfWell2DEngine : public RenderEngine {
public:
	InfWell2DEngine();
	~InfWell2DEngine();

	bool Init();
	bool init;
	void CleanUp();

	void RenderSim();
	void RenderUI();

	// Program data from MainEngine
	//int image_width = 0, image_height = 0;
	float time = 0;
	float angle = 0.f;

	// Particle in an Infinite Potential 2D Well
	float n1 = 1.f, n2 = 1.f, n3 = 0.f, n4 = 0.f, n5 = 0.f, n6 = 0.f;
	float m = 1.f;
	float Lx = 1.f, Ly = 1.f;

	void UpdateTime(float time) {
		this->time += time; 
	}
	void SetTime(float time) {
		this->time = time;
	}
	void UpdateAngle(float angle) {
		this->angle = angle;
	}
};
#include "RenderEngine.h"

class WavePackets2DSim : public RenderEngine {
public:
	WavePackets2DSim();
	~WavePackets2DSim();

	bool Init();
	bool init;
	void CleanUp();

	void RenderSim();
	void RenderUI();

	// Program data from MainEngine
	//int image_width = 0, image_height = 0;
	float time = 0;
	float angle = 45.f;

	// 1D Wave packet simulation
	float length = 100.f;
	int n = 3;

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
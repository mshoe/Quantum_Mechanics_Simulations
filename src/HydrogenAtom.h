#include "RenderEngine.h"

class HydrogenAtom : public RenderEngine {
public:
	HydrogenAtom();
	~HydrogenAtom();

	bool Init();
	bool init;
	void CleanUp();

	void BuildAtom();
	void RenderSim();
	void RenderUI();

	// Program data from MainEngine
	//int image_width = 0, image_height = 0;
	float time = 0;
	float angle = 0.f;

	// Hydrogen Atom
	float dx = 0.01;
	float dB = 0.001;
	int n = 1;
	int l = 0;
	int ml = 0;
	int size_Radial = 0;
	int size_Polar = 0;
	int size_Azimuthal = 0;
	float X_max = 10;

	// Numerical Functions
	float Polar_next(float Polar_before1, float Polar_before2, float beta, float d, float C, float ml);
	float Radial_next(float Radial_before1, float Radial_before2, float x, float d, float C, float E);

	void UpdateTime(float time) {
		this->time = time; 
	}
	void UpdateAngle(float angle) {
		this->angle = angle;
	}
};
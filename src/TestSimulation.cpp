#include "TestSimulation.h"
#include <cmath>

TestSimulation::TestSimulation()
{
	init = Init();
}

TestSimulation::~TestSimulation()
{
	CleanUp();
}

bool TestSimulation::Init()
{
	std::cout << "Test Simulation" << std::endl << std::endl;
	std::cout << "Pset 2 (c) summing Cn^2 * En / E1_i, from n = 1 to 1000" << std::endl;
	float sum = 1.f/3.f;
	for (int n = 1; n < 1000; n++) {
		if (n % 3 == 0 || n % 2 == 0)
			continue;
		sum += prob(n);
	}
	std::cout << sum << std::endl;



	return true;
}

void TestSimulation::CleanUp()
{
}

void TestSimulation::RenderSim()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestSimulation::RenderUI()
{
	ImGui::Begin("Test Simulation");

		



    /*ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;*/
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

float TestSimulation::prob(int n)
{
	return powf(2.f / (3.14159f * sqrt(3.f)) * ( 3.f / (n - 3) * sin((n-3) / 6.f * 3.14159f) + 3.f / (n + 3) * sin((n+3) / 6.f * 3.14159f)), 2) * powf(n, 2) / 9.f;
}

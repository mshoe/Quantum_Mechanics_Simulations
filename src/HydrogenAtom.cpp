#include "HydrogenAtom.h"
#include <cmath>

HydrogenAtom::HydrogenAtom()
{
	init = Init();
}

HydrogenAtom::~HydrogenAtom()
{
	CleanUp();
}

bool HydrogenAtom::Init()
{
	std::cout << "Hydrogen Atom" << std::endl << std::endl;

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	shader = new Shader("HydrogenAtom.vert", "HydrogenAtom.frag");

	BuildAtom();
	

	return true;
}

void HydrogenAtom::CleanUp()
{
	delete shader;
}

void HydrogenAtom::BuildAtom()
{
	std::cout << "Solving Schrodinger's Equation for the Hydrogen Atom..." << std::endl;

	std::cout << "Solving polar equation..." << std::endl;
	// defining size of Polar vector
	size_Polar = 1.57f / dB;
	std::vector<float> Polar(size_Polar);
	float Polar_start = 1.f;
	// decide on whether it is an even or odd function (this is kinda BS rn)
	if (ml % 2 == 0) {
		Polar[0] = Polar_start;
		Polar[1] = Polar_start - dB;
	}
	else {
		Polar[0] = 0;
		Polar[1] = dB;
	}
	float beta = 0 + 2 * dB;
	int C = -l*(l+1);
	// Solving the differential equation with the formula from Exercise 91
	for (int i = 2; i < size_Polar; i++) {
		Polar[i] = Polar_next(Polar[i-1], Polar[i-2], beta, dB, C, ml);
		beta += dB;
	}

	std::cout << "Solving radial equation..." << std::endl;
	// defining size of Radial vector
	size_Radial = X_max / dx;
	std::vector<float> Radial(size_Radial);
	Radial[0] = 0;
	Radial[1] = dx;
	float x = 0 + 2 * dx;
	float E = -1/pow(n,2);
	// Solving the differential equation with the formula from Exercise 92
	for (int i = 2; i < size_Radial; i++) {
		Radial[i] = Radial_next(Radial[i-1], Radial[i-2], x, dx, C, E);
		x += dx;
	}

	std::cout << "Building Atom..." << std::endl;
	// defining size of Azimuthal vector
	size_Azimuthal = 100;
	int render_Size_Radial = size_Radial / 10;
	int render_Size_Polar = size_Polar / 10;
	std::vector<GLfloat> vertices(render_Size_Radial * render_Size_Polar * size_Azimuthal * 4); // 3 components for a spherical position vector (R, theta, phi)
	// 4th component for probability = Radial^2 * Polar^2;
	x = 0;
	float theta = 1.57f;
	float phi = 0;
	float d_phi = 2 * 3.14159f / size_Azimuthal;
	
	for (int k = 0; k < render_Size_Radial; k++) {
		theta = 1.57f;
		for (int j = 0; j < render_Size_Polar; j++) {
				float Prob = pow(Radial[k * 10], 2) * pow(Polar[j * 10], 2);
				phi = 0;
			for (int i = 0; i < size_Azimuthal; i++) {
				vertices[(k * render_Size_Polar * size_Azimuthal + j * size_Azimuthal + i) * 4 + 0] = x; // R
				vertices[(k * render_Size_Polar * size_Azimuthal + j * size_Azimuthal + i) * 4 + 1] = theta; // theta
				vertices[(k * render_Size_Polar * size_Azimuthal + j * size_Azimuthal + i) * 4 + 2] = phi; // phi
				vertices[(k * render_Size_Polar * size_Azimuthal + j * size_Azimuthal + i) * 4 + 3] = Prob; // Probability
				phi += d_phi;
			}
			theta -= 10 * dB;
		}
		x += 0.25 * dx; // distance of x = 10 is too big for the screen
	}

	GLuint VBO;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	// Probability attribute
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_TRUE, 4 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		
	glBindVertexArray(0);

	vertices.clear();
	Radial.clear();
	Polar.clear();

	std::cout << "Building complete" << std::endl << std::endl;
}

void HydrogenAtom::RenderSim()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();

	// view
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(2, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLuint viewLoc = glGetUniformLocation((*shader).Program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// perspective
	glm::mat4 projection = glm::perspective(1.f, (float)800 / (float)600, 0.1f, 100.0f);
	GLuint projectionLoc = glGetUniformLocation((*shader).Program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// model
	glm::mat4 model;
	model = glm::rotate(model, angle / 180.f * 3.14159f, glm::vec3(0, 1, 0));
	GLuint modelLoc = glGetUniformLocation((*shader).Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	//glDrawElements(GL_LINES, size * size * size * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_POINTS, 0, size_Radial / 10 * size_Polar / 10 * size_Azimuthal);
	glBindVertexArray(0);
}

void HydrogenAtom::RenderUI()
{
	ImGui::Begin("Hydrogen Atom");
	ImGui::Text("%d, %d, %d", n, l, ml);
    ImGui::SliderInt("n", &n, 1, 10);
	ImGui::SliderInt("l", &l, 0, 9);
	ImGui::SliderInt("ml", &ml, -9, 9);
	ImGui::SliderFloat("X_max", &X_max, 10, 50);
	if (ImGui::Button("Solve")) {
		if (l < n && abs(ml) <= l)
			BuildAtom();
		else
			std::cout << "Incorrect quantum numbers" << std::endl;
	}
	ImGui::End();
}

float HydrogenAtom::Polar_next(float x1, float x2, float b, float d, float C, float ml)
{
	// x1 = Polar(Beta), x2 = Polar(Beta - delta_Beta), b = beta, d = delta_Beta
	// C = -l(l+1), where l is the orbital quantum number
	// ml = the magnetic quantum number

	return 2.f * x1 - x2 + d * tanf(b) * (x1 - x2) + d * d * (C + (float) ml * ml * (1.f/powf(sinf(b), 2))) * x1;
}

float HydrogenAtom::Radial_next(float x1, float x2, float x, float d, float C, float E)
{
	// x1 = Radial(x), x2 = Radial(x - d), x = x, d = delta_x
	// C = -l(l+1), n = principal quantum number
	return 2.f * x1 - x2 + d * d * (-C / (x*x) - 2/x - E) * x1;
}

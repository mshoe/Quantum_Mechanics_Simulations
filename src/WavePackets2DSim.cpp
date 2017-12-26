#include "WavePackets2DSim.h"

WavePackets2DSim::WavePackets2DSim()
{
	init = Init();
}

WavePackets2DSim::~WavePackets2DSim()
{
	CleanUp();
}

bool WavePackets2DSim::Init()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	shader = new Shader("WavePackets2D.vert", "WavePackets2D.frag");

	// Make array for x and y positions
	int size_x = 100;
	int size_y = 25;
	float Lx = 3.f, Ly = 1.f;
	std::vector<GLfloat> vertices(size_x * size_y * 3); // 3 components for a position vector (x, y, z)
	for (int j = 0; j < size_y; j++) {
		for (int i = 0; i < size_x; i++) {
			vertices[(j * size_x + i) * 3 + 0] = (i - size_x / 2.f) * Lx / size_x; // x
			vertices[(j * size_x + i) * 3 + 1] = 0;
			vertices[(j * size_x + i) * 3 + 2] = (j) * Ly / size_y; // technically z in CG terms, but for physics purposes, this is y
		}
	}

	std::vector<GLuint> indices((size_x - 1) * (size_y - 1) * 6);
	for (int j = 0; j < size_y - 1; j++) {
		for (int i = 0; i < size_x - 1; i++) {
			indices[(j * (size_x - 1) + i) * 6 + 0] = (j + 0) * size_x + i + 0; // 0
			indices[(j * (size_x - 1) + i) * 6 + 1] = (j + 1) * size_x + i + 0; // 1
			indices[(j * (size_x - 1) + i) * 6 + 2] = (j + 0) * size_x + i + 1; // 3
			indices[(j * (size_x - 1) + i) * 6 + 3] = (j + 1) * size_x + i + 0; // 1
			indices[(j * (size_x - 1) + i) * 6 + 4] = (j + 1) * size_x + i + 1; // 2
			indices[(j * (size_x - 1) + i) * 6 + 5] = (j + 0) * size_x + i + 1; // 3
		}
	}

	GLuint VBO, EBO;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);

	vertices.clear();
	indices.clear();

	return true;
}

void WavePackets2DSim::CleanUp()
{
	delete shader;
}

void WavePackets2DSim::RenderSim()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();

	// view
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(2, 1, 2), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 0));
	GLuint viewLoc = glGetUniformLocation((*shader).Program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// perspective
	glm::mat4 projection = glm::perspective(1.f, (float)800 / (float)600, 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(0, image_width, image_height, 0);
	GLuint projectionLoc = glGetUniformLocation((*shader).Program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// model
	glm::mat4 model;
	model = glm::rotate(model, angle /180.f * 3.14159f, glm::vec3(0, 1, 0));
	GLuint modelLoc = glGetUniformLocation((*shader).Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// QM uniforms
	float t = time;// * 0.1f;

	GLuint tLoc = glGetUniformLocation((*shader).Program, "t");
	glUniform1f(tLoc, t);

	int size_x = 100;
	int size_y = 25;

	
	GLuint size_xLoc = glGetUniformLocation((*shader).Program, "size_x");
	glUniform1f(size_xLoc, length);

	GLuint nLoc = glGetUniformLocation(shader->Program, "n");
	glUniform1i(nLoc, n);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (size_x - 1) * (size_y - 1) * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
}

void WavePackets2DSim::RenderUI()
{
	ImGui::Begin("1D Particle-Wave propagation");
	ImGui::Text("Using f(x, y, t) = sum(n-3 to n+3) {A(k) * cos(k*x - w(k)*t}");
	ImGui::Text("k = 1 + n/10");
	ImGui::Text("A(k) = exp(-25*(k-1)^2)");
	ImGui::Text("w(k) = 2 * sqrt(k)");
	ImGui::Text("t = %f", time);
	if (ImGui::Button("Reset Time")) time = 0.f;
	ImGui::SliderFloat("length", &length, 0.0f, 100.0f);
	ImGui::SliderInt("n", &n, 0, 20);
	ImGui::End();
}

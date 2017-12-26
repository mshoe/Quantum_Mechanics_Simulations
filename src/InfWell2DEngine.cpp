#include "InfWell2DEngine.h"


InfWell2DEngine::InfWell2DEngine()
{
	init = Init();
}

InfWell2DEngine::~InfWell2DEngine()
{
	CleanUp();
}

bool InfWell2DEngine::Init()
{
	std::cout << "Infinite Potential 2D Well" << std::endl << std::endl;

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	shader = new Shader("InfWellShader.vert", "InfWellShader.frag");

	// Make array for x and y positions
	int size = 100;
	float Lx = 1.f, Ly = 1.f;
	std::vector<GLfloat> vertices(size * size * 3); // 3 components for a position vector (x, y, z)
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			vertices[(j * size + i) * 3 + 0] = (i - size / 2.f) * Lx / size; // x
			vertices[(j * size + i) * 3 + 1] = 0;
			vertices[(j * size + i) * 3 + 2] = (j - size / 2.f) * Ly / size; // technically z in CG terms, but for physics purposes, this is y
		}
	}

	std::vector<GLuint> indices((size -1) * (size - 1) * 6);
	for (int j = 0; j < size - 1; j++) {
		for (int i = 0; i < size - 1; i++) {
			indices[(j * (size - 1) + i) * 6 + 0] = (j + 0) * size + i + 0; // 0
			indices[(j * (size - 1) + i) * 6 + 1] = (j + 1) * size + i + 0; // 1
			indices[(j * (size - 1) + i) * 6 + 2] = (j + 0) * size + i + 1; // 3
			indices[(j * (size - 1) + i) * 6 + 3] = (j + 1) * size + i + 0; // 1
			indices[(j * (size - 1) + i) * 6 + 4] = (j + 1) * size + i + 1; // 2
			indices[(j * (size - 1) + i) * 6 + 5] = (j + 0) * size + i + 1; // 3
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

void InfWell2DEngine::CleanUp()
{
	delete shader;
}

void InfWell2DEngine::RenderSim()
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

	//GLuint colorLoc = glGetUniformLocation((*shader).Program, "I");

	// QM uniforms
	float t = time * 0.01f;

	GLuint LxLoc = glGetUniformLocation((*shader).Program, "Lx");
	GLuint LyLoc = glGetUniformLocation((*shader).Program, "Ly");
	GLuint tLoc = glGetUniformLocation((*shader).Program, "t");
	GLuint mLoc = glGetUniformLocation((*shader).Program, "m");
	GLuint n1Loc = glGetUniformLocation((*shader).Program, "n1");
	GLuint n2Loc = glGetUniformLocation((*shader).Program, "n2");
	GLuint n3Loc = glGetUniformLocation((*shader).Program, "n3");
	GLuint n4Loc = glGetUniformLocation((*shader).Program, "n4");
	GLuint n5Loc = glGetUniformLocation((*shader).Program, "n5");
	GLuint n6Loc = glGetUniformLocation((*shader).Program, "n6");

	glUniform1f(LxLoc, Lx);
	glUniform1f(LyLoc, Ly);
	glUniform1f(tLoc, t);
	glUniform1f(mLoc, m);
	glUniform1f(n1Loc, n1);
	glUniform1f(n2Loc, n2);
	glUniform1f(n3Loc, n3);
	glUniform1f(n4Loc, n4);
	glUniform1f(n5Loc, n5);
	glUniform1f(n6Loc, n6);

	int size = 100;

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
}

void InfWell2DEngine::RenderUI()
{
	// imgui stuff

		

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"

	ImGui::Begin("Particle in an Infinite Potential 2D Well");


	ImGui::Text("Displaying probability |Psi*||Psi|");
	ImGui::Text("Psi = C1 * Psi_1 + C2 * Psi_2 + ... + C6 * Psi_6");
	ImGui::Text("Psi_n(x, y, t) = psi_n(x, y) * phi_n(t)");
	ImGui::Text("t = %f", time * 0.01f);
    ImGui::SliderFloat("C1", &n1, 0.0f, 1.0f);
	ImGui::SliderFloat("C2", &n2, 0.0f, 1.0f);
	ImGui::SliderFloat("C3", &n3, 0.0f, 1.0f);
	ImGui::SliderFloat("C4", &n4, 0.0f, 1.0f);
	ImGui::SliderFloat("C5", &n5, 0.0f, 1.0f);
	ImGui::SliderFloat("C6", &n6, 0.0f, 1.0f);
	ImGui::SliderFloat("m", &m, 0.0f, 1.0f);
	//ImGui::SliderFloat("Lx", &Lx, 0.0f, 1.0f);
	//ImGui::SliderFloat("Ly", &Ly, 0.0f, 1.0f);


    /*ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;*/
    
	ImGui::End();

	
}

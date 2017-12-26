#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0
  
out vec3 outColor; // Output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float Lx, Ly, t, m;
uniform float n1, n2, n3, n4, n5, n6;

float h = 6.626; // 10^-34, will show on scale
float hbar = 1.055; // 10^-34

float psi(int nx, int ny, float Lx, float Ly, float x, float y) {
	return 2.0 / sqrt(Lx * Ly) * sin( nx * 3.14159 * x / Lx) * sin(ny * 3.14159 * y / Ly);
}

float E(int nx, int ny, float Lx, float Ly, float m) {
	return h * h / (8 * m) * ((nx * nx) / (Lx * Lx) + (ny * ny) / (Ly * Ly));
}

//float phi(int nx, int ny, float E, float t, int conj) {
	
//}

void main()
{
	// sum of multiple quantum solutions
	// for simplicity, nx = ny

	float x = position.x + Lx / 2.f;
	float y = position.z + Ly / 2.f;

	float psi_1 = n1 * psi(1, 1, Lx, Ly, x, y); float E1 = E(1, 1, Lx, Ly, m);
	float psi_2 = n2 * psi(2, 2, Lx, Ly, x, y); float E2 = E(2, 2, Lx, Ly, m);
	float psi_3 = n3 * psi(3, 3, Lx, Ly, x, y); float E3 = E(3, 3, Lx, Ly, m);
	float psi_4 = n4 * psi(4, 4, Lx, Ly, x, y); float E4 = E(4, 4, Lx, Ly, m);
	float psi_5 = n5 * psi(5, 5, Lx, Ly, x, y); float E5 = E(5, 5, Lx, Ly, m);
	float psi_6 = n6 * psi(6, 6, Lx, Ly, x, y); float E6 = E(6, 6, Lx, Ly, m);

	float z = 
		psi_1 * psi_1 + psi_2 * psi_2 + psi_3 * psi_3 + 
		psi_4 * psi_4 + psi_5 * psi_5 + psi_6 * psi_6;

	// computes faster than for loop
	// don't want to worry about enums
	z += 
		2 * psi_1 * psi_2 * cos((E1 - E2) / hbar * t) +
		2 * psi_1 * psi_3 * cos((E1 - E3) / hbar * t) +
		2 * psi_1 * psi_4 * cos((E1 - E4) / hbar * t) +
		2 * psi_1 * psi_5 * cos((E1 - E5) / hbar * t) +
		2 * psi_1 * psi_6 * cos((E1 - E6) / hbar * t) +
		2 * psi_2 * psi_3 * cos((E2 - E3) / hbar * t) +
		2 * psi_2 * psi_4 * cos((E2 - E4) / hbar * t) +
		2 * psi_2 * psi_5 * cos((E2 - E5) / hbar * t) +
		2 * psi_2 * psi_6 * cos((E2 - E6) / hbar * t) +
		2 * psi_3 * psi_4 * cos((E3 - E4) / hbar * t) +
		2 * psi_3 * psi_5 * cos((E3 - E5) / hbar * t) +
		2 * psi_3 * psi_6 * cos((E3 - E6) / hbar * t) +
		2 * psi_4 * psi_5 * cos((E4 - E5) / hbar * t) +
		2 * psi_4 * psi_6 * cos((E4 - E6) / hbar * t) +
		2 * psi_5 * psi_6 * cos((E5 - E6) / hbar * t);

	z *= 0.1;


	vec3 probability = vec3(position.x, z, position.z); // position.z is really y in cartesian coords
    gl_Position = projection * view * model * vec4(probability, 1.0);
    outColor = vec3(z, 0, 0); // Set ourColor to the input color we got from the vertex data
}       
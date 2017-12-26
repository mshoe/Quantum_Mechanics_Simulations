#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0
  
out vec3 outColor; // Output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float t;
uniform float size_x;
uniform int n;
float h = 6.626; // 10^-34, will show on scale
float hbar = 1.055; // 10^-34

float A(float k) {
	return exp(-25 * pow(k - 1, 2));
}

float w(float k) {
	return 2.f * sqrt(k);
}

float k(int n) {
	return 1.f + n/10.f;
}

float f(float x, float y, float t, int n) {
	float sum = 0;
	for (int i = -n; i <= n; i++) {
		float K = k(i);
		sum += A(K) * cos(K * x - w(K) * t);
	}
	return sum;
}

void main()
{
	// sum of multiple quantum solutions
	// for simplicity, nx = ny
	float x = (position.x * size_x + size_x / 2.f) * (1);
	float z = f(x, position.z, t, n);

	z *= 0.3;
	z *= sin(position.z * 3.14159f);


	vec3 probability = vec3(position.x, z, position.z); // position.z is really y in cartesian coords
    gl_Position = projection * view * model * vec4(probability, 1.0);
    outColor = vec3(0, z, 0); // Set ourColor to the input color we got from the vertex data
}       
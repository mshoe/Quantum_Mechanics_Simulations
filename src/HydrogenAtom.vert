#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in float prob; // The probability variable has attribute position 1
  
out vec3 outColor; // Output a color to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	

	// sum of multiple quantum solutions
	float R, theta, phi;
	R = position.x, theta = position.y, phi = position.z;

	vec3 spherical;
	spherical = vec3(R * sin(theta) * cos(phi), R * sin(theta) * sin(phi), R * cos(theta));
    gl_Position = projection * view * model * vec4(spherical, 1.0);

	// point size
		gl_PointSize = 0.25;


    outColor = vec3(prob, prob * prob, prob * prob * prob); // Set ourColor to the input color we got from the vertex data
}
#pragma once
float cube_vertex_count = 36;

float cube_vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

GLfloat cube_colors[] = {
	1.0f, 0.0f, 0.0f,		// Top Right
	0.0f, 1.0f, 0.0f,     // Bottom Right
	0.0f, 0.0f, 1.0f,     // Bottom Left
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,	// Top Left 
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		// Top Right
	0.0f, 1.0f, 0.0f,     // Bottom Right
	0.0f, 0.0f, 1.0f,     // Bottom Left
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,	// Top Left 
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		// Top Right
	0.0f, 1.0f, 0.0f,     // Bottom Right
	0.0f, 0.0f, 1.0f,     // Bottom Left
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,	// Top Left 
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		// Top Right
	0.0f, 1.0f, 0.0f,     // Bottom Right
	0.0f, 0.0f, 1.0f,     // Bottom Left
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,	// Top Left 
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		// Top Right
	0.0f, 1.0f, 0.0f,     // Bottom Right
	0.0f, 0.0f, 1.0f,     // Bottom Left
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,	// Top Left 
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,		// Top Right
	0.0f, 1.0f, 0.0f,     // Bottom Right
	0.0f, 0.0f, 1.0f,     // Bottom Left
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,	// Top Left 
	1.0f, 0.0f, 0.0f,
};
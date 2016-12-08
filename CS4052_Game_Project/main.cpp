#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <mmsystem.h>
#include "SOIL.h"

using namespace std;


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const char VERTEX_SHADER[] = "../Shaders/simpleVertexShader.txt";
const char FRAGMENT_SHADER[] = "../Shaders/simpleFragmentShader.txt";


GLuint shaderProgramID;
GLuint VAO = 0, VBO = 0, EBO = 0;
GLuint texture;

typedef double D_WORD;

GLfloat greenValue;

// Shader Functions
#pragma region SHADER_FUNCTIONS

// Create a NULL-terminated string by reading the provided file
char* readShaderSource(const char* shaderFile) {
	
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb"); //!->Why does binary flag "RB" work and not "R"

	if (fp == NULL) {
		return NULL; 
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

static GLuint AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object, referenced by an ID
	GLuint ShaderObj = glCreateShader(ShaderType);
	
	//check if it was created successfully
	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	//load source
	const char* pShaderSource = readShaderSource(pShaderText);

	//attach the shader source code to the shader object and compile the shader
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	glCompileShader(ShaderObj);

	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);

	return ShaderObj;
}

GLuint CompileShaders()
{
	//Note: we will link all the shaders together into this ID
	
	/*A shader program object is the final linked version of multiple shaders combined. 
	We have to link them to a shader program object and then activate this shader program 
	when rendering objects. When linking the shaders into a program it links the outputs 
	of each shader to the inputs of the next shader. */

	GLuint shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// Create two shader objects, vertex & fragment
	GLuint vertexShader = AddShader(shaderProgramID, VERTEX_SHADER, GL_VERTEX_SHADER);
	GLuint fragmentShader = AddShader(shaderProgramID, FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glDetachShader(vertexShader, shaderProgramID);
	glDetachShader(fragmentShader, shaderProgramID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgramID;
}

#pragma endregion SHADER_FUNCTIONS

#pragma region VBO_FUNCTIONS

void generateObjectBuffers(GLfloat vertices[], GLfloat colors[], GLfloat texCoords[], GLuint indices[]) {
	
	GLuint numVertices = 4;

	//A buffer object is created by binding a name returned by GenBuffers to 
	//a buffer target.The binding is effected by calling glBindBuffers

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind buffer objects
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// After binding, we now fill our object with data, create empty buffer first
	// if you have more data besides vertices (e.g., colours or yextures), use glBufferSubData to 
	//tell the buffer when the vertices array ends and when the colors start
	glBufferData(GL_ARRAY_BUFFER, numVertices * 8 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), numVertices * 3 * sizeof(GLfloat), colors);
	glBufferSubData(GL_ARRAY_BUFFER, numVertices * 6 * sizeof(GLfloat), numVertices * 2 * sizeof(GLfloat), texCoords);
	
	//enable position attribute, pointed to by attribPointer, disabled by default in opengl
	glEnableVertexAttribArray(0);
	// Then set our vertex attributes pointers
	// 1st param again is 0, location of position data in vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(numVertices * 3 * sizeof(GLfloat)));
	// TexCoord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(numVertices * 6 * sizeof(GLfloat)));
	//.. note stride sizes of 0 above as coords are tightly packed

	//bind element array and load indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);


	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("../wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//free the image memory and unbind texture
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind buffers
	// unbind EBO after VAO otherwise EBO wont be associated with VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#pragma endregion VBO_FUNCTIONS


void init() {

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left 
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,		// Top Right
		0.0f, 1.0f, 0.0f,     // Bottom Right
		0.0f, 0.0f, 1.0f,     // Bottom Left
		1.0f, 1.0f, 0.0f	// Top Left 
	};

	GLfloat texCoords[] = {	
		1.0f, 1.0f,	// Top-right corner
		1.0f, 0.0f,  // bottom right corner
		0.0f, 0.0f,  // Lower-left corner 
		0.0f, 1.0f	// Top-left corner
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	// Set up the shaders
	shaderProgramID = CompileShaders();

	// Put the vertices and colors into a vertex buffer object
	generateObjectBuffers(vertices, colors, texCoords, indices);

}

void display() {

	//tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//clear last display
	//clear screen color buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw stuff
	glUseProgram(shaderProgramID);

	// Bind Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	/* Update the uniform color
	float period = 1000.0f;
	double timeValue = glutGet(GLUT_ELAPSED_TIME);
	GLfloat greenValue = (sin(timeValue / period) / 2) + 0.5;
	GLint vertexColorLocation = glGetUniformLocation(shaderProgramID, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	*/

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	// Swap the screen buffers
	glutSwapBuffers();

	//glutPostRedisplay();
}

void idle() {
		
	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static D_WORD  last_time = 0;
	D_WORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;

	greenValue = (sin(last_time) / 2) + 0.5;

	// Draw the next frame
	glutPostRedisplay();

}

void keypress(unsigned char key, int x, int y) {}

int main(int argc, char** argv) {
	// Set up the window
	glutInit(&argc, argv);
	//glut double means we are using two buffers, one is drawn to screen,
	//other is where we render stuff to draw
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	//using version 3.3
	glutInitContextVersion(3, 3);
	
	//let glut know we want to use core profile
	//we'll be notified if we try use legacy functions
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//note we can't set fixed size window as with glfw

	//create window object
	glutCreateWindow("Hello Triangle");

	//glew manages function pointers for openGL, 
	//NB** need to init before calling any openGL fucntions.
	
	//setting glewExp to true ensures GLEW uses modern techniques
	// for managing openGL, important for core profile (default is false)
	glewExperimental = GL_TRUE;
	
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// Tell glut where the display function is
	glutDisplayFunc(display);
	// glutIdleFunc(idle);
	glutKeyboardFunc(keypress);

	// Can check how many vertex attribs are supported in GPU shaders
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	// Set up your objects and shaders
	init();

	// Begin infinite event loop
	// draw/idle/callbacks etc will be called in here
	glutMainLoop();
	
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;
}
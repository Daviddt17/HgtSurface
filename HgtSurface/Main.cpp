#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "LoadShaders.h"
#include "vmath.h"
#include "Hgt.h"

using namespace std;

// Macros
#define HGT_ROW_SIZE 1201

// Globals
GLuint* vao;
GLuint* vbo;
GLint modelLocation, viewLocation, projectionLocation, colorLocation;
int heightData[HGT_ROW_SIZE * 2];
float scaledHeightData[HGT_ROW_SIZE * 2];

void loadHgtData(const char* fileName)
{
	// Loads height data
	Hgt::hgtLoader(fileName, heightData, Hgt::ARC_TYPES::ONE_ARC);

	// Scales data between -1 and 1
	Hgt::scaleHgtBuffer(heightData, Hgt::ARC_TYPES::ONE_ARC, scaledHeightData, 1);
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glLineWidth(2.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGenBuffers(1, vbo);
	glGenVertexArrays(1, vao);

	// Load Shaders
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "vertexShader.vert" },
		{ GL_FRAGMENT_SHADER, "fragmentShader.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	// Plane
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scaledHeightData), scaledHeightData, GL_STATIC_DRAW);

	GLuint position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Load uniform variable location
	viewLocation = glGetUniformLocation(program, "viewTransform");
	modelLocation = glGetUniformLocation(program, "modelTransform");
	projectionLocation = glGetUniformLocation(program, "projectionTransform");
	colorLocation = glGetUniformLocation(program, "color");

	glBindVertexArray(0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

void keypress(unsigned char key, int x, int y)
{
	switch (key)
	{
		default:
			break;
	}
}

int main(int argc, char *argv[])
{
	// Load HGT
	loadHgtData("N36W112.hgt");
	glutInit(&argc, argv);

	// Presets
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Main Window");
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewInit();

	// Bind functions
	glutKeyboardFunc(keypress);
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 30, timer, 1);

	glutMainLoop();
    return 0;
}
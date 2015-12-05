#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "LoadShaders.h"
#include "vmath.h"
#include "Hgt.h"

using namespace std;

// Macros
#define HGT_SIZE 1201
#define HGT_ROW_SIZE 1200
#define NUM_TRIANGLES 1200 * 1200 * 2

// Globals
GLuint vao[1];
GLuint vbo[1];
GLint modelLocation, viewLocation, projectionLocation, colorLocation;
GLfloat surfaceAngle = 0.0f;
GLfloat surfaceX = 0.0f;
GLfloat surfaceY = 0.0f;
GLfloat surfaceZ = 0.0f;
bool pointsEnabled = true;

GLfloat triangles[NUM_TRIANGLES * 9];
int heightData[HGT_SIZE * HGT_SIZE];
float scaledHeightData[HGT_SIZE * HGT_SIZE];

GLfloat identity[16] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

void buildPlane()
{
	int v = 0;
	for (int r = 0; r < HGT_ROW_SIZE; r++)
	{
		for (int c = 0; c < HGT_ROW_SIZE; c++)
		{
			// Two triangles in square

			/*
				  _____
				|    /  |
				| 1 /   |
				|  /  2 |
				| /___  |
			*/
			// Triangle one
			triangles[v++] = -1.0f + (2.0f * c) / (HGT_ROW_SIZE);		// x
			triangles[v++] = 1.0f - (2.0f * r) / (HGT_ROW_SIZE);		// y
			triangles[v++] = scaledHeightData[(HGT_ROW_SIZE * r) + c];	// z

			triangles[v++] = -1.0f + (2.0f * (c + 1.0f)) / (HGT_ROW_SIZE);
			triangles[v++] = 1.0f - (2.0f * r) / (HGT_ROW_SIZE);
			triangles[v++] = scaledHeightData[(HGT_ROW_SIZE * r) + c + 1];

			triangles[v++] = -1.0f + (2.0f * c) / (HGT_ROW_SIZE);
			triangles[v++] = 1.0f - (2.0f * (r + 1.0f)) / (HGT_ROW_SIZE);
			triangles[v++] = scaledHeightData[(HGT_ROW_SIZE * (r + 1)) + c];

			// Tirangle two
			triangles[v++] = -1.0f + (2.0f * (c + 1.0f)) / (HGT_ROW_SIZE);
			triangles[v++] = 1.0f - (2.0f * r) / (HGT_ROW_SIZE);
			triangles[v++] = scaledHeightData[(HGT_ROW_SIZE * r) + c + 1];

			triangles[v++] = -1.0f + (2.0f * c) / (HGT_ROW_SIZE);
			triangles[v++] = 1.0f - (2.0f * (r + 1.0f)) / (HGT_ROW_SIZE);
			triangles[v++] = scaledHeightData[(HGT_ROW_SIZE * (r + 1)) + c];

			triangles[v++] = -1.0f + (2.0f * (c + 1.0f)) / (HGT_ROW_SIZE);
			triangles[v++] = 1.0f - (2.0f * (r + 1.0f)) / (HGT_ROW_SIZE);
			triangles[v++] = scaledHeightData[(HGT_ROW_SIZE * (r + 1)) + c + 1];
		}
	}

	cout << "Loaded\n";
}

void drawPlane()
{
	// Matrix stuff
	// Model
	vmath::mat4 modelTrans = vmath::scale(3.0f);
	modelTrans *= vmath::rotate(-55.0f, 1.0f, 0.0f, 0.0f);
	//modelTrans *= vmath::rotate(surfaceAngle, surfaceX, surfaceY, 1.0f);
	modelTrans *= vmath::translate(surfaceX, surfaceY, surfaceZ);

	// View
	vmath::mat4 viewTrans = vmath::lookat(
			vmath::vec3(0.0f, 0.0f, 1.0f), // camera
			vmath::vec3(0.0f, 0.0f, 0.0f), // origin
			vmath::vec3(0.0f, 1.0f, 0.0f)  // up
		);

	// Projection
	vmath::mat4 projectionTrans = vmath::perspective(45.0f, 0.5f, 0.1f, 100.0f);

	glUniformMatrix4fv(modelLocation, 1, false, modelTrans);
	glUniformMatrix4fv(viewLocation, 1, false, viewTrans);
	glUniformMatrix4fv(projectionLocation, 1, false, projectionTrans);
	glUniform4f(colorLocation, 1.0, 0.0, 0.0, 1.0);

	// Draw
	glBindVertexArray(vao[0]);
	if (pointsEnabled)
	{
		glDrawArrays(GL_POINTS, 0, NUM_TRIANGLES * 3);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, NUM_TRIANGLES * 3);
	}
	glBindVertexArray(0);
}

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

	//glLineWidth(2.0);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	GLuint position = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Load uniform variable location
	viewLocation = glGetUniformLocation(program, "viewTransform");
	modelLocation = glGetUniformLocation(program, "modelTransform");
	projectionLocation = glGetUniformLocation(program, "projectionTransform");
	colorLocation = glGetUniformLocation(program, "color");

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawPlane();

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
		case 'r':
			surfaceAngle += 0.5;
			break;
		case 'p':
			pointsEnabled = !pointsEnabled;
			break;
		default:
			break;
	}
}

void arrowKeypress(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			surfaceY -= 0.005;
			break;
		case GLUT_KEY_DOWN:
			surfaceY += 0.005;
			break;
		case GLUT_KEY_LEFT:
			surfaceX += 0.005;
			break;
		case GLUT_KEY_RIGHT:
			surfaceX -= 0.005;
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[])
{
	// Load HGT
	loadHgtData("N41W081.hgt");
	buildPlane();

	// GL init
	glutInit(&argc, argv);

	// Presets
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Main Window");
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewInit();

	// My init
	init();

	// Bind functions
	glutKeyboardFunc(keypress);
	glutSpecialFunc(arrowKeypress); // Need this for arrow keys
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 30, timer, 1);

	glutMainLoop();
    return 0;
}
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <string>
#include <GL/freeglut.h>

#include "Helicopter.h"
#include "Vector3.h"
#include "Utils.h"

const int font = (int)GLUT_BITMAP_9_BY_15;

float camX = 15.0f;
float camY = 0.0f;
float camZ = 0.0f;

float horizontalAngle = 0.0f;
float verticalAngle = 0.0f;

float dist = 15.0f;

int drawMode = GL_FILL;

int resw = 600;
int resh = 600;
float fovy = 75;

int wID = -1;

bool perspectiveCam = true;
bool on = false; // bool for moving the heli and rotors
bool firstPerson = false;

Helicopter heli;
float a = 50.0f;
float sqrt2 = sqrtf(2.0f);

auto startTime = std::chrono::high_resolution_clock::now();
auto lastTime = std::chrono::high_resolution_clock::now();

// Initiliaze the OpenGL window
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);			// Clear the color 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	GLfloat light_position[] = { 0, 1.0, 0, 0.0 };
	GLfloat light_ambient[] = { 0, 0, 0, 1 };
	GLfloat light_diffuse[] = { .5, .25, .25, 1 };
	GLfloat light_specular[] = { .25, .5, .25, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);

	glShadeModel(GL_SMOOTH);
}

void updateSpeed(float delta)
{
	if (on && heli.speed < heli.topSpeed)
	{
		heli.rotorSpeed = heli.speed += heli.accel * delta / 1000.0f;
	}
	else if (on && heli.speed > heli.topSpeed)
	{
		heli.rotorSpeed = heli.speed -= heli.accel * delta / 1000.0f;
	}
	else if (!on && heli.speed > 0.0f)
	{
		heli.rotorSpeed = heli.speed -= heli.accel * delta / 1000.0f;
	}
	else if (!on && heli.speed < 0.0f)
	{
		heli.rotorSpeed = heli.speed = 0.0f;
	}
}

void updateCamera();
void setOrthoCam();
void setPerspectiveCam();

void renderBitmapString(float x, float y, void *font, const char *string)
{
	glColor3f(1, 0, 0);

	setOrthoCam();

	glPushMatrix();
	glLoadIdentity();
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
	glPopMatrix();
	setPerspectiveCam();
}

float fps = 0;

void idle(void)
{

	auto currentTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> deltaSec = currentTime - lastTime;
	lastTime = currentTime;

	auto deltaMillis = std::chrono::duration_cast<std::chrono::milliseconds>(deltaSec).count();

	static float distanceTravelled = (float)-M_PI_2;
	distanceTravelled += heli.speed * (float)deltaSec.count();

	fps = 1.0f / deltaSec.count();

	Vector3 lastPos = heli.position;

	heli.position.x = (a * sqrt2 * cosf(distanceTravelled)) / (pow(sinf(distanceTravelled), 2) + 1);
	heli.position.z = (a * sqrt2 * cosf(distanceTravelled) * sinf(distanceTravelled)) / (pow(sinf(distanceTravelled), 2) + 1);

	static Vector3 dir;

	if (!(heli.position == lastPos))
	{
		dir = (heli.position - lastPos).normalized();
		heli.forward = dir;
	}
	else
	{
		dir = heli.forward;
	}

	static Vector3 x(1, 0, 0);

	float dotProd = Vector3::dot(dir, x);

	heli.angle = std::acosf(dotProd) * toDegs * -sign(dir.z);

	heli.pilotPos = heli.position + (dir*2.25) + Vector3(0, 2, 0);
	heli.pilotLook = heli.pilotPos + dir;

	static Vector3 spotPos;
	static GLfloat spotPosV[3];
	static GLfloat spotDir[3];

	spotPos = heli.position + (dir * 4.15);
	spotPosV[0] = spotPos.x;
	spotPosV[1] = spotPos.y;
	spotPosV[2] = spotPos.z;

	spotDir[0] = dir.x;
	spotDir[1] = dir.y - sqrt2;
	spotDir[2] = dir.z;

	updateSpeed(deltaMillis);

	updateCamera();

	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);				// Clear the Color Buffer 
	glClear(GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	heli.drawHelicopter();
	glPopMatrix();

	//Draw floor
	glPushMatrix();
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	GLfloat mat_dif[] = { 0, .3, 0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_dif);
	glTranslatef(0, -10, 0);
	glutSolidTeapot(10);
	glPopAttrib();
	glPopMatrix();

	std::string fpsStr = std::to_string(fps);

	std::cout << fps << std::endl;

	//printf_s("%s\n", fpsStr.c_str());

	//renderBitmapString(50, 50, (void*)font, fpsStr.c_str());

	glutSwapBuffers();
}

void setPerspectiveCam();
void setOrthoCam();

// This function is called whenever the window size is changed
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);				// Set the viewport
	glMatrixMode(GL_PROJECTION);								// Set the Matrix mode
	glLoadIdentity();
	resw = w;
	resh = h;
	if (perspectiveCam)
	{
		setPerspectiveCam();
	}
	else
	{
		setOrthoCam();
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 0, 0, 0, 0.0, 1.0, 0.0);
}

void updateCamera()
{
	verticalAngle = clamp(verticalAngle, -89.9f, 89.9f);

	dist = clamp(dist, 5, 100);

	float radsH = horizontalAngle * toRads;
	float radsV = verticalAngle * toRads;

	float t = dist * cos(radsV);

	camY = dist * sin(radsV);
	camX = t * cos(radsH) + heli.position.x;
	camZ = t * sin(radsH) + heli.position.z;

	glLoadIdentity();
	if (firstPerson)
	{
		gluLookAt(heli.pilotPos.x, heli.pilotPos.y, heli.pilotPos.z,
			heli.pilotLook.x, heli.pilotLook.y, heli.pilotLook.z,
			0, 1, 0);
	}
	else
	{
		gluLookAt(camX, camY, camZ, heli.position.x, heli.position.y, heli.position.z, 0, 1, 0);
	}
}

void resetView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	fovy = 75;
	gluPerspective(fovy, (GLfloat)resw / (GLfloat)resh, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	camX = 15.0f;
	camY = 0.0f;
	camZ = 0.0f;
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
	dist = 15.0f;
	drawMode = GL_FILL;
}

void changeFOV(float change)
{
	if (perspectiveCam)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		fovy += change;
		gluPerspective(fovy, (GLfloat)resw / (GLfloat)resh, 1.0f, 100.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}

void setPerspectiveCam()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLfloat)resw / (GLfloat)resh, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	perspectiveCam = true;
}

void setOrthoCam()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)resw / resh;
	float iratio = (float)resh / resw;
	glOrtho(-resw / 100, resw / 100, -resh / 100, resh / 100, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	perspectiveCam = false;
}

bool light0 = true;
bool light1 = true;

// called on special key pressed
void specialKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		verticalAngle += 5.0f;
		break;
	case GLUT_KEY_DOWN:
		verticalAngle -= 5.0f;
		break;
	case GLUT_KEY_LEFT:
		horizontalAngle += 5.0f;
		break;
	case GLUT_KEY_RIGHT:
		horizontalAngle -= 5.0f;
		break;
	case GLUT_KEY_F1:
		if (light0)
		{
			glDisable(GL_LIGHT0);
		}
		else
		{
			glEnable(GL_LIGHT0);
		}
		light0 = !light0;
		break;
	case GLUT_KEY_F2:
		if (light1)
		{
			glDisable(GL_LIGHT1);
		}
		else
		{
			glEnable(GL_LIGHT1);
		}
		light1 = !light1;
		break;
	}

	//updateCamera();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'f':
		dist--;
		printf_s("Forwards\n");
		break;
	case 'b':
		dist++;
		printf_s("Backwards\n");
		break;
	case 'w':
		drawMode = drawMode == GL_FILL ? GL_LINE : GL_FILL;
		glPolygonMode(GL_FRONT_AND_BACK, drawMode);
		printf_s("Wireframe: %s\n", drawMode == GL_FILL ? "FALSE" : "TRUE");
		break;
	case 'z':
		changeFOV(-5.0f);
		printf_s("-fov: %f\n", fovy);
		break;
	case 'Z':
		changeFOV(5.0f);
		printf_s("+fov: %f\n", fovy);
		break;
	case 'c':
		resetView();
		printf_s("View Reset\n");
		break;
	case 'o':
		setOrthoCam();
		printf_s("Orthographic Camera\n");
		break;
	case 'p':
		setPerspectiveCam();
		printf_s("Perspective Camera\n");
		break;
	case 's':
		on = true;
		printf_s("Helicopter ON\n");
		break;
	case 'S':
		on = false;
		printf_s("Helicopter OFF\n");
		break;
	case 'a':
		heli.topSpeed += 0.1f;
		break;
	case 'A':
		heli.topSpeed -= 0.1f;
		if (heli.topSpeed < 0.0f) heli.topSpeed = 0.0f;
		break;
	case '1':
		firstPerson = true;
		printf_s("First-Person Camera\n");
		break;
	case '3':
		firstPerson = false;
		printf_s("Third-Person Camera\n");
		break;
	case 27: // Escape key
		printf_s("Goodbye!\n");
		glutDestroyWindow(wID);
		exit(0);
		break;
	}

	//updateCamera();
}

int lastX = 0;
int lastY = 0;

void mouseLook(int x, int y)
{
	int dx = x - lastX;
	int dy = y - lastY;

	horizontalAngle += dx / 2.0f;
	verticalAngle += dy / 2.0f;

	//updateCamera();

	lastX = x;
	lastY = y;
}

void mouseButtonPressed(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		lastX = x;
		lastY = y;
	}
}

// Main entry point of the program
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);		// Setup display mode to double buffer and RGB color
	glutInitWindowSize(600, 600);						// Set the screen size
	wID = glutCreateWindow("Assignment 1");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(specialKey);	// set window's to specialKey callback
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseLook);
	glutMouseFunc(mouseButtonPressed);
	glutMainLoop();
	return 0;
}
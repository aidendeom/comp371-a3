#include "Helicopter.h"

#include <math.h>

Helicopter::Helicopter()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	forward.x = 1.0f;
	forward.y = 0.0f;
	forward.z = 0.0f;

	angle = 0.0f;

	speed = 0.0f;
	topSpeed = 0.5f;
	accel = 1.0f;

	frontPropAngle = 0.0f;
	backPropAngle = 0.0f;
	rotorSpeed = 0.0f;
}


Helicopter::~Helicopter()
{
}

void Helicopter::drawHelicopter()
{
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(216/255.f, 50/255.f, 52/255.f);
	static GLfloat mat_diffuse[] = { 216 / 255.f, 50 / 255.f, 52 / 255.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(angle, 0, 1, 0);

		// Spot light position
		static GLfloat spos[] = { 4.15, 0, 0, 1};
		// Spot light direction
		static GLfloat sdir[] = {1, 0, 0};

		// Lines to show where spotlight should be aiming
		glBegin(GL_LINES);
		glVertex3fv(spos);
		glVertex3f(spos[0] + sdir[0], 0, 0);
		glEnd();

		glLightfv(GL_LIGHT1, GL_POSITION, spos);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sdir);
		drawHeliBody();
		glPushMatrix();
			glTranslatef(-5.5, 2, 0);
			glRotatef(-90, 0, 1, 0);
			drawHeliTail();
		glPopMatrix();
	glPopMatrix();
	glPopAttrib();
}

void Helicopter::drawMissileLauncher()
{
	GLUquadric *launcher = gluNewQuadric();

	glPushMatrix();
	glPushMatrix();
	glScalef(2, 1, 1);
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(0, 0, 0);
	static GLfloat mat_diffuse[] = { 0, 0, 0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glutSolidCube(1);
	glPopAttrib();
	glPopMatrix();
	glTranslatef(-3, -1.3, 0);
	glRotatef(90, 0, 1, 0);
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(57 / 255.f, 57 / 255.f, 57 / 255.f);
	static GLfloat mat_diffuse2[] = { 57 / 255.f, 57 / 255.f, 57 / 255.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	gluCylinder(launcher, 1, 1, 6, 20, 20);
	gluDisk(launcher, 0, 1, 20, 20);
	glTranslatef(0, 0, 6);
	gluDisk(launcher, 0, 1, 20, 20);
	glPopAttrib();
	glPopMatrix();

	gluDeleteQuadric(launcher);
}

void Helicopter::drawMachinegun()
{
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(0, 0, 0);
	static GLfloat mat_diffuse[] = { 0, 0, 0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	GLUquadric *gun = gluNewQuadric();

	glPushMatrix();
	glutSolidCube(1);
	glTranslatef(1, -1, 0);
	glScalef(4, 1.5, 1.5);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3, -1, 0);
	glRotatef(90, 0, 1, 0);
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(57 / 255.f, 57 / 255.f, 57 / 255.f);
	static GLfloat mat_diffuse2[] = { 57 / 255.f, 57 / 255.f, 57 / 255.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	gluCylinder(gun, 0.5, 0.5, 1, 20, 20);
	glTranslatef(0, 0, 1);
	gluDisk(gun, 0, 0.5, 20, 20);
	glPopAttrib();
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(73 / 255.f, 73 / 255.f, 73 / 255.f);
	static GLfloat mat_diffuse3[] = { 73 / 255.f, 73 / 255.f, 73 / 255.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
	gluCylinder(gun, 0.25, 0.25, 6, 20, 20);
	glTranslatef(0, 0, 6);
	gluDisk(gun, 0, 0.25, 20, 20);
	glPopAttrib();
	glPopMatrix();

	gluDeleteQuadric(gun);
	glPopAttrib();
}

void Helicopter::drawWing()
{
	GLUquadric *wing = gluNewQuadric();

	glPushMatrix();
	glScalef(2, 1, 1);
	gluCylinder(wing, 2, 1, 10, 20, 20);
	glPushMatrix();
	glTranslatef(0, 0, 10);
	gluDisk(wing, 0, 1, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -2, 3);
	drawMissileLauncher();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -1.5, 8);
	drawMissileLauncher();
	glPopMatrix();

	gluDeleteQuadric(wing);
}

void Helicopter::drawProp()
{
	glPushMatrix();
	glTranslatef(10, .25, 0);
	glScalef(20, 0.1, .7);
	glutSolidCube(1);
	glPopMatrix();
}

void Helicopter::drawRotor(float scale)
{
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(1, .5, 0);
	static GLfloat mat_diffuse[] = { 1, .5, 0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	GLUquadric *rotor = gluNewQuadric();
	glPushMatrix();
	glScalef(scale, scale, scale);
	glPushMatrix();
	glScalef(1, 0.1, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(rotor, .5, .5, 0.5, 20, 20);
	gluDisk(rotor, 0, 0.5, 20, 20);
	glPopMatrix();
	glPushMatrix();
	drawProp();
	glRotatef(120, 0, 1, 0);
	drawProp();
	glRotatef(120, 0, 1, 0);
	drawProp();
	glPopMatrix();
	glPopMatrix();

	gluDeleteQuadric(rotor);
	glPopAttrib();
}

void Helicopter::drawGear()
{
	glPushMatrix();
	glScalef(6, .1, .1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(2, .25, .25);
	glRotatef(45, 1, 0, 0);
	glScalef(.1, .75, .1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(-2, .25, .25);
	glRotatef(45, 1, 0, 0);
	glScalef(.1, .75, .1);
	glutSolidCube(1);
	glPopMatrix();
}
void Helicopter::drawLandingGear()
{
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(154/255.f, 117/255.f, 117/255.f);
	static GLfloat mat_diffuse[] = { 154 / 255.f, 117 / 255.f, 117 / 255.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(0, 0, -1);
	drawGear();
	glTranslatef(0, 0, 2);
	glScalef(1, 1, -1);
	drawGear();
	glPopMatrix();
	glPopAttrib();
}

void Helicopter::drawHeliBody()
{
	glPushMatrix();
	glPushMatrix();
	glPopMatrix();
	glScalef(8, 2, 2);
	glutSolidCube(1.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2, 2, 0);
	glScalef(8, 2, 2);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.3, 1.3, 0);
	glRotatef(-45, 0, 0, 1);
	glScalef(2 * sqrt(2.0f), 2, 1.8);
	glPushAttrib(GL_COLOR_MATERIAL_FACE);
	//glColor3f(1, 1, 1);
	static GLfloat mat_diffuse[] = { 1, 1, 1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glutWireCube(1);
	glPopAttrib();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1, 1, 1);
	glScalef(0.4f, 0.4f, 0.4f);
	drawWing();
	glScalef(1, 1, -1);
	glTranslatef(0, 0, 5);
	drawWing();
	glPopMatrix();
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(3, -2.5, 0);
	drawMachinegun();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-1, 3, 0);
		glPushMatrix();
			glRotatef(frontPropAngle, 0, 1, 0);
			drawRotor(.6);
		glPopMatrix();
		glRotatef(60, 0, 1, 0);
		glTranslatef(0, 0.15, 0);
		glPushMatrix();
			glRotatef(-frontPropAngle, 0, 1, 0);
			drawRotor(.6);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -1.5, 0);
	glScalef(1.5, 1.5, 1.5);
	drawLandingGear();
	glPopMatrix();

	frontPropAngle += rotorSpeed;
	backPropAngle -= rotorSpeed;
}

void Helicopter::drawHeliTail()
{
	GLUquadric *tail = gluNewQuadric();

	glPushMatrix();
	glutSolidCone(1, 2, 20, 20);
	glScalef(1, 2, 1);
	gluCylinder(tail, 0.5, 0.25, 10, 20, 20);
	glTranslatef(0, 0, 10);
	gluDisk(tail, 0, 0.25f, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 9.25);
	glRotatef(-85, 1, 0, 0);
	glScalef(1, 3, 1);
	gluCylinder(tail, 0.25, 0.1, 3, 20, 20);
	glTranslatef(0, 0, 3);
	gluDisk(tail, 0, 0.1, 20, 20);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-.1, 2.7, 9.5);
		glRotatef(90, 0, 0, 1);
		glPushMatrix();
			glRotatef(backPropAngle, 0, 1, 0);
			drawRotor(.1);
		glPopMatrix();
		glTranslatef(0, 0.05, 0);
		glRotatef(60, 0, 1, 0);
		glPushMatrix();
			glRotatef(backPropAngle, 0, 1, 0);
			drawRotor(.1);
		glPopMatrix();
	glPopMatrix();

	gluDeleteQuadric(tail);
}
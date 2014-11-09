#include "Helicopter.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <SFML/Graphics/Image.hpp>
#include "Utils.h"

Helicopter::Helicopter(float a) :
a{ a },
position{ Vector3::zero },
forward{ Vector3::right },
angle{ 0 },
speed{ 0 },
topSpeed{ 1 },
accel{ 1 },
frontPropAngle{ 0 },
backPropAngle{ 0 },
rotorSpeed{ 0 },
distanceTravelled{ -M_PI_2 }
{
	sf::Image image;
	image.loadFromFile("resources/camo.jpg");
	glGenTextures(1, &heliBodyTexture);
	glBindTexture(GL_TEXTURE_2D, heliBodyTexture);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


Helicopter::~Helicopter()
{
}

void Helicopter::update(float deltaTime)
{
	static float sqrt2 = sqrt(2.0f);

	distanceTravelled += speed * deltaTime;

	// Parametric equation for lemniscate of Bernoulli
	Vector3 nextPos(
		(a * sqrt2 * cosf(distanceTravelled)) / (pow(sinf(distanceTravelled), 2) + 1),
		0.0f,
		(a * sqrt2 * cosf(distanceTravelled) * sinf(distanceTravelled)) / (pow(sinf(distanceTravelled), 2) + 1)
		);

	if (nextPos != position)
	{
		forward = (nextPos - position).normalized();
	}

	angle = acosf(Vector3::dot(forward, Vector3::right)) * toDegs *-sign(forward.z);

	position = nextPos;

	pilotPos = position + forward * pilotPosAnchor.x + Vector3::up * pilotPosAnchor.y;

	pilotLook = pilotPos + forward;
}

void Helicopter::drawHelicopter()
{
	//glPushAttrib(GL_COLOR_MATERIAL_FACE);
	////glColor3f(216/255.f, 50/255.f, 52/255.f);
	//static GLfloat mat_diffuse[] = { 216 / 255.f, 50 / 255.f, 52 / 255.f };
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(angle, 0, 1, 0);

	// Spot light position
	static GLfloat spos[] = { 4.15, 0, 0, 1 };
	// Spot light direction
	static GLfloat sdir[] = { 1, -sqrt(2.0f), 0 };

	// Lines to show where spotlight should be aiming
	glBegin(GL_LINES);
	glVertex4fv(spos);
	glVertex3f(spos[0] + sdir[0], 0, spos[2] + sdir[2]);
	glEnd();

	glLightfv(GL_LIGHT1, GL_POSITION, spos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, sdir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);

	drawHeliBody();
	glPushMatrix();
	glTranslatef(-5.5, 2, 0);
	glRotatef(-90, 0, 1, 0);
	drawHeliTail();
	glPopMatrix();
	glPopMatrix();
	//glPopAttrib();
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

	glEnable(GL_TEXTURE_2D);

	GLfloat zPlane[] = { 0.0f, 0.0f, 1, 1 };

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, zPlane);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, zPlane);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glBindTexture(GL_TEXTURE_2D, heliBodyTexture);
	glutSolidCube(1.0);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glDisable(GL_TEXTURE_2D);

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
	glColor3f(1, 1, 1);
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
	glRotatef(frontPropAngle, 0, 1, 0);
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
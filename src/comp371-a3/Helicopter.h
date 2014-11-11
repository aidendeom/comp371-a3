#pragma once

#include <GL/freeglut.h>
#include "Vector3.h"

class Helicopter
{
public:
	Helicopter(float a);
	~Helicopter();

	void loadTextures();

	float frontPropAngle;
	float backPropAngle;
	float rotorSpeed;
	float accel;

	Vector3 position;
	Vector3 pilotPos;
	Vector3 pilotLook;
	Vector3	forward;
	const Vector3 pilotPosAnchor{ 2.25f, 2.0f, 0.0f };

	float angle;

	float speed;
	float topSpeed;

	bool highBeams;

	GLfloat spotDirection[3];

	void update(float deltaTime);
	void drawHelicopter();

private:
	float a;
	float distanceTravelled;
	GLuint heliBodyTexture;
	GLuint heliTailTexture;
	GLuint heliWingTexture;
	float lightAngle;
	float lightRotSpeed;

	void drawHeliBody();
	void drawHeliTail();
	void drawWing();
	void drawMachinegun();
	void drawMissileLauncher();
	void drawRotor(float size);
	void drawProp();
	void drawGear();
	void drawLandingGear();
};


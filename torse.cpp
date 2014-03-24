#include <iostream>
using namespace std;
#include "torse.h"
#include "cylinder.h"
#include "math.h"

Torse::Torse() : torse(length, width, nbFace) {
}

void Torse::draw()
{
	glPushMatrix();

	torse.draw();

    //Right arm
	glPushMatrix();
	glTranslatef(width/2.0*1.1, 0, length/2.0*0.80);
	glRotatef(45, 0, 1, 0);
	rightArm.draw();
	glPopMatrix();

    //Left arm
	glPushMatrix();
	glTranslatef(-(width/2.0*1.1), 0, length/2.0*0.80);
	glRotatef(180, 0, 1, 0);
	glRotatef(-45, 0, 1, 0);
	leftArm.draw();
	glPopMatrix();


	glPopMatrix();
}

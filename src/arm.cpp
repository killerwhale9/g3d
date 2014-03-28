#include <iostream>
using namespace std;
#include "arm.h"
#include "cylinder.h"
#include "math.h"


Arm::Arm() : arm(length, width, nbFace) {
}

void Arm::draw()
{
	glPushMatrix();
    glutSolidSphere(0.5, nbFace, nbFace);
    glTranslatef(length/2.0, 0,0);
	glRotatef(90, 0, 1, 0);
	arm.draw();

	glPopMatrix();
}

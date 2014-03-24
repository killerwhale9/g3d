#include <iostream>
using namespace std;
#include "leg.h"
#include "cylinder.h"
#include "math.h"


Leg::Leg() : leg(length, width, nbFace) {
}

void Leg::draw()
{
	glPushMatrix();
    glutSolidSphere(0.5, nbFace, nbFace);
    glTranslatef(0, 0, -(length/2.0));
	glRotatef(180, 0, 1, 0);
	leg.draw();

	glPopMatrix();
}

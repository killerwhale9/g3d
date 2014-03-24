#include <iostream>
using namespace std;
#include "arm.h"
#include "cylinder.h"
#include "math.h"

Arm::Arm() : arm(2.0, 1.0, 50) {

}

void Arm::draw()
{
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(1.0,0,2.5);
	arm.draw();

	glPopMatrix();
}

#include <iostream>
using namespace std;
#include "torse.h"
#include "cylinder.h"
#include "math.h"

Torse::Torse() : torse(3.0, 3.0, 50) {
}

void Torse::draw()
{
	glPushMatrix();

	torse.draw();
	arm.draw();

	glPopMatrix();
}

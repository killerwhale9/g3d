#include <iostream>
using namespace std;
#include "torse.h"
#include "cylinder.h"
#include "math.h"


void Torse::draw()
{
	glPushMatrix();

	torse.draw();

	glPopMatrix();
}

#ifndef _TORSE_
#define _TORSE_

#include "renderable.h"
#include "cylinder.h"
#include "arm.h"
#include "leg.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Torse : public Renderable
{
	public:
		void draw();
		Torse();
	private:
		Cylinder torse;
		Arm leftArm;
		Arm rightArm;
        Leg leftLeg;
        Leg rightLeg;
        static const float length = 3.0;
        static const float width = 3.0;
        static const int nbFace = 50;
};

#endif


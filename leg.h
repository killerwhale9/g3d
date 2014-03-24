#ifndef _LEG_
#define _LEG_

#include "renderable.h"
#include "cylinder.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Leg : public Renderable
{
	public:
		void draw();
		Leg();
	private:
		Cylinder leg; 
        static const float length = 2.0;
        static const float width = 1.0;
        static const int nbFace = 50;
};

#endif


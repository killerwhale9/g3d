#ifndef _TORSE_
#define _TORSE_

#include "renderable.h"
#include "cylinder.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Torse : public Renderable
{
	public:
		void draw();
	private:
		Cylinder torse = Cylinder(3.0, 3.0, 50);
};

#endif


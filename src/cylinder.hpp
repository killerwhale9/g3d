#ifndef _CYLINDER_
#define _CYLINDER_

#include "renderable.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Cylinder : public Renderable
{
	public:
		Cylinder(float, float, int);
		void draw();

	private:
		void drawImmediate();
		void drawElements();
		void drawArrays();
		int nb_face;
		float heigth, width;
};

#endif


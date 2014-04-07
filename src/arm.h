#ifndef _ARM_
#define _ARM_

#include "renderable.h"
#include "cylinder.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Arm : public Renderable
{
	public:
		void draw();
		Arm();
	private:
        float m_length,
              m_width;
        int m_precision;
		Cylinder m_figure; 
};

#endif


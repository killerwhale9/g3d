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
                float m_length,
                      m_width;
                int m_precision;
		Cylinder m_figure; 
};

#endif


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
        inline float getWidth() const { return m_width; }
        inline float getLength() const { return m_length; }
	private:
                float m_length,
                      m_width;
                int m_precision;
		Cylinder m_figure; 
};

#endif


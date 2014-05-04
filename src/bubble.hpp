#ifndef _BUBBLE_
#define _BUBBLE_

#include "renderable.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Bubble : public Renderable
{
    public:
        void draw(int pass);
        Bubble(float r, float x, float y, float z);
    private:
        float m_radius,
              m_x,
              m_y,
              m_z;
        int m_precision;
};

#endif


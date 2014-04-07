#ifndef _ARM_
#define _ARM_

#include "renderable.hpp"
#include "cylinder.hpp"
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
        inline float getWidth() const { return m_width; }
        inline float getLength() const { return m_length; }
    private:
        float m_length,
              m_width;
        int m_precision;
        Cylinder m_figure; 
};

#endif


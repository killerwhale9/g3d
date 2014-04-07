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
        // Values for sizing the body
        // Lengths
        float m_length,
              m_width; // size of the memebers eg: radius as they're cylinders
        int m_precision; // number of faces

        // Body parts
        Cylinder m_figure;
        Arm m_lUArm, // Left Upper arm
            m_rUArm,
            m_lLArm, // Left Lower arm
            m_rLArm;
        Leg m_lULeg,
            m_rULeg,
            m_lLLeg,
            m_rLLeg;

};

#endif


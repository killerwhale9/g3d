#ifndef _TORSE_
#define _TORSE_

#include "renderable.hpp"
#include "cylinder.hpp"
#include "arm.hpp"
#include "leg.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Torse : public Renderable
{
    public:
        void draw(int pass);
        Torse();

        inline float getWidth() const { return m_width; }
        inline float getLength() const { return m_length; }
    private:
        // Values for sizing the body
        // Lengths
        float m_length,
              m_width, // size of the memebers eg: radius as they're cylinders
              m_headRadius;
        int m_precision; // number of faces

        // Body parts
        Cylinder m_figure;
        Cylinder m_bottle;
        Arm m_lUArm, // Left Upper arm
            m_rUArm,
            m_lLArm, // Left Lower arm
            m_rLArm;
        float m_angUArm, // Angle of the upper arm
              m_angLArm; // Angle of the lower arm
        int m_dirArm; // Derivate of the angle
        Leg m_lULeg,
            m_rULeg,
            m_lLLeg,
            m_rLLeg;
        float m_angLeg; // Angle of the Leg
        int m_dirLeg; // Derivate of the angle

        int m_tmp;

};

#endif


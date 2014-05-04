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
#include "globals.hpp"
#include "glm/vec3.hpp"

class Animation;

class Torse : public Renderable
{
    public:
        friend class Animation;
        void draw(int pass);
        virtual void animate();
        Torse();

        inline float getWidth() const { return m_width; }
        inline float getLength() const { return m_length; }
        const glm::vec3& getCurrentRotation(frame_type);
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
        uint32_t m_frame;

        // rotation actuelles
        glm::vec3 m_angULArm,
                  m_angLLArm,
                  m_angURArm,
                  m_angLRArm,
                  m_angULLeg,
                  m_angLLLeg,
                  m_angURLeg,
                  m_angLRLeg,
                  m_angHead,
                  m_angTorse;

        Animation *m_animSwim;

        Animation *m_currentAnim;

        void setAnimation(Animation* a);

};

#endif


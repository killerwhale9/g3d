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
#include "fin.hpp"
#include "objReader.hpp"
#include "dynamicSystem.hpp"

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
        inline void setPosition(const glm::vec3 &p) { m_pos = p; }
        inline virtual void init(Viewer& v) {m_viewer = &v;};
        glm::vec3 getHeadPos();
        glm::vec3 getLookAt();
    private:
        // Values for sizing the body
        // Lengths
        float m_length,
              m_width, // size of the memebers eg: radius as they're cylinders
              m_headRadius;
        int m_precision; // number of faces

DynamicSystem m_syst;

        // Body parts
        Cylinder m_figure;
        Cylinder m_bottle;
        Arm m_lUArm, // Left Upper arm
            m_rUArm,
            m_lLArm, // Left Lower arm
            m_rLArm;
        Leg m_lULeg,
            m_rULeg,
            m_lLLeg,
            m_rLLeg;
        Fin m_lFin,
            m_rFin;

        uint32_t m_frame, m_bubbles;
        Viewer *m_viewer;

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

        Animation *m_animSwim,
                  *m_animGetUp,
                  *m_animSwimTrans,
                  *m_animAim,
                  *m_animRecoil;

        Animation *m_currentAnim;

        glm::vec3 m_pos;

        void setAnimation(Animation* a);


        objReader& m_rpg;
        objReader& m_missile;
        int m_viewRpg;
        int m_viewMissile;
        float m_posMissile;

};

#endif


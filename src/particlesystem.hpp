#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <list>
#include <vector>
#include "renderable.hpp"
#include "glm/vec3.hpp"

class ParticleSystem : public Renderable
{
    public:
        ParticleSystem(glm::vec3 pos);
        ~ParticleSystem();

    private:
        class Particle
        {
            public:
                Particle(glm::vec3 pos);
                int m_lifetime;
                glm::vec3 m_pos,
                    m_speed,
                    m_dSpeed,
                    m_rot,
                    m_dRot;
                float m_scale,
                      m_dScale,
                      m_friction;
        };

        glm::vec3 m_pos;
        std::list<Particle *> m_particles;
        int m_timer;
        //Renderable methods
    public:
        void draw(int pass);
        void animate();
};
#endif

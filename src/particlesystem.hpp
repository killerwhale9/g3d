#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

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
        };

        int m_lifetime;
        glm::vec3 m_pos,
            m_speed,
            m_dSpeed,
            m_rot,
            m_dRot;
        float m_scale,
              m_dScale,
              m_friction;
        std::list<Particle *> m_particles;
    //Renderable methods
    public:
        void draw(int pass);
        void animate();
};
#endif

#include "particlesystem.hpp"
#include <ctime>

#define lifetimeMin 10
#define lifetimeMax 60
#define speedMin 1
#define speedMax 10
ParticleSystem::ParticleSystem(glm::vec3 pos) :
    m_pos(pos)
{
    for (int i = 0; i < 10; i++) {
        m_particles.push_back(new ParticleSystem::Particle(glm::vec3(0, i, 0)));
    }
}

ParticleSystem::Particle::Particle(glm::vec3 pos) :
    m_lifetime((random()%(lifetimeMax-lifetimeMin))+lifetimeMin),
    m_pos(pos),
    m_speed(glm::vec3(
                (random()%(speedMax-speedMin))+speedMin,
                (random()%(speedMax-speedMin))+speedMin,
                (random()%(speedMax-speedMin))+speedMin)),
    m_dSpeed(glm::vec3()),
    m_rot(glm::vec3()),
    m_dRot(glm::vec3()),
    m_scale(1.0f),
    m_dScale(0.0f),
    m_friction(1.0f)
{
}

void ParticleSystem::draw(int pass)
{
    glPushMatrix();

    glPopMatrix();
}

void ParticleSystem::animate()
{
}

ParticleSystem::~ParticleSystem()
{
}

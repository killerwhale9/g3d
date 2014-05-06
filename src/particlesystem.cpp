#include "particlesystem.hpp"
#include <ctime>
#include <iterator>

#define lifetimeMin 10
#define lifetimeMax 60
#define speedMin 1
#define speedMax 10
ParticleSystem::ParticleSystem(glm::vec3 pos) :
    m_pos(pos),
    m_timer(0)
{
    for (int i = 0; i < 10; i++) {
        m_particles.push_back(new ParticleSystem::Particle(glm::vec3(0, 0, 0)));
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
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    for (std::list<Particle *>::iterator it(m_particles.begin()); it != m_particles.end(); it++) {
        glColor3f(1,0,0);
        glPushMatrix();
        glTranslatef((*it)->m_pos.x,(*it)->m_pos.y,(*it)->m_pos.z);
        glutSolidSphere(1, 8, 5);
        glPopMatrix();
    }

    glPopMatrix();
}

void ParticleSystem::animate()
{
    m_timer++;
    for (std::list<Particle *>::iterator it(m_particles.begin()); it != m_particles.end(); it++) {
        if (m_timer > (*it)->m_lifetime) {
            //delete (*it);
        }
        (*it)->m_pos += (*it)->m_speed;
    }
}

ParticleSystem::~ParticleSystem()
{
}

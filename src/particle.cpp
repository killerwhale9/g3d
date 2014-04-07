#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "particle.hpp"

    Particle::Particle(Vec pos, Vec vel, double m, double r)
: position(pos),
    velocity(vel),
    mass(m),
    radius(r)
{
    invMass = (m > 0 ? 1 / m : 0.0);
}

Particle::~Particle()
{
}


const Vec & Particle::getPosition() const
{
    return position;
}

const Vec & Particle::getVelocity() const
{
    return velocity;
}

double Particle::getVolume()
{
    return 4./3.*M_PI*radius*radius*radius;
}

double Particle::getMass() const
{
    return mass;
}

double Particle::getInvMass() const
{
    return invMass;
}

void Particle::setRadius(double r)
{
    radius = r;
}

double Particle::getRadius()
{
    return radius;
}

void Particle::setPosition(const Vec &pos)
{
    position = pos;
}

void Particle::setVelocity(const Vec &vel)
{	
    velocity = vel;
}

void Particle::incrPosition(const Vec &pos)
{
    position += pos;
}

void Particle::incrVelocity(const Vec &vel)
{
    velocity += vel;
}



void Particle::draw() const
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere(radius, 12, 12);
    glPopMatrix();

}



std::ostream& operator<<(std::ostream& os, const Particle& p)
{
    os << "pos (" << p.getPosition() << "), vel (" << p.getVelocity() << ")";
    return os;
}

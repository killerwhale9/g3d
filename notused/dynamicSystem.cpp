#include <cmath>
#include "stdio.h"
#include <iostream>
#include <map>
using namespace std;

#include "viewer.hpp"
#include "dynamicSystem.hpp"


DynamicSystem::DynamicSystem()
    :
        defaultGravity(0.0, 0.0, -10.0),
        defaultMediumViscosity(1.0),
        dt(0.1),
        groundPosition(0.0, 0.0, 0.0),
        groundNormal(0.0, 0.0, 1.0),
        rebound(0.5),
        density(1.0)
{
    // default values reset in init()
}

DynamicSystem::~DynamicSystem()
{
    clear();
}


void DynamicSystem::clear()
{
    vector<Particle *>::iterator itP;
    for (itP = particles.begin(); itP != particles.end(); ++itP) {
        delete(*itP);
    }
    particles.clear();

    vector<Spring *>::iterator itS;
    for (itS = springs.begin(); itS != springs.end(); ++itS) {
        delete(*itS);
    }
    springs.clear();
}

const Vec &DynamicSystem::getFixedParticlePosition() const
{
    //return particles[0]->getPosition();	// no check on 0!
    return Vec();
}

void DynamicSystem::setFixedParticlePosition(const Vec &pos)
{
    if (particles.size() > 0)
        particles[0]->setPosition(pos);
}

void DynamicSystem::setGravity(bool onOff)
{
    gravity = (onOff ? defaultGravity : Vec());
}

void DynamicSystem::setViscosity(bool onOff)
{
    mediumViscosity = (onOff ? defaultMediumViscosity : 0.0);
}

void DynamicSystem::setCollisionsDetection(bool onOff)
{
    handleCollisions = onOff;
}


///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::init(Viewer &viewer)
{
    toggleGravity = true;
    toggleViscosity = true;
    toggleCollisions = true;
    clear();

    // global scene parameters
    defaultGravity = Vec(0.0, 0.0, -10.0);
    gravity = defaultGravity;
    defaultMediumViscosity = 10.0;
    mediumViscosity = defaultMediumViscosity;
    handleCollisions = true;
    dt = 0.1;
    groundPosition = Vec(0.0, 0.0, 0.0);
    groundNormal = Vec(0.0, 0.0, 1.0);
    rebound = 0.5;
    // parameters shared by all particles
    particleMass = 1.0;
    particleRadius = 0.1;
    distanceBetweenParticles = 4.0 * particleRadius;
    // parameters shared by all springs
    springStiffness = 30.0;
    springInitLength = 0.5;
    springDamping = 1.0;

    createSystemScene();
    // or another method, e.g. to test collisions on simple cases...
    // createTestCollisions();

    // add a manipulatedFrame to move particle 0 with the mouse
    viewer.setManipulatedFrame(new qglviewer::ManipulatedFrame());
    viewer.manipulatedFrame()->setPosition(getFixedParticlePosition());
    viewer.setSceneRadius(10.0f);

}


///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::createSystemScene()
{
    // add a fixed particle
    //Vec initPos = Vec(0.0, 0.0, 2.0);
    //particles.push_back(new Particle(initPos, Vec(), 0.0, particleRadius));

    // add a second particle
    //particles.push_back(new Particle(Vec(0.0, 0.0, 3.0), Vec(), particleMass, particleRadius));

    // add a spring between the two particle
    //springs.push_back(new Spring(particles[0], particles[1], springStiffness, springInitLength, springDamping));

    // .. then create a chain of particles
    /*
       for (int i = 2; i < 10; i++) {
       pos1 += Vec(0.0, distanceBetweenParticles, 0.0);
       particles.push_back(new Particle(pos1, Vec(), particleMass, particleRadius));
       springs.push_back(new Spring(particles[i], particles[i-1], springStiffness, springInitLength, springDamping));
       }
       */
}


///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::draw(int pass)
{
    // Particles
    glColor3f(1,0,0);
    vector<Particle *>::iterator itP;
    for (itP = particles.begin(); itP != particles.end(); ++itP) {
        (*itP)->draw(pass);
    }

    // Springs
    glColor3f(1.0, 0.28, 0.0);
    glLineWidth(1.0);
    vector<Spring *>::iterator itS;
    for (itS = springs.begin(); itS != springs.end(); ++itS) {
        (*itS)->draw();
    }

    // Ground :
    /*
       glColor3f(0.0, 0.0, 1.0);
       glBegin(GL_QUADS);
       glVertex3f(-10.0f, -10.0f, 0.0f);
       glVertex3f(-10.0f, 10.0f,  0.0f);
       glVertex3f( 10.0f, 10.0f,  0.0f);
       glVertex3f( 10.0f, -10.0f, 0.0f);
       glEnd();
       */
}


int t = 0;
///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::animate()
{
    // Creates bubbles
    if (t++ % 100 == 0) {
        particles.push_back(new Particle(Vec(), Vec(0.4, 0.0, 0.0), particleMass, particleRadius));
    }

    vector<Particle *>::iterator itP;
    vector<Particle *>::iterator itP2;
    // Make bubbles bigger
    for (itP = particles.begin(); itP != particles.end(); ++itP) {
        Particle *p = *itP;
        p->setRadius(p->getRadius()*1.002);
    }

    //======== 1. Compute all forces
    // map to accumulate the forces to apply on each particle
    map<const Particle *, Vec> forces;

    // weights
    for (itP = particles.begin(); itP != particles.end(); ++itP) {
        Particle *p = *itP;
        forces[p] = density * p->getVolume() * Vec(0.0, 0.0, 1.0);
        forces[p] += -mediumViscosity * p->getVelocity() * p->getVolume();
    }

    // damped springs
    /*
       vector<Spring *>::iterator itS;
       for (itS = springs.begin(); itS != springs.end(); ++itS) {
       Spring *s = *itS;
       Vec f12 = s->getCurrentForce();
       forces[s->getParticle1()] += f12;
       forces[s->getParticle2()] -= f12; // opposite force
       }
       */


    //======== 2. Integration scheme
    // update particles velocity (qu. 1)
    for (itP = particles.begin(); itP != particles.end(); ++itP) {
        Particle *p = *itP;
        // v = v + dt * a / m
        p->incrVelocity(dt * forces[p] * p->getInvMass());
    }

    // update particles positions
    for (itP = particles.begin(); itP != particles.end(); ++itP) {
        Particle *p = *itP;
        // q = q + dt * v
        p->incrPosition(dt * p->getVelocity());
    }


    //======== 3. Collisions
    if (handleCollisions) {
        //TO DO: discuss multi-collisions and order!
        for (itP = particles.begin(); itP != particles.end(); ++itP) {
            collisionParticleGround(*itP);
        }
        for (itP = particles.begin(); itP != particles.end(); ++itP) {
            for (itP2 = itP + 1; itP2 != particles.end();) {
                if (collisionParticleParticle(*itP, *itP2)) {
                    itP2 = particles.erase(itP2);
                } else {
                    itP2++;
                }
            }
        }
    }
}



void DynamicSystem::collisionParticleGround(Particle *p)
{
    // don't process fixed particles (ground plane is fixed)
    if (p->getInvMass() == 0)
        return;

    // particle-plane distance
    double penetration = (p->getPosition() - groundPosition) * groundNormal;
    penetration -= p->getRadius();
    if (penetration >= 0)
        return;

    // penetration velocity
    double vPen = p->getVelocity() * groundNormal;

    // updates position and velocity of the particle
    p->incrPosition(-penetration * groundNormal);
    p->incrVelocity(-(1 + rebound) * vPen * groundNormal);
}


bool DynamicSystem::collisionParticleParticle(Particle *p1, Particle *p2)
{
    if ((p1->getPosition() - p2->getPosition()).norm() < (p1->getRadius() + p2->getRadius())) {
        p1->setPosition((p1->getPosition()*pow(p1->getRadius(), 3)+p2->getPosition()*pow(p2->getRadius(), 3))/(pow(p1->getRadius(), 3)+pow(p2->getRadius(), 3)));
        p1->setRadius(pow(pow(p1->getRadius(), 3) + pow(p1->getRadius(), 3), 0.33));
        return true;
    }
    return false;
}


void DynamicSystem::keyPressEvent(QKeyEvent* e, Viewer& viewer)
{
    // Get event modifiers key
    const Qt::KeyboardModifiers modifiers = e->modifiers();

    /* Controls added for Lab Session 6 "Physicall Modeling" */
    if ((e->key()==Qt::Key_G) && (modifiers==Qt::NoButton)) {
        toggleGravity = !toggleGravity;
        setGravity(toggleGravity);
        viewer.displayMessage("Set gravity to "
                + (toggleGravity ? QString("true") : QString("false")));

    } else if ((e->key()==Qt::Key_V) && (modifiers==Qt::NoButton)) {
        toggleViscosity = !toggleViscosity;
        setViscosity(toggleViscosity);
        viewer.displayMessage("Set viscosity to "
                + (toggleViscosity ? QString("true") : QString("false")));

    } else if ((e->key()==Qt::Key_C) && (modifiers==Qt::NoButton)) {
        toggleCollisions = !toggleCollisions;
        setCollisionsDetection(toggleCollisions);
        viewer.displayMessage("Detects collisions "
                + (toggleCollisions ? QString("true") : QString("false")));

    } else if ((e->key()==Qt::Key_Home) && (modifiers==Qt::NoButton)) {
        // stop the animation, and reinit the scene
        viewer.stopAnimation();
        init(viewer);
        viewer.manipulatedFrame()->setPosition(getFixedParticlePosition());
        toggleGravity = true;
        toggleViscosity = true;
        toggleCollisions = true;
    }
}

void DynamicSystem::mouseMoveEvent(QMouseEvent*, Viewer& v)
{
    //setFixedParticlePosition(v.manipulatedFrame()->position());
}

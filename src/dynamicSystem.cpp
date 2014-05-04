#include <cmath>
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
	rebound(0.5)
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
	return particles[0]->getPosition();	// no check on 0!
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
	defaultMediumViscosity = 1.0;
	mediumViscosity = defaultMediumViscosity;
	handleCollisions = true;
	dt = 0.1;
	groundPosition = Vec(0.0, 0.0, 0.0);
	groundNormal = Vec(0.0, 0.0, 1.0);
	rebound = 0.5;	
	// parameters shared by all particles
	particleMass = 1.0;
	particleRadius = 0.25;
	distanceBetweenParticles = 4.0 * particleRadius;
	// parameters shared by all springs
	springStiffness = 30.0;
	springInitLength = 0.5;
	springDamping = 1.0;

	createSystemScene();
	// or another method, e.g. to test collisions on simple cases...
// 	createTestCollisions();

	// add a manipulatedFrame to move particle 0 with the mouse
        viewer.setManipulatedFrame(new qglviewer::ManipulatedFrame());
	viewer.manipulatedFrame()->setPosition(getFixedParticlePosition());
	viewer.setSceneRadius(10.0f);

	//glEnable(GL_TEXTURE_2D);
    tx_nyan = TextureManager::loadTexture("oddish.png", "nyan", false);

}


///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::createSystemScene()
{
	// add a fixed particle
	Vec initPos = Vec(0.0, 0.0, 8.0);
	particles.push_back(new Particle(initPos, Vec(), 0.0, particleRadius));

    int nParts(20);

    Vec pos, prevPos(initPos),
        l0,
        vel(0.0, 0.0, 0.0);
    for (int i = 0; i < nParts; ++i) {
        pos = initPos + Vec(0.0, -distanceBetweenParticles*(i+1), 0.0);
        particles.push_back(new Particle(pos, vel, particleMass, particleRadius));
        l0 = pos - prevPos;

        Spring *spr = new Spring(particles[i], particles[i+1], springStiffness, l0.norm(), springDamping);
        springs.push_back(spr);

        prevPos = pos;
    }

    particles.push_back(new Particle(Vec(1.0, 3.0, particleRadius), Vec(-2.0, 0.0, 0.0), particleMass, particleRadius));
    particles.push_back(new Particle(Vec(-1.0, 3.0, particleRadius*3.0), Vec(5.0, 0.0, 0.0), particleMass, particleRadius*3.0));
    particles.back()->blue = true;

    particles.push_back(new Particle(Vec(3.0, 3.0, particleRadius), Vec(-3.0, 0.0, 10.0), particleMass, particleRadius));
    particles.push_back(new Particle(Vec(70.0, 1.0, particleRadius*40), Vec(-35, 0.0,0.0), particleMass*40, particleRadius*40));

    for (int i = 0; i < 0; i++) {
    	particles.push_back(new Particle(Vec(-2.0, 3.0, 5.0+i*(particleRadius+1.0)), Vec(0, 0.0, 0.0), particleMass, particleRadius));
    	particles.back()->blue = (i%2);
    }

	
	// .. then create a chain of particles
}


///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::draw()
{
	// Particles
	glColor3f(1,0,0);
    /*glEnable(GL_TEXTURE_2D);*/
	vector<Particle *>::iterator itP;
	for (itP = particles.begin(); itP != particles.end(); ++itP) {
		(*itP)->draw();
	}
    glDisable(GL_TEXTURE_2D);

	// Springs
	glColor3f(1.0, 0.28, 0.0);
	glLineWidth(5.0);
	vector<Spring *>::iterator itS;
	for (itS = springs.begin(); itS != springs.end(); ++itS) {
		(*itS)->draw();
	}

	// Ground :
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tx_nyan);
	glColor3f(1.0, 1.0, 1.0);
	glNormal3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);

    glTexCoord2f( 0.0f, 0.0f );
	glVertex3f(-10.0f, -10.0f, 0.0f);
    
    glTexCoord2f( 0.0f, 1.0f );
	glVertex3f(-10.0f, 10.0f,  0.0f);
    
    glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( 10.0f, 10.0f,  0.0f);
    
    glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( 10.0f, -10.0f, 0.0f);
	
	glEnd();
    glDisable(GL_TEXTURE_2D);
}


///////////////////////////////////////////////////////////////////////////////
void DynamicSystem::animate()
{

//======== 1. Compute all forces
	// map to accumulate the forces to apply on each particle
	map<const Particle *, Vec> forces;

	// weights
	vector<Particle *>::iterator itP;
	for (itP = particles.begin(); itP != particles.end(); ++itP) {
		Particle *p = *itP;
		forces[p] = gravity * p->getMass();
        forces[p] += -mediumViscosity * p->getVelocity();
	}

	// damped springs
	vector<Spring *>::iterator itS;
	for (itS = springs.begin(); itS != springs.end(); ++itS) {
		Spring *s = *itS;
		Vec f12 = s->getCurrentForce();
		forces[s->getParticle1()] += f12;
		forces[s->getParticle2()] -= f12; // opposite force
	}


//======== 2. Integration scheme
	// update particles velocity (qu. 1)
	// TODO!

	// update particles speed
	for (itP = particles.begin(); itP != particles.end(); ++itP) {
		Particle *p = *itP;
        // v = v + dt * a/m
		p->incrVelocity(dt * (forces[p] * p->getInvMass()));
	}

	// update particles positions
	for (itP = particles.begin(); itP != particles.end(); ++itP) {
		Particle *p = *itP;
		// q = q + dt * v
        // v = v + dt * a/m
		p->incrPosition(dt * p->getVelocity());
	}


//======== 3. Collisions
	if (handleCollisions) {
		//TO DO: discuss multi-collisions and order!
		for (itP = particles.begin(); itP != particles.end(); ++itP) {
			collisionParticleGround(*itP);
		}	
		for(unsigned int i = 0; i < particles.size(); ++i) {
			for(unsigned int j = 1; j < particles.size(); ++j) {
				if ( i != j) {
					Particle *p1 = particles[j]; 
					Particle *p2 = particles[i];
	            	collisionParticleParticle(p1, p2);
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
	double penetration = (p->getPosition() - groundPosition) * groundNormal; // Modifier pour noiseterrain XXX TODO
	penetration -= p->getRadius();
	if (penetration >= 0)
		return;

	// penetration velocity
	double vPen = p->getVelocity() * groundNormal;

	// updates position and velocity of the particle
	p->incrPosition(-penetration * groundNormal);
	p->incrVelocity(-(1 + rebound) * vPen * groundNormal);
}


void DynamicSystem::collisionParticleParticle(Particle *p1, Particle *p2)
{
	if (p1->getInvMass() == 0)
    return;

    Vec p(p2->getPosition() - p1->getPosition());

    double penetration = p.norm() - p1->getRadius() - p2->getRadius();

    if (penetration >= 0.0)
        return;

    p.normalize();

    std::cout<<"Collision\n";

    Vec vel(p1->getVelocity() - p2->getVelocity());

    p1->incrPosition(penetration*p*0.5);
    //p1->incrVelocity(-p1->getVelocity()*p1->getMass()/(p1->getMass()+p2->getMass()));
    p1->incrVelocity(-p2->getMass()/(p1->getMass()+p2->getMass())
                    *(p*vel)*(1.0+rebound)*p);

    if (p2->getInvMass() == 0.0)
        return;
    p *= -1.0;
    p2->incrPosition(penetration*p*0.5);
    //p2->incrVelocity(-p2->getVelocity()*p2->getMass()/(p1->getMass()+p2->getMass()));
    p2->incrVelocity(p1->getMass()/(p1->getMass()+p2->getMass())
                   *(p*vel)*(1.0+rebound)*p);
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
        setFixedParticlePosition(v.manipulatedFrame()->position());
}

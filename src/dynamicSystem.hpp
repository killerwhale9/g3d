#ifndef _DYNAMIC_SYSTEM_H_
#define _DYNAMIC_SYSTEM_H_

#include <QGLViewer/vec.h>
#include <QGLViewer/manipulatedFrame.h>
using namespace qglviewer;

#include <vector>
using namespace std;

#include "spring.hpp"
#include "particle.hpp"
#include "renderable.hpp"
#include "TextureManager.hpp"

/*
 * This class represents a dynamic system made of particles
 * linked together with damped springs, within a medium with
 * gravity and viscosity.
 * Particles a represented by small spheres, with a radius and a mass.
 * The initial scene is composed of a fixed plane, a static particle
 * that can be controlled by the mouse, and a dynamic particle.
 */
class DynamicSystem : public Renderable
{

private:
	// system
	vector<Particle *> particles;
	vector<Spring *> springs;
	
	// System parameters (common)
	Vec defaultGravity;
	Vec gravity;			// gravity used in simulation
	double defaultMediumViscosity;
	double mediumViscosity;		// viscosity used in simulation
	double dt;			// time step
	bool handleCollisions;
	
	// Collisions parameters
	Vec groundPosition;
	Vec groundNormal;
	double rebound;	// 0 = pure absorption; 1 = pure elastic impact

	// Parameters shared by all particles
	double particleMass;
	double particleRadius;
	double distanceBetweenParticles;

	// Parameters shared by all springs
	double springStiffness;
	double springInitLength;
	double springDamping;

	// viewer related states
	bool toggleGravity;
	bool toggleViscosity;
	bool toggleCollisions;

	GLuint tx_nyan;

public:
	DynamicSystem();
	virtual ~DynamicSystem();

	// Position of the firt particle ca be set through mouse movements
	const Vec &getFixedParticlePosition() const;
	void setEndPosition(const Vec &pos);
	void setBeginingPosition(const Vec &pos);
    void setEndParticlePosition(const Vec &pos);
	// Activate/desactivate gravity during the simulation
	void setGravity(bool onOff);
	// Activate/desactivate viscosity during the simulation
	void setViscosity(bool onOff);
	// Activate/desactivate contacts during the simulation
	void setCollisionsDetection(bool onOff);

	// event response
	void keyPressEvent(QKeyEvent*, Viewer&);
	
private:
	// Reset the scene (remove all particles and springs)
	void clear();
	
	// Compute collision between a sphere and the fixed) ground
	void collisionParticleGround(Particle *p);

	void collisionParticleParticle(Particle *p1, Particle *p2);

	// Compute collision between a sphere and a moving plane
// 	static void collisionParticlePlane(Particle *p,
// 		Vec planePosition, Vec placeNormal, Vec planeVelocity,
// 		Vec planeInvMass, double rebound);

	// Compute collision between a sphere and a sphere
// 	void collisionSphereSphere(Vec& x1, Vec& v1, double r1, double invm1,
// 			Vec& x2, Vec& v2, double r2, double invm2,
// 			double rebound );

	void createSystemScene(Vec v);
	void createTestCollisions();


// Renderable methods
public:
	// Init the particle-spring system
	void init(Vec v);

	// Draw the particle-spring system
	void draw(int pass);

	// Update positions and velocities of dynamic objects
	void animate();
};

#endif

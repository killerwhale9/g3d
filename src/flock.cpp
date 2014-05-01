#include "flock.hpp"
#include "fish.hpp"
#include "environment.hpp"
#include <iostream>


void Flock::draw(int pass)
{
    for (std::vector<Fish>::iterator it(school.begin()); it != school.end(); ++it) {
        it->draw(pass);
    }
    glColor3f(1.f, 1.f, 1.f);
}

Flock::Flock() : step(0), dx(0), dy(0)
{

}

Flock::~Flock()
{

}

void Flock::animate()
{
    // Update Collsion Environment
    env.setObjs( envPos, envRad );

    // Rush the Leader (0.08 0.05)
    school[0].update( 0.05, 0, school, goal, env );	  // Update Leader 
    for ( uint32_t i = 1; i < school.size(); i++ ) {
        school[i].update( 0.05, i, school, school[0].oldpos, env );	
    }

    // Random Goal
    step++;
    if ( step % 200 == 0 ) {
        goal[0] = (SCENE_WIDTH * (rand() / (float) RAND_MAX )) - (SCENE_WIDTH / 2.0);
        goal[1] = (SCENE_HEIGHT * (rand() / (float) RAND_MAX )) - (SCENE_HEIGHT / 2.0);
        goal[2] = (SCENE_DEPTH * (rand() / (float) RAND_MAX )) - (SCENE_DEPTH / 2.0);
        goal *= 0.95;  // Shrink potential goal area so goal is never "on the glass"
    }
    dx = 0;
    dy = 0;
}

void Flock::init(Viewer& v)
{
    // Make the flocking fish
    Fish* newFish;
    for ( int i = 0; i < NUM_FLOCKING_FISH; i++ ) {
        newFish = new Fish( 10 * (rand() / (float) RAND_MAX),
                10 * (rand() / (float) RAND_MAX),
                10 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX),
                1 * (rand() / (float) RAND_MAX));
        school.push_back( *newFish );
    }

    school[0].leader = true;
    school[0].colour = glm::vec3( 1, 0.5, 0.5 );
    school[0].pos = glm::vec3( 5, 0, 0 );
    goal = glm::vec3( 8, 0, 0 );    // Default Goal
}

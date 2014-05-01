#include "flock.hpp"
#include "fish.hpp"
#include "environment.hpp"
#include <iostream>
#include "glm/geometric.hpp"


void Flock::draw(int pass)
{
    for (std::vector<Fish>::iterator it(school.begin()); it != school.end(); ++it) {
        it->draw(pass);
    }
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
    bool needUpdate;
    float dt = 0.2;
    env.setObjs( envPos, envRad );

    // Rush the Leader (0.08 0.05)
    needUpdate = school[0].update( dt, 0, school, goal, env );	  // Update Leader 
    //std::cout<<"dist:"<<glm::length(school[0].pos - goal)<<"\n";
    for ( uint32_t i = 1; i < school.size(); i++ ) {
        school[i].update( dt, i, school, school[0].oldpos, env );	
    }

    // Random Goal
    step++;
    if ( needUpdate || step % 200 == 0 ) {
        goal[0] = (SCENE_WIDTH * (rand() / (float) RAND_MAX )) - (SCENE_WIDTH / 2.0);
        goal[1] = (SCENE_HEIGHT * (rand() / (float) RAND_MAX )) - (SCENE_HEIGHT / 2.0);
        goal[2] = (SCENE_DEPTH * (rand() / (float) RAND_MAX )) + 0.1;
        goal *= 0.95;  // Shrink potential goal area so goal is never "on the glass"
        if (goal.z < 0.1) goal.z = 0.1;
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
    school[0].pos = glm::vec3( 0, 0, 0 );
    goal = glm::vec3( 8, 5, 5 );    // Default Goal
}

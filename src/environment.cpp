#define GLUT_DISABLE_ATEXIT_HACK

#include "environment.hpp"
#include "glm/geometric.hpp"
#include <iostream>

//#include "flock.hpp"

// Helper
float Environment::intexp( float base, int exp ) {
    float acc = 1;
    while ( exp > 1 ) {
        acc *= base;	
        exp--;
    }	
    return acc;
}

// Helper - Radians/Degree conversion
float Environment::rad2deg( float rad )
{
    return rad * 180.0 / 3.14;
}

float Environment::deg2rad( float deg )
{
    return deg * 3.14 / 180.0;
}

// Returns steering vector
glm::vec3 Environment::steer( glm::vec3 pos, glm::vec3 vel )
{

    glm::vec3 steer;				// Steering Acc
    //	float sphereInfDistance = 3;  	        // Proximity


    // Sphere at 0, 0, 0, radius 1
    //	glm::vec3 sphere1( 0, 0, 0 );
    //	float sphere1r = 3;

    //	glm::vec3 vecToCenter = sphere1 - pos;
    //	float cpmag = glm::length( vecToCenter );


    // ####################### Steer away from centers ##################

    glm::vec3 cToP;
    float dToC;

    //	cout << spherePos.size() << endl;

    for ( uint32_t i = 0; i < spherePos.size(); i++ ) {

        cToP = pos - spherePos[i];
        dToC = glm::length(cToP);

        if ( dToC < sphereRad[i] * 2.f) {
            float p = intexp( 1.f-((dToC - sphereRad[i]) / sphereRad[i]), 3);
            cToP = glm::normalize(cToP);
            steer += p * cToP;
        }

    }
    // ######################### Steer away from environment #############

    float wallInfDistance = 1.f;
    float halfWidth = SCENE_WIDTH/2.f;
    float halfHeight = SCENE_HEIGHT/2.f;
    float halfDepth = SCENE_DEPTH/2.f;

    // Left Wall
    if ( -pos.x > (halfWidth - wallInfDistance) ) {
        steer += intexp( 1-((halfWidth + pos.x)/wallInfDistance), 2) * glm::vec3(1,0,0);
    }
    // Right Wall
    if ( pos.x > (halfWidth - wallInfDistance) ) {
        steer += intexp( 1-((halfWidth - pos.x)/wallInfDistance), 2) * glm::vec3(-1,0,0);
    }
    // Back Wall
    if ( -pos.z > (halfDepth - wallInfDistance) ) {
        steer += intexp( 1-((halfDepth + pos.z)/wallInfDistance), 2) * glm::vec3(0,0,1);
    }
    // Front Wall
    if ( pos.z > (halfDepth - wallInfDistance) ) {
        steer += intexp( 1-((halfDepth - pos.z)/wallInfDistance), 2) * glm::vec3(0,0,-1);
    }
    // Bottom Wall
    if ( -pos.y > (halfHeight - wallInfDistance) ) {
        steer += intexp( 1-((halfHeight + pos.y)/wallInfDistance), 2) * glm::vec3(0,1,0);
    }
    // Top Wall
    if ( pos.y > (halfHeight - wallInfDistance) ) {
        steer += intexp( 1-((halfHeight - pos.y)/wallInfDistance), 2) * glm::vec3(0,-1,0);
    }


    return steer;
}

// Save objects of environment
void Environment::setObjs( std::vector<glm::vec3> pos, std::vector<float> rad ) {
  spherePos = pos;
  sphereRad = rad;
}

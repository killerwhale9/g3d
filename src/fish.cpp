#define MAX_PRIORITY_CONTROL 1    // Amount of weight available

#define GLUT_DISABLE_ATEXIT_HACK

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <vector>

#include "fish.hpp"
#include "environment.hpp"
#include "glm/geometric.hpp"

// Simulation Parameters
#define MAX_VELOCITY 1.40      // Maximum velocity
#define MIN_DISTANCE 2          // Minimum distance between flock members
#define ARRIVAL_DISTANCE 3      // Distance till arrival slow-down
#define FOV_RADIUS 3           // Radius of field of view
#define FOV_ANGLE 110          // Radius of field of view angle

#define SWIM_ANGLE_DELTA_NORM 4   // Change in angle per step
#define SWIM_ANGLE_MAX 30        // Maximum bend in body

using namespace std;

// DEBUG assistance
const bool SHOW_LINE_TO_GOAL = false;
const bool SHOW_GOAL_SEEK_DIR = false;
const bool SHOW_ENVSTEER_DIR = false;
const bool SHOW_FOV_RADIUS = false;
const bool SHOW_FOV_BLIND_CONE = false;
const bool SHOW_LOCAL_AXIS = false;
const bool SHOW_DIRECTION_VECTOR = false;

// Pi approximation
float mypi = 3.14;


// Default Constructor
Fish::Fish() {
    pos = glm::vec3( 0, 0, 0 );
    oldpos = pos;
    vel = glm::vec3( 0, 0, 0 );
    direction = glm::vec3( 1, 0, 0 );
    direction = glm::normalize(direction);
    leader = false;
    swimAngle = 0;
    swimAngleDelta = SWIM_ANGLE_DELTA_NORM;

    colour = glm::vec3( 0.3, 0.5, 0.3 );

}

Fish::~Fish()
{

}

void Fish::debug()
{
    std::cout<<" ==== \n";
    std::cout<<"pos:"<<pos.x<<","<<pos.y<<","<<pos.z<<"\n";
    std::cout<<"oldpos:"<<oldpos.x<<","<<oldpos.y<<","<<oldpos.z<<"\n";
    std::cout<<"vel:"<<vel.x<<","<<vel.y<<","<<vel.z<<"\n";
    std::cout<<"oldvel:"<<oldvel.x<<","<<oldvel.y<<","<<oldvel.z<<"\n";
    std::cout<<"colour:"<<colour.x<<","<<colour.y<<","<<colour.z<<"\n";
    std::cout<<"direction:"<<direction.x<<","<<direction.y<<","<<direction.z<<"\n";
    std::cout<<"lineToGoal:"<<lineToGoal.x<<","<<lineToGoal.y<<","<<lineToGoal.z<<"\n";
    std::cout<<"goalSeekDir:"<<goalSeekDir.x<<","<<goalSeekDir.y<<","<<goalSeekDir.z<<"\n";
    std::cout<<"envSteerDir:"<<envSteerDir.x<<","<<envSteerDir.y<<","<<envSteerDir.z<<"\n";
    std::cout<<"separationDir:"<<separationDir.x<<","<<separationDir.y<<","<<separationDir.z<<"\n";
    std::cout<<"avgNeighbourVel:"<<avgNeighbourVel.x<<","<<avgNeighbourVel.y<<","<<avgNeighbourVel.z<<"\n";
    std::cout<<"velMatchDir:"<<velMatchDir.x<<","<<velMatchDir.y<<","<<velMatchDir.z<<"\n";
    std::cout<<"avgNeighbourPos:"<<avgNeighbourPos.x<<","<<avgNeighbourPos.y<<","<<avgNeighbourPos.z<<"\n";
    std::cout<<"centeringDir:"<<centeringDir.x<<","<<centeringDir.y<<","<<centeringDir.z<<"\n";
    std::cout<<"targetDir:"<<targetDir.x<<","<<targetDir.y<<","<<targetDir.z<<"\n";
    std::cout<<" ==== \n";
}

inline glm::vec3 checkedNormalize(const glm::vec3 v)
{
    if (v.x != 0.f ||
        v.y != 0.f ||
        v.z != 0.f)
        return glm::normalize(v);
    else
        return v;
}

// Param Constructor
Fish::Fish( float x, float y, float z,
        float dirx, float diry, float dirz,
        float vx, float vy, float vz,
        float rand) {
    pos = glm::vec3( x, y, z );
    oldpos = pos;
    vel = glm::vec3( vx, vy, vz );
    direction = glm::vec3( dirx, diry, dirz );
    direction = checkedNormalize(direction);
    leader = false;
    swimAngle = (2 * SWIM_ANGLE_MAX) * rand - SWIM_ANGLE_MAX;
    swimAngleDelta = SWIM_ANGLE_DELTA_NORM;

    colour = glm::vec3( 0.3, 0.5, 0.3 );
}


// Update Simulation
bool Fish::update( float dt, uint32_t schoolID, vector< Fish > &school, glm::vec3 globalGoal,
        Environment &env ) {


    // Save old data
    oldpos = pos;
    oldvel = vel;

    // Reset some data
    zerovec(lineToGoal);
    zerovec(goalSeekDir);
    zerovec(envSteerDir);
    envSteerPriority = 0;
    zerovec(separationDir);
    separationPriority = 0;
    zerovec(avgNeighbourPos);
    zerovec(centeringDir);
    centeringPriority = 0;
    zerovec(avgNeighbourVel);
    zerovec(velMatchDir);
    velMatchPriority = 0;
    zerovec(targetDir);
    int numNeighbours = 0;
    float smallestCloseNeighDist = MIN_DISTANCE;
    float priorityControl = 0;



    // ####################### Controller ##########################

    // Global Goal
    lineToGoal = globalGoal - oldpos;	

    goalSeekDir = globalGoal - oldpos;
    goalSeekDir = checkedNormalize(goalSeekDir);
    goalSeekDir *= 0.4;

    // Environment Effects
    envSteerDir = env.steer( oldpos, oldvel );
    envSteerPriority = glm::length(envSteerDir);
    envSteerDir = checkedNormalize(envSteerDir);
    priorityControl += envSteerPriority;


    // Local Neighbourhood
    for( uint32_t i = 0; i < school.size(); i++ ) {
        if ( schoolID != i ) {  // Don't influence yourself


            glm::vec3 neighbourDir = school[i].oldpos - pos;
            float neighbourDist = glm::length(neighbourDir);

            if (	(neighbourDist < FOV_RADIUS) &&
                    (rad2deg(acos( glm::dot(checkedNormalize(neighbourDir), direction) )) < FOV_ANGLE) ) {  // Is it a neighbour?

                numNeighbours++;

                // Work for Separation
                if ( neighbourDist < MIN_DISTANCE ) {
                    separationDir -= checkedNormalize(neighbourDir) * intexp( 1-(neighbourDist / MIN_DISTANCE), 2 );
                    if ( neighbourDist < smallestCloseNeighDist ) smallestCloseNeighDist = neighbourDist;
                }

                // Work for Velocity Matching
                avgNeighbourVel += school[i].oldvel;

                // Work for Flock Centering
                avgNeighbourPos += school[i].oldpos;

            }

        }	
    }


    // ####################  Navigation  ###########################

    // Separation Influence
    if ( 	(numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL) ) {

        separationPriority = intexp(1-(smallestCloseNeighDist / MIN_DISTANCE), 3);
        priorityControl += separationPriority;

    }


    // Velocity Matching Influence
    if ( 	(numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL) ) {

        glm::vec3 avgVel = avgNeighbourVel / (float)numNeighbours;
        float diffAngle = rad2deg( acos( glm::dot( checkedNormalize(avgVel), checkedNormalize(oldvel) ) ) );
        velMatchDir = (avgNeighbourVel / (float)numNeighbours) - oldvel;
        velMatchPriority = intexp( diffAngle / 180.0, 2);

        if ( velMatchPriority + priorityControl > MAX_PRIORITY_CONTROL ) {

            velMatchPriority = MAX_PRIORITY_CONTROL - priorityControl;
            priorityControl = MAX_PRIORITY_CONTROL;				

        }
        else {

            priorityControl += centeringPriority;

        }
    }		


    // Centering Influence
    if ( 	(numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL) ) {

        centeringDir = (avgNeighbourPos / (float)numNeighbours) - oldpos;
        centeringPriority = intexp( glm::length(centeringDir) / 20, 2);

        if ( centeringPriority + priorityControl > MAX_PRIORITY_CONTROL ) {

            centeringPriority = MAX_PRIORITY_CONTROL - priorityControl;
            priorityControl = MAX_PRIORITY_CONTROL;				

        }
        else {

            priorityControl += centeringPriority;

        }
    }


    // glm::normalize 
    //			cout << (priorityControl) << "  ";
    /*
       if ( priorityControl > 0 && priorityControl < MAX_PRIORITY_CONTROL ) {
       float scale = MAX_PRIORITY_CONTROL / priorityControl;
       envSteerPriority *= scale;
       separationPriority *= scale;
       velMatchPriority *= scale;
       centeringPriority *= scale;
       priorityControl = MAX_PRIORITY_CONTROL;
       }
       */

    // Final Target Direction
    if ( leader ) {

        targetDir = envSteerPriority * envSteerDir +
            (MAX_PRIORITY_CONTROL - priorityControl) * goalSeekDir;  	

    }
    else {

        targetDir = envSteerPriority * envSteerDir +
            separationPriority * separationDir +
            velMatchPriority * velMatchDir +
            centeringPriority * centeringDir +
            (MAX_PRIORITY_CONTROL - priorityControl) * goalSeekDir;  /// Aim to goal with remaining priority
    }


    if ( glm::length(targetDir) > 1 ) targetDir *= (1.f / glm::length(targetDir));



    // Pilot / Flight

    // Arrival
    float modMax;
    if ( glm::length(lineToGoal) < ARRIVAL_DISTANCE ) {

        modMax = intexp( (glm::length(lineToGoal) / ARRIVAL_DISTANCE) * MAX_VELOCITY, 2);

    }
    else {

        modMax = MAX_VELOCITY;

    }



    // Solver
    vel = oldvel + targetDir * dt;
    if ( glm::length(vel) > modMax ) vel *= (modMax / glm::length(vel));   // Truncate velocity
    pos = oldpos + oldvel * dt;


    // Update Direction - Hack
    if ( 	pos[0] != oldpos[0] &&
            pos[1] != oldpos[1] &&
            pos[2] != oldpos[2]) {
        direction = pos - oldpos;
        direction = checkedNormalize(direction);
    }

    // Update Swim Angle
    if ( swimAngle >= SWIM_ANGLE_MAX ) swimAngleDelta *= -1;
    if ( swimAngle <= -SWIM_ANGLE_MAX ) swimAngleDelta *= -1;
    swimAngle += swimAngleDelta;

    return glm::length(pos-globalGoal) < 0.5;

}



// Draw Fish
void Fish::draw(int pass) {

    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    // Line To Goal Direction
    if ( SHOW_LINE_TO_GOAL ) {
        glColor3f( 0.5, 0.5, 0.5 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( lineToGoal[0], lineToGoal[1], lineToGoal[2] );
        glEnd();
    }

    // Goal Seek Direction
    if ( SHOW_ENVSTEER_DIR ) {
        glColor3f( 1, 1, 0 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( envSteerDir[0], envSteerDir[1], envSteerDir[2] );
        glEnd();
    }

    // Goal Seek Direction
    if ( SHOW_GOAL_SEEK_DIR ) {
        glColor3f( 0.8, 0.8, 0.8 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( goalSeekDir[0], goalSeekDir[1], goalSeekDir[2] );
        glEnd();
    }

    // Direction Vector
    if ( SHOW_DIRECTION_VECTOR ) {
        glColor3f( 1, 1, 1 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( direction[0], direction[1], direction[2] );
        glEnd();	
    }


    // Rotate to point in direction
    float xzLen = sqrt ( direction[0] * direction[0] + direction[2] * direction[2] );
    float yRot, xRot;
    if ( xzLen == 0 ) {

        if ( direction[0] > 0 )
            yRot = 90;
        else
            yRot = -90;

    }
    else {

        yRot = rad2deg( acos( direction[2] / xzLen ) );

    }
    xRot = rad2deg( acos( xzLen ) );
    if ( direction[1] > 0 ) xRot *= -1;
    if ( direction[0] < 0 ) yRot *= -1;

    glRotatef( yRot, 0, 1, 0 );
    glRotatef( xRot, 1, 0, 0 );


    // Draw Actual "Fish"
    glColor3f( colour[0], colour[1], colour[2] );

    // Draw "Fish"
    float velRatio = glm::length(vel) / MAX_VELOCITY;
    glPushMatrix();

    GLfloat amDef[4], spDef[4];
    GLfloat am[] = { colour[0], colour[1], colour[2], 1 };
    GLfloat sp[] = { 0, 0.5, 1, 0.5 };

    glGetMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, amDef );
    glGetMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, spDef );

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, am );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, sp );

    // Head
    glRotatef( velRatio * 0.8 * swimAngle, 0, 1, 0 );
    glutSolidCone( 0.2, 0.4, 5, 1 );

    // Body
    glRotatef( 180 - velRatio * swimAngle, 0, 1, 0 );
    glutSolidCone( 0.1, 0.5, 5, 1 );
    glPushMatrix();
    glTranslatef( 0, 0, 0.1 );
    glRotatef( -65, 1, 0, 0 );
    glutSolidCone( 0.08, 0.3, 5, 1 );
    glPopMatrix();

    // Tail
    glTranslatef( 0, 0, 0.5 );
    glRotatef( 180 + velRatio * 1.2 * swimAngle, 0, 1, 0 );
    glTranslatef( 0, 0, -0.2 );
    glScalef( 0.5, 1, 1 );
    glutSolidCone( 0.2, 0.2, 5, 1 );
    glPopMatrix();

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, amDef );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, spDef );

    // Radius of influence
    if ( SHOW_FOV_RADIUS ) {
        glColor3f( 0.2, 0.2, 0.2 );
        glutWireSphere(FOV_RADIUS, 10, 3 );
    }

    // Draw Axis
    if ( SHOW_LOCAL_AXIS ) {
        glColor3f( 1, 0, 0 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( 1, 0, 0 );
        glEnd();

        glColor3f( 0, 1, 0 );
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( 0, 1, 0 );
        glEnd();

        glColor3f( 0, 0, 1 );
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( 0, 0, 1 );
        glEnd();
    }

    // Draw Blind Cone
    if ( SHOW_FOV_BLIND_CONE ) {
        glPushMatrix();
        glTranslatef( 0, 0, -FOV_RADIUS );
        glutWireCone( FOV_RADIUS * tan( deg2rad( 180 - FOV_ANGLE ) ), FOV_RADIUS, 4, 1 );
        glPopMatrix();
    }		

    glPopMatrix();

}



// Draw Helpers
void Fish::drawSeparation() {
    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    glm::vec3 sepVec = separationDir * 3.f * separationPriority;
    glColor3f( 1, 0, 0 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( sepVec[0], sepVec[1], sepVec[2] );
    glEnd();


    glPopMatrix();
}

void Fish::drawVelocityMatching() {
    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    glm::vec3 vmatVec = velMatchDir * 3.f * velMatchPriority;
    glColor3f( 0, 0, 1 );
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( vmatVec[0], vmatVec[1], vmatVec[2] );
    glEnd();

    glPopMatrix();
}

void Fish::drawCentering() {
    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    glm::vec3 cenVec = centeringDir * 3.f * centeringPriority;
    glColor3f( 0, 1, 0 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( cenVec[0], cenVec[1], cenVec[2] );
    glEnd();

    glPopMatrix();
}

void Fish::drawTarget() {
    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    glm::vec3 tarVec = targetDir * 3.f;
    glColor3f( 1, 0, 1 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( tarVec[0], tarVec[1], tarVec[2] );
    glEnd();

    glPopMatrix();
}

void Fish::drawVelocity() {

    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    glColor3f( 0, 1, 1 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( vel[0], vel[1], vel[2] );
    glEnd();	

    glPopMatrix();
}

void Fish::drawEnvSteer() {

    glPushMatrix();
    glTranslatef( pos[0], pos[1], pos[2] );

    glm::vec3 esVec = envSteerDir * 3.f * envSteerPriority;
    glColor3f( 1, 1, 1 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( esVec[0], esVec[1], esVec[2] );
    glEnd();	

    glPopMatrix();
}



// Helpers
// Integer exponent
float Fish::intexp( float base, int exp ) {
    float acc = 1;
    while ( exp > 1 ) {
        acc *= base;	
        exp--;
    }	
    return acc;
}

// Zero out vector
void Fish::zerovec( glm::vec3 &vec ) {
    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0;	
}

// Radians/Degree conversion
float Fish::rad2deg( float rad ) { return rad * 180.0 / mypi; }
float Fish::deg2rad( float deg ) { return deg * mypi / 180.0; }

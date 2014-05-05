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
#include "objManager.hpp"

// Simulation Parameters
#define MAX_VELOCITY 1.40      // Maximum velocity
#define MIN_DISTANCE 2          // Minimum distance between flock members
#define ARRIVAL_DISTANCE 3      // Distance till arrival slow-down
#define FOV_RADIUS 3           // Radius of field of view
#define FOV_ANGLE 110          // Radius of field of view angle

#define SWIM_ANGLE_DELTA_NORM 4   // Change in angle per step
#define SWIM_ANGLE_MAX 30        // Maximum bend in body

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
Fish::Fish() : m_model(objManager::getObj("fish1")) {
    m_pos = glm::vec3( 0, 0, 0 );
    m_oldPos = m_pos;
    m_vel = glm::vec3( 0, 0, 0 );
    m_direction = glm::vec3( 1, 0, 0 );
    m_direction = glm::normalize(m_direction);
    m_leader = false;
    m_swimAngle = 0;
    m_swimAngleDelta = SWIM_ANGLE_DELTA_NORM;

    m_colour = glm::vec3( 0.3, 0.5, 0.3 );

}

Fish::~Fish()
{

}

void Fish::debug()
{
    std::cout<<" ==== \n";
    std::cout<<"m_pos:"<<m_pos.x<<","<<m_pos.y<<","<<m_pos.z<<"\n";
    std::cout<<"m_oldPos:"<<m_oldPos.x<<","<<m_oldPos.y<<","<<m_oldPos.z<<"\n";
    std::cout<<"m_vel:"<<m_vel.x<<","<<m_vel.y<<","<<m_vel.z<<"\n";
    std::cout<<"m_oldVel:"<<m_oldVel.x<<","<<m_oldVel.y<<","<<m_oldVel.z<<"\n";
    std::cout<<"m_colour:"<<m_colour.x<<","<<m_colour.y<<","<<m_colour.z<<"\n";
    std::cout<<"m_direction:"<<m_direction.x<<","<<m_direction.y<<","<<m_direction.z<<"\n";
    std::cout<<"m_lineToGoal:"<<m_lineToGoal.x<<","<<m_lineToGoal.y<<","<<m_lineToGoal.z<<"\n";
    std::cout<<"goalSeekDir:"<<goalSeekDir.x<<","<<goalSeekDir.y<<","<<goalSeekDir.z<<"\n";
    std::cout<<"m_envSteerDir:"<<m_envSteerDir.x<<","<<m_envSteerDir.y<<","<<m_envSteerDir.z<<"\n";
    std::cout<<"m_separationDir:"<<m_separationDir.x<<","<<m_separationDir.y<<","<<m_separationDir.z<<"\n";
    std::cout<<"m_avgNeighbourVel:"<<m_avgNeighbourVel.x<<","<<m_avgNeighbourVel.y<<","<<m_avgNeighbourVel.z<<"\n";
    std::cout<<"m_velMatchDir:"<<m_velMatchDir.x<<","<<m_velMatchDir.y<<","<<m_velMatchDir.z<<"\n";
    std::cout<<"m_avgNeighbourPos:"<<m_avgNeighbourPos.x<<","<<m_avgNeighbourPos.y<<","<<m_avgNeighbourPos.z<<"\n";
    std::cout<<"m_centeringDir:"<<m_centeringDir.x<<","<<m_centeringDir.y<<","<<m_centeringDir.z<<"\n";
    std::cout<<"m_targetDir:"<<m_targetDir.x<<","<<m_targetDir.y<<","<<m_targetDir.z<<"\n";
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
        float rand, const std::string &model) : m_model(objManager::getObj(model)) {
    m_pos = glm::vec3( x, y, z );
    m_oldPos = m_pos;
    m_vel = glm::vec3( vx, vy, vz );
    m_direction = glm::vec3( dirx, diry, dirz );
    m_direction = checkedNormalize(m_direction);
    m_leader = false;
    m_swimAngle = (2 * SWIM_ANGLE_MAX) * rand - SWIM_ANGLE_MAX;
    m_swimAngleDelta = SWIM_ANGLE_DELTA_NORM;

    m_colour = glm::vec3( 0.3, 0.5, 0.3 );
}


// Update Simulation
bool Fish::update( float dt, uint32_t schoolID, std::vector< Fish > &school, glm::vec3 globalGoal,
        Environment &env ) {


    // Save old data
    m_oldPos = m_pos;
    m_oldVel = m_vel;

    // Reset some data
    zerovec(m_lineToGoal);
    zerovec(goalSeekDir);
    zerovec(m_envSteerDir);
    m_envSteerPriority = 0;
    zerovec(m_separationDir);
    m_separationPriority = 0;
    zerovec(m_avgNeighbourPos);
    zerovec(m_centeringDir);
    m_centeringPriority = 0;
    zerovec(m_avgNeighbourVel);
    zerovec(m_velMatchDir);
    m_velMatchPriority = 0;
    zerovec(m_targetDir);
    int numNeighbours = 0;
    float smallestCloseNeighDist = MIN_DISTANCE;
    float priorityControl = 0;



    // ####################### Controller ##########################

    // Global Goal
    m_lineToGoal = globalGoal - m_oldPos;	

    goalSeekDir = globalGoal - m_oldPos;
    goalSeekDir = checkedNormalize(goalSeekDir);
    goalSeekDir *= 0.4;

    // Environment Effects
    m_envSteerDir = env.steer( m_oldPos, m_oldVel );
    m_envSteerPriority = glm::length(m_envSteerDir);
    m_envSteerDir = checkedNormalize(m_envSteerDir);
    priorityControl += m_envSteerPriority;


    // Local Neighbourhood
    for( uint32_t i = 0; i < school.size(); i++ ) {
        if ( schoolID != i ) {  // Don't influence yourself


            glm::vec3 neighbourDir = school[i].m_oldPos - m_pos;
            float neighbourDist = glm::length(neighbourDir);

            if (	(neighbourDist < FOV_RADIUS) &&
                    (rad2deg(acos( glm::dot(checkedNormalize(neighbourDir), m_direction) )) < FOV_ANGLE) ) {  // Is it a neighbour?

                numNeighbours++;

                // Work for Separation
                if ( neighbourDist < MIN_DISTANCE ) {
                    m_separationDir -= checkedNormalize(neighbourDir) * intexp( 1-(neighbourDist / MIN_DISTANCE), 2 );
                    if ( neighbourDist < smallestCloseNeighDist ) smallestCloseNeighDist = neighbourDist;
                }

                // Work for Velocity Matching
                m_avgNeighbourVel += school[i].m_oldVel;

                // Work for Flock Centering
                m_avgNeighbourPos += school[i].m_oldPos;

            }

        }	
    }


    // ####################  Navigation  ###########################

    // Separation Influence
    if ( 	(numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL) ) {

        m_separationPriority = intexp(1-(smallestCloseNeighDist / MIN_DISTANCE), 3);
        priorityControl += m_separationPriority;

    }


    // Velocity Matching Influence
    if ( 	(numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL) ) {

        glm::vec3 avgVel = m_avgNeighbourVel / (float)numNeighbours;
        float diffAngle = rad2deg( acos( glm::dot( checkedNormalize(avgVel), checkedNormalize(m_oldVel) ) ) );
        m_velMatchDir = (m_avgNeighbourVel / (float)numNeighbours) - m_oldVel;
        m_velMatchPriority = intexp( diffAngle / 180.0, 2);

        if ( m_velMatchPriority + priorityControl > MAX_PRIORITY_CONTROL ) {

            m_velMatchPriority = MAX_PRIORITY_CONTROL - priorityControl;
            priorityControl = MAX_PRIORITY_CONTROL;				

        }
        else {

            priorityControl += m_centeringPriority;

        }
    }		


    // Centering Influence
    if ( 	(numNeighbours > 0) &&
            (priorityControl < MAX_PRIORITY_CONTROL) ) {

        m_centeringDir = (m_avgNeighbourPos / (float)numNeighbours) - m_oldPos;
        m_centeringPriority = intexp( glm::length(m_centeringDir) / 20, 2);

        if ( m_centeringPriority + priorityControl > MAX_PRIORITY_CONTROL ) {

            m_centeringPriority = MAX_PRIORITY_CONTROL - priorityControl;
            priorityControl = MAX_PRIORITY_CONTROL;				

        }
        else {

            priorityControl += m_centeringPriority;

        }
    }


    // glm::normalize 
    //			cout << (priorityControl) << "  ";
    /*
       if ( priorityControl > 0 && priorityControl < MAX_PRIORITY_CONTROL ) {
       float scale = MAX_PRIORITY_CONTROL / priorityControl;
       m_envSteerPriority *= scale;
       m_separationPriority *= scale;
       m_velMatchPriority *= scale;
       m_centeringPriority *= scale;
       priorityControl = MAX_PRIORITY_CONTROL;
       }
       */

    // Final Target m_direction
    if ( m_leader ) {

        m_targetDir = m_envSteerPriority * m_envSteerDir +
            (MAX_PRIORITY_CONTROL - priorityControl) * goalSeekDir;  	

    }
    else {

        m_targetDir = m_envSteerPriority * m_envSteerDir +
            m_separationPriority * m_separationDir +
            m_velMatchPriority * m_velMatchDir +
            m_centeringPriority * m_centeringDir +
            (MAX_PRIORITY_CONTROL - priorityControl) * goalSeekDir;  /// Aim to goal with remaining priority
    }


    if ( glm::length(m_targetDir) > 1 ) m_targetDir *= (1.f / glm::length(m_targetDir));



    // Pilot / Flight

    // Arrival
    float modMax;
    if ( glm::length(m_lineToGoal) < ARRIVAL_DISTANCE ) {

        modMax = intexp( (glm::length(m_lineToGoal) / ARRIVAL_DISTANCE) * MAX_VELOCITY, 2);

    }
    else {

        modMax = MAX_VELOCITY;

    }



    // Solver
    m_vel = m_oldVel + m_targetDir * dt;
    if ( glm::length(m_vel) > modMax ) m_vel *= (modMax / glm::length(m_vel));   // Truncate velocity
    m_pos = m_oldPos + m_oldVel * dt;


    // Update m_direction - Hack
    if ( 	m_pos[0] != m_oldPos[0] &&
            m_pos[1] != m_oldPos[1] &&
            m_pos[2] != m_oldPos[2]) {
        m_direction = m_pos - m_oldPos;
        m_direction = checkedNormalize(m_direction);
    }

    // Update Swim Angle
    if ( m_swimAngle >= SWIM_ANGLE_MAX ) m_swimAngleDelta *= -1;
    if ( m_swimAngle <= -SWIM_ANGLE_MAX ) m_swimAngleDelta *= -1;
    m_swimAngle += m_swimAngleDelta;

    return glm::length(m_pos-globalGoal) < 0.5;

}



// Draw Fish
void Fish::draw(int pass) {

    glPushMatrix();
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    // Line To Goal m_direction
    if ( SHOW_LINE_TO_GOAL ) {
        glColor3f( 0.5, 0.5, 0.5 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( m_lineToGoal[0], m_lineToGoal[1], m_lineToGoal[2] );
        glEnd();
    }

    // Goal Seek m_direction
    if ( SHOW_ENVSTEER_DIR ) {
        glColor3f( 1, 1, 0 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( m_envSteerDir[0], m_envSteerDir[1], m_envSteerDir[2] );
        glEnd();
    }

    // Goal Seek m_direction
    if ( SHOW_GOAL_SEEK_DIR ) {
        glColor3f( 0.8, 0.8, 0.8 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( goalSeekDir[0], goalSeekDir[1], goalSeekDir[2] );
        glEnd();
    }

    // m_direction Vector
    if ( SHOW_DIRECTION_VECTOR ) {
        glColor3f( 1, 1, 1 );	
        glBegin( GL_LINES );
        glVertex3f(0,0,0);
        glVertex3f( m_direction[0], m_direction[1], m_direction[2] );
        glEnd();	
    }


    // Rotate to point in m_direction
    float xzLen = sqrt ( m_direction[0] * m_direction[0] + m_direction[2] * m_direction[2] );
    float yRot, xRot;
    if ( xzLen == 0 ) {

        if ( m_direction[0] > 0 )
            yRot = 90;
        else
            yRot = -90;

    }
    else {

        yRot = rad2deg( acos( m_direction[2] / xzLen ) );

    }
    xRot = rad2deg( acos( xzLen ) );
    if ( m_direction[1] > 0 ) xRot *= -1;
    if ( m_direction[0] < 0 ) yRot *= -1;

    glRotatef( yRot, 0, 1, 0 );
    glRotatef( xRot, 1, 0, 0 );


    // Draw Actual "Fish"
    //glColor3f( m_colour[0], m_colour[1], m_colour[2] );

    // Draw "Fish"
    float velRatio = glm::length(m_vel) / MAX_VELOCITY;
    glPushMatrix();

    GLfloat amDef[4], spDef[4];
    GLfloat am[] = { m_colour[0], m_colour[1], m_colour[2], 1 };
    GLfloat sp[] = { 0, 0.5, 1, 0.5 };

    glGetMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, amDef );
    glGetMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, spDef );

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, am );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, sp );

    // Head
    //glRotatef( velRatio * 0.8 * m_swimAngle, 0, 1, 0 );
    //glutSolidCone( 0.2, 0.4, 5, 1 );

    // Body
    glRotatef( 180 - velRatio * m_swimAngle, 0, 1, 0 );
    //glutSolidCone( 0.1, 0.5, 5, 1 );
    glScalef(0.005, 0.005, 0.005);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    m_model.draw(pass);
    //glPushMatrix();
    //glTranslatef( 0, 0, 0.1 );
    //glRotatef( -65, 1, 0, 0 );
    //glutSolidCone( 0.08, 0.3, 5, 1 );
    //glPopMatrix();

    //// Tail
    //glTranslatef( 0, 0, 0.5 );
    //glRotatef( 180 + velRatio * 1.2 * m_swimAngle, 0, 1, 0 );
    //glTranslatef( 0, 0, -0.2 );
    //glScalef( 0.5, 1, 1 );
    //glutSolidCone( 0.2, 0.2, 5, 1 );
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
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    glm::vec3 sepVec = m_separationDir * 3.f * m_separationPriority;
    glColor3f( 1, 0, 0 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( sepVec[0], sepVec[1], sepVec[2] );
    glEnd();


    glPopMatrix();
}

void Fish::drawVelocityMatching() {
    glPushMatrix();
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    glm::vec3 vmatVec = m_velMatchDir * 3.f * m_velMatchPriority;
    glColor3f( 0, 0, 1 );
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( vmatVec[0], vmatVec[1], vmatVec[2] );
    glEnd();

    glPopMatrix();
}

void Fish::drawCentering() {
    glPushMatrix();
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    glm::vec3 cenVec = m_centeringDir * 3.f * m_centeringPriority;
    glColor3f( 0, 1, 0 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( cenVec[0], cenVec[1], cenVec[2] );
    glEnd();

    glPopMatrix();
}

void Fish::drawTarget() {
    glPushMatrix();
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    glm::vec3 tarVec = m_targetDir * 3.f;
    glColor3f( 1, 0, 1 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( tarVec[0], tarVec[1], tarVec[2] );
    glEnd();

    glPopMatrix();
}

void Fish::drawVelocity() {

    glPushMatrix();
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    glColor3f( 0, 1, 1 );	
    glBegin( GL_LINES );
    glVertex3f(0,0,0);
    glVertex3f( m_vel[0], m_vel[1], m_vel[2] );
    glEnd();	

    glPopMatrix();
}

void Fish::drawEnvSteer() {

    glPushMatrix();
    glTranslatef( m_pos[0], m_pos[1], m_pos[2] );

    glm::vec3 esVec = m_envSteerDir * 3.f * m_envSteerPriority;
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


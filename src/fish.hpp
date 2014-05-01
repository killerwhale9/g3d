#ifndef __FISH_H__
#define __FISH_H__
/*******************************************************************************
 *  fish                                                                       *
 *  Thu May 01 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include <vector>
#include "glm/vec3.hpp"
#include "environment.hpp"
#include "objReader.hpp"

class Fish : public Renderable
{
    public:
        glm::vec3 pos;               // New position of time step
        glm::vec3 oldpos;            // Old position of time step
        glm::vec3 vel;               // New velocity
        glm::vec3 oldvel;            // Old velocity
        glm::vec3 colour;            // Body colour
        glm::vec3 direction;         // Estimated direction
        bool leader;             // Is 
        Fish();
        Fish( float x, float y, float z, 
                float dirx, float diry, float dirz,
                float vx, float vy, float vz,
                float rand);
        ~Fish();
        // Update step
        bool update( float dt, uint32_t schoolID, std::vector< Fish > &school, glm::vec3 globalGoal,
                Environment &e );

        // Draw
        void draw(int pass);

        // Draw Helpers
        void drawSeparation();
        void drawVelocityMatching();
        void drawCentering();
        void drawTarget();
        void drawVelocity();
        void drawEnvSteer();

        // Math Helpers
        float intexp( float base, int exp );
        void zerovec( glm::vec3 &vec );
        float rad2deg( float rad );
        float deg2rad( float deg );

    void debug();

    private:
        glm::vec3 lineToGoal;
        glm::vec3 goalSeekDir;
        glm::vec3 envSteerDir;
        float envSteerPriority;
        glm::vec3 separationDir;
        float separationPriority;
        glm::vec3 avgNeighbourVel;
        glm::vec3 velMatchDir;
        float velMatchPriority;
        glm::vec3 avgNeighbourPos;
        glm::vec3 centeringDir;
        float centeringPriority;
        glm::vec3 targetDir;
        float swimAngle;
        float swimAngleDelta;

};

#endif

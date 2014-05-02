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
#include <cmath>

class Fish : public Renderable
{
    public:
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
        inline void zerovec( glm::vec3 &vec ) { vec[0] = 0.; vec[1] = 0.; vec[2] = 0.; }
        inline float rad2deg( float rad ) { return rad * 180.0 / M_PI; }
        inline float deg2rad( float deg ) { return deg * M_PI / 180.0; }

        void debug();

        inline glm::vec3 getPos() const { return m_pos; }
        inline void setPos(const glm::vec3& a) { m_pos = a; }

        inline glm::vec3 getOldPos() const { return m_oldPos; }
        inline void setOldPos(const glm::vec3& a) { m_oldPos = a; }

        inline glm::vec3 getVel() const { return m_vel; }
        inline void setVel(const glm::vec3& a) { m_vel = a; }

        inline glm::vec3 getOldVel() const { return m_oldVel; }
        inline void setOldVel(const glm::vec3& a) { m_oldVel = a; }

        inline glm::vec3 getColour() const { return m_colour; }
        inline void setColour(const glm::vec3& a) { m_colour = a; }

        inline glm::vec3 getDirection() const { return m_direction; }
        inline void setDirection(const glm::vec3& a) { m_direction = a; }

        inline bool getLeader() const { return m_leader; }
        inline void setLeader(bool a) { m_leader = a; }


    private:
        glm::vec3 m_lineToGoal;
        glm::vec3 goalSeekDir;
        glm::vec3 m_envSteerDir;
        float m_envSteerPriority;
        glm::vec3 m_separationDir;
        float m_separationPriority;
        glm::vec3 m_avgNeighbourVel;
        glm::vec3 m_velMatchDir;
        float m_velMatchPriority;
        glm::vec3 m_avgNeighbourPos;
        glm::vec3 m_centeringDir;
        float m_centeringPriority;
        glm::vec3 m_targetDir;
        float m_swimAngle;
        float m_swimAngleDelta;

        glm::vec3 m_pos;               // New position of time step
        glm::vec3 m_oldPos;            // Old position of time step
        glm::vec3 m_vel;               // New velocity
        glm::vec3 m_oldVel;            // Old velocity
        glm::vec3 m_colour;            // Body m_colour
        glm::vec3 m_direction;         // Estimated m_direction
        bool m_leader;             // Is 

};

#endif

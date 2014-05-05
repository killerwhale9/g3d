#ifndef _CORAL_
#define _CORAL_

#include <vector>
#include "renderable.hpp"
#include "cylinder.hpp"
#include "viewer.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Coral : public Renderable
{
    public:
		static const int defaultAngle = 35;
		static const int defaultDepth = 5;

		static const float minMult = 3.0;
		static const float maxMult = 5.0;

        void draw(int pass);
        void drawSub(int pass, int angle);
        Coral(int depth, int x, int y, float mutl, float h);
        static inline float randomBetween(float min, float max) {
            return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
        };

    private:
		int m_nbBranch;
		int m_depthBranch;
		int m_depth;
        int m_x;
        int m_y;
        bool m_initialized;
        float m_pivot;
        float m_height;
        GLuint m_list;
		Cylinder m_coral; //root
		std::vector<Coral> m_smallCorals;
        
        void initDraw(int pass);
};

#endif

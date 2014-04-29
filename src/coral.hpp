#ifndef _CORAL_
#define _CORAL_

#include <vector>
#include "renderable.hpp"
#include "cylinder.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Coral : public Renderable
{
    public:
		static const int defaultAngle = 35;
		static const int defaultDepth = 6;

        void draw();
        void drawSub(int angle);
        Coral(int depth);
    private:
		int m_nbBranch;
		int m_depthBranch;
		int m_depth;
		Cylinder m_coral; //root
		std::vector<Coral> m_smallCorals;
};

#endif

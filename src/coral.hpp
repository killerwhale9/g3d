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
		static const int defaultDepth = 5;

        void draw(int pass);
        void drawSub(int pass, int angle);
        Coral(int depth, int x, int y);

    private:
		int m_nbBranch;
		int m_depthBranch;
		int m_depth;
        int m_x;
        int m_y;
        bool m_initialized;
        float m_pivot;
        GLuint m_list;
		Cylinder m_coral; //root
		std::vector<Coral> m_smallCorals;
        
        void initDraw(int pass);
};

#endif

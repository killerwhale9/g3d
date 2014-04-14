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
        void draw();
        Coral();
    private:
		int m_nbBranch;
		int m_depthBranch;
		Cylinder m_coral; //root
		std::vector<Coral> m_smallCorals;
};

#endif

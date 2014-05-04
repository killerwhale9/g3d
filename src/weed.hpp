#ifndef _WEED_
#define _WEED_

#include "renderable.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Weed : public Renderable
{
    public:
        Weed();

    private:
        void draw(int pass);
};

#endif


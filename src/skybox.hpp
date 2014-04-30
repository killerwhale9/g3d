#ifndef _SKYBOX_
#define _SKYBOX_

#include "renderable.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Skybox : public Renderable
{
    public:
        Skybox();
        void draw(int pass);

    private:
		void drawCrate(float size);
};

#endif


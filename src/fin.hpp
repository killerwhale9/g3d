#ifndef _FIN_
#define _FIN_

#include "renderable.hpp"
#include "cylinder.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class Fin : public Renderable
{
    public:
        void draw(int pass);
        Fin();
    private:
};

#endif


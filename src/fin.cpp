#include <iostream>
using namespace std;
#include "cylinder.hpp"
#include "fin.hpp"
#include <cmath>


Fin::Fin() {}

void Fin::draw(int pass)
{
    glPushMatrix();
	glBegin(GL_TRIANGLES);
    	glVertex3f( 0.0f, -1.0f, 0.0f);
        glVertex3f(1.0f,1.0f, 0.0f);
        glVertex3f(-1.0f,1.0f, 0.0f);
    glEnd();
	glRotatef(-90,1,0,0);
	glTranslatef(0,0,-0.5);
	glutSolidCone(0.4,1.5,10,2);
    glPopMatrix();
}

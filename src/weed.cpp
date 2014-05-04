#include <iostream>
using namespace std;
#include "weed.hpp"
#include "TextureManager.hpp"

Weed::Weed() {

}


void Weed::draw(int pass) {

    glPushMatrix();
    glTranslatef(10.0, 3.0 , 0.0);
    int size=1;
    if (pass == PASS_NORMAL)
		TextureManager::bindTexture("weed");
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0,0.0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(size, 0, 0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(size, 0, size);
	glTexCoord2f(0.0,1.0);
	glVertex3f(0, 0, size);
	glVertex3f(0, 0, 0);
    glEnd();
    glPopMatrix();
}


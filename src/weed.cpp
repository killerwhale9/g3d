#include <iostream>
using namespace std;
#include "weed.hpp"
#include "TextureManager.hpp"

Weed::Weed() {

}


void Weed::draw(int pass) {

    glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB); 
	glDisable(GL_LIGHTING);
    glTranslatef(10.0, 3.0 , 0.0);
    int size=1;
	if (pass == PASS_NORMAL)
		TextureManager::bindTexture("weed");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
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
    glDisable(GL_BLEND);
	// Réactivation de l'écriture dans le DepthBuffer
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);

	// Réinitialisation des états OpenGL
	glDisable(GL_TEXTURE_CUBE_MAP_ARB); 
        glPopAttrib(); // lighting
    glPopMatrix();
}


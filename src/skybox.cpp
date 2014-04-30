#include <iostream>
using namespace std;
#include "skybox.hpp"
#include "TextureManager.hpp"

Skybox::Skybox() {}

void Skybox::draw(int pass) {
	// Configuration des états OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB); 
	glDisable(GL_LIGHTING);

	// Désactivation de l'écriture dans le DepthBuffer
	glDepthMask(GL_FALSE);

	// Dessine la skybox
	glPushMatrix();
	glTranslatef(-100.0, -100.0, 0.0);
	drawCrate(200, pass);
	glPopMatrix();

	// Réactivation de l'écriture dans le DepthBuffer
	glDepthMask(GL_TRUE);

	// Réinitialisation des états OpenGL
	glDisable(GL_TEXTURE_CUBE_MAP_ARB); 
	glEnable(GL_LIGHTING);

}

void Skybox::drawCrate(float size, int pass)
{
	glBegin(GL_QUADS);

    if (pass == PASS_NORMAL)
		TextureManager::bindTexture("sky_left");

	glVertex3f(0, 0, 0);
	glVertex3f(size, 0, 0);
	glVertex3f(size, 0, size);
	glVertex3f(0, 0, size);

	glTexCoord2f(0.0,0.0);
	glVertex3f(size, 0, 0);
	glTexCoord2f(1.0,0.0);
	glVertex3f(size, size, 0);
	glTexCoord2f(1.0,1.0);
	glVertex3f(size, size, size);
	glTexCoord2f(0.0,1.0);
	glVertex3f(size, 0, size);

	glVertex3f(size, size, 0);
	glVertex3f(0, size, 0);
	glVertex3f(0, size, size);
	glVertex3f(size, size, size);

	glVertex3f(0, size, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, size);
	glVertex3f(0, size, size);

	glVertex3f(0, 0, size);
	glVertex3f(size, 0, size);
	glVertex3f(size, size, size);
	glVertex3f(0, size, size);

	glEnd();
}


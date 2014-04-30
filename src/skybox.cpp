#include <iostream>
using namespace std;
#include "skybox.hpp"

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
	//glTranslatef(10.0, 0.0, 0.0);
	drawCrate(20);
	glPopMatrix();

	// Réactivation de l'écriture dans le DepthBuffer
	glDepthMask(GL_TRUE);

	// Réinitialisation des états OpenGL
	glDisable(GL_TEXTURE_CUBE_MAP_ARB); 
	glEnable(GL_LIGHTING);

}

void Skybox::drawCrate(float size)
{
	glBegin(GL_QUADS);

	glVertex3f(0, 0, 0);
	glVertex3f(size, 0, 0);
	glVertex3f(size, 0, size);
	glVertex3f(0, 0, size);

	glVertex3f(size, 0, 0);
	glVertex3f(size, size, 0);
	glVertex3f(size, size, size);
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


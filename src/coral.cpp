#include <iostream>
using namespace std;
#include "coral.hpp"
#include "TextureManager.hpp"

Coral::Coral(int depth, int x, int y) : 
	m_coral(depth/10.0,depth/10.0*0.3,5)
{
	m_depth=depth;
	m_nbBranch = 2;
	m_depthBranch = depth;
    m_x = x;
    m_y = y;
    m_initialized = false;

	//Recursive
	if (depth > 0) {
		for (int i=0; i < m_nbBranch; i++) {
			m_smallCorals.push_back(Coral(depth-1, 0, 0));
		}
	}
}

void Coral::initDraw(int pass) {
    m_list = glGenLists(1);
    glNewList(m_list, GL_COMPILE);

    m_coral.draw(pass);

	vector<Coral>::iterator it;
	int i=0;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
		if (i%2 == 0) {
		    it->drawSub(pass, -defaultAngle);
		} else {
		    it->drawSub(pass, defaultAngle);
		}			
		i++;
	}
    glEndList();
}

void Coral::draw(int pass)
{
    if (!m_initialized) {
        initDraw(pass);
        m_initialized = true;
    }

    glPushMatrix();
        if (pass == PASS_NORMAL)
            TextureManager::bindTexture("corail1");
    //Sur le sol c'est bien aussi
    glTranslatef(4,0.2,0.2);
    //On le bouge
    glTranslatef(m_x, m_y, 0);

    glCallList(m_list);

    glPopMatrix();
}

void Coral::drawSub(int pass, int angle)
{
    glPushMatrix();
	//Decale les branches vers le haut
	glTranslatef(0,0, m_coral.getHeigth() - m_depth/10.0/1.7);
	//Pivote pour détacher le nouveau cylindre
	glRotatef(angle,1,0,0);
	//Pivote pour eviter d'aligner toutes les branches dans le même plan
	glRotatef(angle*2,0,0,1);
	m_coral.draw(pass);

	vector<Coral>::iterator it;
	int i=0;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
		if (i%2 == 0) {
		    it->drawSub(pass, -defaultAngle);
		} else {
		    it->drawSub(pass, defaultAngle);
		}			
		i++;
	}
    glPopMatrix();

}

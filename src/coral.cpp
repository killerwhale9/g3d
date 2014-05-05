#include <iostream>
using namespace std;
#include "coral.hpp"
#include "TextureManager.hpp"
#include <cstdlib>

Coral::Coral(int depth, int x, int y, float mult, float h) : 
	m_coral(depth/mult,depth/mult*0.3,5)
{
	m_depth=depth;
	m_nbBranch = 2;
	m_depthBranch = depth;
    m_x = x;
    m_y = y;
    m_initialized = false;
    m_pivot = randomBetween(0.0, 90.0);
    m_height = h;

	//Recursive
	if (depth > 0) {
		for (int i=0; i < m_nbBranch; i++) {
			m_smallCorals.push_back(Coral(depth-1, 0, 0, randomBetween(minMult,maxMult), h));
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
    //On le bouge
    glTranslatef(m_x, m_y, m_height);

    glRotatef(m_pivot, 0.0, 0.0, 1.0);

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



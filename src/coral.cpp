#include <iostream>
using namespace std;
#include "coral.hpp"

Coral::Coral(int depth) : 
	m_coral(0.1*depth,0.2,10)
{
	m_nbBranch = 3;
	m_depthBranch = depth;

	//Recursive
	if (depth > 0) {
		for (int i=0; i < m_nbBranch; i++) {
			m_smallCorals.push_back(Coral(depth-1));
		}
	}
}

void Coral::draw()
{
    glPushMatrix();
	glTranslatef(4,0.2,0.2);
	m_coral.draw();

	vector<Coral>::iterator it;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
	    it->drawSub();
	}
    glPopMatrix();
}

void Coral::drawSub()
{
    glPushMatrix();
	glRotatef(35,1,0,0);
	m_coral.draw();

	vector<Coral>::iterator it;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
	    it->drawSub();
	}
    glPopMatrix();

}

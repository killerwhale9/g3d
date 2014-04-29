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
	int i=0;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
		if (i%2 == 0) {
		    it->drawSub(defaultAngle);
		} else {
		    it->drawSub(-defaultAngle);
		}			
		i++;
	}
    glPopMatrix();
}

void Coral::drawSub(int angle)
{
    glPushMatrix();
	glRotatef(angle,1,0,0);
	m_coral.draw();

	vector<Coral>::iterator it;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
		int i=0;
		if (i%2 == 0) {
		    it->drawSub(defaultAngle);
		} else {
		    it->drawSub(-defaultAngle);
		}			
		i++;
	}
    glPopMatrix();

}

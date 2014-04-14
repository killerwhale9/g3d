#include <iostream>
using namespace std;
#include "coral.hpp"

Coral::Coral() : m_coral(4.0,4.0,3)
{
}

void Coral::draw()
{
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
	m_coral.draw();

	vector<Coral>::iterator it;
	for (it=m_smallCorals.begin(); it != m_smallCorals.end(); it++) {
	    it->draw();
	}
    glPopMatrix();
}

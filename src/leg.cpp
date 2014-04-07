#include <iostream>
using namespace std;
#include "leg.h"
#include "cylinder.h"
#include "math.h"


Leg::Leg() :
    m_length(2.0),
    m_width(1.0),
    m_precision(50),
    m_figure(m_length, m_width, m_precision) {
}

void Leg::draw()
{
    glPushMatrix();
    glutSolidSphere(0.5, m_precision, m_precision);
    glTranslatef(0, 0, -(m_length/2.0));
    glRotatef(180, 0, 1, 0);
    m_figure.draw();

    glPopMatrix();
}

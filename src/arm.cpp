#include <iostream>
using namespace std;
#include "arm.h"
#include "cylinder.h"
#include <cmath>


Arm::Arm() :
    m_length(2.0),
    m_width(1.0),
    m_precision(50),
    m_figure(m_length, m_width, m_precision)
{
}

void Arm::draw()
{
    glPushMatrix();
    glutSolidSphere(0.5, m_precision, m_precision);
    //glPushMatrix();
    //glTranslatef(m_length/2.0, 0,0);
    glRotatef(90, 0, 1, 0);
    //glRotatef(20, 1, 0, 0);
    m_figure.draw();
    //glPopMatrix();
    //glTranslatef( 0, -m_length*sin(90), 0);
    //glutSolidSphere(0.5, m_precision, m_precision);

    glPopMatrix();
}

#include <iostream>
using namespace std;
#include "arm.hpp"
#include "cylinder.hpp"
#include <cmath>


Arm::Arm(int prec) :
    m_length(2.0),
    m_width(1.0),
    m_precision(prec),
    m_figure(m_length, m_width, m_precision)
{
}

void Arm::draw(int pass)
{
    glPushMatrix();
    glutSolidSphere(0.5, m_precision, m_precision);
    //glPushMatrix();
    //glTranslatef(m_length/2.0, 0,0);
    glRotatef(90, 0, 1, 0);
    //glRotatef(20, 1, 0, 0);
    m_figure.draw(pass);
    //glPopMatrix();
    //glTranslatef( 0, -m_length*sin(90), 0);
    //glutSolidSphere(0.5, m_precision, m_precision);

    glPopMatrix();
}

#include <iostream>
using namespace std;
#include "leg.hpp"
#include "cylinder.hpp"
#include <cmath>


Leg::Leg(int prec) :
    m_length(2.0),
    m_width(1.0),
    m_precision(prec),
    m_figure(m_length, m_width, m_precision) {
    }

void Leg::draw(int pass)
{
    glPushMatrix();
    glutSolidSphere(0.5, m_precision, m_precision);
    glTranslatef(0, 0, 0.0);
    glRotatef(180, 0, 1, 0);
    m_figure.draw(pass);

    glPopMatrix();
}

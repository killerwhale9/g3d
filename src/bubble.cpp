#include "bubble.hpp"
#include <cmath>


Bubble::Bubble(float r, float x, float y, float z) :
    m_radius(r),
    m_x(x),
    m_y(y),
    m_z(z),
    m_precision(50)
{
}

void Bubble::draw(int pass)
{
    glPushMatrix();
    glTranslatef(m_x, m_y, m_z);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.25f, 0.25f);
    glutSolidSphere(m_radius, m_precision, m_precision);
    glDisable(GL_BLEND);
    glPopMatrix();
}

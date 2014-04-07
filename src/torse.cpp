#include <iostream>
using namespace std;
#include "torse.h"
#include "cylinder.h"
#include "math.h"

Torse::Torse() :
    m_length(4.f),
    m_width(3.f),
    m_precision(50),
    m_figure(m_length, m_width, m_precision)
{
}

void Torse::draw()
{
    glPushMatrix();

    m_figure.draw();

    //Right arm
    glPushMatrix();
    glTranslatef(m_width/2.0*1.1, 0, m_length/2.0*0.80);
    glRotatef(45, 0, 1, 0);
    m_rUArm.draw();
    glPopMatrix();

    //Left arm
    glPushMatrix();
    glTranslatef(-(m_width/2.0*1.1), 0, m_length/2.0*0.80);
    glRotatef(180, 0, 1, 0);
    glRotatef(-45, 0, 1, 0);
    m_lUArm.draw();
    glPopMatrix();

    //Right leg
    glPushMatrix();
    glTranslatef(m_width/2.0*0.7, 0, -(m_length/2.0*1.1));
    glRotatef(-10, 0, 1, 0);
    m_rULeg.draw();
    glPopMatrix();

    //Left leg
    glPushMatrix();
    glTranslatef(-(m_width/2.0*0.7), 0, -(m_length/2.0*1.1));
    glRotatef(10, 0, 1, 0);
    m_lULeg.draw();
    glPopMatrix();
    glPopMatrix();
}

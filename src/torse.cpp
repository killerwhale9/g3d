#include <iostream>
using namespace std;
#include "torse.hpp"
#include "cylinder.hpp"
#include <cmath>

Torse::Torse() :
    m_length(4.f),
    m_width(2.f),
    m_headRadius(1.2f),
    m_precision(50),
    m_figure(m_length, m_width, m_precision),
    m_lUArm(),
    m_rUArm(),
    m_lLArm(),
    m_rLArm(),
    m_lULeg(),
    m_rULeg(),
    m_lLLeg(),
    m_rLLeg(),

    m_tmp(0)
{
}

void Torse::draw()
{
    glPushMatrix();
    m_tmp = (m_tmp+1)%360;
    cout<<m_tmp<<"\n";

    //glRotatef(45, 0, 1, 0);
    //glRotatef(45, 1, 0, 0);
    //glRotatef(45, 0, 0, 1);
    glRotatef(m_tmp, 1, 1, 1);
    m_figure.draw();

    glPushMatrix();
    glTranslatef(0, 0, m_length);
    glRotatef(m_tmp, 1, 1, 1);
    glTranslatef(0, 0, m_headRadius);
    glutSolidSphere(m_headRadius, m_precision, m_precision);
    glPopMatrix();

    //Right arm
    glPushMatrix();
    glTranslatef(m_width/2.0*1.1, 0, m_length*0.80);
    glRotatef(60, 0, 0, 1);
    glRotatef(m_tmp, 0, 1, 0);
    //glRotatef(m_tmp, 0, 0, 1);
    m_rUArm.draw();

    glTranslatef(m_rUArm.getLength(), 0, 0);
    glRotatef(m_tmp, 0, 1, 0);
    m_rLArm.draw();

    glPopMatrix();

    //Left arm
    glPushMatrix();
    glTranslatef(-(m_width/2.0*1.1), 0, m_length*0.80);
    //glRotatef(180, 0, 1, 0);
    glRotatef(180, 0, 0, 1);
    m_lUArm.draw();

    glTranslatef(m_lUArm.getLength(), 0, 0);
    glRotatef(m_tmp, 0, 1, 0);
    m_lLArm.draw();

    glPopMatrix();

    //Right leg
    glPushMatrix();
    glTranslatef(m_width/2.0*0.7, 0, 0.0);
    glRotatef(m_tmp, 1, 1, 1);
    m_rULeg.draw();

    glTranslatef(0.0, 0.0, -m_rULeg.getLength());
    glRotatef(m_tmp, 1, 1, 1);
    m_rLLeg.draw();

    glPopMatrix();

    //Left leg
    glPushMatrix();
    glTranslatef(-(m_width/2.0*0.7), 0, 0);
    glRotatef(-m_tmp, 1, 1, 1);
    m_lULeg.draw();

    glTranslatef(0, 0, -m_lULeg.getLength());
    glRotatef(m_tmp, 0, 1, 0);
    m_lLLeg.draw();

    glPopMatrix();

    glPopMatrix();
}

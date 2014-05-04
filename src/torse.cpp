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
    m_bottle(3.f, 1.f, m_precision),
    m_lUArm(),
    m_rUArm(),
    m_lLArm(),
    m_rLArm(),
    m_angUArm(0.0f),
    m_angLArm(0.0f),
    m_dirArm(+1),
    m_lULeg(),
    m_rULeg(),
    m_lLLeg(),
    m_rLLeg(),
    m_angLeg(0.0f),
    m_dirLeg(+1),

    m_tmp(0)
{
}

void Torse::draw(int pass)
{
    glPushMatrix();
    m_tmp = (m_tmp+1)%360;
    //cout<<m_tmp<<"\n";

    //glRotatef(45, 0, 1, 0);
    //glRotatef(45, 1, 0, 0);
    //glRotatef(45, 0, 0, 1);
    //glRotatef(m_tmp, 1, 1, 1);

    // Torse
    glColor3f(0,0,1);
    m_figure.draw(pass);

    // Bottle
    glPushMatrix();
    glTranslatef(0, -1.3f, 0);
    glColor3f(1,1,0);
    m_bottle.draw(pass);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, m_length);
    //glTranslatef(0, m_headRadius, m_headRadius);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 0, 0, 1);
    glColor3f(1,1,0);
    glutSolidSphere(m_headRadius, m_precision, m_precision);
    glPopMatrix();

    //Change arms orientation
    m_angUArm += m_dirArm;

    if (m_dirArm > 0) {
        //Arm going down
        if (m_angUArm < 0) {
            //First half
            if (m_angLArm < 0) {
                m_angLArm += m_dirArm;
            }
        } else {
            //Second half
            if (m_angUArm <= 30) {
                m_angLArm -= m_dirArm;
            } else {
                m_angLArm += m_dirArm;
            }
        }
    } else {
        //Arm going up
        if (m_angLArm + m_angUArm <= -90) {
            m_angLArm = -90 - m_angUArm;
        } else {
            m_angLArm += m_dirArm;
        }
    }

    if (m_angUArm >= 60 || m_angUArm <= -50)
        m_dirArm = -m_dirArm;

    //cout<<"angUArm:"<<m_angUArm<<" angLArm:"<<m_angLArm<<"\n";

    //Right arm
    glPushMatrix();
    glTranslatef(m_width/2.0*1.1, 0, m_length*0.80);
    glRotatef(m_angUArm, 0, 1, 0);
    glColor3f(0,0,1);
    m_rUArm.draw(pass);

    glTranslatef(m_rUArm.getLength(), 0, 0);
    glRotatef(m_angLArm, 0, 1, 0);
    glColor3f(1,0,0);
    m_rLArm.draw(pass);

    glPopMatrix();

    //Left arm
    glPushMatrix();
    glTranslatef(-(m_width/2.0*1.1), 0, m_length*0.80);
    glRotatef(180, 0, 0, 1);
    glRotatef(m_angUArm, 0, 1, 0);
    glColor3f(0,0,1);
    m_lUArm.draw(pass);

    glTranslatef(m_lUArm.getLength(), 0, 0);
    glRotatef(m_angLArm, 0, 1, 0);
    glColor3f(1,0,0);
    m_lLArm.draw(pass);

    glPopMatrix();

    //Change legs orientation
    m_angLeg += m_dirLeg;
    if (m_angLeg >= 25 || m_angLeg <= -25)
        m_dirLeg = -m_dirLeg;

    //Right leg
    glPushMatrix();
    glTranslatef(m_width/2.0*0.7, 0, 0.0);
    glRotatef(m_angLeg, 1, 0, 0);
    glColor3f(0,0,1);
    m_rULeg.draw(pass);

    glTranslatef(0.0, 0.0, -m_rULeg.getLength());
    if (m_angLeg > 0)
        glRotatef(-m_angLeg, 1, 0, 0);
    glColor3f(1,0,0);
    m_rLLeg.draw(pass);

    glPopMatrix();

    //Left leg
    glPushMatrix();
    glTranslatef(-(m_width/2.0*0.7), 0, 0);
    glRotatef(-m_angLeg, 1, 0, 0);
    glColor3f(0,0,1);
    m_lULeg.draw(pass);

    glTranslatef(0, 0, -m_lULeg.getLength());
    if (m_angLeg < 0)
        glRotatef(m_angLeg, 1, 0, 0);
    glColor3f(1,0,0);
    m_lLLeg.draw(pass);

    glPopMatrix();

    glPopMatrix();
}

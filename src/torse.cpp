#include <iostream>
using namespace std;
#include "torse.hpp"
#include "cylinder.hpp"
#include <cmath>
#include "animation.hpp"
#include "bubble.hpp"
#include "viewer.hpp"
#include <stdlib.h>

Torse::Torse() :
    m_length(4.f),
    m_width(2.f),
    m_headRadius(1.2f),
    m_precision(16),
    m_figure(m_length, m_width, m_precision),
    m_bottle(3.f, 1.f, m_precision),
    m_lUArm(m_precision),
    m_rUArm(m_precision),
    m_lLArm(m_precision),
    m_rLArm(m_precision),
    m_lULeg(m_precision),
    m_rULeg(m_precision),
    m_lLLeg(m_precision),
    m_rLLeg(m_precision),
    m_frame(0),
    m_bubbles(0),
    m_viewer(NULL),
    m_animSwim(new Animation(30)),
    m_currentAnim(NULL),
    m_pos(0, -30, 10)
{
    float tmp = 10;
    m_animSwim->addFrame(0, e_torse, glm::vec3(-90, 0, -tmp));
    m_animSwim->addFrame(0, e_head, glm::vec3(15, 0, tmp));

    m_animSwim->addFrame(0, e_legUL, glm::vec3(15, 0, 0));
    m_animSwim->addFrame(0, e_legLL, glm::vec3(0, 0, 0));
    m_animSwim->addFrame(0, e_legUR, glm::vec3(-15, 0, 0));
    m_animSwim->addFrame(0, e_legLR, glm::vec3(-tmp, 0, 0));

    m_animSwim->addFrame(0, e_armUL, glm::vec3(0, -75, 180));
    m_animSwim->addFrame(0, e_armUR, glm::vec3(0, 75, 10));
    m_animSwim->addFrame(0, e_armLL, glm::vec3(0, 0, 0));
    m_animSwim->addFrame(0, e_armLR, glm::vec3(0, 0, 0));

    m_animSwim->addFrame(14, e_torse, glm::vec3(-90, 0, tmp));
    m_animSwim->addFrame(14, e_head, glm::vec3(15, 0, -tmp));
    m_animSwim->addFrame(14, e_legUL, glm::vec3(-15, 0, 0));
    m_animSwim->addFrame(14, e_legLL, glm::vec3(-tmp, 0, 0));
    m_animSwim->addFrame(14, e_legUR, glm::vec3(15, 0, 0));
    m_animSwim->addFrame(14, e_legLR, glm::vec3(0, 0, 0));
    m_animSwim->addFrame(14, e_armUL, glm::vec3(0, -75, 170));
    m_animSwim->addFrame(14, e_armUR, glm::vec3(0, 75, 0));

    m_animSwim->addFrame(29, e_torse, glm::vec3(-90, 0, -tmp));
    m_animSwim->addFrame(29, e_head, glm::vec3(15, 0, tmp));
    m_animSwim->addFrame(29, e_legUL, glm::vec3(15, 0, 0));
    m_animSwim->addFrame(29, e_legLL, glm::vec3(0, 0, 0));
    m_animSwim->addFrame(29, e_legUR, glm::vec3(-15, 0, 0));
    m_animSwim->addFrame(29, e_legLR, glm::vec3(-tmp, 0, 0));
    m_animSwim->addFrame(29, e_armUL, glm::vec3(0, -75, 180));
    m_animSwim->addFrame(29, e_armUR, glm::vec3(0, 75, 10));

    setAnimation(m_animSwim);
    animate();// otherwise it all angs are at 0

}

void Torse::setAnimation(Animation* a)
{
    m_currentAnim = a;
    a->generateInterpolation(*this);
}

void Torse::draw(int pass)
{
    glPushMatrix();
    if (pass == PASS_NORMAL)
        glBindTexture(GL_TEXTURE_2D, 0);

    glTranslatef(m_pos.x, m_pos.y, m_pos.z);

    //glRotatef(45, 0, 1, 0);
    //glRotatef(45, 1, 0, 0);
    //glRotatef(45, 0, 0, 1);
    //glRotatef(m_tmp, 1, 1, 1);
    glRotatef(m_angTorse.x, 1, 0, 0);
    glRotatef(m_angTorse.y, 0, 1, 0);
    glRotatef(m_angTorse.z, 0, 0, 1);

    // Torse
    glColor3f(0,0,1);
    m_figure.draw(pass);

    // Bottle
    glPushMatrix();
    glTranslatef(0, -1.3f, 0.1f);
    glColor3f(1,1,0);
    m_bottle.draw(pass);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, m_length);
    //glTranslatef(0, m_headRadius, m_headRadius);
    glRotatef(m_angHead.x, 1, 0, 0);
    glRotatef(m_angHead.y, 0, 1, 0);
    glRotatef(m_angHead.z, 0, 0, 1);
    glColor3f(1,1,0);
    glutSolidSphere(m_headRadius, m_precision, m_precision);
    glPopMatrix();

    //Right arm
    glPushMatrix();
    glTranslatef(m_width/2.0*1.1, 0, m_length*0.80);
    glRotatef(m_angURArm.x, 1, 0, 0);
    glRotatef(m_angURArm.y, 0, 1, 0);
    glRotatef(m_angURArm.z, 0, 0, 1);
    glColor3f(0,0,1);
    m_rUArm.draw(pass);

    glTranslatef(m_rUArm.getLength(), 0, 0);
    glRotatef(m_angLRArm.x, 1, 0, 0);
    glRotatef(m_angLRArm.y, 0, 1, 0);
    glRotatef(m_angLRArm.z, 0, 0, 1);
    glColor3f(1,0,0);
    m_rLArm.draw(pass);

    glPopMatrix();

    //Left arm
    glPushMatrix();
    glTranslatef(-(m_width/2.0*1.1), 0, m_length*0.80);
    glRotatef(m_angULArm.x, 1, 0, 0);
    glRotatef(m_angULArm.y, 0, 1, 0);
    glRotatef(m_angULArm.z, 0, 0, 1);
    glColor3f(0,0,1);
    m_lUArm.draw(pass);

    glTranslatef(m_lUArm.getLength(), 0, 0);
    glRotatef(m_angLLArm.x, 1, 0, 0);
    glRotatef(m_angLLArm.y, 0, 1, 0);
    glRotatef(m_angLLArm.z, 0, 0, 1);
    glColor3f(1,0,0);
    m_lLArm.draw(pass);

    glPopMatrix();

    //Right leg
    glPushMatrix();
    glTranslatef(m_width/2.0*0.7, 0, 0.0);
    glRotatef(m_angURLeg.x, 1, 0, 0);
    glRotatef(m_angURLeg.y, 0, 1, 0);
    glRotatef(m_angURLeg.z, 0, 0, 1);
    glColor3f(0,0,1);
    m_rULeg.draw(pass);

    glTranslatef(0.0, 0.0, -m_rULeg.getLength());
    glRotatef(m_angLRLeg.x, 1, 0, 0);
    glRotatef(m_angLRLeg.y, 0, 1, 0);
    glRotatef(m_angLRLeg.z, 0, 0, 1);
    glColor3f(1,0,0);
    m_rLLeg.draw(pass);

    glPopMatrix();

    //Left leg
    glPushMatrix();
    glTranslatef(-(m_width/2.0*0.7), 0, 0);
    glRotatef(m_angULLeg.x, 1, 0, 0);
    glRotatef(m_angULLeg.y, 0, 1, 0);
    glRotatef(m_angULLeg.z, 0, 0, 1);
    glColor3f(0,0,1);
    m_lULeg.draw(pass);

    glTranslatef(0, 0, -m_lULeg.getLength());
    glRotatef(m_angLLLeg.x, 1, 0, 0);
    glRotatef(m_angLLLeg.y, 0, 1, 0);
    glRotatef(m_angLLLeg.z, 0, 0, 1);
    glColor3f(1,0,0);
    m_lLLeg.draw(pass);

    glPopMatrix();

    glPopMatrix();
}

const glm::vec3& Torse::getCurrentRotation(frame_type t)
{
    switch(t) {
        case e_armUL:
            return m_angULArm;
            break;
        case e_armLL:
            return m_angLLArm;
            break;
        case e_armUR:
            return m_angURArm;
            break;
        case e_armLR:
            return m_angLRArm;
            break;
        case e_legUL:
            return m_angULLeg;
            break;
        case e_legLL:
            return m_angLLLeg;
            break;
        case e_legUR:
            return m_angURLeg;
            break;
        case e_legLR:
            return m_angLRLeg;
            break;
        case e_head:
            return m_angHead;
            break;
        case e_torse:
            return m_angTorse;
            break;
        default:
            break;
    }
    return m_angULArm;
}

void Torse::animate()
{
    m_currentAnim->update(*this, m_frame);
    m_frame = m_frame == m_currentAnim->getSize()-1?0: m_frame+1;
    m_pos.y += 0.3f;
    m_bubbles++;
    if (m_bubbles >= 30*2) {
        m_bubbles = 0;
        m_viewer->addRenderable(new Bubble(0.0065*(rand()%40), m_pos.x, m_pos.y, m_pos.z));
    }
}


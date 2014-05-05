#include "shark.hpp"
#include "objManager.hpp"

#define fps 30
#define altitude 10.0f
#define profondeur (-0.0f)
#define rotation 45
#define vitesselente 0.1f
#define vitesserapide 1.0f
#define distanceFaceAFace 1.0f
#define distanceRecule 10.0f

Shark::Shark() :
    m_body(objManager::getObj("shark")),
    m_teeth(objManager::getObj("shark_teeth")),
    m_eyes(objManager::getObj("shark_eyes")),
    m_viewer(NULL),
    m_timer(0),
    m_pos(0, fps*4*vitesselente + fps*1*vitesserapide + distanceFaceAFace, altitude),
    m_rot(0)
{}

void Shark::draw(int pass)
{
    glPushMatrix();
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glRotatef(90, 1, 0, 0);
    glRotatef(m_rot, 1, 0, 0);
    m_body.draw(pass);
    m_teeth.draw(pass);
    m_eyes.draw(pass);
    glPopMatrix();
}

void Shark::animate()
{
    if (m_timer > fps*4 && m_timer < fps*8) {
        //Le requin avance doucement
        m_pos.y -= vitesselente;
    }
    if (m_timer > fps*11 && m_timer < fps*12) {
        //Le requin avance rapidement
        m_pos.y -= vitesserapide;
    }
    if (m_timer > fps*15 && m_timer < fps*16) {
        //Bisou
        m_pos.y -= distanceFaceAFace/fps;
    }
    if (m_timer > fps*16 && m_timer < fps*17) {
        //Bisou
        m_pos.y += distanceFaceAFace/fps;
    }
    if (m_timer > fps*18 && m_timer < fps*19) {
        //Le requin recule
        m_pos.y -= distanceRecule/fps;
    }
    if (m_timer > fps*19 && m_timer < fps*20) {
        //Le requin s'enfonce
        m_pos.z -= (altitude-profondeur)/fps;
        m_rot += rotation/fps;
    }
    m_timer++;
}

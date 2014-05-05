#include "shark.hpp"
#include "objManager.hpp"

Shark::Shark() :
    m_body(objManager::getObj("shark")),
    m_teeth(objManager::getObj("shark_teeth")),
    m_eyes(objManager::getObj("shark_eyes")),
    m_viewer(NULL),
    m_timer(0),
    m_pos(0, 0, 0)
{}

void Shark::draw(int pass)
{
    glPushMatrix();
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glRotatef(90, 1, 0, 0);
    m_body.draw(pass);
    m_teeth.draw(pass);
    m_eyes.draw(pass);
    glPopMatrix();
}

void Shark::animate()
{
}

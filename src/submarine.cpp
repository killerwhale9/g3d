#include "submarine.hpp"
#include "objManager.hpp"
#include "viewer.hpp"

Submarine::Submarine() : m_model(objManager::getObj("submarine")), m_viewer(NULL),
     m_pos(-248.f, 60.f, 0.3f), m_size(2.f, 1.f, 1.5f)
{
}

void Submarine::draw(int pass)
{
    glPushMatrix();
    //glRotatef(20, 1, 0, 0);
    glRotatef(50, 1, 0, 1);
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    m_model.draw(pass);
    glPopMatrix();
}

void Submarine::animate()
{
}

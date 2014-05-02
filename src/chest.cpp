#include "chest.hpp"
#include "objManager.hpp"

Chest::Chest() : m_model(objManager::getObj("chest"))
{
}

void Chest::draw(int pass)
{
    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glRotatef(90, 1, 0, 0);
    m_model.draw(pass);
    glPopMatrix();
}

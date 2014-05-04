#include "stone.hpp"
#include "objManager.hpp"
#include <stdlib.h>
#include <stdio.h>
#define MAX_STONES 5

Stone::Stone(): m_model(NULL), m_pos(), m_size(1.f), m_n(rand()%MAX_STONES+1)
{
    char file[100];
    m_n = 1;
    sprintf(file, "stone%d", m_n);
    m_model = &objManager::getObj(file);
}

void Stone::draw(int pass)
{
    glPushMatrix();
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glScalef(m_size, m_size, m_size);
    m_model->draw(pass);
    glPopMatrix();
}

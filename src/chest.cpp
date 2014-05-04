#include "chest.hpp"
#include "objManager.hpp"
#include "viewer.hpp"
#include "bubble.hpp"

Chest::Chest() : m_model(objManager::getObj("chest")), m_viewer(NULL),
    m_timer(0), m_pos(-28.f, 10.f, 0.3f), m_size(2.f, 1.f, 1.5f)
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

void Chest::animate()
{
    if (m_timer == 0) {
        int  n = rand()%3+1;
        for (int i = 0; i < n; ++i)
            m_viewer->addRenderable(new Bubble((rand()%40)/65.f, m_pos.x+m_size.x/2.f,
                    m_pos.y-m_size.y/.2f,
                    m_pos.z+m_size.z/2.f));
    }
   m_timer = m_timer >= 30*5? 0:m_timer+1; 
}

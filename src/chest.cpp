#include "chest.hpp"

Chest::Chest() : objReader("models/treasure_chest.obj", "gfx/treasure_chest.jpg")
{
}

void Chest::draw(int pass)
{
    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glRotatef(90, 1, 0, 0);
    objReader::draw(pass);
    glPopMatrix();
}

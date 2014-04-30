#include <iostream>
using namespace std;
#include "cylinder.hpp"
#include <cmath>

Cylinder::Cylinder(float heigth, float width, int nb_face) {
    this->heigth = heigth;
    this->width = width;
    this->nb_face = nb_face;
}

void Cylinder::draw(int pass)
{
    glPushMatrix();

    // draw immediate (center cube)
    //drawImmediate();

    // draw arrays (left cube)
    //glTranslatef(-2, 0, 0);
    //drawArrays();

    // draw elements (right cube)
    //glTranslatef(+4, 0, 0);
    drawElements();

    glPopMatrix();
}


//==================== 1. Immediate method ===================================
// immediate definition of individual vertex properties

void Cylinder::drawImmediate()
{
    //Side of the cylinder
    float x, y;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i<=nb_face; i++) {
        glNormal3f(cos((i+0.5/nb_face)*2*M_PI/nb_face), sin((i+0.5/nb_face)*2*M_PI/nb_face),0);
        x = cos(i*2.f*M_PI/nb_face)*0.5;
        y = sin(i*2.f*M_PI/nb_face)*0.5;
        glTexCoord2f((float)i/nb_face, 1.f);
        glVertex3f(x, y, +0.5);
        glNormal3f(cos((i+0.5/nb_face)*2*M_PI/nb_face), sin((i+0.5/nb_face)*2*M_PI/nb_face),0);
        glTexCoord2f((float)i/nb_face, 0.f);
        glVertex3f(x, y, -0.5);
    }
    glEnd();

    //Top and bottom of the cylinder
    for (float f = -0.5; f <= +0.5; f++) {
        glBegin(GL_POLYGON);
        glNormal3f(0.0, 0.0, 2*f);
        for (int i = 0; i<=nb_face; i++) {
            glVertex3f(cos(i*2*M_PI/nb_face)*0.5, sin(i*2*M_PI/nb_face)*0.5, f);
        }
        glEnd();
    }
}




//==================== 2. Arrays - drawArrays ================================
// - store vertex-related data (coordinates, normals, colors, etc) in arrays
//   => reduces the number of function calls

void Cylinder::drawArrays()
{
    GLfloat vertices[nb_face*3*2];
    GLfloat normals[nb_face*3];

    for (int i = 0; i < nb_face; i++) {
        normals[i*3+0] = cos((i+0.5/nb_face)*2*M_PI/nb_face);
        normals[i*3+1] = sin((i+0.5/nb_face)*2*M_PI/nb_face);
        normals[i*3+2] = 0;
        vertices[i    *3*2+0] = cos(i*2*M_PI/nb_face)*0.5;
        vertices[i    *3*2+1] = sin(i*2*M_PI/nb_face)*0.5;
        vertices[i    *3*2+2] = +0.5;
        vertices[(i+1)*3*2+0] = cos(i*2*M_PI/nb_face)*0.5;
        vertices[(i+1)*3*2+1] = sin(i*2*M_PI/nb_face)*0.5;
        vertices[(i+1)*3*2+2] = -0.5;
    }

    // activate the use of GL_VERTEX_ARRAY and GL_NORMAL_ARRAY
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // specify the arrays to use
    glNormalPointer(GL_FLOAT, 0, normals);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // draw quads using 24 data stored in arrays, starting at index 0
    glDrawArrays(GL_QUAD_STRIP, 0, nb_face*2);

    // disable the use of arrays (do not forget!)
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // Others arrays could also be used: 
    // GL_COLOR_ARRAY, GL_INDEX_ARRAY, GL_TEXTURE_COORD_ARRAY, ...
};



//==================== 3. Arrays - drawElements ==============================
// - single definition of shared data
// - draw face by face, using face indices

// TODO on calcule une seule fois pas +a chaque draw
void Cylinder::drawElements()
{
    GLfloat vertices[nb_face*3*2];//top bot top bot ...
    GLfloat texcoords[nb_face*2*2];//top bot top bot ...
    GLfloat normals[nb_face][3];
    GLubyte indices[nb_face][4];
    GLubyte indices_top_bot[2][nb_face];

    for (int i = 0; i < nb_face; i++) {
        normals[i][0] = cos((i+0.5/nb_face)*2*M_PI/nb_face);
        normals[i][1] = sin((i+0.5/nb_face)*2*M_PI/nb_face);
        normals[i][2] = 0;
        vertices[i*3*2    ] = cos(i*2*M_PI/nb_face)*0.5*this->width;
        texcoords[i*2*2   ] = (float)i/nb_face;
        vertices[i*3*2+3  ] = cos(i*2*M_PI/nb_face)*0.5*this->width;
        texcoords[i*2*2+2 ] = (float)i/nb_face;
        vertices[i*3*2  +1] = sin(i*2*M_PI/nb_face)*0.5*this->width;
        vertices[i*3*2+3+1] = sin(i*2*M_PI/nb_face)*0.5*this->width;
        vertices[i*3*2  +2] = this->heigth;
        texcoords[i*2*2 +1] = 1.f;
        vertices[i*3*2+3+2] = 0.0;
        texcoords[i*2*2+2+1] = 0.f;
        indices_top_bot[0][i] = i*2;
        indices_top_bot[1][i] = i*2+1;
        indices[i][0] = i*2;
        indices[i][1] = i*2+1;
        indices[i][2] = (i*2+3)%(2*nb_face);
        indices[i][3] = (i*2+2)%(2*nb_face);
    }

    // activate the use of GL_VERTEX_ARRAY (not GL_NORMALS_ARRAY)
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0 , vertices);
    glTexCoordPointer(2, GL_FLOAT, 0 , texcoords);

    // Draw the top
    glNormal3f(0,0,1);
    glDrawElements(GL_POLYGON, nb_face, GL_UNSIGNED_BYTE, indices_top_bot[0]);

    //Draw the bottom
    glNormal3f(0,0,-1);
    glDrawElements(GL_POLYGON, nb_face, GL_UNSIGNED_BYTE, indices_top_bot[1]);

    //Draw the rest
    for (int i = 0; i < nb_face; i++) {
        glNormal3fv(normals[i]);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices[i]);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
}


float Cylinder::getHeigth() {
	return heigth;
}


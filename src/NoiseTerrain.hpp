#ifndef __NOISETERRAIN_H__
#define __NOISETERRAIN_H__
/*******************************************************************************
 *  NoiseTerrain                                                               *
 *  Mon Apr 14 CEST 2014                                                       *
 *  Copyright Eduardo San Martin Morote                                        *
 *  eduardo.san-martin-morote@ensimag.fr                                       *
 *  http://posva.net                                                           *
 ******************************************************************************/

#include "renderable.hpp"
#include "cylinder.hpp"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <cmath>
#include <iostream>
#include <list>
#include "glm/vec3.hpp"
#include "TextureManager.hpp"

// TODO Renderable is debug purpose
class NoiseTerrain : public Renderable {

    inline double findnoise2(double x,double y)
    {
        int n=(int)x+(int)y*57;
        n=(n<<13)^n;
        int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
        return 1.0-((double)nn/1073741824.0);
    }

    inline double interpolate(double a,double b,double x)
    {
        double ft=x * 3.1415927;
        double f=(1.0-cos(ft))* 0.5;
        return a*(1.0-f)+b*f;
    }

    double noise(double x,double y);

    struct triangle_t {
        glm::vec3 vx[3];
        glm::vec3 normal;

        void computeNormal(uint32_t w, uint32_t h);
    };

    /////// VARS ///////

    double *m_hmap; // 2d heighmap acces with x+y*w
    uint32_t m_w, m_h;
    glm::vec3 *m_normals;
    std::list<triangle_t*> *m_triangles;
    std::list<triangle_t*> m_trianglePool;

public:
    // Cons dest
    NoiseTerrain();
    ~NoiseTerrain();

    void generateClouds(uint32_t w, uint32_t h, double zoom, double persistence, int octaves);

    void draw() {
        glPushMatrix();
        glScalef(50.0, 50.0, 1.0);
        glTranslatef(-0.5, -.5, 0.f);
        glColor3f(1, 1, 1);
        float reps = 3.f;
        TextureManager::bindTexture("sand1");
        for (uint32_t y = 0; y < m_h-1; y++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (uint32_t x = 0; x < m_w; x++) {
                double col = m_hmap[x+y*m_w];
                glNormal3f(m_normals[x+y*m_w].x, m_normals[x+y*m_w].y, m_normals[x+y*m_w].z);
                glTexCoord2f(x/((float)m_w)*reps,
                             y/((float)m_h)*reps);
                //glColor3f(col, col, col);
                glVertex3f((double)(x)/m_w, (double)(y)/m_h, col);
                y++;
                col = m_hmap[x+y*m_w];
                glNormal3f(m_normals[x+y*m_w].x, m_normals[x+y*m_w].y, m_normals[x+y*m_w].z);
                glTexCoord2f(x/((float)m_w)*reps,
                             y/((float)m_h)*reps);
                //glColor3f(col, col, col);
                glVertex3f((double)(x)/m_w, (double)(y)/m_h, col);
                y--;
            }
            glEnd();
        }

        //glDisable(GL_LIGHTING);
        //glBegin(GL_LINES);
        //glColor3f(1, 0, 0);
        //for (uint32_t y = 0; y < m_h; y++) {
            //for (uint32_t x = 0; x < m_w; x++) {
                //glVertex3f((double)(x)/m_w, (double)(y)/m_h, m_hmap[x+y*m_w]);
                //glVertex3f((double)(x)/m_w+m_normals[x+y*m_w].x*0.1, (double)(y)/m_h+m_normals[x+y*m_w].y*0.1, m_hmap[x+y*m_w]+m_normals[x+y*m_w].z*0.1);
            //}
        //} 
        //glEnd();
        //glEnable(GL_LIGHTING);
        glColor3f(1,1,1);

        glPopMatrix();
    }

    void computeNormals();
    void drawHMap();

};

#endif
